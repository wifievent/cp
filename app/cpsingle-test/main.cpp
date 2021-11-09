#include "stdafx.h"
#include "captiveportal.h"

int main(int argc, char *argv[])
{
    GApp a(argc, argv);

    CaptivePortal* cp = new CaptivePortal("test.gilgil.net");

    return a.exec();
}
