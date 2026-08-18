cmake_minimum_required(VERSION 3.16)
project(audio_player_live VERSION 0.2 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

option(ENABLE_BLUETOOTH_DISCOVERY "Build in-app Bluetooth scan/pair support (needs Qt6::Bluetooth)" OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

set(APP_SOURCES
    main.cpp
    MediaPlayerBackend.h
    MediaPlayerBackend.cpp
    AudioOutputModel.h
    AudioOutputModel.cpp
    MediaLibraryModel.h
    MediaLibraryModel.cpp
    UsbVolumeWatcher.h
    UsbVolumeWatcher.cpp
)

set(APP_QML_FILES
    Main.qml
    qml/Theme.js
    qml/IconGlyph.qml
    qml/SplashScreen.qml
    qml/HomeScreen.qml
    qml/PlayerScreen.qml
    qml/PlayerControls.qml
    qml/SourceSelector.qml
    qml/PlaylistPanel.qml
    qml/BluetoothPanel.qml
)

qt_add_executable(appaudio_player_live
    ${APP_SOURCES}
)

if(ENABLE_BLUETOOTH_DISCOVERY)
    find_package(Qt6 REQUIRED COMPONENTS Bluetooth)
    target_sources(appaudio_player_live PRIVATE BluetoothManager.h BluetoothManager.cpp)
    target_compile_definitions(appaudio_player_live PRIVATE APP_WITH_BLUETOOTH_DISCOVERY)
    target_link_libraries(appaudio_player_live PRIVATE Qt6::Bluetooth)
endif()

qt_add_qml_module(appaudio_player_live
    URI audio_player_live
    QML_FILES
        ${APP_QML_FILES}
        RESOURCES assets.qrc
)

set_target_properties(appaudio_player_live PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appaudio_player_live
    PRIVATE Qt6::Quick Qt6::Multimedia Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS appaudio_player_live
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
