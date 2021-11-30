#pragma once

#include "stdafx.h"
#include "tcpblock.h"
#include "pcapdevice.h"
#include "ip.h"
#include "spdlog/spdlog.h"
class CaptivePortal : public Obj
{
    Q_OBJECT
    Q_PROPERTY(QString intfname MEMBER intfname_)
    Q_PROPERTY(QString redirectpage MEMBER redirectpage_)

    GAutoArpSpoof capturer_;
    TcpBlock tcpblock_;
    PcapDevice writer_;

    Ip host_;

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

public:
    //void propLoad(QJsonObject jo) override;
    //void propSave(QJsonObject& jo) override;

public slots:
    void processPacket(Packet* packet);
};
