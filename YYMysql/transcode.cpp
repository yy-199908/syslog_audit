#include "transcode.h"
#ifndef _WIN32
static size_t Convert(char* from_cha, char* to_cha, char* in, size_t inlen, char* out, size_t outlen)
{
    //转换上下文
    iconv_t cd;
    cd = iconv_open(to_cha, from_cha);
    if (cd == 0) return -1;
    memset(out, 0, outlen);
    char** pin = &in;
    char** pout = &out;
    size_t re = iconv(cd, pin, &inlen, pout, &outlen);

    if (cd != 0)
        iconv_close(cd);
    return re;
}
#endif


using namespace std;
string UTF8ToGBK(const char* data)
{
    string gbk = "";
    //window版本
#ifdef _WIN32
    //UTF8转为unicode ：win utf16

    //统计转换后字节数
    int len = MultiByteToWideChar(CP_UTF8,//转换格式
        0,//默认转换方式
        data,//输入的字节
        -1,//输入字符串大小，-1找、0结束
        0,  //输出
        0   //输出的空间大小
    );
    if (len <= 0) return gbk;
    wstring udata;
    udata.resize(len);
    MultiByteToWideChar(CP_UTF8, 0, data, -1, (wchar_t*)udata.data(), len);

    //unicode转换为GBK
    len = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, 0, 0,
        0,//失败默认替换字符
        0//是否使用默认替代
    );
    if (len <= 0) return gbk;
    gbk.resize(len);
    WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, (char*)gbk.data(), len, 0, 0);

#else
  //Linux 代码
    gbk.resize(1024);//最大支持1024
    int inlen = strlen(data);
    Convert((char*)"utf-8", (char*)"gbk", (char*)data, inlen, (char*)gbk.data(), gbk.size());
    int outlen = gbk.size();
    gbk.resize(outlen);
#endif
    return gbk;
}

string GBKToUTF8(char const* data)
{
    string utf8;

#ifdef _WIN32
    ////GBK转为unicode ：win utf16

    //统计转换后字节数
    int len = MultiByteToWideChar(CP_ACP,//转换格式
        0,//默认转换方式
        data,//输入的字节
        -1,//输入字符串大小，-1找、0结束
        0,  //输出
        0   //输出的空间大小
    );
    if (len <= 0) return utf8;
    wstring udata;
    udata.resize(len);
    MultiByteToWideChar(CP_ACP, 0, data, -1, (wchar_t*)udata.data(), len);


    //unicode转换为UTF8
    len = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, 0, 0,
        0,//失败默认替换字符
        0//是否使用默认替代
    );
    if (len <= 0) return utf8;
    utf8.resize(len);
    WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, (char*)utf8.data(), len, 0, 0);
#else
    //Linux 代码
    utf8.resize(1024);//最大支持1024
    int inlen = strlen(data);
    Convert((char*)"gbk", (char*)"utf-8", (char*)data, inlen, (char*)utf8.data(), utf8.size());
    int outlen = utf8.size();
    utf8.resize(outlen);
#endif
    return utf8;
}