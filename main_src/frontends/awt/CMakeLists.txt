add_custom_command(
    OUTPUT "${PROJECT_SOURCE_DIR}/assets/awt.class"
    COMMAND "javac"
    ARGS "-cp" ".:${PROJECT_SOURCE_DIR}/main_src/frontends/common/" "-d" "${PROJECT_SOURCE_DIR}/assets/" "${PROJECT_SOURCE_DIR}/main_src/frontends/awt/awt.java"
    DEPENDS "${PROJECT_SOURCE_DIR}/main_src/frontends/awt/awt.java"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

if (JNI_FOUND AND JAVAC)
    find_program(JAVAC javac)
    if (MODULE_AWT OR STATIC_AWT)
        add_custom_target(java_class_awt ALL DEPENDS "${PROJECT_SOURCE_DIR}/assets/awt.class" symlink_arfjavabindings)
    endif()

    if (MODULE_AWT)
        add_library(awt MODULE "awt.c")
        target_compile_definitions(awt PUBLIC _MODULE_)
        target_include_directories(awt PUBLIC ${JNI_INCLUDE_DIRS})
        target_link_libraries(awt ${JNI_LIBRARIES} dl)
        add_dependencies(awt java_class_arfbackend java_class_awt)
        add_custom_target(symlink_awt ALL
            COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
            DEPENDS awt module_dir
        )
        message(STATUS "Building with module awt")
    else()
        message(STATUS "Not building with module awt")
    endif()

    if (STATIC_AWT)
        target_sources(arfminesweeper PRIVATE "awt.c")
        target_include_directories(arfminesweeper PUBLIC ${JNI_INCLUDE_DIRS})
        target_link_libraries(arfminesweeper ${JNI_LIBRARIES} dl)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_AWT)
        add_dependencies(arfminesweeper java_class_arfbackend java_class_awt)
        message(STATUS "Building with static awt")
    else()
        message(STATUS "Not building with static awt")
    endif()
else()
    message(STATUS "No Java/JNI found")
endif()

