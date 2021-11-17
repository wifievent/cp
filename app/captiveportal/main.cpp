#include "weudpclient.h"
#include "weudpserver.h"
#include "mainwindow.h"

const char* version()
{
    return
#include "../../version.txt"
#ifdef _DEBUG
            " Debug Build(" __DATE__ " " __TIME__ ")";
#else
            " Release Build(" __DATE__ " " __TIME__ ")";
#endif
}

int main(int argc, char *argv[])
{
    GApp a(argc, argv);
    WEUdpClient client;
    if(client.searchProduct(7284, 1, 0, "run already?"))
    {
        qDebug() << "Captive Portal stop thanks to using";
        return 0;
    }
    else
    {
        qInfo() << "CaptivePortal Started" << version();
    }

    WEUdpServer ws;
    ws.start(7284);

    QIcon icon(":/image/logo/logo1.ico"); // :/image == image.qrc
    a.setWindowIcon(icon);

    MainWindow w;
    if (!w.initCheck)
    {
        qDebug() << QString("NB Do not open");
    }
    else
    {
        w.show();
        a.exec();
    }
    ws.stop();
}
