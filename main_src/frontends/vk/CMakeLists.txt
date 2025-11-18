add_custom_command(
    OUTPUT "${PROJECT_SOURCE_DIR}/assets/msboard4vs.spv"
    COMMAND "glslc"
    ARGS "${PROJECT_SOURCE_DIR}/main_src/frontends/vk/msboard4.vert" "-o" "${PROJECT_SOURCE_DIR}/assets/msboard4vs.spv"
    DEPENDS "${PROJECT_SOURCE_DIR}/main_src/frontends/vk/msboard4.vert"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_command(
    OUTPUT "${PROJECT_SOURCE_DIR}/assets/msboard4fs.spv"
    COMMAND "glslc"
    ARGS "${PROJECT_SOURCE_DIR}/main_src/frontends/vk/msboard4.frag" "-o" "${PROJECT_SOURCE_DIR}/assets/msboard4fs.spv"
    DEPENDS "${PROJECT_SOURCE_DIR}/main_src/frontends/vk/msboard4.frag"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

if (Vulkan_FOUND AND glfw3_FOUND)
    if (MODULE_VK)
        add_library(vk MODULE "vk.c" "../common/glad/glad.c")
        target_compile_definitions(vk PUBLIC _MODULE_)
        target_include_directories(vk PUBLIC "../common/glad/include/")
        target_link_libraries(vk Vulkan::Vulkan glfw)
        add_custom_target(symlink_vk ALL
            COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
            DEPENDS vk module_dir)
        message(STATUS "Building with module vk")
    else()
        message(STATUS "Not building with module vk")
    endif()

    if (STATIC_VK)
        target_sources(arfminesweeper PRIVATE "vk.c" "../common/glad/glad.c")
        target_include_directories(arfminesweeper PUBLIC "../common/glad/include/")
        target_link_libraries(arfminesweeper Vulkan::Vulkan)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_VK)
        add_custom_target(vulkan_shaders ALL
            DEPENDS
                "${PROJECT_SOURCE_DIR}/assets/msboard4vs.spv"
                "${PROJECT_SOURCE_DIR}/assets/msboard4fs.spv"
        )
        message(STATUS "Building with static vk")
    else()
        message(STATUS "Not building with static vk")
    endif()
else()
    message(STATUS "No Vulkan/GLFW3 found")
endif()

