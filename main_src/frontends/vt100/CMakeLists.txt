if (SHARED AND NOT NO_VT100)

else()
    message(STATUS "Not building with shared vt100")
endif()

if (NOT NO_STATIC AND NOT NO_VT100)
    target_sources(arfminesweeper PRIVATE "vt100.c")
    target_compile_definitions(arfminesweeper PUBLIC FRONTEND_VT100)
    message(STATUS "Building with static vt100")
else()
    message(STATUS "Not building with static vt100")
endif()

