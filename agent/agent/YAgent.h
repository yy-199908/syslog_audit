#ifndef YAGENT_H_
#define YAGENT_H_
 // !YAGENT_H_
#include "YYMysql.h"
#include <string>
class YAgent
{
public:
	bool Init(std::string ip);//只能调用一次，再所有接口之前调用
	static YAgent* Get()//每次使用同一个YAgent实例
	{
		static YAgent a;
		return &a;
	}
	void Main();//程序主循环,读取最新日志

	//日志写入数据库
	bool SaveLog(std::string log);

	std::string GetLocalIp();//获取本机ip地址

	~YAgent();
private:
	YAgent();//防止生成实例，将其构造函数放在私有
	YY::YYMysql* my = 0;//数据库对象
	FILE* fp = 0;//打开日志文件

};
#endif

