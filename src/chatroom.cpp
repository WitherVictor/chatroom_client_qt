#include "chatroom.h"

//  Qt
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>

//  第三方依赖
#include <chrono>
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
    auto bytes_sent = net.write(std::move(raw_data), [this, &net]() {
        auto data = net.read_raw_data();
        process_request(data);
    });

    spdlog::debug("请求已发送, 数据长度: {}.", bytes_sent);
}

void chatroom::process_request(QByteArray raw_data) {
    spdlog::debug("接收到创建聊天室请求返回数据: {}", raw_data.toStdString());

    auto request_json = QJsonDocument::fromJson(raw_data).object();

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
    auto bytes_sent = net.write(std::move(raw_data), [this, &net]() {
        auto data = net.read_raw_data();
        process_message(data);
    });

    spdlog::debug("聊天消息请求已发送, 数据长度: {}.", bytes_sent);
}

void chatroom::process_message(QByteArray raw_data) {
    const auto& json_data = QJsonDocument::fromJson(raw_data).object();

    const auto& request_type = json_data["request_type"].toString().toStdString();
    if (request_type != "message") {
        spdlog::error("处理消息请求类型错误! 实际请求类型: {}.", request_type);
        return;
    }

    const auto& timestamp_count = json_data["timestamp"].toInteger();
    auto time_point = std::chrono::system_clock::time_point{
        std::chrono::nanoseconds(timestamp_count)
    };

    spdlog::debug("当前时区: {}.", std::chrono::current_zone()->name());
    auto local_time_point = std::chrono::zoned_time("Asia/Shanghai", time_point);
    const auto& timestamp = std::format("{:%Y-%m-%d %H:%M:%S}", local_time_point);

    const auto& username = json_data["username"].toString();
    const auto& message = json_data["message"].toString();
    emit newMessage(QString::fromStdString(timestamp), username, message);
}