if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (SHARED AND NOT NO_FBDEV)

    else()
        message(STATUS "Not building with shared fbdev")
    endif()

    if (NOT STATIC AND NOT NO_FBDEV)
        target_sources(arfminesweeper PRIVATE "fbdev.c" "../common/fb.c")
        target_link_libraries(arfminesweeper m)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_FBDEV)
        message(STATUS "Building with static fbdev")
    else()
        message(STATUS "Not building with static fbdev")
    endif()
else()
    message(STATUS "No fbdev support in non-Linux")
endif()

