if (MODULE_ANSI)

else()
    message(STATUS "Not building with module ansi")
endif()

if (STATIC_ANSI)
    target_sources(arfminesweeper PRIVATE "ansi.c")
    target_compile_definitions(arfminesweeper PUBLIC FRONTEND_ANSI)
    message(STATUS "Building with static ansi")
else()
    message(STATUS "Not building with static ansi")
endif()

