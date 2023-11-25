set (NASM_PROGRAM "nasm")

# MBR bootloader
file (GLOB BOOT_SRC "mbr_boot.s")
set (BOOT_BIN "mbr_boot.bin")

add_custom_command(
    OUTPUT ${BOOT_BIN}
    COMMAND ${NASM_PROGRAM} ${BOOT_SRC} -f bin -o ${BOOT_BIN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${BOOT_SRC} VERBATIM
)

add_custom_target(bootloader ALL DEPENDS ${BOOT_BIN})

message(STATUS "Building bootable image")
