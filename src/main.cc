#include <chrono>
#include <cstddef>
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <vector>

#include "cache.hh"
#include "page.hh"

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

    std::cout << "Total requests: "     << requests.size()          << std::endl;
    std::cout << "Cache hits: "         << hits                     << std::endl;
    std::cout << "Cache misses: "       << requests.size() - hits   << std::endl;

    double hit_ratio = static_cast<double>(hits) / requests.size() * 100.0;
    std::cout << "Hit ratio: " << hit_ratio << "%" << std::endl;

    return 0;
}
