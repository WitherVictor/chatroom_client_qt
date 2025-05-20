#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>

class signup : public QObject {
    Q_OBJECT
public:
    signup(const signup&) = delete;
    signup& operator=(const signup&) = delete;

    signup(signup&&) noexcept = delete;
    signup& operator=(signup&&) noexcept = delete;

    ~signup() override = default;

    Q_INVOKABLE void process_request(const QString&, const QString&);
    void process_reply(QByteArray);

    static signup& instance();
signals:
    void registerSuccess();
    void registerFailed();
private:
    signup() = default;
};