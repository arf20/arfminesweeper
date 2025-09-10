# find release
execute_process(
        COMMAND uname -r
        OUTPUT_VARIABLE KERNEL_RELEASE
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(KERNELHEADERS_DIR /usr/src/linux-headers-${KERNEL_RELEASE})

message(STATUS "Kernel release: ${KERNEL_RELEASE}")
message(STATUS "Kernel headers: ${KERNELHEADERS_DIR}")

if (KERNELHEADERS_DIR)
    set(KERNELHEADERS_INCLUDE_DIRS
            ${KERNELHEADERS_DIR}/include
            ${KERNELHEADERS_DIR}/arch/x86/include
            CACHE PATH "Kernel headers include dirs"
    )
    set(KERNELHEADERS_FOUND 1 CACHE STRING "Set to 1 if kernel headers were found")
else (KERNELHEADERS_DIR)
    set(KERNELHEADERS_FOUND 0 CACHE STRING "Set to 1 if kernel headers were found")
endif (KERNELHEADERS_DIR)

mark_as_advanced(KERNELHEADERS_FOUND)

# include headers
include_directories(${KERNELHEADERS_INCLUDE_DIRS})

# make module
set(MODULE_FILE "arfminesweeper.ko")
set(BUILD_CMD ${CMAKE_MAKE_PROGRAM} -C ${KERNELHEADERS_DIR} modules M=${CMAKE_CURRENT_BINARY_DIR} src=${CMAKE_CURRENT_SOURCE_DIR})
FILE(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/Kbuild "obj-m := arfminesweeper.o
arfminesweeper-y := module.o ../common/game.o
")

add_custom_command(
    OUTPUT ${MODULE_FILE}
    COMMAND ${BUILD_CMD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS module.c VERBATIM
)

if (KERNELHEADERS_FOUND)
    add_custom_target(module ALL DEPENDS ${MODULE_FILE})
    message(STATUS "Building linux kernel module")
else()
    message(STATUS "Kernel headers required to build module")
    message(STATUS "Not building linux kernel module")
endif()
