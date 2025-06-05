#include "chatroom.h"

//  标准库
#include <chrono>

//  Qt
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>

//  第三方依赖
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qobject.h>
#include <spdlog/spdlog.h>

//  项目内
#include "network.h"

chatroom& chatroom::instance() {
    static chatroom singleton;
    return singleton;
}

void chatroom::create_chatroom() {
    //  准备发送创建聊天室请求
    QJsonObject request_json;
    request_json["request_type"] = "create_chatroom";

    //  准备字节流数据
    auto raw_data = QJsonDocument{request_json}
                                        .toJson(QJsonDocument::Compact);

    spdlog::debug("准备发送创建聊天室请求: {}.", raw_data.toStdString());

    //  获取连接并向其发送请求
    auto& net = network::instance();
    auto bytes_sent = net.write(std::move(raw_data));

    spdlog::debug("请求已发送, 数据长度: {}.", bytes_sent);
}

void chatroom::process_request(QJsonObject request_json) {
    spdlog::debug("接收到创建聊天室请求返回数据: {}",
        QJsonDocument{request_json}.toJson(QJsonDocument::Compact).toStdString());

    auto request_type = request_json["request_type"].toString().toStdString();
    if (request_type != "create_chatroom") {
        spdlog::error("接收的请求类型错误！实际类型：{}", request_type);
        return;
    }

    m_id = request_json["uuid"].toString();
    emit chatroomIdReady();
}

QString chatroom::get_id() const {
    return m_id;
}

void chatroom::set_username(QString username) {
    m_username = std::move(username);
}

QString chatroom::get_username() const {
    return m_username;
}

void chatroom::send_message(const QString& message) {
    //  构建请求 json
    QJsonObject request_json;
    request_json["request_type"] = "message";
    request_json["room_id"] = m_id;
    request_json["username"] = m_username;
    request_json["message"] = message;

    //  将 json 转换为字节流
    auto raw_data = QJsonDocument{request_json}
                                    .toJson(QJsonDocument::Compact);


    auto& net = network::instance();
    auto bytes_sent = net.write(std::move(raw_data));

    spdlog::debug("聊天消息请求已发送, 数据长度: {}.", bytes_sent);
}

void chatroom::process_message(QJsonObject request_json) {
    spdlog::debug("正在处理聊天消息: {}",
        QJsonDocument{request_json}.toJson(QJsonDocument::Compact).toStdString());

    const auto& request_type = request_json["request_type"].toString().toStdString();
    if (request_type != "message") {
        spdlog::error("处理消息请求类型错误! 实际请求类型: {}.", request_type);
        return;
    }

    const auto& timestamp_count = request_json["timestamp"].toInteger();
    auto time_point = std::chrono::system_clock::time_point{
        std::chrono::floor<std::chrono::seconds>(
            std::chrono::nanoseconds(timestamp_count)
        )
    };

    spdlog::debug("当前时区: {}.", std::chrono::current_zone()->name());
    auto local_time_point = std::chrono::zoned_time("Asia/Shanghai", time_point);
    auto full_timestamp = std::format("{:%F %T}", local_time_point);
    auto timestamp = full_timestamp.substr(
        0, full_timestamp.find('.')
    );

    const auto& username = request_json["username"].toString();
    const auto& message = request_json["message"].toString();
    emit newMessage(QString::fromStdString(timestamp), username, message);
}

void chatroom::join_chatroom(const QString& chatroom_id) {
    QJsonObject request_json;
    request_json["request_type"] = "join_chatroom";

    request_json["uuid"] = chatroom_id;

    //  准备字节流数据
    auto raw_data = QJsonDocument{request_json}
                                        .toJson(QJsonDocument::Compact);

    spdlog::debug("准备发送加入聊天室请求: {}.", raw_data.toStdString());

    //  获取连接并向其发送请求
    auto& net = network::instance();
    auto bytes_sent = net.write(std::move(raw_data));

    spdlog::debug("请求已发送, 数据长度: {}.", bytes_sent);
}

void chatroom::join_reply(QJsonObject json_data) {
    spdlog::debug("正在处理加入回应请求: {}",
        QJsonDocument{json_data}.toJson(QJsonDocument::Compact).toStdString());

    auto join_result = json_data["result"].toString();
    if (join_result == "success") {
        m_id = json_data["uuid"].toString();
        emit joinSuccess(m_id);
    } else if (join_result == "failed") {
        auto message = json_data["reason"].toString();
        emit joinFailed(message);
    }
}