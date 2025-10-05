#pragma once

#include <string>

struct Page {
    int			id;
	std::string data;
};

inline Page slow_get_page(const int key) {
	return Page(key, "data " + std::to_string(key));
}
