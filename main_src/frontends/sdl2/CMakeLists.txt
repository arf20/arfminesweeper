if (SDL2_FOUND AND SDL2_ttf_FOUND AND SDL2_image_FOUND)
    if (SHARED AND NOT NO_SDL2)

    else()
        message(STATUS "Not building with shared sdl2")
    endif()

    if (NOT STATIC AND NOT NO_SDL2)
        target_sources(arfminesweeper PRIVATE "sdl2.c")
        target_link_libraries(arfminesweeper ${SDL2_LIBRARIES} ${SDL2_ttf_LIBRARIES} ${SDL2_image_LIBRARIES})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_SDL2)
        message(STATUS "Building with static sdl2")
    else()
        message(STATUS "Not building with static sdl2")
    endif()
else()
    message(STATUS "No SDL2 found (SDL, SDL2_ttf, SDL2_image)")
endif()

