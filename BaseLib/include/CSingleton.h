/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CSingleton.h $
 *
 *  @brief 单件类模板
 *
 *  $Id: CSingleton.h 16 2008-11-18 11:36:40Z gang.chen $
 *
 *  @author gang chen <eyesfour@gmail.com>
 ***************************************************/
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#ifdef _WIN32
#define   WIN32_LEAN_AND_MEAN  
#include <windows.h>
#endif


/**
* @brief 单件类模板， TClass为具体的类
*/
template <typename TClass> 
class CSingleton
{
public:

	/**
	* @brief 取单件类实例，如没有创建，返回创建后的实例，如果已创建，直接返回已创建的实例
	*/
	static TClass* Instance()
	{
		if (NULL == g_Instance)
		{
			g_Instance = new TClass;
			g_nRef = 1;
		}
		else
		{
			g_nRef ++;
		}
		return g_Instance;
	};

	/**
	* @brief 销毁单件类实例，如果引用参考计数变为0,则真正销毁，否则只是减少计数
	*/
	static void DestroyInstance()
	{ 
		g_nRef--;
		if (g_nRef <= 0)
		{
			delete g_Instance;
			g_Instance = NULL;
		}
	};

protected:

	/**
	* @brief 单件类构造函数
	*/
	CSingleton()
	{
	};

	/**
	* @brief 单件类析构函数
	*/
	virtual ~CSingleton()
	{
	};

private:
	static TClass* g_Instance;
	static int g_nRef;
};


/**
* @brief 单件类实例
*/
template <typename TClass> TClass* CSingleton<TClass>::g_Instance = NULL;
/**
* @brief 单件类引用参考计数
*/
template <typename TClass> int CSingleton<TClass>::g_nRef = 0;

#endif
