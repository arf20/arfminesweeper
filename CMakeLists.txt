cmake_minimum_required(VERSION 3.19)

# CMake modules search path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GitVersionDetect)
include(CheckLanguage)

set(ARFMINESWEEPER_VERSION_MAJOR ${GITVERSIONDETECT_VERSION_MAJOR})
set(ARFMINESWEEPER_VERSION_MINOR ${GITVERSIONDETECT_VERSION_MINOR})
set(ARFMINESWEEPER_VERSION_PATCH ${GITVERSIONDETECT_VERSION_PATCH})
set(ARFMINESWEEPER_VERSION ${ARFMINESWEEPER_VERSION_MAJOR}.${ARFMINESWEEPER_VERSION_MINOR}.${ARFMINESWEEPER_VERSION_PATCH})
set(ARFMINESWEEPER_NUM_COMMIT ${GITVERSIONDETECT_VERSION_COMMIT_NUM}-${GITVERSIONDETECT_VERSION_COMMIT_SHA})

message(STATUS "Git version ${ARFMINESWEEPER_VERSION}-${ARFMINESWEEPER_NUM_COMMIT}")

project(
    arfminesweeper
    VERSION ${ARFMINESWEEPER_VERSION}
    DESCRIPTION "Minesweeper, but in every way possible"
    HOMEPAGE_URL "http://github.org/arf20/arfminesweeper"
    LANGUAGES C CXX ASM ASM_NASM
)

check_language(C)
if(CMAKE_C_COMPILER)
    enable_language(C)
else()
    message(STATUS "No C support")
endif()

check_language(CXX)
if(CMAKE_CXX_COMPILER)
    enable_language(CXX)
else()
    message(STATUS "No CXX support")
endif()

check_language(ASM)
if(CMAKE_ASM_COMPILER)
    enable_language(ASM)
else()
    message(STATUS "No ASM support")
endif()

check_language(ASM_NASM)
if(CMAKE_ASM_NASM_COMPILER)
    enable_language(ASM_NASM)
else()
    message(STATUS "No ASM_NASM support")
endif()


add_subdirectory("main_src")


if (LINUX AND NOT NO_MODULE)
	add_subdirectory("mod_src")    
else()
    message(STATUS "Not building linux kernel module")
endif()

if(CMAKE_ASM_NASM_COMPILER AND NOT NO_KERNEL)
    add_subdirectory("kernel_src")
else()
    message(STATUS "Not building kernel")
endif()

if (NOT NO_EFIAPP)
    add_subdirectory("efi_src")
else()
	message(STATUS "Not building EFI app")
endif()

