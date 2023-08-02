#pragma once
#ifndef YYMSQL_H_
#define YYMSQL_H_

#include <vector>
#include <string>
#include "YYDATA.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <iconv.h>
#endif


//���еĺ��������ܱ�֤�̰߳�ȫ


struct MYSQL;
struct MYSQL_RES;
namespace YY{
class  YYMysql //�������lib
{
public:
	//��ʼ��
	bool Init();
	
	//������Դ
	void Close();
	//���ݿ����ӣ��������̰߳�ȫ��,flag����֧�ֶ������
	bool Connect(const char *host, const char* user, const char* passwd, const char* db,unsigned short port=3306,unsigned long flag=0);
	
	//ִ��sql��� 
	bool Query(const char*sql,unsigned long sqllen);
	bool Query(const char* sql);

	//Mysql�����趨 connect֮ǰ����
	//���ӳ�ʱʱ���趨
	bool SetConnectTimeout(int sec);
	//�������趨
	bool SetReconnect(bool is=true);


	//���������
	bool StoreResult();
	bool UseResult();
	//�ͷŽ����
	void FreeResult();



	//��ȡһ������
	std::vector<YYDATA> FetchRow();


	//��������޸����
	//��ͨ�ַ���
	//����insert���,�ֶ�����ǰ��@������ȥֵ������
	std::string GetInsetSql(YDATA &kv,std::string &table);


	//����Ƕ���������
	bool Insert(YDATA &kv, std::string table);


	//�������������
	bool InsertBin(YDATA& kv, std::string table);


	//Update
	std::string GeyUpdateSql(YDATA& kv, std::string& table, std::string &where);
	//���ظ���������ʧ�ܷ���-1   �Ƕ���������
	int Update(YDATA& kv, std::string table, std::string where);
	//���¶���������
	int UpdateBin(YDATA& kv, std::string table, std::string where);



	//������
	//��ʼ
	bool StartTransaction();
	//����
	bool StopTransaction();
	//commit
	bool Commit();
	//�ع�
	bool Rollback();

	//�ַ���ת��
#ifndef _WIN32
	 size_t Convert(char* from_cha, char* to_cha, char* in, size_t inlen, char* out, size_t outlen);
#endif

	std::string GBKToUTF8(const char* data);
	std::string UTF8ToGBK(const char* data);

	//���׽ӿ�,����select�����ݽ����ÿ�ε���������һ�εĽ����
	YYROWS GetResult(const char* sql);

protected:
	MYSQL* mysql = nullptr;
	MYSQL_RES* res = nullptr;
};
}




#endif // !YYMSQL_H_

