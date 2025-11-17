if (CURSES_FOUND)
    if (MODULE_CURSES)

    else()
        message(STATUS "Not building with module curses")
    endif()

    if (STATIC_CURSES)
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

