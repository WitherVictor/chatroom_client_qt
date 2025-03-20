#include "network.h"

#include <QtNetwork>

#include <qtcpsocket.h>
#include <spdlog/spdlog.h>

#include "login.h"

network::network(QObject* parent)
    : QObject{parent} {}

void network::try_connect_to_server() {
    m_socket_ptr = std::make_unique<QTcpSocket>();
    m_socket_ptr->connectToHost("127.0.0.1", 4242);

    QObject::connect(m_socket_ptr.get(), &QTcpSocket::connected, [this]() {
        spdlog::info("服务器连接成功！");
        login_handler::instance().register_socket(std::move(m_socket_ptr));
        emit connectSuccess();
    });
}