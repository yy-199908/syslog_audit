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


//所有的函数都不能保证线程安全


struct MYSQL;
struct MYSQL_RES;
namespace YY{
class  YYMysql //设置输出lib
{
public:
	//初始化
	bool Init();
	
	//清理资源
	void Close();
	//数据库连接（不考虑线程安全）,flag设置支持多条语句
	bool Connect(const char *host, const char* user, const char* passwd, const char* db,unsigned short port=3306,unsigned long flag=0);
	
	//执行sql语句 
	bool Query(const char*sql,unsigned long sqllen);
	bool Query(const char* sql);

	//Mysql参数设定 connect之前调用
	//连接超时时间设定
	bool SetConnectTimeout(int sec);
	//重连接设定
	bool SetReconnect(bool is=true);


	//结果集处理
	bool StoreResult();
	bool UseResult();
	//释放结果集
	void FreeResult();



	//获取一行数据
	std::vector<YYDATA> FetchRow();


	//插入语和修改语句
	//普通字符串
	//生成insert语句,字段名称前有@符，则去值的引号
	std::string GetInsetSql(YDATA &kv,std::string &table);


	//插入非二进制数据
	bool Insert(YDATA &kv, std::string table);


	//插入二进制数据
	bool InsertBin(YDATA& kv, std::string table);


	//Update
	std::string GeyUpdateSql(YDATA& kv, std::string& table, std::string &where);
	//返回更新数量，失败返回-1   非二进制数据
	int Update(YDATA& kv, std::string table, std::string where);
	//更新二进制数据
	int UpdateBin(YDATA& kv, std::string table, std::string where);



	//事务处理
	//开始
	bool StartTransaction();
	//结束
	bool StopTransaction();
	//commit
	bool Commit();
	//回滚
	bool Rollback();

	//字符集转换
#ifndef _WIN32
	 size_t Convert(char* from_cha, char* to_cha, char* in, size_t inlen, char* out, size_t outlen);
#endif

	std::string GBKToUTF8(const char* data);
	std::string UTF8ToGBK(const char* data);

	//简易接口,返回select的数据结果，每次调用清理上一次的结果集
	YYROWS GetResult(const char* sql);

protected:
	MYSQL* mysql = nullptr;
	MYSQL_RES* res = nullptr;
};
}




#endif // !YYMSQL_H_

