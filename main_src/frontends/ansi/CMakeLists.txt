if (MODULE AND NOT NO_ANSI)

else()
    message(STATUS "Not building with module ansi")
endif()

if (NOT NO_STATIC AND NOT NO_ANSI)
    target_sources(arfminesweeper PRIVATE "ansi.c")
    target_compile_definitions(arfminesweeper PUBLIC FRONTEND_ANSI)
    message(STATUS "Building with static ansi")
else()
    message(STATUS "Not building with static ansi")
endif()

