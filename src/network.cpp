#include "network.h"

#include <QtNetwork>

#include <spdlog/spdlog.h>

network::network(QObject* parent)
    : QObject{parent},
    m_socket_ptr{std::make_unique<QTcpSocket>()},
    m_buffer{},

    m_write_mutex{}
{
    //  注册连接成功回调
    QObject::connect(m_socket_ptr.get(), &QTcpSocket::connected, [this]() {
        spdlog::info("服务器连接成功！");

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
    auto bytes_sent = m_socket_ptr->write(std::move(buffer));
    m_socket_ptr->flush();
    return bytes_sent;
}