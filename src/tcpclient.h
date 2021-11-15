#pragma once
#include "stdafx.h"
#include "tcpsocket.h"

using namespace std;

class TcpClient : public TcpSocket //tcp
{


public:
    //how about get TcpSocket* tcpsock_ for arg
    TcpClient();
    ~TcpClient();

public:
    int connect(string ip, int port);
};
