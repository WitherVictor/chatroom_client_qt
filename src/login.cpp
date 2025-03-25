//  C++
#include <memory>

//  Qt
#include <QString>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>

//  spdlog
#include <qjsondocument.h>
#include <spdlog/spdlog.h>

//  项目内头文件
#include "login.h"
#include "session.h"
#include "qstring_formatter.hpp"

//  返回 login_handler 的单例对象
login_handler& login_handler::instance() {
    static login_handler instance;
    return instance;
}

void login_handler::try_login(const QString& username, const QString& password) {
    QJsonObject request_json{};
    request_json["request_type"] = "login";
    request_json["username"] = username;
    request_json["password"] = password;

    spdlog::info(fmt::format("发起登录请求，用户名：{}，密码：{}", username, password));

    auto raw_data = QJsonDocument{request_json}.toJson(QJsonDocument::Compact);
    constexpr char separator = '\x1e';
    raw_data.push_back(separator);

    spdlog::info("准备发送登录请求，数据长度：{}", raw_data.size());
    auto bytes_sent = m_session_ptr->write(std::move(raw_data));
    spdlog::info("数据已发送，写入数据长度：{}", bytes_sent);
}

void login_handler::process_login_request_reply(const QByteArray& data) {
    auto request_json = QJsonDocument::fromJson(data).object();
    if (request_json["request_type"] != "login") {
        spdlog::warn("接收的请求类型错误！实际类型：{}", request_json["request_type"].toString());
        return;
    }

    if (request_json["result"] == "passed") {
        spdlog::info("用户登陆成功！");
        emit loginSuccess();
    } else {
        spdlog::info("用户登录失败！");
        emit loginFailed();
    }
}

void login_handler::register_socket(std::unique_ptr<QTcpSocket> socket_ptr) {
    m_session_ptr = std::make_unique<session>(std::move(socket_ptr));
}