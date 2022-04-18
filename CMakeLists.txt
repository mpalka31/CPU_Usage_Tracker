cmake_minimum_required(VERSION 3.10)
project(cut VERSION 1.0)

file(GLOB_RECURSE SRC_FILES src/*.c)
add_executable(cut ${SRC_FILES})
target_include_directories(cut PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)