if (${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD" OR
    ${CMAKE_SYSTEM_NAME} STREQUAL "NetBSD" OR
    ${CMAKE_SYSTEM_NAME} STREQUAL "OpenBSD")
    set(BSD true)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(PkgConfig)

find_package(Curses)
pkg_check_modules(DRM libdrm)
find_package(X11)
find_package(Motif)
pkg_check_modules(XAW xaw7)
if (EXISTS "/usr/include/forms.h")
    set(XFORMS_FOUND true)
endif()
pkg_check_modules(WAYLAND wayland-client)
pkg_check_modules(GTK3 gtk+-3.0)
find_package(Qt5Widgets)
find_package(Qt5 COMPONENTS Core)
find_package(SDL2)
pkg_check_modules(SDL2_ttf SDL2_ttf)
pkg_check_modules(SDL2_image SDL2_image)
find_package(SDL)
pkg_check_modules(SDL_ttf SDL_ttf)
pkg_check_modules(SDL_image SDL_image)
cmake_policy(SET CMP0072 OLD)
find_package(OpenGL)
find_package(glfw3 3.3)
find_package(GLUT)
find_package(Vulkan)
find_package(JNI)
find_program(JAVAC javac)
find_package(FLTK)
find_package(wxWidgets COMPONENTS core base)


file (GLOB SRC
    "main.c"
    "frontend.c"
    "${PROJECT_SOURCE_DIR}/common/game.c"
)

add_executable(arfminesweeper ${SRC})

set_target_properties(arfminesweeper PROPERTIES ENABLE_EXPORTS TRUE)

target_compile_definitions(arfminesweeper PUBLIC ARFMINESWEEPER_VERSION="${ARFMINESWEEPER_VERSION}")
target_compile_definitions(arfminesweeper PUBLIC ARFMINESWEEPER_NUM_COMMIT="${ARFMINESWEEPER_NUM_COMMIT}")

include_directories(arfminesweeper PUBLIC "${PROJECT_SOURCE_DIR}/" "frontends/")

# module dir
set(MODULE_DIR "${CMAKE_BINARY_DIR}/main_src/modules/")

add_subdirectory("frontends/")

# taget propieties
set_property(TARGET arfminesweeper PROPERTY VERSION ${ARFMINESWEEPER_VERSION})


# target install
install(TARGETS arfminesweeper RUNTIME DESTINATION bin)

# base package
set(CPACK_PACKAGE_NAME arfminesweeper)
set(CPACK_DEBIAN_PACKAGE_VERSION ${GITVERSIONDETECT_VERSION})
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "arf20 <arf20@arf20.com>")
if (NOT NO_STATIC)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS
        "libdrm2, libx11-6, libforms2, libgtk-3-0, libxcb1, libxm4, libxt6, libxaw7,
        libforms2, libgtk-3-0, libglib2.0-0, libglib2.0-0, libqt5widgets5, 
        libqt5gui5, libqt5core5a, 
        libsdl2-2.0-0, libsdl2-ttf-2.0-0, libsdl2-image-2.0-0, libsdl1.2debian,
        libgl1, libglut3.12, libglfw3, libvulkan1, libwayland-client0,
        libstdc++6, libc6, libgcc-s1")
else()
    set(CPACK_DEBIAN_PACKAGE_DEPENDS
        "libc6")
endif()
set(CPACK_DEBIAN_PACKAGE_NAME "arfminesweeper")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")

include(CPack)

