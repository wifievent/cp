#include <QCoreApplication>
#include <GPcapDevice>
#include <GAutoArpSpoof>
#include "myobj.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GAutoArpSpoof capturer;
    //capturer.intfName_ = "eth0";
    //Open component

    return a.exec();
}
