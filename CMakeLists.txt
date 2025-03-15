cmake_minimum_required(VERSION 3.16)

project(chatroom_client_qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.1
)
FetchContent_MakeAvailable(spdlog)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appchatroom_client_qt
    main.cpp
)

qt_add_qml_module(appchatroom_client_qt
    URI chatroom_client_qt
    VERSION 1.0
    QML_FILES
        Main.qml
        ui/Login.qml
        ui/Register.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appchatroom_client_qt PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appchatroom_client_qt
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE FALSE
)

target_include_directories(appchatroom_client_qt
    PRIVATE spdlog::headers
)

target_link_libraries(appchatroom_client_qt
    PRIVATE Qt6::Quick
    PRIVATE spdlog
)

include(GNUInstallDirs)
install(TARGETS appchatroom_client_qt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
