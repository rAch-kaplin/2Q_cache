#pragma once

#include <algorithm>
#include <cstddef>
#include <list>
#include <unordered_map>
#include <utility>

namespace Cache2Q {

template <typename KeyT, typename ElemT>
class Cache2Q {
	std::size_t a_in_size_{ 0 };
	std::size_t a_out_size_{ 0 };
	std::size_t a_main_size_{ 0 };

	using ListOfPage = std::list<std::pair<KeyT, ElemT>>;
	using ListOfKey  = std::list<KeyT>;

	using PageIter = typename ListOfPage::iterator;
	using KeyIter  = typename ListOfKey ::iterator;

	ListOfPage a_main_list_;
	std::unordered_map<KeyT, PageIter> a_main_map_;

	ListOfPage a_in_list_;
	std::unordered_map<KeyT, PageIter> a_in_map_;

	ListOfKey  a_out_list_;
	std::unordered_map<KeyT, KeyIter>  a_out_map_;

public:
	explicit Cache2Q(std::size_t size) {
		if (size == 0) {
			return;
		}

		a_in_size_   = std::max<std::size_t>(1, size / 4);
		a_main_size_ = size - a_in_size_;
		a_out_size_  = std::max<std::size_t>(1, size / 2);
	}

	template <typename FuncT>
	bool lookup_update(const KeyT& key, FuncT slow_get_page) {
		if (auto it = a_main_map_.find(key); it != a_main_map_.end()) {
			a_main_list_.splice(a_main_list_.begin(), a_main_list_, it->second);

			return true;
		}

		if (auto it = a_out_map_.find(key); it != a_out_map_.end()) {
			a_out_list_.erase(it->second);
			a_out_map_.erase(it);

			if (a_main_size_ == 0) {
				insert_in_a_in(key, slow_get_page(key));
			} else {
				insert_in_a_main(key, slow_get_page(key));
			}

			return false;
		}

		if (auto it = a_in_map_.find(key); it != a_in_map_.end()) {
			return true;
		}

		insert_in_a_in(key, slow_get_page(key));
		return false;
	}
private:
	void insert_in_a_main(const KeyT& key, ElemT page) {
		if (a_main_size_ == 0) {
			return;
		}

		if (a_main_list_.size() >= a_main_size_) {
			const auto& last_elem_key = a_main_list_.back().first;
			a_main_map_.erase(last_elem_key);
			a_main_list_.pop_back();
		}

		a_main_list_.emplace_front(key, page);
		a_main_map_.emplace(key, a_main_list_.begin());
	}

	void insert_in_a_in(const KeyT& key, ElemT page) {
		if (a_in_size_ == 0) {
			return;
		}

		if (a_in_list_.size() >= a_in_size_) {
			const auto& last_elem_key = a_in_list_.back().first;
			a_in_map_.erase(last_elem_key);
			a_in_list_.pop_back();

			if (auto out_it = a_out_map_.find(last_elem_key); out_it != a_out_map_.end()) {
				a_out_list_.erase(out_it->second);
				a_out_map_.erase(out_it);
			}

			a_out_list_.push_front(last_elem_key);
			a_out_map_[last_elem_key] = a_out_list_.begin();

			if (a_out_list_.size() > a_out_size_) {
				auto out_last_iter = a_out_list_.back();
				a_out_map_.erase(out_last_iter);
				a_out_list_.pop_back();
			}
		}

		a_in_list_.emplace_front(key, page);
		a_in_map_.emplace(key, a_in_list_.begin());
	}
};

} //namespace Cache2Q
