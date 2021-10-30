#pragma once

#include <QObject>
#include <GPacket>

class MyObj : public QObject
{
    Q_OBJECT

public:
    MyObj();
    ~MyObj() override {};

public slots:
    void capture(GPacket* packet);
};
