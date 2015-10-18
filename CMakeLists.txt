cmake_minimum_required(VERSION 2.6)
project(CMAKEDEMO)

set(CMAKE_C_FLAGS "-g -Wall")

add_executable(shelltrace src/shelltrace.c src/options.c src/tracer.c)
target_link_libraries(shelltrace argp unicorn)

