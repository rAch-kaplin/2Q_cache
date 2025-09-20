#pragma once

struct Page {
    int			id;
	std::string data;

	Page(int i, const std::string& d = "") : id(i), data(d) {}
};

static Page slow_get_page_test(const int& key) {
	return Page(key, "data " + std::to_string(key));
}
