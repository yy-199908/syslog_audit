
#include "YAgent.h"
#include <iostream>
using namespace std;
int main(int argc,char* argv[])
{
    if (argc < 2)
    {
        cout << "please set ip" << endl;
        cout << "./agent 127.0.0.1" << endl;
        return -1;
    }
    //1.初始化agent 连接数据库
    if (!YAgent::Get()->Init(argv[1]))
    {
        cout << "Agent init failed!" << endl;
        return -1;
    }
    YAgent::Get()->Main();
    //读取日志文件
    

    cout << "Agent start!\n";
}

