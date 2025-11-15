if (SHARED AND NOT NO_CONSOLE)

else()
    message(STATUS "Not building with shared console")
endif()

if (NOT STATIC AND NOT NO_CONSOLE)
    target_sources(arfminesweeper PRIVATE "console.c")
    target_compile_definitions(arfminesweeper PUBLIC FRONTEND_CONSOLE)
    message(STATUS "Building with static console")
else()
    message(STATUS "Not building with static console")
endif()

