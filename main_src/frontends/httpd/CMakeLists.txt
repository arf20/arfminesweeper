if (SHARED AND NOT NO_HTTPD)

else()
    message(STATUS "Not building with dynamic httpd")
endif()

if (NOT STATIC AND NOT NO_HTTPD)
    target_sources(arfminesweeper PRIVATE "httpd.c")
    target_link_libraries(arfminesweeper Threads::Threads)
    target_compile_definitions(arfminesweeper PUBLIC FRONTEND_HTTPD)
    if (WIN32)
        target_link_libraries(arfminesweeper ws2_32)
    endif()
    message(STATUS "Building with static httpd")
else()
    message(STATUS "Not building with static httpd")
endif()

