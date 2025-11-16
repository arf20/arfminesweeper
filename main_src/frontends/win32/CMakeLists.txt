if (WIN32)
    if (SHARED AND NOT NO_WIN32)

    else()
        message(STATUS "Not building with shared win32")
    endif()

    if (NOT NO_STATIC AND NOT NO_WIN32)
        target_sources(arfminesweeper PRIVATE "win32.c")
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_WIN32)
        message(STATUS "Building with static win32")
    else()
        message(STATUS "Not building with static win32")
    endif()
else()
    message(STATUS "No Win32 ComCtl support")
endif()

