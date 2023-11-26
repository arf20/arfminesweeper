set (NASM_PROGRAM "nasm")

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

#set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS ${CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS} s S) # doesn't work
#set(CMAKE_ASM_NASM_LINK_EXECUTABLE "ld <CMAKE_ASM_NASM_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>") # CMake bug
#set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32) # doesn't work
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -o <OBJECT> <SOURCE>") # workaround

add_compile_options(
    "$<$<COMPILE_LANGUAGE:ASM_NASM>:-f $<IF:$<BOOL:$<TARGET_PROPERTY:NASM_OBJ_FORMAT>>, \
    $<TARGET_PROPERTY:NASM_OBJ_FORMAT>, ${CMAKE_ASM_NASM_OBJECT_FORMAT}>>"
)

file (GLOB KERN_SRC
    "kernel_entry.asm"
    "kernel.c"
    "port.c"
    "vgaterm.c"
    "idt.c"
    "isr_irq.asm"
    "isr_irq_handler.c"
    "keyb.c"
)

add_executable(kernel ${KERN_SRC})

#target_compile_options(kernel PRIVATE
#    -Wall -Wextra -Wpedantic -Werror
#    -m32 -ffreestanding)

target_compile_options(kernel PUBLIC $<$<COMPILE_LANGUAGE:C>:
    -Wall -Wextra -Wpedantic -Werror
    -m32 -ffreestanding -nostdlib -fno-pie>
)

set_target_properties(kernel PROPERTIES NASM_OBJ_FORMAT elf32)

#set_property(TARGET kernel PROPERTY C_STANDARD 90)

#target_compile_options(kernel PUBLIC $<$<COMPILE_LANGUAGE:ASM_NASM>:
#    -felf32>
#)

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

#add_executable(kernel_dbg ${KERN_SRC})
#target_compile_options(kernel_dbg PUBLIC $<$<COMPILE_LANGUAGE:C>:
#    -Wall -Wextra -Wpedantic -Werror
#    -m32 -ffreestanding -nostdlib -fno-pie>
#)
#set_target_properties(kernel_dbg PROPERTIES NASM_OBJ_FORMAT elf32)
#target_link_options(kernel PRIVATE 
#    -m32 -ffreestanding -nostdlib -fno-pie
#    -Wl,-m,elf_i386 -Wl,--oformat=elf32-i386
#    #-m elf_i386 -Ttext 0x1000 --oformat binary
#)

message(STATUS "Building bootable image")
