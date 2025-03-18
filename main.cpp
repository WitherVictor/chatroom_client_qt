//  QML
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

//  spdlog
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "network.h"

//  将 QML 的日志转发到全局函数的 spdlog 中调用
inline void qml_log_handler(QtMsgType log_level, const QMessageLogContext& log_context, const QString& msg) {
    auto message = msg.toStdString();

    switch (log_level) {
        case QtDebugMsg: {
            spdlog::debug(message);
            break;
        }
        case QtInfoMsg: {
            spdlog::info(message);
            break;
        }
        case QtWarningMsg: {
            spdlog::warn(message);
            break;
        }
        case QtFatalMsg: {
            spdlog::error(message);
            break;
        }
        case QtCriticalMsg: {
            spdlog::critical(message);
            break;
        }
        default: {
            spdlog::error("未知 Log 等级, 日志消息为: {}", message);
        }
    }
}

int main(int argc, char *argv[])
{
    //  设置文件输出并绑定到默认 logger 上
    spdlog::set_level(spdlog::level::debug);
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/client.log");
    spdlog::default_logger()->sinks().push_back(file_sink);

    //  将 QML 中 console.log() 打印的日志绑定到全局处理函数上
    qInstallMessageHandler(qml_log_handler);

    network network_handler;

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
    engine.rootContext()->setContextProperty("network", &network_handler);

    return app.exec();
}