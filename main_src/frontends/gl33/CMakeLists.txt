if (OpenGL_OpenGL_FOUND AND glfw3_FOUND)
    if (MODULE_GL33)
        add_library(gl33 MODULE "gl33.c" "../common/glutil.c"
            "../common/glad/glad.c")
        target_compile_definitions(gl33 PUBLIC _MODULE_)
        target_include_directories(gl33 PUBLIC "../common/glad/include/")
        target_link_libraries(gl33 OpenGL::GL glfw dl)
        add_custom_target(symlink_gl33 ALL
            COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
            DEPENDS gl33 module_dir)
        message(STATUS "Building with module gl33")
    else()
        message(STATUS "Not building with module gl33")
    endif()

    if (STATIC_GL33)
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

