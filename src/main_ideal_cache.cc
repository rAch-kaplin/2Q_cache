#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <thread>
#include <vector>
#include <iostream>

#include "page.hh"
#include "ideal_cache.hh"

int slow_get_page(int key) {
	return key;
}

int main() {
    std::size_t cache_size;
    std::cin >> cache_size;
    if (!std::cin.good()) {
        std::cerr << "error reading cache size\n";
        return 1;
    }

    std::size_t count;
    std::cin >> count;
    if (!std::cin.good()) {
        std::cerr << "error reading count elems\n";
        return 1;
    }

    std::vector<int> requests;
    requests.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        int page_id;
        std::cin >> page_id;
        if (!std::cin.good()) {
            std::cerr << "error reading page" << i << '\n';
            return 1;
        }
        requests.push_back(page_id);
    }

    IdealCache<int, Page> cache(cache_size, requests);

    int hits = 0;
    for (int key : requests) {
        if (cache.lookup_update(key, slow_get_page)) {
            ++hits;
        }
    }

    std::cout << hits << std::endl;
    return 0;
}
