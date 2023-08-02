#include "YClient.h"
#include "YYMysql.h"
#include <iostream>
using namespace std;
int main(int argc, char * argv[])
{
    string ip = "127.0.0.1";
    if (argc > 1) ip = argv[1];
    if (!YClient::Get()->Init(ip))
    {
        cout << "Client Init Failed！" << endl;
        return -1;
    }
    std::cout << "Client start!\n";
    YClient::Get()->Main();
    return 0;
}

