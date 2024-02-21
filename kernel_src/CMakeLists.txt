set (NASM_PROGRAM "nasm")

add_compile_definitions(ARFMINESWEEPER_VERSION="${ARFMINESWEEPER_VERSION}")
add_compile_definitions(ARFMINESWEEPER_NUM_COMMIT="${ARFMINESWEEPER_NUM_COMMIT}")

# MBR bootloader
file (GLOB BOOT_SRC "mbr_boot.asm")
set (BOOT_BIN "mbr_boot.bin")

add_custom_command(
    OUTPUT ${BOOT_BIN}
    COMMAND ${NASM_PROGRAM} ${BOOT_SRC} -f bin -o ${BOOT_BIN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${BOOT_SRC} VERBATIM
)

add_custom_target(bootloader ALL DEPENDS ${BOOT_BIN})

# kernel

set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -o <OBJECT> <SOURCE>") # workaround

add_compile_options(
    "$<$<COMPILE_LANGUAGE:ASM_NASM>:-f $<IF:$<BOOL:$<TARGET_PROPERTY:NASM_OBJ_FORMAT>>, \
    $<TARGET_PROPERTY:NASM_OBJ_FORMAT>, ${CMAKE_ASM_NASM_OBJECT_FORMAT}>>"
)

file (GLOB KERN_SRC
    "kernel_entry.asm"
    "kernel.c"
    "port.c"
    "vgacon.c"
    "vgafb.c"
    "keyb.c"
    "alloc.c"
    "random.c"
    "rtc_time.c"
    "int32.asm"
    "plibc.c"
    "../common/game.c"
    "kfrontends/vgacli.c"
    "kfrontends/vgatui.c"
    "kfrontends/vgagra.c"
)

add_compile_definitions(FRONTENDS_KERNEL)

add_executable(kernel ${KERN_SRC})

target_compile_options(kernel PUBLIC $<$<COMPILE_LANGUAGE:C>:
    -Wall -Wextra -Wpedantic
    -m32 -ffreestanding -nostdlib -fno-pie>
)

set_target_properties(kernel PROPERTIES NASM_OBJ_FORMAT elf32)

target_link_options(kernel PRIVATE 
    -m32 -ffreestanding -nostdlib -fno-pie
    -Wl,-m,elf_i386 -Wl,-T,${PROJECT_SOURCE_DIR}/kernel_src/linker.ld -Wl,--oformat=binary
    #-m elf_i386 -Ttext 0x1000 --oformat binary
)

# build disk image
set(IMAGE_BIN "arfminesweeper.bin")

add_custom_command(
    OUTPUT ${IMAGE_BIN}
    COMMAND cat ${BOOT_BIN} kernel > ${IMAGE_BIN} && truncate -s 64K ${IMAGE_BIN} 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${BOOT_BIN} kernel VERBATIM
)

add_custom_target(image ALL DEPENDS ${IMAGE_BIN})

message(STATUS "Building bootable image")
