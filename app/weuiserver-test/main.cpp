#include "weuiserver.h"

int main()
{
    WEUIServer uiserver;
    uiserver.start(80);
    uiserver.stop();
    return 0;
}
