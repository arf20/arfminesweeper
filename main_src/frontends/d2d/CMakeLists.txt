if (WIN32)
    if (SHARED AND NOT NO_D2D)

    else()
        message(STATUS "Not building with shared d2d")
    endif()

    if (NOT NO_STATIC AND NOT NO_D2D)
        target_sources(arfminesweeper PRIVATE "d2d.c")
        target_link_libraries(arfminesweeper D2d1)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_D2D)
        message(STATUS "Building with static d2d")
    else()
        message(STATUS "Not building with static d2d")
    endif()
else()
    message(STATUS "No Win32 Direct2D support")
endif()

