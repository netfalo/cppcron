cmake_minimum_required(VERSION 3.10)
project(cppcron VERSION 0.1)

find_package(PkgConfig)

file(GLOB SOURCES "src/*.cpp")
file(GLOB TESTS "tests/*.cpp")
add_executable(unittest ${SOURCES} ${TESTS})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

pkg_check_modules(GTEST "gtest" REQUIRED)

target_link_libraries(unittest ${GTEST_LIBRARIES})
