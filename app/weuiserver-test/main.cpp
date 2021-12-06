#include "weuiserver.h"

int main()
{
    WEUIServer uiserver;
    uiserver.rootdir_ = "/home/kali/cp/app/weuiserver-test/webui";
    uiserver.start(80);
    uiserver.stop();
    return 0;
}
