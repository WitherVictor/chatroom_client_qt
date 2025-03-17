#pragma once

#include <QtMessageHandler>
#include <QString>

#include <spdlog/spdlog.h>

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