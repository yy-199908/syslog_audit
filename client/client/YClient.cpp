#include "YClient.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <chrono>
#ifdef _WIN32
#include <conio.h>
#else
#include <termio.h>//更改显示模式，隐藏密码
char _getch()
{
	termios tm_new, tm_old;
	int fd = 0;
	if (tcgetattr(fd, &tm_old) < 0) return -1;
	//更改为原始模式没有回显
	cfmakeraw(&tm_new);
	if (tcsetattr(fd, TCSANOW, &tm_new) < 0)return -1;
	char c = getchar();
	if (tcsetattr(fd, TCSANOW, &tm_old) < 0)return -1;
	return c;
}
#endif

using namespace std;
using namespace chrono;
using namespace YY;


bool YClient::Init(std::string ip)
{
	cout << "Client Init() " << ip << endl;
	my = new YYMysql();
	my->Init();
	my->SetReconnect(1);
	if (!my->Connect(ip.c_str(), "root", "123456", "yy"))
	{
		cerr << "db connect failed!" << endl;
	}
	cout << "db connect success!" << endl;
	my->Query("set names utf8");
	return true;
}


string  YClient::InputPasswd()
{
	//清空缓冲
	cin.ignore(4096,'\n');
	string passwd;

	for (;;)
	{
		char a = _getch();
		if (a <= 0 or a == '\n' or a == '\r') { 
			cout << "\n";
			break;
		}
		
		if (a == 127)
		{
			cout << "\b"<<" \b";
			passwd = passwd.substr(0,passwd.size()-1);
			continue;
		}
		cout << "*" ;
		passwd += a;
	}
	return passwd;
}


string YClient::Input()
{
	
	string input;
	for (;;)
	{
		char a = getchar();
		if (a <= 0 or a == '\n' or a == '\r') {
			break;
		}
		input += a;
	}
	return input;
}




bool YClient::CheckInput(std::string username)
{
	for (char a : username)
	{
		if (a >= 65 and a <= 90) continue;
		if (a >= 97 and a <= 122) continue;
		if (a == 95) continue;
		return false;
	}
	return true;
}


bool YClient::Login()
{

	//注入攻击：人工修改你拼接成的sql字符串
	//正常情况 select id from t_user where user='root' and passwd=ma5('123456');
	//注入攻击 select id from t_user where user='·root'or'1'='1·' and passwd=md5('·12345')or'c4ca4238a0b923820dcc509a6f75849b'=md5('1·');
	//··之间为输入的username和passwd
	//username=root'or'1'='1 
	//passwd=5')or'c4ca4238a0b923820dcc509a6f75849b'=md5('1




	bool isLogin = false;
	for (int i = 0; i < maxLoginTimes; i++)
	{
		string username;
		string passwd;
		//1.接受用户输入
		cout << "input username: " << flush;
		cin >> username;
		//接受密码
		if (!CheckInput(username))
		{
			cout << "illegal username! Correct username only include A-Z, a-z, 1-9 and _!" << endl;
			continue;
		}

		cout << "input password: " << flush;
		passwd = InputPasswd();
		
		string sql = "select id from t_user where user='";
		sql += username;
		sql += "' and passwd=md5('";
		sql += passwd;
		sql += "')";
		
		auto rows = my->GetResult(sql.c_str());
		if (rows.size() > 0)
		{
			cout << "Login success!" << endl;
			isLogin = true;
			break;
		}
		else
		{
			cout << "Username or passwd is wrong!" << endl;
		}
	}
	return isLogin;
}






void YClient::c_like(vector<string>cmds)
{
	
	string log;
	if (cmds.size() == 1)return;
	log = cmds[1];
	//记录查询时间
	//记录开始时间
	auto start = system_clock::now();


	string sql = "select * from `t_log` where log like'%";
	sql += log;
	sql += "%'";
	auto rows = my->GetResult(sql.c_str());
	for (auto row : rows)
	{
		for (auto c : row)
		{
			cout << c.data << ' ';
		}
		cout << endl;
	}

	//纪律结束时间，得出耗时
	auto end = system_clock::now();
	auto duration = duration_cast<microseconds>(end - start);
	cout << "search time : " << double(duration.count()) * microseconds::period::num / microseconds::period::den << "s" << endl;


	//统计总数
	 sql = "select count(*) from `t_log` where log like'%";
	sql += log;
	sql += "%'";
	rows = my->GetResult(sql.c_str());
	int total = 0;
	if (rows.size() > 0 and rows[0][0].data)
		total = atoi(rows[0][0].data);
	cout << "total: " << total << endl;


}

