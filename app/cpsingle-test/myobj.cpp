#include "myobj.h"

MyObj::MyObj()
{
}

void MyObj::capture(GPacket* packet)
{
    qDebug() << packet->buf_.data_;
}
