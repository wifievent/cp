#include "tcpsocket.h"

TcpSocket::TcpSocket()
{
    if((sock_=socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        qDebug() << "server socket create failed";
    }
}

TcpSocket::~TcpSocket()
{
    disconnect();
}

int TcpSocket::send(char *buf, size_t len)
{
    return ::send(sock_, buf, len, 0);
    //success sent data length, fail -1
}

int TcpSocket::recv(char *buf, size_t len)
{
    int recv_len;
    memset(buf, '\0', len);
    recv_len = ::recv(this->sock_, buf, len, 0);
    if(recv_len==-1)
    {
        qDebug() << "Can't recv data";
    }
    return recv_len;
}

int TcpSocket::disconnect()
{
    int result = 0;
    if(sock_ != 0)
    {
        ::shutdown(sock_, SHUT_RDWR);
        result = ::close(sock_);
        sock_ = 0;
    }
    return result;
    //success 0, fail -1
}
