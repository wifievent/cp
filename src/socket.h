#pragma once

#include "udp.h"

#define BUFSIZE 65536

class Socket
{
public:
    Socket();
    virtual ~Socket() {}

    virtual int disconnect() = 0;
    virtual int send(char* buf, size_t len) = 0;
    virtual int recv(char* buf, size_t len) = 0;
};
