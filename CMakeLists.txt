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
        file (GLOB SRC ${SRC} "frontends/motif.c")
        add_compile_definitions(FRONTEND_MOTIF)
        message(STATUS "Building with motif")
    endif()

    if (EXISTS "/usr/include/forms.h")
        set (XFORMS_FOUND true)
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
    #set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    add_compile_definitions(FRONTEND_QT5)
    message(STATUS "Building with Qt5")
endif()

# executable
add_executable(arfminesweeper ${SRC})

# libraries

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
