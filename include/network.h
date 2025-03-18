#pragma once

#include <QObject>
#include <qtmetamacros.h>

class network : public QObject {
    Q_OBJECT
public:
    explicit network(QObject* parent = nullptr);
    Q_INVOKABLE void try_connect_to_server();

    virtual ~network() = default;
};