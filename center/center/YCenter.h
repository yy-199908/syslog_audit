#ifndef CENTER_H_
#define CENTER_H_
#include <string>
#include "YYMysql.h"
class YCenter
{
public:
	static YCenter* Get()
	{
		static YCenter a;
		return &a;
	}

	bool Install(std::string ip);//安装
	bool Init();//初始化数据库
	
	//添加设备
	bool AddDevice(std::string ip,std::string name);
	void Main();
	~YCenter();

private:
	YCenter();
	YY::YYMysql* my = 0;



};



#endif // !CENTER_H_
