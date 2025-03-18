#pragma once

#include <QtNetwork>

class login_handler {
public:
    login_handler(const login_handler&) = delete;
    login_handler& operator=(const login_handler&) = delete;

    static login_handler& get_instance();
    void try_login();
    void register_socket(std::unique_ptr<QTcpSocket>);

private:
    login_handler() = default;

    std::unique_ptr<QTcpSocket> m_socket_ptr;
};