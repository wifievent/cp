#pragma once

#include "stdafx.h"

class CaptivePortal : public GStateObj
{
    Q_OBJECT

    GAutoArpSpoof arpspoof_;
    GTcpBlock tcpblock_;
    GPcapDeviceWrite writer_;

    string redirectpage_;
    GIp host_;

public:
    CaptivePortal(QString redirectpage);
    ~CaptivePortal() override {close();};

private:
    void setComponent();

protected:
    bool doOpen() override;
    bool doClose() override;

public slots:
    void processPacket(GPacket* packet);
};
