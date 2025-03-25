cmake_minimum_required(VERSION 3.18)
project(chatroom_client_qt VERSION 0.1 LANGUAGES CXX)

# 导出编译指令供 clangd 使用
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置 C++ 标准为 23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 导入依赖 spdlog 作为日志系统
include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.1
)
FetchContent_MakeAvailable(spdlog)

# 开始声明 Qt 项目
find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.8)

# 必须手动设置头文件路径供 MOC 查找
set(PROJECT_HEADERS
    include/login.h
    include/network.h
    include/session.h
)

# 必须手动设置源文件路径供 MOC 查找
set(PROJECT_SOURCES
    src/login.cpp
    src/network.cpp
    src/session.cpp
)

# 将头文件和源文件添加到项目内
qt_add_executable(appchatroom_client_qt
    main.cpp
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

# 添加 QML 文件和使用的资源
qt_add_qml_module(appchatroom_client_qt
    URI chatroom_client_qt
    VERSION 1.0
    QML_FILES
        Main.qml
        ui/Login.qml
        ui/Register.qml
        ui/WaitConnection.qml
    RESOURCES
        icon/loading-50px.gif
)

# 为项目添加头文件路径
target_include_directories(appchatroom_client_qt
    PRIVATE
        include
        spdlog::headers
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

# 链接 spdlog 和 Qt Quick
target_link_libraries(appchatroom_client_qt
    PRIVATE
        Qt6::Quick
        spdlog
)

include(GNUInstallDirs)
install(TARGETS appchatroom_client_qt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
