cmake_minimum_required(VERSION 3.20)

project(TimeParser CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
add_subdirectory(googletest)

add_library(TimeParser STATIC
	TimeParser.cpp
	TimeParser.h
)

add_executable(TimeParserTest TestSuite.cpp)
target_link_libraries(TimeParserTest PUBLIC
	gtest_main
	TimeParser
)
set_target_properties(TimeParserTest PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

add_test(NAME TimeParserTest COMMAND TimeParserTest)
