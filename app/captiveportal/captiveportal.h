#pragma once

#include "stdafx.h"
#include "tcpblock.h"
#include "pcapdevice.h"
#include "ip.h"
#include "spdlog/spdlog.h"
#include "arpspoof.h"
#include "capture.h"
class CaptivePortal : public Obj
{
    //Q_OBJECT
    //Q_PROPERTY(QString intfname MEMBER intfname_)
    //Q_PROPERTY(QString redirectpage MEMBER redirectpage_)
    ArpSpoof capturer_;
    TcpBlock tcpblock_;
    PcapDevice writer_;
    std::thread recv_;
    Capture& capInstance = Capture::getInstance();
    Ip host_;
    Packet* packet;

public:
    std::string intfname_;
    std::string redirectpage_;
    Ip gwIp_;

public:
    CaptivePortal();
    ~CaptivePortal() override {close();};

private:
    void setComponent();

protected:
    bool doOpen() override;
    bool doClose() override;

    void processPacket(Packet* packet);
};
