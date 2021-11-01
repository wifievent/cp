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
    void processCaptured1(GPacket* packet);
    void processCaptured2(GPacket* packet);
    void processCaptured3(GPacket *packet);
};
