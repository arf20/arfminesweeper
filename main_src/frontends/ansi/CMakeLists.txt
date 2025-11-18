if (MODULE_ANSI)
    add_library(ansi MODULE "ansi.c")
    target_compile_definitions(ansi PUBLIC _MODULE_)
    add_custom_target(symlink_ansi ALL
        COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
        DEPENDS ansi module_dir)
    message(STATUS "Building with module ansi")
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

