add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c"
    COMMAND Wayland::Scanner
    ARGS "private-code" "<" "${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml" ">" "xdg-shell-protocol.c"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.h"
    COMMAND Wayland::Scanner
    ARGS "client-header" "<" "${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml" ">" "xdg-shell-client-protocol.h"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/xdg-decoration-v1.c"
    COMMAND Wayland::Scanner
    ARGS "private-code" "<" "${WaylandProtocols_DATADIR}/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml" ">" "xdg-decoration-v1.c"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/xdg-decoration-v1-client.h"
    COMMAND Wayland::Scanner
    ARGS "client-header" "<" "${WaylandProtocols_DATADIR}/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml" ">" "xdg-decoration-v1-client.h"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

file(GLOB WAYLAND_SRC
    "wayland.c"
    "wl_util.c"
    "xdg-shell-protocol.c"
    "../common/fb.c"
    "../common/stb_image_impl.c"
)

set(PROTOCOL_SRC
    "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c"
    "${CMAKE_CURRENT_BINARY_DIR}/xdg-decoration-v1.c"
)

if (WAYLAND_FOUND AND WaylandScanner_FOUND AND WaylandProtocols_FOUND)
    add_custom_target(wayland-protocol-xdg-shell ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-protocol.c")
    add_custom_target(wayland-protocol-xdg-shell-client ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.h")
    add_custom_target(wayland-protocol-xdg-decoration-v1 ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/xdg-decoration-v1.c")
    add_custom_target(wayland-protocol-xdg-decoration-v1-client ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/xdg-decoration-v1-client.h")
    set_source_files_properties(${PROTOCOL_SRC} PROPERTIES GENERATED TRUE)

    add_library(arfminesweeper-wl-protocols STATIC ${PROTOCOL_SRC})
    add_dependencies(arfminesweeper-wl-protocols wayland-protocol-xdg-shell wayland-protocol-xdg-shell-client wayland-protocol-xdg-decoration-v1 wayland-protocol-xdg-decoration-v1-client)

    if (MODULE_WAYLAND)
        add_library(arfminesweeper-wayland MODULE ${WAYLAND_SRC} ${PROTOCOL_SRC})
        target_compile_definitions(arfminesweeper-wayland PUBLIC _MODULE_)
        target_link_libraries(arfminesweeper-wayland wayland-client m arfminesweeper-wl-protocols)
        target_include_directories(arfminesweeper-wayland PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
        add_custom_target(symlink_arfminesweeper-wayland ALL
            COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
            DEPENDS arfminesweeper-wayland module_dir)
        message(STATUS "Building with module wayland")
    else()
        message(STATUS "Not building with module wayland")
    endif()

    if (STATIC_WAYLAND)
        target_sources(arfminesweeper PRIVATE ${WAYLAND_SRC} ${PROTOCOL_SRC})
        target_link_libraries(arfminesweeper wayland-client m arfminesweeper-wl-protocols)
        target_include_directories(arfminesweeper PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_WAYLAND)
        message(STATUS "Building with static wayland")
    else()
        message(STATUS "Not building with static wayland")
    endif()

    # this is stupid, cant figure out how not to repeat it
    #ecm_add_wayland_client_protocol(
    #    arfminesweeper-wayland
    #    PROTOCOL ${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml
    #    BASENAME xdg-shell
    #)
    #ecm_add_wayland_client_protocol(
    #    arfminesweeper
    #    PROTOCOL ${WaylandProtocols_DATADIR}/stable/xdg-shell/xdg-shell.xml
    #    BASENAME xdg-shell
    #)
    #ecm_add_wayland_client_protocol(
    #    arfminesweeper-wayland
    #    BASENAME
    #    xdg-decoration-v1
    #    PROTOCOL
    #    ${WaylandProtocols_DATADIR}/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml
    #)
    #ecm_add_wayland_client_protocol(
    #    arfminesweeper
    #    BASENAME
    #    xdg-decoration-v1
    #    PROTOCOL
    #    ${WaylandProtocols_DATADIR}/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml
    #)

else()
    message(STATUS "No Wayland, Extra CMake Modules, WaylandScanner or WaylandProtocols found")
endif()

