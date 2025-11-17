if (OpenGL_OpenGL_FOUND AND X11_FOUND)
    if (MODULE_GLX)

    else()
        message(STATUS "Not building with module glx")
    endif()

    if (STATIC_GLX)
        target_sources(arfminesweeper PRIVATE "glx.c" "../common/gl_ffp.c")
        target_link_libraries(arfminesweeper OpenGL::GL ${X11_LIBRARIES})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_GLX)
        message(STATUS "Building with static glx")
    else()
        message(STATUS "Not building with static glx")
    endif()
else()
    message(STATUS "No OpenGL/Xlib found")
endif()

