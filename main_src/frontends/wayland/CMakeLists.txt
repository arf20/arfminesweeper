file(GLOB WAYLAND_SOURCES
    "wayland.c"
    "wl_util.c"
    "xdg-shell-protocol.c"
    "../common/fb.c"
    "../common/stb_image_impl.c"
)

if (WaylandScanner_FOUND AND WaylandProtocols_FOUND)
    ecm_add_wayland_client_protocol(
        WAYLAND_SOURCES
        PROTOCOL ${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml
        BASENAME xdg-shell
    )
    message(STATUS "Generating Wayland protocol files")
else()
    message(STATUS "Not generating Wayland protocol files")
endif()

if (WAYLAND_FOUND AND LIBDECOR_FOUND)
    if (MODULE_WAYLAND)
        add_library(arfminesweeper-wayland MODULE ${WAYLAND_SOURCES})
        target_compile_definitions(arfminesweeper-wayland PUBLIC _MODULE_)
        target_link_libraries(arfminesweeper-wayland wayland-client decor-0 m)
        target_include_directories(arfminesweeper-wayland PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${LIBDECOR_INCLUDE_DIRS})
        add_custom_target(symlink_arfminesweeper-wayland ALL
            COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
            DEPENDS arfminesweeper-wayland module_dir)
        message(STATUS "Building with module wayland")
    else()
        message(STATUS "Not building with module wayland")
    endif()

    if (STATIC_WAYLAND)
        target_sources(arfminesweeper PRIVATE ${WAYLAND_SOURCES})
        target_link_libraries(arfminesweeper wayland-client decor-0 m)
        target_include_directories(arfminesweeper-wayland PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${LIBDECOR_INCLUDE_DIRS})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_WAYLAND)
        message(STATUS "Building with static wayland")
    else()
        message(STATUS "Not building with static wayland")
    endif()
else()
    message(STATUS "No Wayland or libdecor found")
endif()

