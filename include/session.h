#pragma once

#include <memory>

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class session : public QObject {
    Q_OBJECT
public:
    explicit session(std::unique_ptr<QTcpSocket>, QObject* = nullptr);

    qint64 write(QByteArray);

    inline static constexpr char separator = '\x1E';
public slots:
    void data_received();
signals:
    void data_ready(const QByteArray&);
private:
    QByteArray m_buffer;
    std::unique_ptr<QTcpSocket> m_socket_ptr;
};