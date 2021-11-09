#include "stdafx.h"
#include "captiveportal.h"

int main(int argc, char *argv[])
{
    GApp a(argc, argv);

    //Json setting

    CaptivePortal* cp = new CaptivePortal("test.gilgil.net");
    cp->open();

    return a.exec();
}
