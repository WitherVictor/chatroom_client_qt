#pragma once

#include <QObject>

class network : public QObject {
    Q_OBJECT
public:
    explicit network(QObject* parent = nullptr);
    void try_connect_to_server();

    virtual ~network() = default;
};