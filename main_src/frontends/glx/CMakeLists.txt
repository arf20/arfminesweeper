if (OpenGL_OpenGL_FOUND AND X11_FOUND)
    if (SHARED AND NOT NO_GLX)

    else()
        message(STATUS "Not building with shared glx")
    endif()

    if (NOT NO_STATIC AND NOT NO_GLX)
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

