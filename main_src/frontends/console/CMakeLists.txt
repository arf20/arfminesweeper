if (MODULE_CONSOLE)
    add_library(console MODULE "console.c")
    target_compile_definitions(console PUBLIC _MODULE_)
    add_custom_target(symlink_console ALL
        COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
        DEPENDS console module_dir)
    message(STATUS "Building with module console")
else()
    message(STATUS "Not building with module console")
endif()

if (STATIC_CONSOLE)
    target_sources(arfminesweeper PRIVATE "console.c")
    target_compile_definitions(arfminesweeper PUBLIC FRONTEND_CONSOLE)
    message(STATUS "Building with static console")
else()
    message(STATUS "Not building with static console")
endif()

