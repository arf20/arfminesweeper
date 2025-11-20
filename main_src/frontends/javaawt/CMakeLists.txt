add_custom_command(
    OUTPUT "${PROJECT_SOURCE_DIR}/assets/javaawt.class"
    COMMAND "javac"
    ARGS "-cp" ".:${PROJECT_SOURCE_DIR}/main_src/frontends/common/" "-d" "${PROJECT_SOURCE_DIR}/assets/" "${PROJECT_SOURCE_DIR}/main_src/frontends/javaawt/javaawt.java"
    DEPENDS "${PROJECT_SOURCE_DIR}/main_src/frontends/javaawt/javaawt.java"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

if (JNI_FOUND AND JAVAC)
    find_program(JAVAC javac)
    if (MODULE_JAVAAWT OR STATIC_JAVAAWT)
        add_custom_target(java_class_javaawt ALL DEPENDS "${PROJECT_SOURCE_DIR}/assets/javaawt.class" symlink_arfjavabindings)
    endif()

    if (MODULE_JAVAAWT)
        add_library(javaawt MODULE "javaawt.c")
        target_compile_definitions(javaawt PUBLIC _MODULE_)
        target_include_directories(javaawt PUBLIC ${JNI_INCLUDE_DIRS})
        target_link_libraries(javaawt ${JNI_LIBRARIES} dl)
        add_dependencies(javaawt java_class_arfbackend java_class_javaawt)
        add_custom_target(symlink_javaawt ALL
            COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
            DEPENDS javaawt module_dir
        )
        message(STATUS "Building with module javaawt")
    else()
        message(STATUS "Not building with module javaawt")
    endif()

    if (STATIC_JAVAAWT)
        target_sources(arfminesweeper PRIVATE "javaawt.c")
        target_include_directories(arfminesweeper PUBLIC ${JNI_INCLUDE_DIRS})
        target_link_libraries(arfminesweeper ${JNI_LIBRARIES} dl)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_JAVAAWT)
        add_dependencies(arfminesweeper java_class_arfbackend java_class_javaawt)
        message(STATUS "Building with static javaawt")
    else()
        message(STATUS "Not building with static javaawt")
    endif()
else()
    message(STATUS "No Java/JNI found")
endif()

