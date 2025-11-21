add_custom_command(
    OUTPUT "${PROJECT_SOURCE_DIR}/assets/swing.class"
    COMMAND "javac"
    ARGS "-cp" ".:${PROJECT_SOURCE_DIR}/main_src/frontends/common/" "-d" "${PROJECT_SOURCE_DIR}/assets/" "${PROJECT_SOURCE_DIR}/main_src/frontends/swing/swing.java"
    DEPENDS "${PROJECT_SOURCE_DIR}/main_src/frontends/swing/swing.java"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

if (JNI_FOUND AND JAVAC)
    find_program(JAVAC javac)
    if (MODULE_SWING OR STATIC_SWING)
        add_custom_target(java_class_swing ALL DEPENDS "${PROJECT_SOURCE_DIR}/assets/swing.class" symlink_arfjavabindings)
    endif()

    if (MODULE_SWING)
        add_library(arfminesweeper-swing MODULE "swing.c")
        target_compile_definitions(arfminesweeper-swing PUBLIC _MODULE_)
        target_include_directories(arfminesweeper-swing PUBLIC ${JNI_INCLUDE_DIRS})
        target_link_libraries(arfminesweeper-swing ${JNI_LIBRARIES} dl)
        add_dependencies(arfminesweeper-swing java_class_arfbackend java_class_swing)
        add_custom_target(symlink_arfminesweeper-swing ALL
            COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
            DEPENDS arfminesweeper-swing module_dir
        )
        message(STATUS "Building with module swing")
    else()
        message(STATUS "Not building with module swing")
    endif()

    if (STATIC_SWING)
        target_sources(arfminesweeper PRIVATE "swing.c")
        target_include_directories(arfminesweeper PUBLIC ${JNI_INCLUDE_DIRS})
        target_link_libraries(arfminesweeper ${JNI_LIBRARIES} dl)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_SWING)
        add_dependencies(arfminesweeper java_class_arfbackend java_class_swing)
        message(STATUS "Building with static swing")
    else()
        message(STATUS "Not building with static swing")
    endif()
else()
    message(STATUS "No Java/JNI found")
endif()

