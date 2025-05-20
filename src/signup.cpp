#include "signup.h"

#include <QString>
#include <QJsonObject>

#include <qjsondocument.h>
#include <qstringview.h>
#include <spdlog/spdlog.h>

#include "network.h"
#include "qstring_formatter.hpp"

signup& signup::instance() {
    static signup singleton;
    return singleton;
}

void signup::process_request(const QString& username, const QString& password) {
    QJsonObject request_json;
    request_json["request_type"] = "register";
    request_json["username"] = username;
    request_json["password"] = password;

    spdlog::debug("发起注册请求，用户名: {}, 密码: {}.",
        username, password    
    );

    auto raw_data = QJsonDocument{request_json}
                                    .toJson(QJsonDocument::Compact);

    raw_data.push_back(network::separator);

    spdlog::debug("发起注册请求, 数据: {}", raw_data.toStdString());
    auto& net = network::instance();
    auto bytes_sent = net.write(std::move(raw_data), [this, &net]() {
        auto data = net.read_raw_data();
        process_reply(std::move(data));
    });
    spdlog::debug("数据已发送, 数据长度: {}", bytes_sent);
}

void signup::process_reply(QByteArray raw_data) {
    spdlog::debug("接收到注册请求的回应数据: {}", raw_data.toStdString());

    auto request_json = QJsonDocument::fromJson(raw_data).object();
    auto request_type = request_json["request_type"].toString().toStdString();
    if (request_type != "register") {
        spdlog::error("请求类型错误! 实际类型: {}", request_type);
        return;
    }

    auto request_result = request_json["result"].toString().toStdString();
    if (request_result == "passed") {
        spdlog::debug("注册成功, 发送注册成功信号.");
        emit registerSuccess();
    } else {
        spdlog::debug("注册失败! 原因: {}.", request_json["reason"].toString());
        emit registerFailed();
    }
}