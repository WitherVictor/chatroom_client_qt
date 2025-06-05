#pragma once

#include <memory>

#include <QObject>
#include <QtNetwork>
#include <QByteArray>

class login : public QObject {
    Q_OBJECT
public:
    login(const login&) = delete;
    login& operator=(const login&) = delete;

    static login& instance();
    Q_INVOKABLE void try_login(const QString&, const QString&);
    void register_socket(std::unique_ptr<QTcpSocket>);
public slots:
    void process_request(QJsonObject);
signals:
    void loginSuccess();
    void loginFailed(QString);
private:
    login(QObject* parent = nullptr)
        : QObject{parent} {}
};