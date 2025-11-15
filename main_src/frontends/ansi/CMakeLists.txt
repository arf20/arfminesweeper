if (SHARED AND NOT NO_ANSI)

else()
    message(STATUS "Not building with shared ansi")
endif()

if (NOT STATIC AND NOT NO_ANSI)
    target_sources(arfminesweeper PRIVATE "ansi.c")
    target_compile_definitions(arfminesweeper PUBLIC FRONTEND_ANSI)
    message(STATUS "Building with static ansi")
else()
    message(STATUS "Not building with static ansi")
endif()

