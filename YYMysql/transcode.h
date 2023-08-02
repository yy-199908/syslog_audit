#pragma once
#ifndef TRANSCODE_H_
#define TRANSCODE_H_
#include <iostream>
#include <string>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <iconv.h>
static size_t Convert(char* from_cha, char* to_cha, char* in, size_t inlen, char* out, size_t outlen);
#endif
std::string UTF8ToGBK(const char* data);
std::string GBKToUTF8(char const* data);

#endif // !TRANSCODE_H_
