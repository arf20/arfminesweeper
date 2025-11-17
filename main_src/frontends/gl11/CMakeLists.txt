if (OpenGL_OpenGL_FOUND AND GLUT_FOUND)
    if (MODULE_GL11)

    else()
        message(STATUS "Not building with module gl11")
    endif()

    if (STATIC_GL11)
        target_sources(arfminesweeper PRIVATE "gl11.c" "../common/gl_ffp.c")
        target_link_libraries(arfminesweeper OpenGL::GL ${GLUT_LIBRARIES})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_GL11)
        message(STATUS "Building with static gl11")
    else()
        message(STATUS "Not building with static gl11")
    endif()
else()
    message(STATUS "No OpenGL/FreeGLUT found")
endif()

