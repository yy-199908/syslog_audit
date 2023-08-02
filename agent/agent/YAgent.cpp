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


	//��ȡ��־�ļ�
	fp = fopen(LOGPATH,"rb");
	if (!fp)
	{
		cerr << "Agent Init Failed! open logfile failed!" << endl;
		return false;
	}
	//ֻ������ϵͳ����ʱ��֮�����־
	//�ļ��ƶ�����β���ӽ�β����ʼ���֮����µ�����
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
			if (strcmp(it->ifa_name, "lo") != 0)//ȥ�������ػ���ַ
			{
				//ת������ipΪ�ַ���
				
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
	string ip=GetLocalIp();//��ȡ����ip��ַ
	data["ip"] = ip;
	//YYMysql��sql����м���@�ֶε�ֵ�������Զ�ȥ��
	data["@time"] = "now()";//����ʱ��
	if (my->Insert(data, "t_log") == 0)
		return false;
	return true;
}

void YAgent::Main()
{
	//��ȡһ��
	string row;
	while (1)
	{

		char c = fgetc(fp);
		if (c <= 0)//û���ַ����߶�����β
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
