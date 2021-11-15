#pragma once
#include "udpsocket.h"

using namespace std;

class UdpClient : public UdpSocket
{
public:
    UdpClient();
    ~UdpClient();

public:
    bool setSocketBroadcastable();
    void setSockAddr(string ip, int port);
    void setRecvTimeout(int sec, int millisec);
};
