#include "network.h"

#include <QTcpSocket>
#include <QByteArray>

#include <spdlog/spdlog.h>

#include "login.h"
#include "signup.h"
#include "chatroom.h"

network::network(QObject* parent)
    : QObject{parent},
    m_socket_ptr{std::make_unique<QTcpSocket>()}
{
    //  注册连接成功回调
    QObject::connect(m_socket_ptr.get(), &QTcpSocket::connected, [this]() {
        spdlog::debug("服务器连接成功！");

        emit connected();
    });

    //  注册链接失败回调
    QObject::connect(m_socket_ptr.get(), &QTcpSocket::disconnected, [this]() {
        spdlog::error("服务器已断开连接！");

        emit disconnected();
    });

    //  数据准备被读取时通知
    QObject::connect(m_socket_ptr.get(), &QTcpSocket::readyRead, [this]() {
        m_buffer += m_socket_ptr->readAll();

        auto data = read_raw_data();
        dispatch_request(std::move(data));
    });
}

network& network::instance() {
    static network singleton;
    return singleton;
}

void network::try_connect_to_server() {
    m_socket_ptr->connectToHost("127.0.0.1", 4242);
}

QByteArray network::read_raw_data() {
    auto separator_index = m_buffer.indexOf(network::separator);
    if (separator_index != -1) {
        auto result_byte_array = m_buffer.left(separator_index);
        m_buffer = m_buffer.mid(separator_index + 1);
        return result_byte_array;
    }
}

qint64 network::write(QByteArray buffer) {
    //  写入时独占锁
    std::unique_lock lock{m_write_mutex};
    buffer.push_back(network::separator);
    auto bytes_sent = m_socket_ptr->write(std::move(buffer));
    m_socket_ptr->flush();
    return bytes_sent;
}

void network::dispatch_request(QByteArray raw_data) {
    spdlog::debug("开始分发请求: {}", raw_data.toStdString());

    auto json_data = QJsonDocument::fromJson(raw_data).object();
    const auto& request_type = json_data["request_type"].toString().toStdString();

    if (request_type == "login") {
        login::instance().process_request(std::move(json_data));
    } else if (request_type == "register") {
        signup::instance().process_request(std::move(json_data));
    } else if (request_type == "create_chatroom") {
        chatroom::instance().process_request(std::move(json_data));
    } else if (request_type == "message") {
        chatroom::instance().process_message(std::move(json_data));
    } else if (request_type == "join_chatroom") {
        chatroom::instance().join_reply(std::move(json_data));
    } else if (request_type == "update_usercount") {
        chatroom::instance().update_usercount(std::move(json_data));
    } else {
        spdlog::error("未知请求类型, 实际类型: {}", request_type);
    }
}