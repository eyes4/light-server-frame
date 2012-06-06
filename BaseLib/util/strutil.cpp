/**
* @file $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/util/strutil.cpp $
* @brief 字符串相关函数
*   C++ string类相关字符串处理函数
*
* $Id: strutil.cpp 4 2008-06-17 11:55:23Z gang.chen $
*
* @author gang chen <eyesfour@gmail.com>
*/  
#include "../include/strutil.h"

string IntToStr(int nNum)
{
	char szNum[100];
	sprintf(szNum, "%d", nNum);
	return string(szNum);
}

string Int64ToStr(long long nNum)
{
	char szNum[100];
	sprintf(szNum, "%I64d", nNum);
	return string(szNum);
}

void string_replace_all(string & str, const string &old_pattern, const string &new_pattern)
{
    string::size_type pos = 0;
    string::size_type oldlen = old_pattern.size();
    string::size_type newlen = new_pattern.size();
    while( (pos=str.find(old_pattern, pos)) != string::npos)
    {
        str.replace(pos, oldlen, new_pattern);
        pos += newlen;
    }
}

void string_replace_word(string & str, const string &old_pattern, const string &new_pattern)
{
    string::size_type pos = 0;
    string::size_type oldlen = old_pattern.size();
    string::size_type newlen = new_pattern.size();
    while( (pos=str.find(old_pattern, pos)) != string::npos)
    {
		char  split = str[pos + oldlen];
		if (split == ' ' || split <47 || (split >= 58 && split <= 64)|| 
			(split >= 91 && split <= 96) || split >= 123 ||
			pos + oldlen == str.length())
		{
			str.replace(pos, oldlen, new_pattern);
		}
        pos += newlen;
    }
}
    
void string_replace(string & str, const string &old_pattern, const string &new_pattern)
{
    string::size_type pos = 0;
    string::size_type oldlen = old_pattern.size();
    string::size_type newlen = new_pattern.size();
    if( (pos=str.find(old_pattern, pos)) != string::npos)
    {
        str.replace(pos, oldlen, new_pattern);
    }
}

string CutFirstField(string &str, const string &strSplit)
{
    int first = (int)str.find_first_of(strSplit);
    string result;
    if (first == string::npos)
    {
		result = str;
		str = "";
		return result;
    }
    result = str.substr(0, first);
    str.erase(0, first + 1);
    return result;
}

string LTrim(const string& str) 
{ 
	return str.substr(str.find_first_not_of(" \n\r\t")); 
} 

string RTrim(const string& str) 
{ 
	return str.substr(0,str.find_last_not_of(" \n\r\t")+1); 
} 

string Trim(const string& str) 
{ 
	return LTrim(RTrim(str)); 
}
