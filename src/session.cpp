#include <spdlog/spdlog.h>

#include <QJsonObject>
#include <QJsonDocument>

#include "session.h"
#include "login.h"

session::session(std::unique_ptr<QTcpSocket> socket_ptr, QObject* parent)
    : QObject{parent}, m_buffer{}, m_socket_ptr{std::move(socket_ptr)}
{
    QObject::connect(m_socket_ptr.get(), &QTcpSocket::readyRead, this, &session::data_received);
    QObject::connect(this, &session::data_ready, &login_handler::instance(), &login_handler::process_login_request_reply);
}

qint64 session::write(QByteArray buffer) {
    auto bytes_sent = m_socket_ptr->write(buffer);
    m_socket_ptr->flush();
    return bytes_sent;
}

void session::data_received() {
    m_buffer += m_socket_ptr->readAll();
    qsizetype separator_index = m_buffer.indexOf(session::separator);
    if (separator_index != -1) {
        auto result_byte_array = m_buffer.left(separator_index);
        m_buffer = m_buffer.mid(separator_index + 1);
        emit data_ready(result_byte_array);
    }
}