#include "transcode.h"
#ifndef _WIN32
static size_t Convert(char* from_cha, char* to_cha, char* in, size_t inlen, char* out, size_t outlen)
{
    //ת��������
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
    //window�汾
#ifdef _WIN32
    //UTF8תΪunicode ��win utf16

    //ͳ��ת�����ֽ���
    int len = MultiByteToWideChar(CP_UTF8,//ת����ʽ
        0,//Ĭ��ת����ʽ
        data,//������ֽ�
        -1,//�����ַ�����С��-1�ҡ�0����
        0,  //���
        0   //����Ŀռ��С
    );
    if (len <= 0) return gbk;
    wstring udata;
    udata.resize(len);
    MultiByteToWideChar(CP_UTF8, 0, data, -1, (wchar_t*)udata.data(), len);

    //unicodeת��ΪGBK
    len = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, 0, 0,
        0,//ʧ��Ĭ���滻�ַ�
        0//�Ƿ�ʹ��Ĭ�����
    );
    if (len <= 0) return gbk;
    gbk.resize(len);
    WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, (char*)gbk.data(), len, 0, 0);

#else
  //Linux ����
    gbk.resize(1024);//���֧��1024
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
    ////GBKתΪunicode ��win utf16

    //ͳ��ת�����ֽ���
    int len = MultiByteToWideChar(CP_ACP,//ת����ʽ
        0,//Ĭ��ת����ʽ
        data,//������ֽ�
        -1,//�����ַ�����С��-1�ҡ�0����
        0,  //���
        0   //����Ŀռ��С
    );
    if (len <= 0) return utf8;
    wstring udata;
    udata.resize(len);
    MultiByteToWideChar(CP_ACP, 0, data, -1, (wchar_t*)udata.data(), len);


    //unicodeת��ΪUTF8
    len = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, 0, 0,
        0,//ʧ��Ĭ���滻�ַ�
        0//�Ƿ�ʹ��Ĭ�����
    );
    if (len <= 0) return utf8;
    utf8.resize(len);
    WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, (char*)utf8.data(), len, 0, 0);
#else
    //Linux ����
    utf8.resize(1024);//���֧��1024
    int inlen = strlen(data);
    Convert((char*)"gbk", (char*)"utf-8", (char*)data, inlen, (char*)utf8.data(), utf8.size());
    int outlen = utf8.size();
    utf8.resize(outlen);
#endif
    return utf8;
}