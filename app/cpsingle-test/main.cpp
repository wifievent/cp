#include <QCoreApplication>
#include <GBpFilter>
#include <GAutoArpSpoof>
#include <GTcpBlock>
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

    //GBpFilter filter;
    //filter.filter_ = "port 80 and tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x47455420";

    GTcpBlock tcpblock;
    tcpblock.backwardRst_ = false;
    tcpblock.backwardFin_ = true;
    tcpblock.backwardFinMsg_ = QStringList{"HTTP/1.0 302 Redirected\r\nLocation: http://wifievent.io\r\n\r\n"};

    QObject::connect(
                &arpspoof,
                &GCapture::captured,
                &tcpblock,
                &GTcpBlock::block
                );

    tcpblock.open();
    arpspoof.open();

    return a.exec();
}
