#pragma once
#include "socket.h"

class UdpSocket : public Socket
{
protected:
    int sock_{0};
    struct sockaddr_in sockAddr_;

public:
    UdpSocket();
    ~UdpSocket() override;

    int send(char *buf, size_t len) override;
    int recv(char* buf, size_t len) override;
    int disconnect() override;
};
