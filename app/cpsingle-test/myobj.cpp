#include "myobj.h"

MyObj::MyObj()
{
}

void MyObj::processCaptured(GPacket* packet)
{
    qDebug() << packet->buf_.size_;
}
