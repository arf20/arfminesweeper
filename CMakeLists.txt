cmake_minimum_required(VERSION 3.10)

project(arfminesweeper)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

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

    find_package(Motif)
    if (MOTIF_FOUND)
        file (GLOB SRC ${SRC} "frontends/motif.c")
        add_compile_definitions(FRONTEND_MOTIF)
    endif()
endif()

# executable
add_executable(arfminesweeper ${SRC})

# libraries

if (X11_FOUND)
    target_link_libraries(arfminesweeper ${X11_LIBRARIES})

    if (MOTIF_FOUND)
        target_link_libraries(arfminesweeper ${MOTIF_LIBRARIES} Xt)
    endif()
endif()
