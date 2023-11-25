cmake_minimum_required(VERSION 3.10)

project(arfminesweeper C CXX ASM_NASM)

add_subdirectory("main_src")

if (LINUX)
    add_subdirectory("mod_src")
    add_subdirectory("kernel_src")
else()
    message(STATUS "Not building linux kernel module on non-linux")
    message(STATUS "Not building bootable image on non-linux")
endif()
