if (WIN32)
    if (MODULE_WIN32)
        add_library(win32 MODULE "win32.c")
        target_compile_definitions(win32 PUBLIC _MODULE_)
        add_custom_target(symlink_win32 ALL
            COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
            DEPENDS win32 module_dir)
        message(STATUS "Building with module win32")
    else()
        message(STATUS "Not building with module win32")
    endif()

    if (STATIC_WIN32)
        target_sources(arfminesweeper PRIVATE "win32.c")
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_WIN32)
        message(STATUS "Building with static win32")
    else()
        message(STATUS "Not building with static win32")
    endif()
else()
    message(STATUS "No Win32 ComCtl support")
endif()

