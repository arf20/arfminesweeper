if (OpenGL_OpenGL_FOUND AND glfw3_FOUND)
    if (MODULE AND NOT NO_GL33)

    else()
        message(STATUS "Not building with module gl33")
    endif()

    if (NOT NO_STATIC AND NOT NO_GL33)
        target_sources(arfminesweeper PRIVATE "gl33.c" "../common/glutil.c"
            "../common/glad/glad.c")
        target_include_directories(arfminesweeper PUBLIC "../common/glad/include/")
        target_link_libraries(arfminesweeper OpenGL::GL glfw dl)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_GL33)
        message(STATUS "Building with static gl33")
    else()
        message(STATUS "Not building with static gl33")
    endif()
else()
    message(STATUS "No OpenGL/GLFW found")
endif()

