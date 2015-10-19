cmake_minimum_required(VERSION 2.6)
project(CMAKEDEMO)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-field-initializers -Wno-unused-parameter")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

add_executable(shelltrace src/shelltrace.c src/options.c src/tracer.c src/util.c src/hook.c)
target_link_libraries(shelltrace argp unicorn)

