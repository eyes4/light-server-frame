/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/strutil.h $
* @brief 字符串相关函数
*   C++ string类相关字符串处理函数
*
* $Id: strutil.h 2 2008-06-06 14:59:37Z gang.chen $
*
* @author gang chen <eyesfour@gmail.com>
*/
#ifndef __STRUTIL_H__
#define __STRUTIL_H__

#include <iostream>
#include <string>

#include "common.h"
using namespace std;

/**
* @brief 整型数转化为字符串
*
* @param nNum 要转换的整型数
*
* @return 转换后的字符串
*/
extern string IntToStr(int nNum);
extern string Int64ToStr(long long nNum);
/**
* @brief 将字符串中的所有指定子串替换成新子串
*	例如：有字符串str的值为"testa1ddda1"，调用string_replace_all(str, "a1","_");后，
*		  str的值为"test_ddd_"
* @param str 源字符串，函数执行后，值被修改为结果
* @param old_pattern 需要替换的子串
* @param new_pattern 替换为的新子串
*/
extern void string_replace_all(string & str, const string &old_pattern, const string &new_pattern);

/**
* @brief 替换一个完整的单词为新子串
*  单词的前面一个字符可以是任意字符，单词的后面一个字符是除字母和数字以外的字符，如符串str的值为"test(test2)"，
*  test和test2是两个单独的单词，调用string_replace_word(str, "test", "TEST");后，str值为"TEST(test2)"，由于
*  test2与test是两个不同的单词，因此test2不被替换
* @param str 源字符串，函数执行后，值被修改为结果
* @param old_pattern 需要替换的单词
* @param new_pattern 替换为的新子串
*/
extern void string_replace_word(string & str, const string &old_pattern, const string &new_pattern);

/**
* @brief 将字符串中的第一个指定子串替换成新子串
*	例如：有字符串str的值为"testa1ddda1"，调用string_replace_all(str, "a1","_");后，
*		  str的值为"test_ddda1"
* @param str 源字符串，函数执行后，值被修改为结果
* @param old_pattern 需要替换的子串
* @param new_pattern 替换为的新子串
*/
extern void string_replace(string & str, const string &old_pattern, const string &new_pattern);

/**
* @brief 切取用指定分隔符分隔源字符串后的第一个分段，并将分段从源字符串中删除
* 如指定分隔符为","，源字符串为"1,2,3,4",调用此函数后，源字符串被为"2,3,4"，返回"1"。
* 
* @param str 源字符串
* @param strSplit 分隔符 注意：分隔符可以由多个字符组成
* @return 返回分割出来的第一个分段
*/
extern string CutFirstField(string &str, const string &strSplit);

/**
* @brief 去除字符串左边的空字符
* @param str 需处理的字符串
* @return 处理后的字符串
*/
extern string LTrim(const string& str);

/**
* @brief 去除字符串右边的空字符
* @param str 需处理的字符串
* @return 处理后的字符串
*/
extern string RTrim(const string& str); 

/**
* @brief 去除字符串两边的空字符
* @param str 需处理的字符串
* @return 处理后的字符串
*/
extern string Trim(const string& str);
#endif
