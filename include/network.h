#pragma once

#include <QObject>
#include <QtNetwork>

class network : public QObject {
    Q_OBJECT
public:
    explicit network(QObject* parent = nullptr);
    Q_INVOKABLE void try_connect_to_server();

    virtual ~network() = default;
signals:
    void connectSuccess();
private:
    std::unique_ptr<QTcpSocket> m_socket_ptr;
};