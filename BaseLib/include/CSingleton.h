/****************************************************
 *  @file    $URL: file:///D:/SourceCode/MyPrj/SourceLib/TopViewXP/trunk/lib/BaseLib/include/CSingleton.h $
 *
 *  @brief ������ģ��
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
* @brief ������ģ�壬 TClassΪ�������
*/
template <typename TClass> 
class CSingleton
{
public:

	/**
	* @brief ȡ������ʵ������û�д��������ش������ʵ��������Ѵ�����ֱ�ӷ����Ѵ�����ʵ��
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
	* @brief ���ٵ�����ʵ����������òο�������Ϊ0,���������٣�����ֻ�Ǽ��ټ���
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
	* @brief �����๹�캯��
	*/
	CSingleton()
	{
	};

	/**
	* @brief ��������������
	*/
	virtual ~CSingleton()
	{
	};

private:
	static TClass* g_Instance;
	static int g_nRef;
};


/**
* @brief ������ʵ��
*/
template <typename TClass> TClass* CSingleton<TClass>::g_Instance = NULL;
/**
* @brief ���������òο�����
*/
template <typename TClass> int CSingleton<TClass>::g_nRef = 0;

#endif
