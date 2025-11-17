if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (MODULE AND NOT NO_FBDEV)

    else()
        message(STATUS "Not building with module fbdev")
    endif()

    if (NOT NO_STATIC AND NOT NO_FBDEV)
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

