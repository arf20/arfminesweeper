if (X11_FOUND AND X11_xcb_FOUND)
    if (SHARED AND NOT NO_XCB)

    else()
        message(STATUS "Not building with shared xcb")
    endif()

    if (NOT NO_STATIC AND NOT NO_XCB)
        target_sources(arfminesweeper PRIVATE "xcb.c")
        target_link_libraries(arfminesweeper X11::xcb)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_XCB)
        message(STATUS "Building with static xcb")
    else()
        message(STATUS "Not building with static xcb")
    endif()
else()
    message(STATUS "No XCB found")
endif()

