#pragma once

#include <unordered_map>
#include <list>
#include <vector>

namespace IdealCache {

template <typename KeyT, typename ElemT>
class IdealCache {
private:
    static constexpr std::size_t    MIN_CACHE_SIZE = 5;    
    std::size_t                     cache_size_;
    std::size_t                     cur_pos_;

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
            set_future_requests(requests.begin(), requests.end());
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

        auto it_to_evict = find_key_to_evict();
        hash_t_.erase(it_to_evict);
        hash_t_.emplace(key, elem);
        cur_pos_++;
        return false;
    }

private:
    typename decltype(hash_t_)::iterator find_key_to_evict() {
        auto it_on_evict_key = hash_t_.begin();
        std::size_t distant_pos = 0;

        for (auto it = hash_t_.begin(); it != hash_t_.end(); it++) {
            const auto& key = it->first;
            auto& positions = key_positions_[key];

            while (!positions.empty() && positions.front() <= cur_pos_) {
                positions.pop_front();
            }

            if (positions.empty()) {
                return it;
            }

            if (positions.front() > distant_pos) {
                distant_pos = positions.front();
                it_on_evict_key = it; 
            }
        }

       return it_on_evict_key;
    }

    template<typename InputIt>
    void set_future_requests(InputIt begin, InputIt end) {
        cur_pos_ = 0;
        key_positions_.clear();

        std::size_t i = 0;
        for (auto it = begin;  it != end; it++, i++) {
            key_positions_[*it].push_back(i);
        }
    }
};

} // namespace IdealCache
