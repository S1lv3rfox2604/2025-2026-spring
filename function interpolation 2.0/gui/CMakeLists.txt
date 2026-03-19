find_package(Qt5 REQUIRED Widgets)

set(CMAKE_AUTOMOC ON)

add_library(gui
    mainwindow.cpp
    plotwidget.cpp
)

target_link_libraries(gui
    Qt5::Widgets
    input
    solver
    output
)

target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})