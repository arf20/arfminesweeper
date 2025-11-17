add_custom_command(
    OUTPUT "${PROJECT_SOURCE_DIR}/assets/awt.class"
    COMMAND "javac"
    ARGS "-d" "${PROJECT_SOURCE_DIR}/assets/" "${PROJECT_SOURCE_DIR}/main_src/frontends/java/awt.java"
    DEPENDS "${PROJECT_SOURCE_DIR}/main_src/frontends/java/awt.java"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

if (JNI_FOUND AND JAVAC)
    if (MODULE AND NOT NO_JAVA)

    else()
        message(STATUS "Not building with module java")
    endif()

    if (NOT NO_STATIC AND NOT NO_JAVA)
        target_sources(arfminesweeper PRIVATE "java.c")
        target_include_directories(arfminesweeper PUBLIC ${JNI_INCLUDE_DIRS})
        target_link_libraries(arfminesweeper ${JNI_LIBRARIES})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_JAVA)
        add_custom_target(java_awt ALL
            DEPENDS "${PROJECT_SOURCE_DIR}/assets/awt.class"
        )
        message(STATUS "Building with static java")
    else()
        message(STATUS "Not building with static java")
    endif()
else()
    message(STATUS "No Java/JNI found")
endif()

