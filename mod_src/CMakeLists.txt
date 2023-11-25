

execute_process(
    COMMAND uname -r
    OUTPUT_VARIABLE KERNEL_RELEASE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set (KERNELHEADERSAMD64_DIR /usr/src/linux-headers-${KERNEL_RELEASE})
string(REPLACE amd64 common KERNELHEADERSCOMMON_DIR ${KERNELHEADERSAMD64_DIR})

message(STATUS "Kernel release: ${KERNEL_RELEASE}")
message(STATUS "Kernel headers arch: ${KERNELHEADERSAMD64_DIR}")
message(STATUS "Kernel headers common: ${KERNELHEADERSCOMMON_DIR}")

include_directories("${KERNELHEADERSAMD64_DIR}/include")
include_directories("${KERNELHEADERSAMD64_DIR}/arch/x86/generated/include")
include_directories("${KERNELHEADERSCOMMON_DIR}/include")

set(MODULE_FILE "arfminesweeper.ko")
set(KBUILD_CMD $(MAKE) -C ${KERNELHEADERSAMD64_DIR} modules M=${CMAKE_CURRENT_BINARY_DIR} src=${CMAKE_CURRENT_SOURCE_DIR})

FILE(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/Kbuild "obj-m := module.o")

add_custom_command(OUTPUT ${MODULE_FILE}
    COMMAND ${KBUILD_CMD}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS module.c VERBATIM)

add_custom_target(driver ALL DEPENDS ${MODULE_FILE})

add_library(dummy_target module.c)

target_compile_definitions(dummy_target PUBLIC __KERNEL__ MODULE)


