#include <QCoreApplication>
#include <GAutoArpSpoof>
#include <GBpFilter>
#include <GTcpBlock>
#include <GBlock>
#include <GPcapDeviceWrite>
#include <GApp>
#include "myobj.h"

int main(int argc, char *argv[])
{
    GApp a(argc, argv);

    MyObj myobj1;
    MyObj myobj2;
    MyObj myobj3;

    GAutoArpSpoof arpspoof;
    /*
    //X
    QObject::connect(
                &arpspoof,
                SIGNAL(GCapture::captured),
                &myobj1,
                SLOT(processCaptured1),
                Qt::DirectConnection
                );*/
    //X

//    QObject::connect(
//                &arpspoof,
//                SIGNAL(captured(GPacket*)),
//                &myobj1,
//                SLOT(processCaptured1(GPacket*)),
//                Qt::DirectConnection
//                );
    /*
    //O
    QObject::connect(
                &arpspoof,
                &GCapture::captured,
                &myobj1,
                &MyObj::processCaptured1,
                Qt::DirectConnection
                );
    */
    /*
    GPcapDeviceWrite writer;

    GBpFilter filter1;
    filter1.filter_ = "(port 80 and tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x47455420) && !(host 175.213.35.39)";

    GBpFilter filter2;
    filter2.filter_ = "tcp port 443";

    GTcpBlock tcpblock;
    tcpblock.backwardRst_ = false;
    tcpblock.backwardFin_ = true;
    tcpblock.backwardFinMsg_ = QStringList{"HTTP/1.0 302 Redirected\r\nLocation: http://test.gilgil.net/ek.jpg\r\n\r\n"};
    tcpblock.writer_ = &writer;

    GBlock block;

    QObject::connect(
                &arpspoof,
                SIGNAL(captured),
                &filter1,
                SLOT(check)
                );

    QObject::connect(
                &arpspoof,
                SIGNAL(captured),
                &filter2,
                SLOT(check)
                );

    QObject::connect(
                &filter1,
                &GBpFilter::filtered,
                &tcpblock,
                &GTcpBlock::block
                );

    QObject::connect(
                &filter2,
                &GBpFilter::filtered,
                &block,
                &GBlock::block
                );

    arpspoof.open();
    filter1.open();
    filter2.open();
    block.open();
    tcpblock.open();
    writer.open();
    */

    arpspoof.open();
    return a.exec();
}
