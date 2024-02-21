set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_LINKER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

include_directories(/usr/include/efi /usr/include/efi/x86_64 $/usr/include/efi/protocol)

#REMOVED -fPIC 
#We use GNU_EFI_USE_MS_ABI to be sure EFI includes are in the correct convention
#We use also -m64 to be sure we have picked the correct cross-compiler and not i686-w64 
#REMOVED -maccumulate-outgoing-args -fshort-wchar -mno-red-zone -fno-stack-protector 
set(COMMONFLAGS "-DGNU_EFI_USE_MS_ABI -m64 -ffreestanding  -Wall -Werror")

#subsystem 10 is application
#NOTE: under 32bit is _efi_main
#REMOVED -Wl,-znocombreloc
set(LDFLAGS -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -Wl,-eefi_main -lgcc -Wl,-Bsymbolic -Wl,-nostdlib -L/usr/lib)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=c11 ${COMMONFLAGS}")

add_library(efi_app SHARED efi.c /usr/lib/crt0-efi-x86_64.o)
target_link_options(efi_app PRIVATE ${LDFLAGS})
target_link_libraries(efi_app efi gnuefi)


#set(GNUEFILIB ${GNUEFI}/lib/data.c ${GNUEFI}/lib/str.c ${GNUEFI}/lib/print.c ${GNUEFI}/lib/misc.c ${GNUEFI}/lib/error.c ${GNUEFI}/lib/x86_64/math.c ${GNUEFI}/lib/guid.c ${GNUEFI}/lib/runtime/efirtlib.c  ${GNUEFI}/lib/console.c  ${GNUEFI}/lib/event.c  ${GNUEFI}/lib/runtime/rtdata.c ${GNUEFI}/lib/runtime/rtstr.c ${GNUEFI}/lib/runtime/efirtlib.c ${GNUEFI}/lib/dpath.c)
