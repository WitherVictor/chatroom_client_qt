//  C++
#include <memory>

//  Qt
#include <QString>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>

//  spdlog
#include <spdlog/spdlog.h>

//  项目内头文件
#include "login.h"
#include "network.h"
#include "qstring_formatter.hpp"

//  返回 login 的单例对象
login& login::instance() {
    static login instance;
    return instance;
}

void login::try_login(const QString& username, const QString& password) {
    //  准备请求数据
    QJsonObject request_json{};
    request_json["request_type"] = "login";
    request_json["username"] = username;
    request_json["password"] = password;

    spdlog::debug(fmt::format("发起登录请求，用户名：{}，密码：{}", username, password));

    //  将请求数据转换为字节流，并在末尾插入数据分隔符。
    auto raw_data = QJsonDocument{request_json}.toJson(QJsonDocument::Compact);

    //  发送数据
    spdlog::debug("准备发送登录请求数据：{}", raw_data.toStdString());
    auto& net = network::instance();
    auto bytes_sent = net.write(std::move(raw_data));

    spdlog::debug("数据已发送，写入数据长度：{}", bytes_sent);
}

void login::process_request(QJsonObject request_json) {
    spdlog::debug("接收到登录请求的回应数据：{}",
        QJsonDocument{request_json}.toJson(QJsonDocument::Compact).toStdString());

    auto request_type = request_json["request_type"].toString().toStdString();
    if (request_type != "login") {
        spdlog::error("接收的请求类型错误！实际类型：{}", request_type);
        return;
    }

    auto request_result = request_json["result"].toString().toStdString();
    if (request_result == "passed") {
        spdlog::debug("用户登陆成功！");
        emit loginSuccess();
    } else {
        spdlog::debug("用户登录失败！原因：{}", request_json["reason"].toString());
        emit loginFailed();
    }
}