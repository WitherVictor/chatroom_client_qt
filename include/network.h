#pragma once

#include <QObject>
#include <QtNetwork>
#include <QByteArray>

#include <concepts>
#include <mutex>

class network : public QObject {
    Q_OBJECT
public:
    explicit network(QObject* parent = nullptr);
    virtual ~network() = default;

    //  单例禁用复制构造函数
    network(const network&) = delete;
    network& operator=(const network&) = delete;

    //  返回单例对象
    static network& instance();

    //  尝试连接服务器
    Q_INVOKABLE void try_connect_to_server();

    //  数据处理函数
    QByteArray read_raw_data();

    //  写入数据与返回数据
    qint64 write(QByteArray);

    template <std::invocable Func>
    qint64 write(QByteArray buffer, Func&& callback) {
        //  注册信号
        auto conn_ptr = std::make_shared<QMetaObject::Connection>();
        *conn_ptr = QObject::connect(m_socket_ptr.get(), &QTcpSocket::readyRead, [this, conn_ptr, callback]() {
                        callback();
                        QObject::disconnect(*conn_ptr);
                    });

        //  写入时独占锁
        std::unique_lock lock{m_write_mutex};
        auto bytes_sent = m_socket_ptr->write(std::move(buffer));
        m_socket_ptr->flush();
        return bytes_sent;
    }

    inline static constexpr char separator = '\x1E';
signals:
    void connected();
    void disconnected();
private:
    //  socket 指针与缓冲区
    std::unique_ptr<QTcpSocket> m_socket_ptr;
    QByteArray m_buffer;

    mutable std::mutex m_write_mutex;   //  socket 写入锁
};