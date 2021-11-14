#pragma once
#include "udp.h"
#include "udpclient.h"

class CpUdpClient : public UdpClient
{
    char sendbuf_[BUFSIZE];
    char recvbuf_[BUFSIZE];

public:
    CpUdpClient();
    ~CpUdpClient();

public:
    bool searchProduct(int port, int sec, int millisec, string protocol);
};
