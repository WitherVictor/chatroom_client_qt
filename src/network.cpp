#include "network.h"

#include <QtNetwork>

#include <spdlog/spdlog.h>

#include "login.h"

network::network(QObject* parent)
    : QObject{parent} {}

void network::try_connect_to_server() {
    auto socket_ptr = std::make_unique<QTcpSocket>();
    socket_ptr->connectToHost("127.0.0.1", 4242);

    if (socket_ptr->state() == QAbstractSocket::ConnectedState) {
        spdlog::info("服务器连接成功！");

        auto& login_processor = login_handler::get_instance();
        login_processor.register_socket(std::move(socket_ptr));
    } else {
        spdlog::error("服务器连接失败！");
        spdlog::error("错误信息：{}", socket_ptr->errorString().toStdString());
    }
}