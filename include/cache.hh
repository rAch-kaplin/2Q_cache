/*
================================================================================
                            STL Reference for Cache2Q
================================================================================
1. std::list<T>
   - Doubly-linked list. Maintains element order, efficient insert/remove at both ends.

   Methods:
   ------------------------------------------------------------------------
   iterator emplace_front(Args&&... args)
       - Parameters: arguments to construct element T in-place
       - Returns: iterator to the newly inserted element
       - Function: inserts new element at the beginning of the list

   void push_front(const T& value)
       - Parameters: const reference to element of type T
       - Returns: void
       - Function: inserts element at the front (copy)

   void pop_back()
       - Parameters: none
       - Returns: void
       - Function: removes last element

   T& back()
       - Parameters: none
       - Returns: reference to last element
       - Function: access the last element

   void splice(iterator pos, list& other, iterator it)
       - Parameters: 
           pos   - iterator in target list where element will be moved
           other - source list
           it    - iterator to element in source list
       - Returns: void
       - Function: moves element from 'other' list to 'this' list at position 'pos'

--------------------------------------------------------------------------------
2. std::unordered_map<Key, Value>
   - Hash table for O(1) access by key. Stores key-value pairs.

   Methods:
   ------------------------------------------------------------------------
   iterator find(const Key& key)
       - Parameters: key to search
       - Returns: iterator to element if found, else end()
       - Function: check existence of key

   std::pair<iterator, bool> emplace(const Key& k, const Value& v)
       - Parameters: key k, value v
       - Returns: pair<iterator,bool> where bool=true if insertion happened
       - Function: insert key-value pair if key does not exist

   void erase(const Key& key)
       - Parameters: key to remove
       - Returns: void
       - Function: remove element by key

--------------------------------------------------------------------------------
3. std::pair<Key, Elem>
   - Simple struct storing two elements (first=Key, second=Elem)
   - Usage: store (key, page) in a_main_list_ and a_in_list_
   - Access: pair.first, pair.second

--------------------------------------------------------------------------------
4. iterator (list::iterator)
   - Object that points to an element in a list
   - Supports:
       *it           : dereference
       ++it, --it    : increment/decrement
       it1 == it2    : comparison
   - Usage: store position of element in list for O(1) removal or move
--------------------------------------------------------------------------------
*/

#pragma once

#include <list>
#include <unordered_map>
#include <utility>

const std::size_t CACHE_SIZE = 8;

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
		if (size < CACHE_SIZE) {
			size = CACHE_SIZE;
		}

		a_in_size_ = size / 4;
		a_out_size_ = size;
		a_main_size_ = size - a_in_size_;
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

			insert_in_a_main(key, slow_get_page(key));

			return false;
		}

		if (auto it = a_in_map_.find(key); it != a_in_map_.end()) {
			a_main_list_.splice(a_main_list_.begin(), a_in_list_, it->second);

			a_in_map_.erase(it);
			a_main_map_[key] = a_main_list_.begin();

			return true;
		}

		insert_in_a_in(key, slow_get_page(key));
		return false;
	}
private:
	void insert_in_a_main(const KeyT& key, ElemT page) {
		if (a_main_list_.size() >= a_main_size_) {
			auto last_key = a_main_list_.back().first;
			a_main_map_.erase(last_key);
			a_main_list_.pop_back();
		}

		a_main_list_.emplace_front(key, page);
		a_main_map_.emplace(key, a_main_list_.begin());
	}
	
	void insert_in_a_in(const KeyT& key, ElemT page) {
		if (a_in_list_.size() >= a_in_size_) {
			auto last_key = a_in_list_.back().first;
			a_in_map_.erase(last_key);
			a_in_list_.pop_back();

			a_out_list_.push_front(last_key);
			a_out_map_.emplace(last_key, a_out_list_.begin());

			if (a_out_list_.size() >= a_out_size_) {
				auto out_last_iter = a_out_list_.back();
				a_out_map_.erase(out_last_iter);
				a_out_list_.pop_back();
			}
		}	

		a_in_list_.emplace_front(key, page);
		a_in_map_.emplace(key, a_in_list_.begin());
	}
};

