#pragma once
#ifndef YYDATA_H_
#define YYDATA_H_

#include <map>
#include <string>
#include <vector>






namespace YY {

	enum MYSQL_TYPES

	{
		YY_TYPE_DECIMAL,
		YY_TYPE_TINY,
		YY_TYPE_SHORT,
		YY_TYPE_LONG,
		YY_TYPE_FLOAT,
		YY_TYPE_DOUBLE,
		YY_TYPE_NULL,
		YY_TYPE_TIMESTAMP,
		YY_TYPE_LONGLONG,
		YY_TYPE_INT24,
		YY_TYPE_DATE,
		YY_TYPE_TIME,
		YY_TYPE_DATETIME,
		YY_TYPE_YEAR,
		YY_TYPE_NEWDATE, /**< Internal to MySQL. Not used in protocol */
		YY_TYPE_VARCHAR,
		YY_TYPE_BIT,
		YY_TYPE_TIMESTAMP2,
		YY_TYPE_DATETIME2,   /**< Internal to MySQL. Not used in protocol */
		YY_TYPE_TIME2,       /**< Internal to MySQL. Not used in protocol */
		YY_TYPE_TYPED_ARRAY, /**< Used for replication only */
		YY_TYPE_JSON = 245,
		YY_TYPE_NEWDECIMAL = 246,
		YY_TYPE_ENUM = 247,
		YY_TYPE_SET = 248,
		YY_TYPE_TINY_BLOB = 249,
		YY_TYPE_MEDIUM_BLOB = 250,
		YY_TYPE_LONG_BLOB = 251,
		YY_TYPE_BLOB = 252,
		YY_TYPE_VAR_STRING = 253,
		YY_TYPE_STRING = 254,
		YY_TYPE_GEOMETRY = 255
	};


	struct YYDATA
	{
		YYDATA(const char* data=0);
		YYDATA(const int *d);
		YYDATA(std::string &data);
		//��ȡ�ļ�����д��data�С�size�洢��С,���ڶ�������data�ռ䣬��ҪDrop�ͷ�
		bool LoadFile(const char* filename);
		bool SaveFile(const char* filename);
		const char* data = nullptr;
		int size = 0;
		MYSQL_TYPES type;
		void Drop();//�ͷ�loadfile����Ŀռ�
	};

	//����͸������ݵ����ݽṹ
	typedef std::map<std::string, YYDATA> YDATA;
	//�����б�
	typedef std::vector<std::vector<YYDATA>> YYROWS;

}

#endif


