//  QML
#include <QGuiApplication>
#include <QQmlApplicationEngine>

//  spdlog
#include <qlogging.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

//  绑定 QML 日志输出到 spdlog
#include <backend/bind_qml_logger.hpp>

int main(int argc, char *argv[])
{
    //  设置文件输出并绑定到默认 logger 上
    spdlog::set_level(spdlog::level::debug);
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/client.log");
    spdlog::default_logger()->sinks().push_back(file_sink);

    //  将 QML 中 console.log() 打印的日志绑定到全局处理函数上
    qInstallMessageHandler(qml_log_handler);

    //  启动 QML 应用
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("chatroom_client_qt", "Main");

    return app.exec();
}
