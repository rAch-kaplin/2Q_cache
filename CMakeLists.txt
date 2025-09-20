cmake_minimum_required(VERSION 3.15)
project(Cache_2Q LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh)

add_executable(cache ${SOURCES} ${HEADERS})

target_include_directories(
	cache
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
add_subdirectory(tests)
