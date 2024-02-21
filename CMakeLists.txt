cmake_minimum_required(VERSION 3.19)

# CMake modules search path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GitVersionDetect)

set(ARFMINESWEEPER_VERSION_MAJOR ${GITVERSIONDETECT_VERSION_MAJOR})
set(ARFMINESWEEPER_VERSION_MINOR ${GITVERSIONDETECT_VERSION_MINOR})
set(ARFMINESWEEPER_VERSION_PATCH ${GITVERSIONDETECT_VERSION_PATCH})
set(ARFMINESWEEPER_VERSION ${ARFMINESWEEPER_VERSION_MAJOR}.${ARFMINESWEEPER_VERSION_MINOR}.${ARFMINESWEEPER_VERSION_PATCH})
set(ARFMINESWEEPER_NUM_COMMIT ${GITVERSIONDETECT_VERSION_COMMIT_NUM}-${GITVERSIONDETECT_VERSION_COMMIT_SHA})

message(STATUS "Git version ${ARFMINESWEEPER_VERSION}")

project(
    arfminesweeper
    VERSION ${ARFMINESWEEPER_VERSION}
    DESCRIPTION "Minesweeper, but in every way possible"
    HOMEPAGE_URL "http://github.org/arf20/arfminesweeper"
    LANGUAGES C CXX ASM ASM_NASM
)

add_subdirectory("main_src")

if (LINUX)
    add_subdirectory("mod_src")
    add_subdirectory("kernel_src")
    #add_subdirectory("efi_src")
    message(STATUS "Building linux kernel module")
    message(STATUS "Building bootable image")
    message(STATUS "Building EFI image")
else()
    message(STATUS "Not building linux kernel module on non-linux")
    message(STATUS "Not building bootable image on non-linux")
    message(STATUS "Not building EFI image on non-linux")
endif()
