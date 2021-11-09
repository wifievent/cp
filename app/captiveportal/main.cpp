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
    qDebug() << "CaptivePortal Started" << version();
    GApp a(argc, argv);

//    QIcon icon(":/image/logo/logo.ico");
//    a.setWindowIcon(icon);

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
}
