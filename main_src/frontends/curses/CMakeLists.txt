if (CURSES_FOUND)
    if (SHARED AND NOT NO_CONSOLE)

    else()
        message(STATUS "Not building with shared console")
    endif()

    if (NOT NO_STATIC AND NOT NO_CONSOLE)
        target_sources(arfminesweeper PRIVATE "curses.c")
        target_link_libraries(arfminesweeper ${CURSES_LIBRARIES})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_CURSES)
        message(STATUS "Building with static curses")
    else()
        message(STATUS "Not building with static curses")
    endif()
else()
    message(STATUS "No curses/ncurses found")
endif()

