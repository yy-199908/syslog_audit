#ifndef YCLIENT_H_
#define YCLIENT_H_


#include <string>
#include "YYMysql.h"
#include <vector>
class YClient
{
public:
	static YClient* Get()
	{
		static YClient a;
		return &a;
	}

	bool Init(std::string ip);
	void Main();
	bool Login();
	~YClient();
	std::string InputPasswd();
	std::string Input();
	//检查用户名格式，防止注入攻击，用户名只能是大小写字符,数字加_
	bool CheckInput(std:: string username);

	//最大登录失败次数
	int maxLoginTimes = 10;
	
private:
	YClient();
	void c_log(std::vector<std::string>cmds);
	//插入测试数据
	void c_test(std::vector<std::string>cmds);
	//search 127.0.0.1
	void c_search(std::vector<std::string>cmds);
	//like 127  模糊查找  like索引失效
	void c_like(std::vector<std::string>cmds);
	YY::YYMysql *my = 0;

};

#endif