cmake_minimum_required(VERSION 3.10)

project(arfminesweeper)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)

file (GLOB SRC
    "main.c"
    "game.c"
)

# frontends

file (GLOB SRC ${SRC} "frontends/console.c")
add_compile_definitions(FRONTEND_CONSOLE)

file (GLOB SRC ${SRC} "frontends/fbdev.c" "frontends/bmp.c")
add_compile_definitions(FRONTEND_FBDEV)

find_package(X11)
if (X11_FOUND)
    file (GLOB SRC ${SRC} "frontends/x11.c")
    add_compile_definitions(FRONTEND_X11)
    message(STATUS "Building with X11")

    find_package(Motif)
    if (MOTIF_FOUND)
        file(GLOB SRC ${SRC} "frontends/motif.c")
        add_compile_definitions(FRONTEND_MOTIF)
        message(STATUS "Building with motif")
    endif()

    if (EXISTS "/usr/include/forms.h")
        set(XFORMS_FOUND true)
        file (GLOB SRC ${SRC} "frontends/xforms.c")
        add_compile_definitions(FRONTEND_XFORMS)
        message(STATUS "Building with XForms")
    endif()
endif()

pkg_check_modules(GTK3 gtk+-3.0) 
if (GTK3_FOUND)
    file (GLOB SRC ${SRC} "frontends/gtk3.c")
    add_compile_definitions(FRONTEND_GTK3)
    message(STATUS "Building with gtk3")
endif()

find_package(Qt5Widgets)
if (Qt5Widgets_FOUND)
    file (GLOB SRC ${SRC} "frontends/qt5.cpp" "frontends/qt5.hpp")
    include_directories(${Qt5Widgets_INCLUDES})
    add_definitions(${Qt5Widgets_DEFINITIONS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    add_compile_definitions(FRONTEND_QT5)
    message(STATUS "Building with Qt5")
endif()

find_package(SDL2)
pkg_check_modules(SDL2_ttf SDL2_ttf)
pkg_check_modules(SDL2_image SDL2_image)
if (SDL2_FOUND)
    if (SDL2_ttf_FOUND AND SDL2_image_FOUND)
        file (GLOB SRC ${SRC} "frontends/sdl2.c")
        add_compile_definitions(FRONTEND_SDL2)
        message(STATUS "Building with SDL2")
    else()
        message(WARNING "SDL2_ttf and SDL2_image required for building SDL2 frontend")
    endif()
endif()

find_package(SDL)
pkg_check_modules(SDL_ttf SDL_ttf)
pkg_check_modules(SDL_image SDL_image)
if (SDL_FOUND)
    if (SDL_ttf_FOUND AND SDL_image_FOUND)
        file (GLOB SRC ${SRC} "frontends/sdl1.c")
        add_compile_definitions(FRONTEND_SDL1)
        message(STATUS "Building with SDL 1.2")
    else()
        message(WARNING "SDL_ttf and SDL_image required for building SDL1.2 frontend")
    endif()
endif()

cmake_policy(SET CMP0072 OLD)
find_package(OpenGL)
if (OpenGL_OpenGL_FOUND)
    find_package(GLUT)
    if (GLUT_FOUND)
        file (GLOB SRC ${SRC} "frontends/gl11.c")
        add_compile_definitions(FRONTEND_GL11)
        message(STATUS "Building with OpenGL 1.1/FreeGLUT")
    else()
        message(WARNING "FreeGLUT required for building OpenGL 1.1 frontend")
    endif()

    find_package(glfw3 3.3)
    if (glfw3_FOUND)
        file (GLOB SRC ${SRC} "frontends/gl33.c" "frontends/glad/glad.c")
        include_directories("frontends/glad/include/")
        add_compile_definitions(FRONTEND_GL33)
        message(STATUS "Building with OpenGL 3.3/GLFW/GLAD")
    else()
        message(WARNING "GLFW required for building OpenGL 3.3 frontend")
    endif()
endif()



# executable
add_executable(arfminesweeper ${SRC})


# link ibraries

if (X11_FOUND)
    target_link_libraries(arfminesweeper ${X11_LIBRARIES})

    if (MOTIF_FOUND)
        target_link_libraries(arfminesweeper ${MOTIF_LIBRARIES} Xt)
    endif()

    if (XFORMS_FOUND)
        target_link_libraries(arfminesweeper forms)
    endif()
endif()

if (GTK3_FOUND)
    target_include_directories(arfminesweeper PUBLIC ${GTK3_INCLUDE_DIRS})
    target_link_libraries(arfminesweeper ${GTK3_LIBRARIES})
endif()

if (Qt5Widgets_FOUND)
    target_link_libraries(arfminesweeper Qt5::Widgets)
endif()

if (SDL2_FOUND AND SDL2_ttf_FOUND AND SDL2_image_FOUND)
    target_link_libraries(arfminesweeper ${SDL2_LIBRARIES} ${SDL2_ttf_LIBRARIES} ${SDL2_image_LIBRARIES})
endif()

if (SDL_FOUND AND SDL_ttf_FOUND AND SDL_image_FOUND)
    target_link_libraries(arfminesweeper ${SDL_LIBRARIES} ${SDL_ttf_LIBRARIES} ${SDL_image_LIBRARIES})
endif()

if (OpenGL_OpenGL_FOUND)
    target_link_libraries(arfminesweeper OpenGL::GL)

    if (GLUT_FOUND)
        target_link_libraries(arfminesweeper ${GLUT_LIBRARIES})
    endif()

    if (glfw3_FOUND)
        target_link_libraries(arfminesweeper glfw dl)
    endif()
endif()

