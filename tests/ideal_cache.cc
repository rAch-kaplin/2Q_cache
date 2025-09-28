#include <cstddef>
#include <gtest/gtest.h>
#include <string>

#include "ideal_cache.hh"
#include "page.hh"

struct TestCase {
    std::size_t         cache_size;
    std::vector<int>    elems;
    std::size_t         expected_hits;
};

static std::size_t RUN_TEST_E2E_IDEAL(std::size_t cache_size, const std::vector<int>& requests) {
    IdealCache<int, Page> cache(cache_size, requests);
    std::size_t hits = 0;

    for (int key : requests) {
        if (cache.lookup_update(key, slow_get_page_test)) hits++;
    }
    return hits;
}

TEST(IdealCache, Test_Case_1) {
    TestCase test{
        5,
        {1, 2, 2, 4, 1, 5, 1, 6, 3, 7},
        2
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_2) {
    TestCase test{
        10,
        {1, 2, 3, 1, 1},
        1
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_3) {
    TestCase test{
        10,
        {1, 2, 3, 4, 1, 2, 5, 6, 3, 4, 3},
        1
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_4) {
    TestCase test{
        8,
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        0
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_5) {
    TestCase test{
        15,
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5},
        3
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_6) {
    TestCase test{
        10,
        {1, 2, 3, 4, 5, 6, 7, 8, 1, 1},
        1
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_7) {
    TestCase test{
        10,
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        9
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_8) {
    TestCase test{
        5,
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 1},
        1
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_9) {
    TestCase test{
        3,
        {1, 2, 3, 4, 1, 6, 3, 6, 3, 7},
        4
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_10) {
    TestCase test{
        10,
        {1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
        13
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_11) {
    TestCase test{
        10,
        {5, 12, 5, 18, 12, 5, 3, 18, 12, 5, 3, 18, 12, 5, 3, 18, 12, 5, 3, 18,
         12, 5, 3, 18, 12, 5, 3, 18, 5, 3},
        26
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_12) {
    TestCase test{
        15,
        {1, 6, 11, 16, 21, 1, 6, 11, 16, 21, 26, 31, 1, 6, 11, 16, 21, 26, 31,
         1, 6, 11, 16, 21, 26, 31, 1, 6, 11, 26, 21, 31, 6, 11, 1},
        28
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_13) {
    TestCase test{
        10,
        {3, 4, 3, 5, 4, 6, 5, 7, 6, 8, 7, 9, 8, 10, 9, 11,
         10, 12, 11, 13, 12, 14, 13, 15, 14, 16, 13, 17},
        13
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_14) {
    TestCase test{
        15,
        {2, 5, 8, 2, 5, 8, 11, 14, 2, 5, 8, 11, 14, 17, 20,
         2, 5, 8, 11, 14, 17, 20, 23, 26, 2, 5, 8, 11, 14,
         17, 20, 23, 26, 29, 32, 2, 5, 8, 11, 14},
        29
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

TEST(IdealCache, Test_Case_15) {
    TestCase test{
        3,
        {1, 2, 3, 4, 1, 1, 2, 2, 3, 3, 4},
        6
    };

    std::size_t hits = RUN_TEST_E2E_IDEAL(test.cache_size, test.elems);
    EXPECT_GE(hits, test.expected_hits);
}

