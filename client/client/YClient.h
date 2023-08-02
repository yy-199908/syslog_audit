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
	//����û�����ʽ����ֹע�빥�����û���ֻ���Ǵ�Сд�ַ�,���ּ�_
	bool CheckInput(std:: string username);

	//����¼ʧ�ܴ���
	int maxLoginTimes = 10;
	
private:
	YClient();
	void c_log(std::vector<std::string>cmds);
	//�����������
	void c_test(std::vector<std::string>cmds);
	//search 127.0.0.1
	void c_search(std::vector<std::string>cmds);
	//like 127  ģ������  like����ʧЧ
	void c_like(std::vector<std::string>cmds);
	YY::YYMysql *my = 0;

};

#endif