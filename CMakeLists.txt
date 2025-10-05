cmake_minimum_required(VERSION 3.15)
project(Cache_2Q LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cache_2q src/main_2q.cc)

target_include_directories(
	cache_2q
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(ideal_cache src/main_ideal_cache.cc)

target_include_directories(
	ideal_cache
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/common/test_cases/test*.dat")

foreach(testfile ${TEST_FILES})
    get_filename_component(testname ${testfile} NAME_WE)

    set(answer_file_ideal_cache "${CMAKE_CURRENT_SOURCE_DIR}/common/test_cases/ideal_ans/${testname}.ans")
    set(answer_file_2q_cache    "${CMAKE_CURRENT_SOURCE_DIR}/common/test_cases/2q_ans/${testname}.ans")

    add_test(
        NAME ideal_cache_${testname}
        COMMAND /bin/sh -c "./ideal_cache < ${testfile} | diff -u - ${answer_file_ideal_cache}"
    )

    add_test(
        NAME twoq_cache_${testname}
        COMMAND /bin/sh -c "./cache_2q < ${testfile} | diff -u - ${answer_file_2q_cache}"
    )
endforeach()

