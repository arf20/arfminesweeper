cmake_minimum_required(VERSION 3.10)

project(arfminesweeper)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

file (GLOB SRC
    "*.c"
    "frontends/*.c"
)

add_executable(arfminesweeper ${SRC})
