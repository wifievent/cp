#include "weudpclient.h"

WEUdpClient::WEUdpClient()
{
}

WEUdpClient::~WEUdpClient()
{
}

bool WEUdpClient::searchProduct(int port, int sec, int millisec, string protocol)
{
    strncpy(sendbuf_, protocol.c_str(), protocol.length());
    setSocketBroadcastable();
    setRecvTimeout(sec, millisec);
    setSockAddrBroadcast(port);
    send(sendbuf_, strlen(sendbuf_) + 1);
    if(recv(recvbuf_, BUFSIZE) != -1)
    {
        if(strcmp(recvbuf_, "run already!") == 0)
        {
            qDebug() << "There is already wifievent product running...";
            return true;
        }
    }
    qDebug() << "There is no wifievent product...";
    return false;
}
