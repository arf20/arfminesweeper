if (Qt5Widgets_FOUND)
    if (MODULE_QT5)
        add_library(arfminesweeper-qt5 MODULE "qt5.cpp")
        target_compile_definitions(arfminesweeper-qt5 PUBLIC _MODULE_)
        target_include_directories(arfminesweeper-qt5 PUBLIC ${Qt5Widgets_INCLUDES})
        add_definitions(${Qt5Widgets_DEFINITIONS})
        target_link_libraries(arfminesweeper-qt5 Qt5::Widgets)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTOMOC ON)
        set_target_properties(arfminesweeper-qt5 PROPERTIES
            AUTOMOC TRUE
            QT_VERSION_MAJOR ${QT_VERSION_MAJOR}
            QT_VERSION_MINOR ${QT_VERSION_MINOR}
        )
        set(CMAKE_GLOBAL_AUTOGEN_TARGET ON)
        set(CMAKE_AUTORCC ON)
        set(CMAKE_CXX_STANDARD 11)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        add_custom_target(symlink_arfminesweeper-qt5 ALL
            COMMAND ln -fs "${CMAKE_CURRENT_BINARY_DIR}/*.so" ${MODULE_DIR}
            DEPENDS arfminesweeper-qt5 module_dir)
        message(STATUS "Building with module qt5")
    else()
        message(STATUS "Not building with module qt5")
    endif()

    if (STATIC_QT5)
        target_sources(arfminesweeper PRIVATE "qt5.cpp" "qt5.hpp")
        target_include_directories(arfminesweeper PUBLIC ${Qt5Widgets_INCLUDES})
        add_definitions(${Qt5Widgets_DEFINITIONS})
        target_link_libraries(arfminesweeper Qt5::Widgets)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
        set(CMAKE_INCLUDE_CURRENT_DIR ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTOMOC ON)
        set_target_properties(arfminesweeper PROPERTIES
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

