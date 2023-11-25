cmake_minimum_required(VERSION 3.10)

project(arfminesweeper)

add_subdirectory("main_src")

if (LINUX)
    add_subdirectory("mod_src")
    message(STATUS "Building linux kernel module")
else()
    message(STATUS "Not building linux kernel module")
endif()
