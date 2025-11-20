add_custom_command(
    OUTPUT "${PROJECT_SOURCE_DIR}/assets/ArfBackend.class" "${PROJECT_SOURCE_DIR}/main_src/frontends/common/ArfBackend.h"
    COMMAND "javac"
    ARGS "-h" "${PROJECT_SOURCE_DIR}/main_src/frontends/common/" "-d" "${PROJECT_SOURCE_DIR}/assets/" "${PROJECT_SOURCE_DIR}/main_src/frontends/common/ArfBackend.java"
    DEPENDS "${PROJECT_SOURCE_DIR}/main_src/frontends/common/ArfBackend.java"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_command(
    OUTPUT "${BINDINGS_DIR}/libarfjavabindings.so"
    COMMAND "ln"
    ARGS "-s" "${CMAKE_CURRENT_BINARY_DIR}/*.so" "${BINDINGS_DIR}"
    DEPENDS arfjavabindings
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(module_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${MODULE_DIR})

add_custom_target(bindings_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BINDINGS_DIR})

if ((JNI_FOUND AND JAVAC) AND (MODULE_AWT OR STATIC_AWT OR MODULE_SWING OR STATIC_SWING))
    add_custom_target(java_class_arfbackend ALL DEPENDS "${PROJECT_SOURCE_DIR}/assets/ArfBackend.class")
    add_library(arfjavabindings MODULE "${PROJECT_SOURCE_DIR}/main_src/frontends/common/arfjavabindings.c")
    target_include_directories(arfjavabindings PUBLIC ${JNI_INCLUDE_DIRS})
    add_custom_target(symlink_arfjavabindings ALL DEPENDS bindings_dir "${BINDINGS_DIR}/libarfjavabindings.so")
    message(STATUS "Building with java bindings")
else()
    message(STATUS "Not building with java bindings")
endif()


