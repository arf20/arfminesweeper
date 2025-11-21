if (MODULE_VT100)
    add_library(arfminesweeper-vt100 MODULE "vt100.c")
    target_compile_definitions(arfminesweeper-vt100 PUBLIC _MODULE_)
    add_custom_target(symlink_arfminesweeper-vt100 ALL
        COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
        DEPENDS arfminesweeper-vt100 module_dir)
    message(STATUS "Building with module vt100")
else()
    message(STATUS "Not building with module vt100")
endif()

if (STATIC_VT100)
    target_sources(arfminesweeper PRIVATE "vt100.c")
    target_compile_definitions(arfminesweeper PUBLIC FRONTEND_VT100)
    message(STATUS "Building with static vt100")
else()
    message(STATUS "Not building with static vt100")
endif()

