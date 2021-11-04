#include "captiveportal.h"

CaptivePortal::CaptivePortal()
{
    setComponent();

    QObject::connect(
                &arpspoof,
                SIGNAL(captured(GPacket*)),
                &filter1,
                SLOT(check(GPacket*)),
                Qt::DirectConnection
                );

    QObject::connect(
                &arpspoof,
                SIGNAL(captured(GPacket*)),
                &filter2,
                SLOT(check(GPacket*)),
                Qt::DirectConnection
                );

    QObject::connect(
                &filter1,
                SIGNAL(filtered(GPacket*)),
                &tcpblock,
                SLOT(block(GPacket*)),
                Qt::DirectConnection
                );

    QObject::connect(
                &filter2,
                SIGNAL(filtered(GPacket*)),
                &block,
                SLOT(block(GPacket*)),
                Qt::DirectConnection
                );
}

void CaptivePortal::setComponent()
{
    filter1.filter_ = "(port 80 and tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x47455420) && !(host 175.213.35.39)";
    filter2.filter_ = "tcp port 443";

    tcpblock.backwardRst_ = false;
    tcpblock.backwardFin_ = true;
    tcpblock.backwardFinMsg_ = QStringList{"HTTP/1.0 302 Redirected\r\nLocation: http://test.gilgil.net/ek.jpg\r\n\r\n"};
    tcpblock.writer_ = &writer;
}

bool CaptivePortal::doOpen()
{
    writer.open();
    filter1.open();
    filter2.open();
    block.open();
    tcpblock.open();
    arpspoof.open();

    return true;
}

bool CaptivePortal::doClose()
{
    writer.close();
    filter1.close();
    filter2.close();
    block.close();
    tcpblock.close();
    arpspoof.close();

    return true;
}

void CaptivePortal::checkPacket(GPacket *packet)
{

}

void CaptivePortal::blockPacket(GPacket *packet)
{

}
