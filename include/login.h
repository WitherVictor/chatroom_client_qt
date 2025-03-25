#pragma once

#include <memory>

#include <QObject>
#include <QtNetwork>
#include <QByteArray>

#include "session.h"

class login_handler : public QObject {
    Q_OBJECT
public:
    login_handler(const login_handler&) = delete;
    login_handler& operator=(const login_handler&) = delete;

    static login_handler& instance();
    Q_INVOKABLE void try_login(const QString&, const QString&);
    void register_socket(std::unique_ptr<QTcpSocket>);
public slots:
    void process_login_request_reply(const QByteArray&);
signals:
    void loginSuccess();
    void loginFailed();
private:
    login_handler(QObject* parent = nullptr)
        : QObject{parent} {}

    std::unique_ptr<session> m_session_ptr;
};