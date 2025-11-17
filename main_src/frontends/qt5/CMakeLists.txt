if (Qt5Widgets_FOUND)
    if (MODULE AND NOT NO_QT5)

    else()
        message(STATUS "Not building with module qt5")
    endif()

    if (NOT NO_STATIC AND NOT NO_QT5)
        target_sources(arfminesweeper PRIVATE "qt5.cpp" "qt5.hpp")
        target_include_directories(arfminesweeper PUBLIC ${Qt5Widgets_INCLUDES})
        add_definitions(${Qt5Widgets_DEFINITIONS})
        target_link_libraries(arfminesweeper Qt5::Widgets)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTOMOC ON)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            AUTOMOC TRUE
            QT_VERSION_MAJOR ${QT_VERSION_MAJOR}
            QT_VERSION_MINOR ${QT_VERSION_MINOR}
        )
        set(CMAKE_GLOBAL_AUTOGEN_TARGET ON)
        set(CMAKE_AUTORCC ON)
        set(CMAKE_CXX_STANDARD 11)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        target_compile_definitions(arfminesweeper PUBLIC FRONTEND_QT5)
        message(STATUS "Building with static qt5 ${QT_VERSION_MAJOR}")
    else()
        message(STATUS "Not building with static qt5")
    endif()
else()
    message(STATUS "No Wayland found")
endif()

