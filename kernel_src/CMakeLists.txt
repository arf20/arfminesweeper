set (NASM_PROGRAM "nasm")
set (C_STANDARD 99)

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

#  nasm thing
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -o <OBJECT> <SOURCE>") # workaround
add_compile_options(
    "$<$<COMPILE_LANGUAGE:ASM_NASM>:-f $<IF:$<BOOL:$<TARGET_PROPERTY:NASM_OBJ_FORMAT>>, \
    $<TARGET_PROPERTY:NASM_OBJ_FORMAT>, ${CMAKE_ASM_NASM_OBJECT_FORMAT}>>"
)

# source
file (GLOB KERN_SRC
    "kernel_entry.asm"
    "kernel.c"
    "port.c"
    "console.c"
    "convga.c"
    "fbrgb.c"
    "fbvga.c"
    "font_8x16.c"
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

# build disk image
add_executable(kernel-flat ${KERN_SRC})

target_compile_options(kernel-flat PUBLIC $<$<COMPILE_LANGUAGE:C>:
    -Wall -Wextra -Wpedantic
    -m32 -ffreestanding -nostdlib -fno-pie>
)

set_target_properties(kernel-flat PROPERTIES NASM_OBJ_FORMAT elf32)

target_link_options(kernel-flat PRIVATE 
    -m32 -ffreestanding -nostdlib -fno-pie
    -Wl,-m,elf_i386 -Wl,-T,${PROJECT_SOURCE_DIR}/kernel_src/linker-flat.ld -Wl,--oformat=binary
)

set(IMAGE_BIN "arfminesweeper.img")

add_custom_command(
    OUTPUT ${IMAGE_BIN}
    COMMAND cat ${BOOT_BIN} kernel-flat > ${IMAGE_BIN} && truncate -s 64K ${IMAGE_BIN} 
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${BOOT_BIN} kernel-flat VERBATIM
)

# build ISO image
add_executable(kernel-mb2 ${KERN_SRC})

target_compile_options(kernel-mb2 PUBLIC $<$<COMPILE_LANGUAGE:C>:
    -Wall -Wextra -Wpedantic
    -m32 -ffreestanding -nostdlib -fno-pie>
)

set_target_properties(kernel-mb2 PROPERTIES NASM_OBJ_FORMAT elf32)

target_link_options(kernel-mb2 PRIVATE 
    -m32 -ffreestanding -nostdlib -no-pie
    -Wl,-m,elf_i386 -Wl,-T,${PROJECT_SOURCE_DIR}/kernel_src/linker-mb2.ld
)

set(IMAGE_ISO "arfminesweeper.iso")

add_custom_command(
    OUTPUT isofs
    COMMAND mkdir -p isofs/boot/grub && cp ${CMAKE_CURRENT_SOURCE_DIR}/grub.cfg isofs/boot/grub/ && cp kernel-mb2 isofs/boot/kernel
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS kernel-mb2
)

add_custom_command(
    OUTPUT ${IMAGE_ISO}
    COMMAND grub-mkrescue -o ${IMAGE_ISO} isofs
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS isofs VERBATIM
)

add_custom_target(image ALL DEPENDS ${IMAGE_BIN})
add_custom_target(iso ALL DEPENDS ${IMAGE_ISO})

message(STATUS "Building BIOS kernel image")

