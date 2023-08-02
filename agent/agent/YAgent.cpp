#include "YAgent.h"
#include <iostream>
#include <thread>
#include <string.h>.
#include  <ifaddrs.h>
#include <arpa/inet.h>
#define LOGPATH "/var/log/auth.log"
using namespace std;
using namespace YY;

bool YAgent::Init(string ip)
{
	
	if (ip.empty())
	{
		cerr << "Agent Init Failed! ip is empty!" << endl;
		return false;
	}
	my=new YYMysql();
	if (!my->Connect(ip.c_str(), "root", "123456", "yy",3306,0))
	{
		cerr << "Agent Init Failed! " << endl;
		return false;
	}


	//读取日志文件
	fp = fopen(LOGPATH,"rb");
	if (!fp)
	{
		cerr << "Agent Init Failed! open logfile failed!" << endl;
		return false;
	}
	//只审计审计系统运行时间之后的日志
	//文件移动到结尾，从结尾处开始审计之后更新的内容
	fseek(fp, 0, SEEK_END);


	return true;
}

string YAgent::GetLocalIp()
{
	char ip[16] = { 0 };
	ifaddrs* ifadd = 0;
	if (getifaddrs(&ifadd) != 0)
		return ip;
	ifaddrs* it = ifadd;
	while (it != NULL)
	{
		//ipv4
		if (it->ifa_addr->sa_family == AF_INET)
		{
			if (strcmp(it->ifa_name, "lo") != 0)//去掉本机回环地址
			{
				//转换整形ip为字符串
				
				void* tmp = &((sockaddr_in*)it->ifa_addr)->sin_addr;
				inet_ntop(AF_INET,tmp,ip,INET_ADDRSTRLEN);
				break;
			}
			
		}
		it = it->ifa_next;
		
	}
	freeifaddrs(ifadd);
	return ip;
	
}

bool YAgent::SaveLog(std::string log)
{
	cout << log;
	YDATA data;
	data["log"] = log.c_str();
	string ip=GetLocalIp();//获取本机ip地址
	data["ip"] = ip;
	//YYMysql将sql语句中加了@字段的值的引号自动去除
	data["@time"] = "now()";//插入时间
	if (my->Insert(data, "t_log") == 0)
		return false;
	return true;
}

void YAgent::Main()
{
	//读取一行
	string row;
	while (1)
	{

		char c = fgetc(fp);
		if (c <= 0)//没有字符或者读到结尾
		{
			this_thread::sleep_for(100ms);
			continue;
		}
		if (c == '\n')
		{
			cout << row << endl;
			SaveLog(row);
			row = "";
			continue;
		}
		row += c;
		

	}
}


YAgent::YAgent()
{

}
YAgent::~YAgent()
{

}