void YClient::c_search(vector<string>cmds)
{
	string ip;
	if (cmds.size() == 1)return;
	ip = cmds[1];
	//记录查询时间
	//记录开始时间
	auto start = system_clock::now();


	string sql = "select * from `t_log` where ip='";
	sql += ip;
	sql += "'";
	auto rows = my->GetResult(sql.c_str());
	for (auto row : rows)
	{
		for (auto c : row)
		{
			cout << c.data << ' ';
		}
		cout << endl;
	}

	//纪律结束时间，得出耗时
	auto end= system_clock::now();
	auto duration = duration_cast<microseconds>(end - start);
	cout << "search time : " << double(duration.count())*microseconds::period::num/microseconds::period::den  <<"s"<< endl;



	//统计总数
	sql = "select count(*) from `t_log` where ip='";
	sql += ip;
	sql += "'";
	rows = my->GetResult(sql.c_str());
	int total = 0;
	if(rows.size()>0 and rows[0][0].data)
	total = atoi(rows[0][0].data);
	cout << "total: " << total << endl;

}


void YClient::c_test(vector<string>cmds)
{
	int count = 10000;
	if(cmds.size()>1)
		count= atoi(cmds[1].c_str());
	my->StartTransaction();
	for (int i = 0; i < count; i++)
	{
		YDATA data;
		stringstream ss;
		ss << "testlog";
		ss << (i + 1);
		string tmp = ss.str();
		data["log"] = tmp.c_str();
		data["ip"] = "127.0.0.1";
		data["@time"] = "now()";
		my->Insert(data,"t_log");

	}


	{
		YDATA data;
		stringstream ss;
		ss << "search001";
		string tmp = ss.str();
		data["log"] = tmp.c_str();
		data["ip"] = "10.0.0.1";
		data["@time"] = "now()";
		my->Insert(data, "t_log");

	}
	my->Commit();
	my->StopTransaction();
}


void YClient::c_log(vector<string>cmds)
{
	int pagecount = 10;
	int page = 1;
	if (cmds.size() > 1)
	{
		page = atoi(cmds[1].c_str());
		pagecount = atoi(cmds[2].c_str());
	}
	if (page <= 0 or pagecount <= 0)
	{
		cerr << "Page and pagecount must >=0" << endl;
		return;
	}
	string sql = "select * from t_log limit ";
	stringstream ss;
	ss<< sql;
	ss << (page - 1) * pagecount<<",";
	ss <<  pagecount;
	sql = ss.str();
	cout << sql << endl;
	auto rows = my->GetResult(sql.c_str());
	for (auto row : rows)
	{
		for (auto c : row)
		{
			cout << c.data << ' ';
		}
		cout << endl;
	}

}




void YClient::Main()
{
	//1. 用户登录
	//if (!Login()) return;
	//2.分页显示t_log

	//获取用户输入
	while (1)
	{
		cout << "input:";
		string cmd = Input();
		//log 1 10 第一页 一页十行
		//切割空格
		vector<string> cmds;
		char* str = strtok((char*)cmd.c_str(), " ");
		while (str)
		{
			cmds.push_back(str);
			str = strtok(0, " ");
		}

		string type = cmd;
		if (!cmds.empty())
			type = cmds[0]; 
		if (type == "log")
		{
			c_log(cmds);
		}
		else if (type == "test")
			c_test(cmds);
		else if (type == "search")
			c_search(cmds);
		else if (type == "like")
			c_like(cmds);
	}
}


YClient::YClient() {}
YClient::~YClient() {}