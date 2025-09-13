cmake_minimum_required(VERSION 3.15)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh)

add_executable(cache ${SOURCES} ${HEADERS})

target_include_directories(
	cache
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ 
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(cache)
