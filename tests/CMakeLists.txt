file(GLOB TEST_SOURCES *.cc)
file(GLOB PROJECT_SOURCES ../include/*.hh)

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    message(STATUS "GTest not found, downloading GoogleTest...")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(cache_tests
    ${TEST_SOURCES}
    ${PROJECT_SOURCES}
)
target_include_directories(cache_tests PRIVATE ../include)
target_link_libraries(cache_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(cache_tests)

