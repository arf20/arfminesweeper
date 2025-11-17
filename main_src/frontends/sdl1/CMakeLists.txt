if (SDL_FOUND AND SDL_ttf_FOUND AND SDL_image_FOUND)
    if (MODULE_SDL1)

    else()
        message(STATUS "Not building with module sdl1")
    endif()

    if (STATIC_SDL1)
        target_sources(arfminesweeper PRIVATE "sdl1.c")
        target_link_libraries(arfminesweeper ${SDL_LIBRARIES} ${SDL_ttf_LIBRARIES} ${SDL_image_LIBRARIES})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_SDL1)
        message(STATUS "Building with static sdl1")
    else()
        message(STATUS "Not building with static sdl1")
    endif()
else()
    message(STATUS "No SDL (1.2) found (SDL, SDL_ttf, SDL_image)")
endif()

