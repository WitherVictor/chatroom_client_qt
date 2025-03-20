#include "login.h"
#include "spdlog/spdlog.h"
#include <qjsondocument.h>
#include <qjsonobject.h>

login_handler& login_handler::instance() {
    static login_handler instance;
    return instance;
}

void login_handler::try_login(const QString& username, const QString& password) {
    QJsonObject request_json{};
    request_json["request_type"] = "login";
    request_json["username"] = username;
    request_json["password"] = password;

    spdlog::info("发起登录请求，用户名：{}，密码：{}", username.toStdString(), password.toStdString());
    m_socket_ptr->write(QJsonDocument{request_json}.toJson(QJsonDocument::Compact));
}

void login_handler::register_socket(std::unique_ptr<QTcpSocket> socket_ptr) {
    m_socket_ptr = std::move(socket_ptr);
}