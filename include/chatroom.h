#pragma once

#include <QObject>
#include <QByteArray>

class chatroom : public QObject {
    Q_OBJECT
public:
    chatroom(const chatroom&) = delete;
    chatroom& operator=(const chatroom&) = delete;

    ~chatroom() override = default;

    static chatroom& instance();

    Q_INVOKABLE void create_chatroom();
    Q_INVOKABLE QString get_id() const;
    void process_request(QByteArray);
signals:
    void chatroomIdReady();
private:
    chatroom() = default;
    
    QString m_id;
};