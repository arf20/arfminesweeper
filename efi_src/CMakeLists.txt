#set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_C_COMPILER /usr/bin/gcc)
#set(CMAKE_LINKER /usr/bin/ld)
set(CMAKE_LINKER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_C_CREATE_SHARED_LIBRARY "<CMAKE_LINKER> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(ARCH x86_64)
set(EFI_INCLUDE_PATH /usr/include/efi)
set(TARGET_NAME efi_app)
set(ESP_NAME esp.img)
set(ISO_NAME arfminesweeper.iso)

list(APPEND COMPILE_FLAGS "-fno-stack-protector" "-fPIC" "-fshort-wchar" "-mno-red-zone" "-Wall" "-DEFI_FUNCTION_WRAPPER")
    #"-ffreestanding" "-fno-stack-check" "-maccumulate-outgoing-args" "-DGNU_EFI_USE_MS_ABI")

#set(LINK_FLAGS -nostdlib -T ${CMAKE_CURRENT_SOURCE_DIR}/elf_x86_64_efi.lds -shared -Bsymbolic -L ${EFI_LIB_PATH} -L ${LIB_PATH})
    #-znocombreloc) 

set(LINK_FLAGS -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main)

include_directories(${EFI_INCLUDE_PATH} ${EFI_INCLUDE_PATH}/${ARCH} ${EFI_INCLUDE_PATH}/protocol)

#add_library(${TARGET_NAME} SHARED crt0-efi-x86_64.S reloc_x86_64.c efi.c)
add_executable(${TARGET_NAME} crt0-efi-x86_64.S reloc_x86_64.c efi.c)
target_compile_options(${TARGET_NAME} PRIVATE ${COMPILE_FLAGS})
target_link_options(${TARGET_NAME} PRIVATE ${LINK_FLAGS})
#target_link_libraries(${TARGET_NAME} PRIVATE efi gnuefi)

function(efi_objcopy)
    cmake_parse_arguments(parsedargs "" "INFILE;OUTFILE" "SECTIONS" ${ARGN})
    add_custom_command(TARGET "${TARGET_NAME}" POST_BUILD
            COMMAND ${CMAKE_OBJCOPY}
            ${parsedargs_SECTIONS}
            --target=efi-app-x86_64 #  "efi-app", "efi-bsd", "efi-rtd"
            "${parsedargs_INFILE}"
            "${parsedargs_OUTFILE}")
endfunction()

set(ELF_SECTIONS
        -j .text
        -j .sdata
        -j .data
        -j .dynamic
        -j .dynsym
        -j .rel
        -j .rela
        -j .reloc)

#efi_objcopy(INFILE "lib${TARGET_NAME}.so"
#        OUTFILE "${TARGET_NAME}.efi"
#        SECTIONS ${ELF_SECTIONS})

add_custom_command(
    OUTPUT ${ESP_NAME}
    COMMAND
        dd if=/dev/zero of=${ESP_NAME} bs=1k count=1440 &&
        mformat -i ${ESP_NAME} -f 1440 :: &&
        mmd -i ${ESP_NAME} ::/EFI &&
        mmd -i ${ESP_NAME} ::/EFI/BOOT &&
        mcopy -i ${ESP_NAME} ${TARGET_NAME}.efi ::/EFI/BOOT/BOOTX64.EFI
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${TARGET_NAME}.efi VERBATIM
)

add_custom_command(
    OUTPUT ${ISO_NAME}
    COMMAND
        mkdir iso &&
        cp ${ESP_NAME} iso/ &&
        xorriso -as mkisofs -R -f -e ${ESP_NAME} -no-emul-boot -o ${ISO_NAME} iso/
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ESP VERBATIM
)

add_custom_target(ESP ALL DEPENDS ${ESP_NAME})

add_custom_target(ISO ALL DEPENDS ${ISO_NAME})
