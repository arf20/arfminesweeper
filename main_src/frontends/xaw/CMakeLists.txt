if (X11_FOUND AND XAW_FOUND)
    if (MODULE AND NOT NO_XAW)

    else()
        message(STATUS "Not building with module xaw")
    endif()

    if (NOT NO_STATIC AND NOT NO_XAW)
        target_sources(arfminesweeper PRIVATE "xaw.c")
        target_link_libraries(arfminesweeper ${XAW_LIBRARIES})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_XAW)
        message(STATUS "Building with static xaw")
    else()
        message(STATUS "Not building with static xaw")
    endif()
else()
    message(STATUS "No xaw found")
endif()

