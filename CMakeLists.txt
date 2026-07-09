cmake_minimum_required(VERSION 3.16)

project(Thief60Seconds VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appThief60Seconds
    main.cpp
    src/gameengine.h
    src/gameengine.cpp

    # Core
    src/core/GameMap.h
    src/core/GameMap.cpp
    src/core/TimeManager.h
    src/core/TimeManager.cpp

    # Entities
    src/entities/Character.h
    src/entities/Character.cpp

    # Objects
    src/objects/InteractableObject.h
    src/objects/InteractableObject.cpp
    src/objects/DoorObject.h
    src/objects/DoorObject.cpp
    src/objects/SafeObject.h
    src/objects/SafeObject.cpp
    src/objects/CarObject.h
    src/objects/CarObject.cpp
)

qt_add_qml_module(appThief60Seconds
    URI Thief60Seconds
    VERSION 1.0
    QML_FILES
        qml/entities/Main.qml
        qml/entities/Player.qml
        qml/entities/Wall.qml
        qml/entities/Door.qml
        qml/entities/Car.qml
        qml/entities/Safe.qml
        qml/components/GameGrid.qml
        qml/components/GameHeader.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appThief60Seconds PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appThief60Seconds
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appThief60Seconds
    PRIVATE Qt6::Quick
)

target_include_directories(appThief60Seconds PRIVATE src)

include(GNUInstallDirs)
install(TARGETS appThief60Seconds
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
