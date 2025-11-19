add_custom_command(
    OUTPUT "${PROJECT_SOURCE_DIR}/assets/awt.class" "${PROJECT_SOURCE_DIR}/main_src/frontends/javaawt/Backend.h"
    COMMAND "javac"
    ARGS "-h" "${PROJECT_SOURCE_DIR}/main_src/frontends/javaawt/" "-d" "${PROJECT_SOURCE_DIR}/assets/" "${PROJECT_SOURCE_DIR}/main_src/frontends/javaawt/awt.java"
    DEPENDS "${PROJECT_SOURCE_DIR}/main_src/frontends/javaawt/awt.java"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

if (JNI_FOUND AND JAVAC)
    find_program(JAVAC javac)
    if (MODULE_JAVAWT OR STATIC_JAVAAWT)
        add_custom_target(awt_class_java ALL
            DEPENDS "${PROJECT_SOURCE_DIR}/assets/awt.class"
        )
    endif()

    if (MODULE_JAVAAWT)
        add_library(javaawt MODULE "javaawt.c" "backend.c")
        target_compile_definitions(javaawt PUBLIC _MODULE_)
        target_include_directories(javaawt PUBLIC ${JNI_INCLUDE_DIRS})
        target_link_libraries(javaawt ${JNI_LIBRARIES})
        add_dependencies(javaawt awt_class_java)
        add_custom_target(symlink_java ALL
            COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
            DEPENDS javaawt module_dir)
        message(STATUS "Building with module javaawt")
    else()
        message(STATUS "Not building with module javaawt")
    endif()

    if (STATIC_JAVAAWT)
        target_sources(arfminesweeper PRIVATE "javaawt.c" "backend.c")
        target_include_directories(arfminesweeper PUBLIC ${JNI_INCLUDE_DIRS})
        target_link_libraries(arfminesweeper ${JNI_LIBRARIES})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_JAVAAWT)
        add_dependencies(arfminesweeper awt_class_java)
        message(STATUS "Building with static javaawt")
    else()
        message(STATUS "Not building with static javaawt")
    endif()
else()
    message(STATUS "No Java/JNI found")
endif()

