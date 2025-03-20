#include "login.h"

login_handler& login_handler::instance() {
    static login_handler instance;
    return instance;
}

void login_handler::try_login() {}

void login_handler::register_socket(std::unique_ptr<QTcpSocket> socket_ptr) {
    m_socket_ptr = std::move(socket_ptr);
}