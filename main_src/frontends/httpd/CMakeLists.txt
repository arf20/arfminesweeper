if (MODULE_HTTPD)
    add_library(httpd MODULE "httpd.c")
    target_compile_definitions(httpd PUBLIC _MODULE_)
    add_custom_target(symlink_httpd ALL
        COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
        DEPENDS httpd module_dir)
    message(STATUS "Building with module httpd")
else()
    message(STATUS "Not building with dynamic httpd")
endif()

if (STATIC_HTTPD)
    target_sources(arfminesweeper PRIVATE "httpd.c")
    target_compile_definitions(arfminesweeper PUBLIC FRONTEND_HTTPD)
    if (WIN32)
        target_link_libraries(arfminesweeper ws2_32)
    endif()
    message(STATUS "Building with static httpd")
else()
    message(STATUS "Not building with static httpd")
endif()

