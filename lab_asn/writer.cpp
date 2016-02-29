#include <unistd.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "report.h"
#include "shared.h"

namespace sh = shared;
namespace bip = boost::interprocess;
    
void writer_thread(int N, int threadNum, bool *kill, sh::deque_ptr<Report> shQueue, sh::mutex_ptr mutex, sh::condition_ptr condition)
{	
	int count = 1;
	auto t1 = std::chrono::system_clock::now();
	while(!*kill)
	{	
		//claim semaphore here
                //sem->wait();
            shared::scoped_lock lock(*mutex);
		if(count == 1)
		{	
			
			//create a report with 0 as time since last, add to deque
                        Report new_report = Report(threadNum, count, std::chrono::duration_cast<std::chrono::milliseconds>(t1-t1));
			shQueue->push_front(new_report);
			
		}
		else
		{
			auto t2 = std::chrono::system_clock::now();
			auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1);

			//create report with time since last as 'diff'
                        Report new_report = Report(threadNum, count, diff);
			shQueue->push_front(new_report);		

                        t1 = std::chrono::system_clock::now();
			
		}
		
                t1 = std::chrono::system_clock::now();

		//yield semaphore here
                //sem->yield();
                lock.unlock();
                condition->notify_one();
		
		//wait for N seconds before making another report
		std::this_thread::sleep_for(std::chrono::seconds(N));

		count++; //increment count
	}
}

int main(int argc, char **argv) {
    
	//create deque of reports to share
        sh::factory shared(bip::create_only, "block-name");
        sh::deque_ptr<Report> queue = shared.createDeque<Report>("queue");
        //sh::semaphore_ptr sem = shared.createSemaphore("sem");
        sh::mutex_ptr mutex = shared.createMutex("mutex");
        sh::condition_ptr condition = shared.createCondition("condition");
	std::vector<std::thread*> threadVector;

	bool cont = true;
	bool kill = false;
	int count = 1;

	while(cont)
	{
		std::cout << "Would you like to create a new thread?" << std::endl;
		std::string choice; 		
		std::cin >> choice;
		std::cout << std::endl;
		//choice = std::toupper(choice);
		if(choice == "YES" || choice == "yes")
		{
			std::cout << "Please enter number of seconds between reports: " <<std::endl;
			int N;
			std::cin >> N;
			std::cout << std::endl;

			//create thread with N as second b/w reports
                        std::thread* new_thread = new std::thread(writer_thread, N, count, &kill, queue, mutex, condition);

			//add thread to vector
			threadVector.push_back(new_thread);
                        count++;
		}

                if(choice == "NO" || choice == "no")
                {
                    kill = true;
                    sh::scoped_lock lock(*mutex);
                    auto time = std::chrono::system_clock::now();
                    Report repo(-1,-1,std::chrono::duration_cast<std::chrono::milliseconds>(time-time));
                    queue->push_front(repo);
                    lock.unlock();
                    condition->notify_one();
                    for(int i = 0; i < threadVector.size(); i++)
                    {
                        threadVector.at(i)->join();
                    }
                    int length = threadVector.size();
                    for(int i = 0; i < length; i++)
                    {
                        delete threadVector.at(i);
                    }
                    cont = false;
                }
	}

        /*
	kill = true;
	std::this_thread::sleep_for(std::chrono::seconds(10));
	for(int i = 0; i < threadVector.size(); i++)
	{
		threadVector.at(i)->join();
		threadVector.pop_back();
		delete threadVector.at(i);
	}
        */
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                bip::shared_memory_object::remove("queue");
                bip::shared_memory_object::remove("mut");
                bip::shared_memory_object::remove("condition");
    std::cout << "Ending writer...\n";
    
    return 0;
}
