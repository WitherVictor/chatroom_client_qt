#pragma once

#include <QObject>
#include <QtNetwork>

class login_handler : public QObject {
    Q_OBJECT
public:
    login_handler(const login_handler&) = delete;
    login_handler& operator=(const login_handler&) = delete;

    static login_handler& instance();
    Q_INVOKABLE void try_login(const QString&, const QString&);
    void register_socket(std::unique_ptr<QTcpSocket>);
private:
    login_handler(QObject* parent = nullptr)
        : QObject{parent} {}

    std::unique_ptr<QTcpSocket> m_socket_ptr;
};