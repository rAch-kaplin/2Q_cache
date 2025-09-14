/*
================================================================================
                     STL / Standard Library Reference for main.cpp
================================================================================

1. <chrono>
   - Provides types for time measurement and durations.
   
   Methods / Types:
   ------------------------------------------------------------------------
   std::chrono::milliseconds(unsigned long ms)
       - Parameters: number of milliseconds
       - Returns: duration object
       - Function: represents a time duration in milliseconds

--------------------------------------------------------------------------------
2.. <string>
   - Provides std::string for text manipulation.
   
   Methods / Usage:
   ------------------------------------------------------------------------
   std::string::operator+ (const std::string& rhs)
       - Parameters: rhs string to concatenate
       - Returns: new std::string
       - Function: concatenate strings

   std::to_string(int value)
       - Parameters: numeric value (int, long, etc.)
       - Returns: std::string representation

--------------------------------------------------------------------------------
5. <cstdlib>
   - Provides general utilities like random number generation and exit.
   
   Methods:
   ------------------------------------------------------------------------
   int rand()
       - Parameters: none
       - Returns: int (pseudo-random number in 0..RAND_MAX)

--------------------------------------------------------------------------------
6. <thread>
   - Provides multithreading utilities.
   
   Methods:
   ------------------------------------------------------------------------
   std::this_thread::sleep_for(const std::chrono::duration& dur)
       - Parameters: duration object
       - Returns: void
       - Function: pauses current thread for given duration

--------------------------------------------------------------------------------
7. <vector>
   - Dynamic array, contiguous storage.
   
   Methods / Usage:
   ------------------------------------------------------------------------
   void push_back(const T& value)
       - Parameters: value of type T
       - Returns: void
       - Function: appends element at end of vector

   size_t size() const
       - Parameters: none
       - Returns: number of elements in vector

--------------------------------------------------------------------------------
*/

#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <cstdlib>  
#include <thread>
#include <vector>

#include "cache.hh"

struct Page {
    int			id;
	std::string data;

	Page(int i, const std::string& d = "") : id(i), data(d) {}
};

Page slow_get_page(const int& key) {
	std::cout << "slow_get_page" << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	return Page(key, "Data for page " + std::to_string(key));
}    

int main() {
    const size_t cache_size = 4;

    Cache2Q<int, Page> cache(cache_size);

    std::vector<int> requests;
	for (int i = 0; i < 30; i++) {
		requests.push_back(rand() % 10 + 1);  
	}
    int hits = 0;

    std::cout << "Processing requests...\n";
    for (int key : requests) {
        if (cache.lookup_update(key, slow_get_page)) {
            std::cout << "HIT" << std::endl;
            hits++;
        } else {
            std::cout << "MISS" << std::endl;
        }
    }

    std::cout << "\n--- Cache Statistics ---\n";
    std::cout << "Total requests: " << requests.size() << std::endl;
    std::cout << "Cache hits: " << hits << std::endl;
    std::cout << "Cache misses: " << requests.size() - hits << std::endl;
    double hit_ratio = static_cast<double>(hits) / requests.size() * 100.0;
    std::cout << "Hit ratio: " << hit_ratio << "%" << std::endl;

    return 0;
}
