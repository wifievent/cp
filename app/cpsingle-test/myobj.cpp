#include "myobj.h"
#include <string.h>

MyObj::MyObj()
{
}

void MyObj::processCaptured1(GPacket* packet)
{
    qDebug() << packet->buf_.size_;
    const char* data = reinterpret_cast<const char*>(packet->tcpData_.data_);
    if(packet->tcpData_.data_ == nullptr)
        return;
    if(packet->tcpData_.size_ < 4)
        return;
    if(strncmp(data, "GET ", 4) != 0)
        return;
}

void MyObj::processCaptured2(GPacket *packet)
{
    qDebug() << "filter1: ";
}

void MyObj::processCaptured3(GPacket *packet)
{
    qDebug() << "filter2: ";
}
