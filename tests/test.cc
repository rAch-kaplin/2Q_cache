#include <cstddef>
#include <gtest/gtest.h>
#include <string>

#include "cache.hh"

struct TestCase {
        std::size_t cache_size;
        std::vector<int> elems;
        std::size_t expected_hits;
};

static Page slow_get_page(const int& key) {
	return Page(key, "data " + std::to_string(key));
}


static std::size_t RUN_TEST_E2E(std::size_t cache_size, const std::vector<int>& elems) {
    Cache2Q<int, Page> cache(cache_size);
    std::size_t hits = 0;

    for (int key : elems) {
        if (cache.lookup_update(key, slow_get_page)) hits++;
    }

    return hits;
}

TEST(Cache_2Q, Test_Case_1) {
    TestCase test{
        5,
        {1, 2, 2, 4, 1, 5, 1, 6, 3, 7},
        2
    };

    std::size_t hits = RUN_TEST_E2E(test.cache_size, test.elems);
    EXPECT_EQ(hits, test.expected_hits);
}

TEST(Cache_2Q, Test_Case_2) {
    TestCase test{
        10,
        {1, 2, 3, 1, 1},
        1
    };

    std::size_t hits = RUN_TEST_E2E(test.cache_size, test.elems);
    EXPECT_EQ(hits, test.expected_hits);
}

TEST(Cache_2Q, Test_Case_3) {
    TestCase test{
        10,
        {1, 2, 3, 4, 1, 2, 5, 6, 3, 4, 3},
        1
    };

    std::size_t hits = RUN_TEST_E2E(test.cache_size, test.elems);
    EXPECT_EQ(hits, test.expected_hits);
}

TEST(Cache_2Q, Test_Case_4) {
    TestCase test{
        8,
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        0
    };

    std::size_t hits = RUN_TEST_E2E(test.cache_size, test.elems);
    EXPECT_EQ(hits, test.expected_hits);
}

TEST(Cache_2Q, Test_Case_5) {
    TestCase test{
        15,
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5},
        3
    };

    std::size_t hits = RUN_TEST_E2E(test.cache_size, test.elems);
    EXPECT_EQ(hits, test.expected_hits);
}

TEST(Cache_2Q, Test_Case_6) {
    TestCase test{
        10,
        {1, 2, 3, 4, 5, 6, 7, 8, 1, 1},
        1
    };

    std::size_t hits = RUN_TEST_E2E(test.cache_size, test.elems);
    EXPECT_EQ(hits, test.expected_hits);
}

TEST(Cache_2Q, Test_Case_7) {
    TestCase test{
        10,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        9
    };

    std::size_t hits = RUN_TEST_E2E(test.cache_size, test.elems);
    EXPECT_EQ(hits, test.expected_hits);
}

TEST(Cache_2Q, Test_Case_8) {
    TestCase test{
        5,
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 1},
        1
    };

    std::size_t hits = RUN_TEST_E2E(test.cache_size, test.elems);
    EXPECT_EQ(hits, test.expected_hits);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
