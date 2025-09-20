#pragma once

#include <unordered_map>
#include <list>
#include <vector>

const std::size_t MIN_CACHE_SIZE = 5;

template <typename KeyT, typename ElemT>
class IdealCache {
private:
    std::size_t cache_size_;
    std::size_t cur_pos_;

    std::unordered_map      <KeyT, ElemT>                   hash_t_;
    std::unordered_map      <KeyT, std::list<std::size_t>>  key_positions_;
    std::vector             <KeyT>                          future_requests_;

public:
    explicit IdealCache(std::size_t cache_size, const std::vector<KeyT>& requests) {
        if (cache_size < MIN_CACHE_SIZE) {
            cache_size = MIN_CACHE_SIZE;
        }

        cache_size_     = cache_size;
        cur_pos_        = 0;

        if (!requests.empty()) {
            set_future_requests(requests);
        }
    }

    template <typename FuncT>
    bool lookup_update(const KeyT& key, FuncT slow_get_page) {
        if (hash_t_.find(key) != hash_t_.end()) {
            cur_pos_++;
            return true;
        }

        auto it = key_positions_.find(key);
        bool will_be_used = false;
        if (it != key_positions_.end() && !it->second.empty()) {
            auto& pos = it->second;

            while (!pos.empty() && pos.front() <= cur_pos_) {
                pos.pop_front();
            }

            will_be_used = !pos.empty();
        }

        if (!will_be_used) {
            cur_pos_++;
            return false;
        }

        ElemT elem = slow_get_page(key);
        if (hash_t_.size() < cache_size_) {
            hash_t_.emplace(key, elem);
            cur_pos_++;
            return false;
        }

        KeyT key_to_evict = find_key_to_evict();
        hash_t_.erase(key_to_evict);
        hash_t_.emplace(key, elem);
        cur_pos_++;
        return false;
    }

private:
    KeyT find_key_to_evict() {
        KeyT key_to_evict = hash_t_.begin()->first;
        std::size_t distant_pos = 0;

        for (const auto& [key, elem] : hash_t_) {
            auto& positions = key_positions_[key];

            while (!positions.empty() && positions.front() <= cur_pos_) {
                positions.pop_front();
            }

            if (positions.empty()) {
                return key;
            }

            if (positions.front() > distant_pos) {
                distant_pos = positions.front();
                key_to_evict = key;
            }
        }

        return key_to_evict;
    }

    void set_future_requests(const std::vector<KeyT>& requests) {
        cur_pos_ = 0;
        key_positions_.clear();

        for (std::size_t i = 0; i < requests.size(); ++i) {
            key_positions_[requests[i]].push_back(i);
        }
    }
};
