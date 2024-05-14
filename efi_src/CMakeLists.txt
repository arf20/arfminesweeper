find_program(MINGW x86_64-w64-mingw32-gcc)
find_program(MTOOLS mcopy)

set(CMAKE_C_COMPILER   x86_64-w64-mingw32-gcc)

set(GNUEFI ${PROJECT_SOURCE_DIR}/efi_src/gnu_efi)

include_directories(${GNUEFI}/inc ${PROJECT_SOURCE_DIR}/include ${GNUEFI}/inc/x86_64 ${GNUEFI}/inc/protocol ${GNUEFI}/lib)

#REMOVED -fPIC 
#We use GNU_EFI_USE_MS_ABI to be sure EFI includes are in the correct convention
#We use also -m64 to be sure we have picked the correct cross-compiler and not i686-w64 
#REMOVED -maccumulate-outgoing-args -fshort-wchar -mno-red-zone -fno-stack-protector 
set(COMMONFLAGS "-DGNU_EFI_USE_MS_ABI -m64 -ffreestanding  -Wall -Werror -fshort-wchar")

#subsystem 10 is application
#NOTE: under 32bit is _efi_main
#REMOVED -Wl,-znocombreloc
set(LDFLAGS "-nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -Wl,-eefi_main -lgcc -Wl,-Bsymbolic -Wl,-nostdlib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 ${COMMONFLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c11 ${COMMONFLAGS}")

if (MINGW)
    add_library(efiapp SHARED efi_app.c data.c)
    target_link_libraries(efiapp ${LDFLAGS})
    message(STATUS "Building EFI stub")
elseif()
    message(STATUS "Not building EFI without MINGW")
endif()

set(GNUEFILIB ${GNUEFI}/lib/data.c ${GNUEFI}/lib/str.c ${GNUEFI}/lib/print.c ${GNUEFI}/lib/misc.c ${GNUEFI}/lib/error.c ${GNUEFI}/lib/x86_64/math.c ${GNUEFI}/lib/guid.c ${GNUEFI}/lib/runtime/efirtlib.c  ${GNUEFI}/lib/console.c  ${GNUEFI}/lib/event.c  ${GNUEFI}/lib/runtime/rtdata.c ${GNUEFI}/lib/runtime/rtstr.c ${GNUEFI}/lib/runtime/efirtlib.c ${GNUEFI}/lib/dpath.c)

set(IMAGE_BIN esp.img)

add_custom_command(
    OUTPUT ${IMAGE_BIN}
    COMMAND dd if=/dev/zero of=${IMAGE_BIN} bs=1k count=1440 
    COMMAND mformat -i ${IMAGE_BIN} -f 1440 ::
    COMMAND mmd -i ${IMAGE_BIN} ::/EFI
    COMMAND mmd -i ${IMAGE_BIN} ::/EFI/BOOT
    COMMAND echo "\\EFI\\BOOT\\BOOTX64.EFI" > startup.nsh
    COMMAND mcopy -i ${IMAGE_BIN} startup.nsh ::/
    COMMAND mcopy -oi ${IMAGE_BIN} libefiapp.so ::/EFI/BOOT/BOOTX64.EFI
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS efiapp VERBATIM
)

if (MINGW AND MTOOLS)
    add_custom_target(esp_image ALL DEPENDS ${IMAGE_BIN})
    message(STATUS "Building ESP")
elseif()
    message(STATUS "Not building ESP without mtools")
endif()
