// BDKitGTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WINDOWS.H>
#include "gtest\gtest.h"
#ifdef _DEBUG
#include "BDKitUtils\BDKitUtils.h"
#else
#include "E:\Code_Doc_Store\Company\Baidu\Src\BDAVClient_Trunk\Basic\include\ftInclude\BDDrvHelper\BDKitUtils.h"
#endif

#include "Interface\Interface.hpp"
#include "Utils\SystemTools.hpp"
#include "FileTest.hpp"
#include "ProcessTest.hpp"


#ifdef _DEBUG
#pragma comment(lib, "../Pub/DynamicLibrary/Debug/gtestd.lib")
#else
#pragma comment(lib, "../Pub/DynamicLibrary/Release/gtest.lib")
#endif

IKernelUtils*	g_pSysKit = NULL;
tstring			g_strModulePath;
tstring			g_strModuleFileFullName;

static CONST TCHAR g_ExitProcessEventName[] = _T("{F377EE55-A6A0-4e16-B718-C9112DDEF9BA}_GTEST_EXIT");
LPCTSTR	g_TestRunCommand = _T("testRun");
HANDLE	g_ExitSubProcess = NULL;

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR			wszPath[MAX_PATH] = L"\0";
	HMODULE			hUtils		= NULL;
	DWORD			dwErrorCode	= ERROR_SUCCESS;

	
	do 
	{
		if ( argc == 2 && wcscmp(argv[1], L"testRun") == 0 )
		{
			HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, g_ExitProcessEventName);
			if (hEvent != NULL)
			{
				WaitForSingleObject (hEvent, INFINITE);
			}
			return 0;	
		}

		//if (!SystemTools::EnableDriverMode (tstring(BDArKit_DRIVER_NAMEW), 1, TRUE)) {
		//	_tprintf(_T("Enable BDArKit Debug Mode Failed!, please Check self-protect!\r\n"));
		//	break;
		//} else {
		//	_tprintf(_T("Enable BDArKit Debug Mode needs reboot to make it effective!!\r\nIf you have reboot and last Debug Mode setting is successful, please ignore!!\r\n"));
		//	system ("pause");
		//}
		
		g_ExitSubProcess = CreateEvent(NULL, TRUE, FALSE, g_ExitProcessEventName);

		testing::InitGoogleTest (&argc, argv);

		GetModuleFileName(NULL, wszPath, MAX_PATH);
		g_strModuleFileFullName.assign (wszPath);
		g_strModulePath.assign (wszPath);
		g_strModulePath.erase (g_strModulePath.rfind (L'\\') + 1, g_strModulePath.length () - 1);

		memset(wszPath, 0, sizeof(wszPath));
		dwErrorCode = GetUtilsLocatorPath (wszPath, sizeof(wszPath) / sizeof(TCHAR));
		if ( dwErrorCode != ERROR_SUCCESS )
		{
			break;
		}

		_tprintf(_T("BDKitUtils->%s\r\n"), wszPath);

		pfnGetBDKitInterface pfnSysKit = NULL;
		hUtils = LoadLibrary(wszPath);
		if ( hUtils == NULL )
		{
			printf("loadlibrary Failed [%u]\r\n", GetLastError());
			dwErrorCode = GetLastError();
			break;
		}
		pfnSysKit = (pfnGetBDKitInterface)GetProcAddress (hUtils, "GetBDKitInterface");
		if ( pfnSysKit == NULL )
		{
			dwErrorCode = GetLastError();
			break;
		}
		g_pSysKit = pfnSysKit();

		g_pSysKit->Init (NULL);

		RUN_ALL_TESTS();

	} while (FALSE);

	if ( g_pSysKit != NULL )
	{
		g_pSysKit->UnInit (NULL);
	}

	if ( hUtils != NULL )
	{
		FreeLibrary (hUtils);
	}

	system ("pause");

	return 0;
}