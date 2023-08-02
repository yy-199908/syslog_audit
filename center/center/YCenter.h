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

	bool Install(std::string ip);//��װ
	bool Init();//��ʼ�����ݿ�
	
	//����豸
	bool AddDevice(std::string ip,std::string name);
	void Main();
	~YCenter();

private:
	YCenter();
	YY::YYMysql* my = 0;



};



#endif // !CENTER_H_
