if (WIN32)
    if (SHARED AND NOT NO_GDI)

    else()
        message(STATUS "Not building with shared gdi")
    endif()

    if (NOT STATIC AND NOT NO_GDI)
        target_sources(arfminesweeper PRIVATE "gdi.c")
        target_link_libraries(arfminesweeper msimg32)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_GDI)
        message(STATUS "Building with static gdi")
    else()
        message(STATUS "Not building with static gdi")
    endif()
else()
    message(STATUS "No Win32 GDI support")
endif()

