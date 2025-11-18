if (wxWidgets_FOUND)
    if (MODULE AND NOT NO_WXWIDGETS)

    else()
        message(STATUS "Not building with module wxwidgets")
    endif()

    if (NOT NO_STATIC AND NOT NO_WXWIDGETS)
        target_sources(arfminesweeper PRIVATE "wxwidgets.cpp")
        target_link_libraries(arfminesweeper ${wxWidgets_LIBRARIES})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_WXWIDGETS)
        message(STATUS "Building with static wxwidgets")
    else()
        message(STATUS "Not building with static wxwidgets")
    endif()
else()
    message(STATUS "No WXWIDGETS found")
endif()

