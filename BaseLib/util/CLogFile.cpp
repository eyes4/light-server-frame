
#include "../include/CLogFile.h"

CLogFile::CLogFile()
{
    m_LogFile = new ofstream();
}

CLogFile::~CLogFile()
{
    delete m_LogFile;
}

void CLogFile::Open(void)
{
	char szLogPath[512];
	sprintf(szLogPath, "%s%s", GetCurrPath(), "log");
	if (!DirExists(szLogPath)) 
		CreateDir(szLogPath);
    GetCurrLogFileName(m_szLogFileName);
    DEBUGMSG(("new logfile:%s\n", m_szLogFileName));
    m_LogFile->open(m_szLogFileName, ios::app);
    m_StrmBuffer = clog.rdbuf(m_LogFile->rdbuf());
    m_bRunning = true;
	m_hSwitchThread = ThreadCreate(_SwitchThreadProc, this);
}

void CLogFile::Close(void)
{
	WaitForThread(m_hSwitchThread);
	CloseThreadHandle(m_hSwitchThread);
	m_bRunning = false;
    m_LogFile->close();

}

void CLogFile::GetCurrLogFileName(char *szLogFileName)
{
	char szFileName[512];
	time_t now;
    struct tm *t;
    now = time(NULL);
    t = localtime(&now);

	strftime(szFileName, 17, "log/%Y%m%d.log",t);
	sprintf(szLogFileName, "%s%s", GetCurrPath(), szFileName);
}

THREAD_FUN CLogFile::_SwitchThreadProc(void *pContext)
{
    CLogFile *pLogFile = static_cast<CLogFile *> (pContext);
    pLogFile->SwitchThreadProc();
	return 0;
}


void CLogFile::SwitchThreadProc()
{
    time_t tLast = time(NULL);
	time_t tNow;
	while(m_bRunning)
    {
		tNow = time(NULL);

		if (int(tNow/24/3600) != int(tLast/24/3600))
		{
			fflush(NULL);
			m_LogFile->close();
			GetCurrLogFileName(m_szLogFileName);
			m_LogFile->open(m_szLogFileName, ios::app);
			m_StrmBuffer = clog.rdbuf(m_LogFile->rdbuf());
			clog<<"new log file"<<endl;
		}
        //ÐÝÃß60Ãë
        for (int i = 0; i < 60; i++)
        {
            SleepMs(1000);
            if (!m_bRunning)
                break;
        }
    }
}
