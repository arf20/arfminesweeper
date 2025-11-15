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
    if (SHARED AND NOT NO_VK)

    else()
        message(STATUS "Not building with shared vk")
    endif()

    if (NOT STATIC AND NOT NO_VK)
        target_sources(arfminesweeper PRIVATE "vk.c")
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

