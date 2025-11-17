if (X11_FOUND AND EXISTS "/usr/include/forms.h")
    if (MODULE AND NOT NO_XFORMS)

    else()
        message(STATUS "Not building with module xforms")
    endif()

    if (NOT NO_STATIC AND NOT NO_XFORMS)
        target_sources(arfminesweeper PRIVATE "xforms.c")
        target_link_libraries(arfminesweeper forms)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_XFORMS)
        message(STATUS "Building with static xforms")
    else()
        message(STATUS "Not building with static xforms")
    endif()
else()
    message(STATUS "No XForms found")
endif()

