#pragma once

#include <list>
#include <limits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace IdealCache {

template <typename KeyT, typename ElemT>
class IdealCache {
private:
    static constexpr std::size_t NO_FUTURE_USE = std::numeric_limits<std::size_t>::max();

    std::size_t                     cache_size_;
    std::size_t                     cur_pos_;

    std::unordered_map      <KeyT, ElemT>                   hash_t_;
    std::unordered_map      <KeyT, std::list<std::size_t>>  key_positions_;

public:
    explicit IdealCache(std::size_t cache_size, const std::vector<KeyT>& requests) {
        cache_size_     = cache_size;
        cur_pos_        = 0;

        if (!requests.empty()) {
            set_future_requests(requests.begin(), requests.end());
        }
    }

    template <typename FuncT>
    bool lookup_update(const KeyT& key, FuncT slow_get_page) {
        if (hash_t_.find(key) != hash_t_.end()) {
            drop_processed_requests(key);
            cur_pos_++;
            return true;
        }

        const std::size_t key_next_use = get_next_use(key);

        if (cache_size_ == 0) {
            cur_pos_++;
            return false;
        }

        if (hash_t_.size() < cache_size_) {
            if (key_next_use != NO_FUTURE_USE) {
                hash_t_.emplace(key, slow_get_page(key));
            }
            cur_pos_++;
            return false;
        }

        auto [it_to_evict, evict_next_use] = find_key_to_evict();
        if (key_next_use == NO_FUTURE_USE || key_next_use > evict_next_use) {
            cur_pos_++;
            return false;
        }

        if (it_to_evict != hash_t_.end()) {
            hash_t_.erase(it_to_evict);
        }

        hash_t_.emplace(key, slow_get_page(key));
        cur_pos_++;
        return false;
    }

private:
    void drop_processed_requests(const KeyT& key) {
        auto it = key_positions_.find(key);
        if (it == key_positions_.end()) {
            return;
        }

        auto& positions = it->second;
        while (!positions.empty() && positions.front() <= cur_pos_) {
            positions.pop_front();
        }
    }

    std::size_t get_next_use(const KeyT& key) {
        auto it = key_positions_.find(key);
        if (it == key_positions_.end()) {
            return NO_FUTURE_USE;
        }

        auto& positions = it->second;
        while (!positions.empty() && positions.front() <= cur_pos_) {
            positions.pop_front();
        }

        if (positions.empty()) {
            return NO_FUTURE_USE;
        }

        return positions.front();
    }

    std::pair<typename decltype(hash_t_)::iterator, std::size_t> find_key_to_evict() {
        if (hash_t_.empty()) {
            return {hash_t_.end(), 0};
        }

        auto it_on_evict_key = hash_t_.begin();
        std::size_t distant_pos = get_next_use(it_on_evict_key->first);
        if (distant_pos == NO_FUTURE_USE) {
            return {it_on_evict_key, distant_pos};
        }

        for (auto it = hash_t_.begin(); it != hash_t_.end(); it++) {
            const std::size_t next_use = get_next_use(it->first);
            if (next_use == NO_FUTURE_USE) {
                return {it, next_use};
            }

            if (next_use > distant_pos) {
                distant_pos = next_use;
                it_on_evict_key = it; 
            }
        }

       return {it_on_evict_key, distant_pos};
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
