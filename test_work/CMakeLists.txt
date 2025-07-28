cmake_minimum_required(VERSION 3.10)
project(test_work)

add_library(test SHARED source/lib.cpp)  

target_include_directories(test PUBLIC include)  

add_subdirectory(app) 