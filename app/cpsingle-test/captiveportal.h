#pragma once

#include "stdafx.h"

using namespace std;

class CaptivePortal : public GStateObj
{
    GAutoArpSpoof capturer_;
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
