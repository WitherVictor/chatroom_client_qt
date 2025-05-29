#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QJsonObject>

class chatroom : public QObject {
    Q_OBJECT
public:
    chatroom(const chatroom&) = delete;
    chatroom& operator=(const chatroom&) = delete;

    ~chatroom() override = default;

    static chatroom& instance();

    Q_INVOKABLE void create_chatroom();
    Q_INVOKABLE QString get_id() const;
    Q_INVOKABLE void set_username(QString);
    Q_INVOKABLE void send_message(const QString&);

    void process_request(QJsonObject);
    void process_message(QJsonObject);
signals:
    void chatroomIdReady();
    void newMessage(QString timestamp, QString username, QString message);
private:
    chatroom() = default;
    
    QString m_id;
    QString m_username;
};