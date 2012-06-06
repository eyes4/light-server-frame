/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/strutil.h $
* @brief �ַ�����غ���
*   C++ string������ַ���������
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
* @brief ������ת��Ϊ�ַ���
*
* @param nNum Ҫת����������
*
* @return ת������ַ���
*/
extern string IntToStr(int nNum);
extern string Int64ToStr(long long nNum);
/**
* @brief ���ַ����е�����ָ���Ӵ��滻�����Ӵ�
*	���磺���ַ���str��ֵΪ"testa1ddda1"������string_replace_all(str, "a1","_");��
*		  str��ֵΪ"test_ddd_"
* @param str Դ�ַ���������ִ�к�ֵ���޸�Ϊ���
* @param old_pattern ��Ҫ�滻���Ӵ�
* @param new_pattern �滻Ϊ�����Ӵ�
*/
extern void string_replace_all(string & str, const string &old_pattern, const string &new_pattern);

/**
* @brief �滻һ�������ĵ���Ϊ���Ӵ�
*  ���ʵ�ǰ��һ���ַ������������ַ������ʵĺ���һ���ַ��ǳ���ĸ������������ַ��������str��ֵΪ"test(test2)"��
*  test��test2�����������ĵ��ʣ�����string_replace_word(str, "test", "TEST");��strֵΪ"TEST(test2)"������
*  test2��test��������ͬ�ĵ��ʣ����test2�����滻
* @param str Դ�ַ���������ִ�к�ֵ���޸�Ϊ���
* @param old_pattern ��Ҫ�滻�ĵ���
* @param new_pattern �滻Ϊ�����Ӵ�
*/
extern void string_replace_word(string & str, const string &old_pattern, const string &new_pattern);

/**
* @brief ���ַ����еĵ�һ��ָ���Ӵ��滻�����Ӵ�
*	���磺���ַ���str��ֵΪ"testa1ddda1"������string_replace_all(str, "a1","_");��
*		  str��ֵΪ"test_ddda1"
* @param str Դ�ַ���������ִ�к�ֵ���޸�Ϊ���
* @param old_pattern ��Ҫ�滻���Ӵ�
* @param new_pattern �滻Ϊ�����Ӵ�
*/
extern void string_replace(string & str, const string &old_pattern, const string &new_pattern);

/**
* @brief ��ȡ��ָ���ָ����ָ�Դ�ַ�����ĵ�һ���ֶΣ������ֶδ�Դ�ַ�����ɾ��
* ��ָ���ָ���Ϊ","��Դ�ַ���Ϊ"1,2,3,4",���ô˺�����Դ�ַ�����Ϊ"2,3,4"������"1"��
* 
* @param str Դ�ַ���
* @param strSplit �ָ��� ע�⣺�ָ��������ɶ���ַ����
* @return ���طָ�����ĵ�һ���ֶ�
*/
extern string CutFirstField(string &str, const string &strSplit);

/**
* @brief ȥ���ַ�����ߵĿ��ַ�
* @param str �账����ַ���
* @return �������ַ���
*/
extern string LTrim(const string& str);

/**
* @brief ȥ���ַ����ұߵĿ��ַ�
* @param str �账����ַ���
* @return �������ַ���
*/
extern string RTrim(const string& str); 

/**
* @brief ȥ���ַ������ߵĿ��ַ�
* @param str �账����ַ���
* @return �������ַ���
*/
extern string Trim(const string& str);
#endif
