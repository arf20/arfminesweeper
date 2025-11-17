if (FLTK_FOUND)
    if (MODULE AND NOT NO_FLTK)

    else()
        message(STATUS "Not building with module fltk")
    endif()

    if (NOT NO_STATIC AND NOT NO_FLTK)
        target_sources(arfminesweeper PRIVATE "fltk.cpp")
        target_link_libraries(arfminesweeper ${FLTK_LIBRARIES})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_FLTK)
        message(STATUS "Building with static fltk")
    else()
        message(STATUS "Not building with static fltk")
    endif()
else()
    message(STATUS "No FLTK found")
endif()

