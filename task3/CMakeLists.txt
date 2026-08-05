cmake_minimum_required(VERSION 3.16)

project(SmartHomeDashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(SmartHomeDashboard
    main.cpp
)

qt_add_qml_module(SmartHomeDashboard
    URI smart_home
    VERSION 1.0
    QML_FILES
        Main.qml
        LoginPage.qml
        DashboardPage.qml
        SettingsPage.qml
    RESOURCES
        images/home.svg
        images/light.svg
        images/fan.svg
        images/ac.svg
        images/garage.svg
        images/user.svg
)

set_target_properties(SmartHomeDashboard PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(SmartHomeDashboard
    PRIVATE Qt6::Quick Qt6::QuickControls2
)
