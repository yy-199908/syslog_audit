#include <iostream>
#include <string>
#include "YCenter.h"
using namespace std;

void Usage()
{
    cout << "============ Center Usage =============" << endl;
    cout << "./center install 127.0.0.1" << endl;
    cout << "./center add 192.168.234.100 fileserver1" << endl;
}


int main(int argc, char* argv[])
{
    string cmd;
    string ip;
    if (argc > 1) cmd = argv[1];
    YCenter::Get()->Init();
    //初始化系统
    if (cmd == "install")
    {
        if (argc < 3)
        {
            Usage();
            return -1;
        }

        ip = argv[2];
        cout << "Begin install center" << endl;
        if(!YCenter::Get()->Install(ip)) return -1;
        return 0;
    }
     if(cmd == "add")
    {

        if (argc < 4)
        {
            Usage();
            return -1;
        }
        cout << "add device " << endl;
        if (!YCenter::Get()->AddDevice(argv[2], argv[3]))
            return -1;
        return 0;
    }
     YCenter::Get()->Main();


    std::cout << "Center start!\n";
    return 0;
}

