#pragma once

#include "stdafx.h"

class CaptivePortal : public GStateObj
{
    Q_OBJECT

    GAutoArpSpoof arpspoof;
    GPcapDeviceWrite writer; //delete and make it code
    GBpFilter filter1; //delete and make it code
    GBpFilter filter2; //delete and make it code
    GTcpBlock tcpblock; //delete and make it code
    GBlock block; //delete and make it code

public:
    CaptivePortal();
    ~CaptivePortal() override {};

private:
    void setComponent();

protected:
    bool doOpen() override;
    bool doClose() override;

public slots:
    void checkPacket(GPacket* packet);
    void blockPacket(GPacket* packet);

signals:
    void filtered80(GPacket* packet);
    void filtered443(GPacket* pakcet);
};
