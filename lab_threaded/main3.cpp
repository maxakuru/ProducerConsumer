#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <atomic>
#include <chrono>
#include <deque>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

std::string get_random_food(){
	static const std::vector<std::string> RANDOM_WORDS { "fish", "steak", "ice cream", "sushi","pork","KD","eggs","bread"};
	const size_t idx = std::round((1.0*rand()/RAND_MAX)*(RANDOM_WORDS.size()-1));
	return RANDOM_WORDS[idx];}

void chef_thread(bool* kill, std::mutex* mutex, std::deque<std::string>* foodQ, const int threadNumber) 
{
	while(*kill!=true){
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::string food = get_random_food();
		foodQ->push_front(food);
		std::cout<<"Cooked "<<threadNumber<<" -> "<<food<<std::endl;
	}
	std::cout<<"Chef "<<threadNumber<<" quit."<<std::endl;
    // Create a loop that checks "kill" to see if it is true, if it is, break the loop.
    // Within the loop, sleep for 500ms then push a random word onto the queue, then 
    // print "Cooked $threadNumber -> $food" 
    
}

void patron_thread(bool* kill, std::mutex* mutex, std::deque<std::string>* foodQ, const int threadNumber) {
	while(*kill!=true){
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::string food = foodQ->front();
		foodQ->pop_front();
		std::cout<<"Eaten "<<threadNumber<<" -> "<<food<<std::endl;
	}
	std::cout<<"Patron "<<threadNumber<<" left."<<std::endl;
    // Create a loop that checks "kill" to see if it is true, if it is then break the loop.
    // Within the loop, sleep for 500ms then eat as much food as you can from `foodQ`. 
    // For each food item, print "Eaten $threadNumber -> $food"
    
}

int main(int argc, char **argv) {
    /* do not remove me */
    srand(time(0));
    
    std::deque<std::string> foodQ;
    std::mutex *mu;
    std::vector<std::thread*> threads;
    bool kill(true);
    for(int i=0;i<3;i++){
	std::thread* new_thread = new std::thread(chef_thread,&kill,mu,&foodQ,i);
	threads.push_back(new_thread);
    }
    for(int i=0;i<2;i++){
	std::thread* new_thread = new std::thread(patron_thread,&kill,mu,&foodQ,i);
	threads.push_back(new_thread);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(30000));
    kill=true;
    
    // Create two loops, first loop creates 3 threads for "chef_thread" and passes
    // a pointer to `kill`, and `foodQ` and the thread thread number 
    // For the second loop, create two threads, passing the same parameters but using
    // patron_thread instead
    
    // Be sure to read the constructor documentation for std::thread ( http://bit.ly/1jX1nxq )
    // Also, make sure the threads that are created are stored within the vector of threads so
    // they can be joined and deleted later.
    
    // Add work below:
    
    return 0;
}
