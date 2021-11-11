#pragma once
#include "stdafx.h"

#define BUF_SIZE 8192

class Socket
{
public:
    Socket();
    virtual ~Socket() {};

public:
    virtual int disconnect() = 0;
    virtual int send(char* buf, size_t len) = 0;
    virtual int recv(char* buf, size_t len) = 0;
};
