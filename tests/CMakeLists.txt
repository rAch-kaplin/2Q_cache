find_package(GTest QUIET)

add_executable(
    cache_tests
    cache_2q_test.cc 
    ideal_cache.cc
)

target_include_directories(cache_tests PRIVATE ../include)
target_link_libraries(cache_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(cache_tests)
enable_testing()
