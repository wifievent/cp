#include "core.h"

int main()
{
    Core core;
    core.start();
    cout << "press any key to close"<<endl;
    string s;
    getline(cin,s);
    core.stop();
    return 0;
}
