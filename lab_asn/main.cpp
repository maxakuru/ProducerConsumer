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
#include <condition_variable>

// Returns a random food from the set listed below
// You should not ever change code in this function.
std::string get_random_food() 
{
    static const std::vector<std::string> RANDOM_WORDS { "fish", "steak", "icecream", "sushi", "pork", "KD", "eggs", "bread" };
    
    const size_t idx = std::round((1.0 * rand() / RAND_MAX) * (RANDOM_WORDS.size() - 1));
    return RANDOM_WORDS[idx];
}

void chef_thread(bool* kill, std::mutex* mutex, std::deque<std::string>* foodQ, const int threadNumber) 
{
    // Create a loop that checks "kill" to see if it is true, if it is, break the loop.
    while(! *kill){
    // Within the loop, sleep for 500ms then push a random word onto the queue, then
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

	std::lock_guard<std::mutex> lock(*mutex);

        std::string foodToAdd = get_random_food();
        foodQ->push_front(foodToAdd);
	
        
    // print "Cooked $threadNumber -> $food"
        std::cout<<"Cooked "<<threadNumber<<" -> "<<foodToAdd<<std::endl;
    }
    
    std::cout<<"Chef "<<threadNumber<<" quit."<<std::endl;
}

void patron_thread(bool* kill, std::mutex* mutex, std::deque<std::string>* foodQ, const int threadNumber) {
    // Create a loop that checks "kill" to see if it is true, if it is then break the loop.
    while(! *kill) {
        // Within the loop, sleep for 500ms then eat as much food as you can from `foodQ`.
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

	std::lock_guard<std::mutex> lock(*mutex);

        std::string foodToRemove = foodQ->back();
        foodQ->pop_back();
        // For each food item, print "Eaten $threadNumber -> $food"
        std::cout << "Eaten " << threadNumber << " -> " << foodToRemove << std::endl;
    }

    std::cout<<"Patron "<<threadNumber<<" left."<<std::endl;
}

int main(int argc, char **argv) {
    /* do not remove me */
    srand(time(0));
    
    std::deque<std::string> foodQ;
    bool kill(false);
    std::mutex mut;
    std::vector<std::thread*> threadVector;

    // Create two loops, first loop creates 3 threads for "chef_thread" and passes
    for(int i=1; i <= 3; i++){
        std::thread* new_thread = new std::thread(chef_thread, &kill, &mut, &foodQ, i);
        threadVector.push_back(new_thread);
    }
    // For the second loop, create two threads, passing the same parameters but using
    // patron_thread instead

    for(int i=1; i <= 2; i++){
        std::thread* new_thread = new std::thread(patron_thread, &kill, &mut, &foodQ, i);
        threadVector.push_back(new_thread);
    }

    // Be sure to read the constructor documentation for std::thread ( http://bit.ly/1jX1nxq )
    // Also, make sure the threads that are created are stored within the vector of threads so
    // they can be joined and deleted later.
    
    // Add work below
    std::this_thread::sleep_for(std::chrono::seconds(30));

    kill=true;

    for(int i=0;i<threads.size();i++){
        threads.at(i)->join();
    }
    int size=threads.size();
    for(int i=0;i<size;i++){
        delete threads.at(i);
    }

    
    std::cout << "Hello World!\n";

    return 0;
}
