#ifndef YAGENT_H_
#define YAGENT_H_
 // !YAGENT_H_
#include "YYMysql.h"
#include <string>
class YAgent
{
public:
	bool Init(std::string ip);//ֻ�ܵ���һ�Σ������нӿ�֮ǰ����
	static YAgent* Get()//ÿ��ʹ��ͬһ��YAgentʵ��
	{
		static YAgent a;
		return &a;
	}
	void Main();//������ѭ��,��ȡ������־

	//��־д�����ݿ�
	bool SaveLog(std::string log);

	std::string GetLocalIp();//��ȡ����ip��ַ

	~YAgent();
private:
	YAgent();//��ֹ����ʵ�������乹�캯������˽��
	YY::YYMysql* my = 0;//���ݿ����
	FILE* fp = 0;//����־�ļ�

};
#endif

