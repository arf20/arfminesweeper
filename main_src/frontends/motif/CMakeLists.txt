if (X11_FOUND AND MOTIF_FOUND)
    if (MODULE_MOTIF)

    else()
        message(STATUS "Not building with module motif")
    endif()

    if (STATIC_MOTIF)
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

