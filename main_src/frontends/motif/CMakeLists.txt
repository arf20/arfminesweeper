if (X11_FOUND AND MOTIF_FOUND)
    if (SHARED AND NOT NO_MOTIF)

    else()
        message(STATUS "Not building with shared motif")
    endif()

    if (NOT STATIC AND NOT NO_MOTIF)
        target_sources(arfminesweeper PRIVATE "motif.c")
        target_link_libraries(arfminesweeper ${MOTIF_LIBRARIES} Xt)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_MOTIF)
        message(STATUS "Building with static motif")
    else()
        message(STATUS "Not building with static motif")
    endif()
else()
    message(STATUS "No MOTIF found")
endif()

