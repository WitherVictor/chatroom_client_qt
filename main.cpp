#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <spdlog/spdlog.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("chatroom_client_qt", "Main");

    spdlog::info("初始化完成，开始执行窗口。");

    return app.exec();
}
