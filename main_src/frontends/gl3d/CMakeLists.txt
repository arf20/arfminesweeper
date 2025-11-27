if (OpenGL_OpenGL_FOUND AND glfw3_FOUND AND CGLM_FOUND)
    if (MODULE_GL3D)
        add_library(arfminesweeper-gl3d MODULE "gl3d.c" "../common/glutil.c"
            "glutil3d.c" "models.c" "../common/glad/glad.c")
        target_compile_definitions(arfminesweeper-gl3d PUBLIC _MODULE_)
        target_include_directories(arfminesweeper-gl3d PUBLIC "../common/glad/include/" CGLM_INCLUDE_DIRS)
        target_link_libraries(arfminesweeper-gl3d OpenGL::GL glfw dl cglm m)
        add_custom_target(symlink_arfminesweeper-gl3d ALL
            COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
            DEPENDS arfminesweeper-gl3d module_dir)
        message(STATUS "Building with module gl3d")
    else()
        message(STATUS "Not building with module gl3d")
    endif()

    if (STATIC_GL3D)
        target_sources(arfminesweeper PRIVATE "gl3d.c" "../common/glutil.c"
            "glutil3d.c" "models.c" "../common/glad/glad.c")
        target_include_directories(arfminesweeper PUBLIC "../common/glad/include/" CGLM_INCLUDE_DIRS)
        target_link_libraries(arfminesweeper OpenGL::GL glfw dl cglm m)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_GL3D)
        message(STATUS "Building with static gl3d")
    else()
        message(STATUS "Not building with static gl3d")
    endif()
else()
    message(STATUS "No OpenGL/GLFW or cglm found")
endif()

