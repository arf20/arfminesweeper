if (WAYLAND_FOUND)
    if (SHARED AND NOT NO_WAYLAND)

    else()
        message(STATUS "Not building with shared wayland")
    endif()

    if (NOT NO_STATIC AND NOT NO_WAYLAND)
        target_sources(arfminesweeper PRIVATE "wayland.c")
        target_link_libraries(arfminesweeper wayland-client)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_WAYLAND)
        message(STATUS "Building with static wayland")
    else()
        message(STATUS "Not building with static wayland")
    endif()
else()
    message(STATUS "No Wayland found")
endif()

