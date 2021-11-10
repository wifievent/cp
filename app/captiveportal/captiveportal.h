#pragma once

#include "stdafx.h"

using namespace std;

class CaptivePortal : public GStateObj
{
    Q_OBJECT

    GAutoArpSpoof capturer_;
    GTcpBlock tcpblock_;
    GPcapDeviceWrite writer_;
    GIp host_;

public:
    QString intfname_;
    QString redirectpage_;

public:
    CaptivePortal(QWidget *parent = nullptr);
    ~CaptivePortal() override {close();};

private:
    void setComponent();

protected:
    bool doOpen() override;
    bool doClose() override;

public:
    void propLoad(QJsonObject jo) override;
    void propSave(QJsonObject& jo) override;

public slots:
    void processPacket(GPacket* packet);
};
