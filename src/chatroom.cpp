#include "chatroom.h"

//  Qt
#include <QJsonObject>
#include <QJsonDocument>

//  第三方依赖
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
    raw_data.push_back(network::separator);
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