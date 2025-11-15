if (X11_FOUND)
    if (SHARED AND NOT NO_XLIB)

    else()
        message(STATUS "Not building with shared xlib")
    endif()

    if (NOT STATIC AND NOT NO_XLIB)
        target_sources(arfminesweeper PRIVATE "xlib.c")
        target_link_libraries(arfminesweeper ${X11_LIBRARIES})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_XLIB)
        message(STATUS "Building with static xlib")
    else()
        message(STATUS "Not building with static xlib")
    endif()
else()
    message(STATUS "No Xlib found")
endif()

