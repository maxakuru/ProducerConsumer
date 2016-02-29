#include <unistd.h>

#include <chrono>
#include <iostream>
#include <vector>

#include "report.h"
#include "shared.h"

namespace sh = shared;
namespace bip = boost::interprocess;

// Add your reader code here

int main(int argc, char **argv) {

    //sleep(1);
        sh::factory shared(bip::open_only, "block-name");
        sh::deque_ptr<Report> queue = shared.getDeque<Report>("queue");
        //sh::semaphore_ptr sem = shared.getSemaphore("sem");
        sh::mutex_ptr mutex = shared.getMutex("mutex");
        sh::condition_ptr condition = shared.getCondition("condition");

        //sem->wait();
        std::cout << "Starting reader..." << std::endl;
        while(true){
            sh::scoped_lock lock(*mutex);
            condition->wait(lock);
            if(!queue->empty()){
                Report repo = queue->front();
                if(repo.threadId == -1){
                    break;
                }
                std::cout << repo;
                queue->pop_front();
                lock.unlock();
            }
            else
            {
                lock.unlock();
            }

        //std::cout << shQueue.front();
        //shQueue.pop_front();
        //sem->signal();
        //std::cout << "Hello World!\n";
        }
    std::cout << "Ending reader..." << std::endl;
    return 0;
}
