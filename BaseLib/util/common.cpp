/****************************************************
*  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/util/common.cpp $
*
*  @brief common c file
*
*  $Id: common.cpp 16 2008-11-18 11:36:40Z gang.chen $
*
*  @author gang chen <eyesfour@gmail.com>
***************************************************/
#include "../include/common.h"


void SRand(void)
{
	//static bool init = false;
	//if ( !init )
	{ 
		UINT64 tick;
		int seed;
		//init = true;

#if defined(WIN32)
		tick = GetTickCount ();
#elif defined(_MSC_VER)
		{
			volatile unsigned int lowtick=0,hightick=0;
			__asm
			{
				rdtsc 
					mov lowtick, eax
					mov hightick, edx
			}
			tick = hightick;
			tick <<= 32;
			tick |= lowtick;
		}
#elif defined(__GNUC__) && ( defined(__i686__) || defined(__i386__) )
		asm("rdtsc" : "=A" (tick));
#elif defined(__GNUC__) && defined(__amd64__)
		asm("rdtsc" : "=A" (tick));
#elif defined (__SUNPRO_CC) && defined( __sparc__ )	
		tick = gethrtime();
#elif defined(__MACH__) 
		{
			int fd=open("/dev/random",O_RDONLY);
			read(fd,&tick,sizeof(tick));
			closesocket(fd);
		}
#elif defined(_LINUX) 
		{
			fd_set fdSet;
			int maxFd=0;
			struct timeval tv;
			int e;

			int fd=open("/dev/random",O_RDONLY);

			if (fd<0)
			{
				printf("Failed to open random device\n");
				return random();
			}
			FD_ZERO(&fdSet);
			FD_SET(fd,&fdSet);
			maxFd=fd+1;

			printf("random device opened\n");
			tv.tv_sec = 0;
			tv.tv_usec = 500;

			e = select( maxFd, &fdSet, NULL,NULL, &tv );
			if (e <= 0)
			{
				printf("Failed to get data from random device\n");
				closesocket(fd);
				return random();
			}
			read(fd,&tick,sizeof(tick));
			printf("random device returned data\n");
			closesocket(fd);
		}
#else
#     error Need some way to seed the random number generator 
#endif 
		seed = (int)(tick);
#if	defined(WIN32) || defined(WIN32_WCE)
		srand(seed);
#else
		srandom(seed);
#endif
	}
}

int Rand(void)
{
#ifdef WIN32
	int r1 = rand();
	int r2 = rand();
	int ret = (r1<<16) + r2;
	return ret;
#elif defined _LINUX
	return random(); 
#endif
}

void SleepMs(int mSec)
{
#ifdef WIN32  
	Sleep(mSec);
#elif defined _LINUX
	usleep(mSec * 1000);
#endif
}

int GetTimeofDay(struct timeval* tp)
{
#ifdef WIN32  
	static const unsigned __int64 epoch = 116444736000000000L;

	FILETIME    file_time;
	SYSTEMTIME  system_time;
	ULARGE_INTEGER ularge;

	GetSystemTime(&system_time);
	if (SystemTimeToFileTime(&system_time, &file_time) == FALSE) return -1;
	ularge.LowPart = file_time.dwLowDateTime;
	ularge.HighPart = file_time.dwHighDateTime;

	tp->tv_sec = (long) ((ularge.QuadPart - epoch) / 10000000L);
	tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
	return 0;
#elif defined _LINUX
	return gettimeofday(tp, NULL);
#endif
}

char *GetCurrPath(void)
{
	static char path[261];
#ifdef WIN32
	DWORD res = GetModuleFileName(NULL, path, MAX_PATH);
	char*   s   =   strrchr(path,   '\\');
	if   (s!=0)
		*(s+1)   =   '\0';
#elif defined _LINUX
	getcwd(path, 261);
#endif
	return path;
}

char *GetTmpPath(void)
{
	static char path[1024];
#ifdef WIN32
	DWORD res = GetTempPath(1024, path);
	char* s = strrchr(path, '\\');
	if   (s!=0)
		*(s+1)   =   '\0';
#elif defined _LINUX
	sprintf(path, "%s", "/tmp/");
#endif
	return path;
}

const char* newGUID()
{
	static char buf[64] = {0};
#ifdef WIN32
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		_snprintf(buf, sizeof(buf)
			, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
			, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
			, guid.Data4[6], guid.Data4[7]
			);
	}
#elif defined _LINUX
	memset(buf, 0, 64);
	uuid_t uu;
    uuid_generate(uu);
	uuid_unparse(uu, buf);
#endif
	return (const char*)buf;
}

bool DirExists(char *szPathName)
{
#ifdef WIN32
	return PathIsDirectory(szPathName) == TRUE?true:false;
#elif defined _LINUX
	return access(szPathName, 0) == 0?true:false;
#endif
}

bool CreateDir(char *szPathName)
{
#ifdef WIN32
	return CreateDirectory(szPathName, NULL) == TRUE?true:false;
#elif defined _LINUX
	return mkdir(szPathName, 666) == 0?true:false;
#endif
}