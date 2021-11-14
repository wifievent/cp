#pragma once
#include "udp.h"
#include "udpclient.h"

class WEUdpClient : public UdpClient
{
    char sendbuf_[BUFSIZE];
    char recvbuf_[BUFSIZE];

public:
    WEUdpClient();
    ~WEUdpClient();

public:
    bool searchProduct(int port, int sec, int millisec, string protocol);
};
