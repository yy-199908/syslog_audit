#include "YCenter.h"
#define CENTER_CONF "ip"
#include <iostream>
#include <fstream>
#include <thread>
#include <regex>
using namespace std;
using namespace YY;
bool YCenter::Init()
{
	my = new YYMysql();
	my->Init();
	string ip;
	fstream fs;
	fs.open(CENTER_CONF);
	if (!fs.is_open())
	{
		cout << "Please install center" << endl;
		return false;
	}
	fs >> ip;
	fs.close();
	if (ip.empty())
	{
		cout << "Please install center" << endl;
		return false;
	}
	cout << "Init center " << ip << endl;
	my->SetReconnect();
	if (!my->Connect(ip.c_str(), "root", "123456", "yy"))
	{
		cout << "DB connect Failed!" << endl;
		return false;
	}
	my->Query("set names utf8");
	return true;
}



bool YCenter::Install(string ip)
{

	//1.生成配置文件 数据库的ip
	cout << "YCenter::install() " << ip << endl;
	ofstream of;
	of.open(CENTER_CONF);
	if (!of.is_open())
	{
		cout << "open config " << CENTER_CONF <<"Failed!" << endl;
		return false;
	}
	of << ip;
	of.close();

	//2.初始化表和数据
	if(!Init())
	return false;
	//创建策略表
	//清空已有表
	my->StartTransaction();
	my->Query("DROP TABLE IF EXISTS `t_strategy`");
	string sql = "CREATE TABLE `t_strategy`(\
				`id` INT AUTO_INCREMENT,\
				`name` VARCHAR(256) CHARACTER SET 'utf8' COLLATE 'utf8_bin',\
				`strategy` VARCHAR(4096),\
				PRIMARY KEY(`id`))";
	if (!my->Query(sql.c_str()))
	{
		my->Rollback();
		cout << "Create strategy table failed!" << endl;
		return false;
	}



	{
		YDATA data;
		data["name"] = "用户登录失败";
		data["strategy"] = ".*Failed password for (.+) from ([0-9.]+) port ([0-9]+).*";
		my->Insert(data, "t_strategy");


		data["name"] = "用户登录成功";
		data["strategy"] = ".*Accepted password for (.+) from ([0-9.]+) port ([0-9]+).*";
		my->Insert(data, "t_strategy");
	}

	//创建用户表 初始化管理员用户 root 123456 md5
	//t_user id user passwd
	my->Query("DROP TABLE IF EXISTS `t_user`");
	 sql = "CREATE TABLE `t_user`(\
				`id` INT AUTO_INCREMENT,\
				`user` VARCHAR(256) CHARACTER SET 'utf8' COLLATE 'utf8_bin',\
				`passwd` VARCHAR(1024),\
				PRIMARY KEY(`id`))";
	if (!my->Query(sql.c_str()))
	{
		my->Rollback();
		cout << "Create user table failed!" << endl;
		return false;
	}

	{
		YDATA data;
		data["user"] = "root";
		data["@passwd"] = "md5('123456')";
		my->Insert(data, "t_user");
	}



	//创建设备表
	my->Query("DROP TABLE IF EXISTS `t_device`");
	sql = "CREATE TABLE `t_device`(\
				`id` INT AUTO_INCREMENT,\
				`ip` VARCHAR(16),\
				`name` VARCHAR(2048),\
				`last_heart` datetime,\
				PRIMARY KEY(`id`))";
	if (!my->Query(sql.c_str()))
	{
		my->Rollback();
		cout << "Create device table failed!" << endl;
		return false;
	}




	//创建日志表
	
	my->Query("DROP TABLE IF EXISTS `t_log`");
	sql = "CREATE TABLE `t_log`(\
				`id` INT AUTO_INCREMENT,\
				`ip` VARCHAR(16),\
				`log` VARCHAR(2048),\
				`time` datetime,\
				PRIMARY KEY(`id`))";
	if (!my->Query(sql.c_str()))
	{
		my->Rollback();
		cout << "Create log table failed!" << endl;
		return false;
	}

	//创建审计结果表

	my->Query("DROP TABLE IF EXISTS `t_audit`");
	sql = "CREATE TABLE `t_audit`(\
				`id` INT AUTO_INCREMENT,\
				`name` VARCHAR(2048),\
				`context` VARCHAR(2048),\
				`user` VARCHAR(256),\
				`device_ip` VARCHAR(16),\
				`log_ip` VARCHAR(16),\
				`port` INT,\
				`last_heart` datetime,\
				PRIMARY KEY(`id`))";
	if (!my->Query(sql.c_str()))
	{
		my->Rollback();
		cout << "Create audit table failed!" << endl;
		return false;
	}
	my->Commit();
	my->StopTransaction();
	return true;
}



bool YCenter::AddDevice(string ip, string name)
{

	/*`id` INT AUTO_INCREMENT,\
				`ip` VARCHAR(16),\
				`name` VARCHAR(2048),\
				`last_heart` datatime,\*/

	YDATA data;
	data["ip"] = ip.c_str();
	data["name"] = name.c_str();
	return my->Insert(data,"t_device");

	return true;
}



void YCenter::Main()
{	//取审计策略
	
	auto rows = my->GetResult("SELECT * from t_strategy");
	//正则表达式
	map<string, regex> strategys;
	for (auto row : rows)
	{
		//初始化正则
		if (row[1].data && row[2].data)
			strategys[row[1].data] = regex(row[2].data);
	}




	//审计t_log的事件
	int last_id = 0;
	rows=my->GetResult("SELECT max(id) from t_log");
	if (rows[0][0].data)
	{
		last_id = atoi(rows[0][0].data);
	}
	
	
	while (1)
	{
		//获取agent存入的最新数据
		string buf= "select * from t_log where id>";
		buf += to_string(last_id);
		rows = my->GetResult(buf.c_str());
	
		if (rows.empty())
		{
			this_thread::sleep_for(100ms);
			continue;
		}
	
		for (auto row : rows)
		{
			
			last_id = atoi(row[0].data);
			if (!row[2].data)
				continue;
			//正则匹配之后的结果
			for (auto strategy : strategys)
			{
				smatch match;
				string log = row[2].data;
				//匹配正则结果返回到match
				bool ret = regex_match(log, match,strategy.second);
				if (!ret or match.size() <= 0) continue;//匹配失败
				YDATA data;
				data["name"] = strategy.first.c_str();
				data["context"] = log.c_str();
				if (row[1].data)
					data["device_ip"] = row[1].data;
				//匹配结果下标0是整个字符串，1是第一个匹配结果,2是第二个匹配结果....
				string user = match[1];
				string log_ip = match[2];
				string port = match[3];
				data["user"] = user.c_str();
				data["log_ip"] = log_ip.c_str();
				data["port"] = port.c_str();
				my->Insert(data, "t_audit");
				cout << strategy.first.c_str() << endl;


			}
			



		}
	}

}
YCenter::YCenter()
{
}
YCenter::~YCenter()
{
}