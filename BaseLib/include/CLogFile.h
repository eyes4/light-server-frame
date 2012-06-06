/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CLogFile.h $
 *
 *  @brief 日志文件记录类
 *
 *  $Id: CLogFile.h 17 2008-11-18 11:38:38Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef _CLOGFILE_H_
#define _CLOGFILE_H_

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <cstring>
#include <errno.h>

//#ifdef _WIN32
//#include <windows.h>
//#include <process.h>
//#include <shlwapi.h>
//#pragma warning(disable: 4996)
//#else
//#include <dirent.h>
//#include <unistd.h>
//#include <pthread.h>
//#endif

#include "common.h"
#include "CSingleton.h"
#include "common_thread.h"
using namespace std;



class CLogFile
  : public CSingleton<CLogFile>
{
public:
	CLogFile();
	~CLogFile();
	void Open(void);
	void Close(void);

	template <typename T>
	friend ostream & operator<<(CLogFile& strm, T *t)
	{
		time_t nowtime;
		tm *ts;
		char szTimeStr[23];
		nowtime = time(NULL);
		ts= localtime(&nowtime);

		strftime(szTimeStr, 23, "[%Y-%m-%d %H:%M:%S] ", ts);
		clog << szTimeStr << t;

		return clog;
	}

private:
	ofstream *m_LogFile;
	char m_szLogFileName[1024];
    streambuf *m_StrmBuffer;

	bool m_bRunning;
	ThreadHandle m_hSwitchThread;			/**<工作线程句柄*/
	static THREAD_FUN _SwitchThreadProc(void *context);	/**<log文件切换线程桥接函数*/
	void SwitchThreadProc();				/**<log文件切换线程主体函数*/
    
	void GetCurrLogFileName(char *szLogFileName);
    friend class CSingleton<CLogFile>;
};

#endif
