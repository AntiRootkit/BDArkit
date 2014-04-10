// BDKitApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WINDOWS.H>
#include <winioctl.h>
#include "BDKitUtils\BDKitUtils.h"
#include "Interface\Interface.hpp"
#include "Utils\Console.hpp"
#include "Interface\BDarKitData.hpp"

#define MAX_FILE_PATH	250

#ifdef _UNICODE
#define _tcout cout
#else
#define _tcout wcout
#endif // _UNICODE

VOID Usage()
{
	_tprintf(_T("Usage:\tBDKitApp [Options] FilePath|ID [Options2]\r\n"));
	_tprintf(_T("\tdf:\tFocus Delete File.\r\n"));
	_tprintf(_T("\tdfb:\tDelete File On Reboot.\r\n"));
	_tprintf(_T("\tkp:\tFocus Kill Process with ProcessId.(Using OpenProcess Oper)\r\n"));
	_tprintf(_T("\tkpid:\tFocus Kill Process With ProcessId.\r\n"));
	_tprintf(_T("\tqf:\tFocusQuery Directory Files With RegExp.\r\n"));
	_tprintf(_T("\trf:\tFocus Read File untill \\0 terminated.\r\n"));
	_tprintf(_T("\tcpb\tCopy File On Reboot.\r\n"));
	_tprintf(_T("\tcp:\tCopy File.\r\n"));
	_tprintf(_T("\tparse:\tParse the BDArKit.dat.\r\n"));
	_tprintf(_T("\r\n"));

}

LPCTSTR	g_TestRunCommand = _T("testRun");

int _tmain(int argc, _TCHAR* argv[])
{
	ULONG	bytesReturn = 0;
	HANDLE	hDevice		= NULL;
	PVOID	pInData		= NULL;
	ULONG	ulInLength	= 0;
	ULONG	ulIOCTL		= 0;
	WCHAR	wszPath[MAX_PATH] = L"\0";

	HMODULE hInstance	= NULL;

	do 
	{
		GetUtilsLocatorPath (wszPath, sizeof(wszPath) / sizeof(TCHAR));
		_tprintf(_T("BDKitUtils->%s\r\n"), wszPath);

		if ( argc == 1 )
		{
			Usage ();
			break;
		}
		
		hInstance = LoadLibrary(wszPath);
		if ( hInstance == NULL )
		{
			break;
		}

		if ( wcscmp (argv[1], L"parse") == 0) 
		{
			tstring strDataPath;
			if (argv[2] == NULL) 
			{
				TCHAR wszPath[MAX_PATH] = {0x00};
				GetWindowsDirectory(wszPath, MAX_PATH);
				strDataPath.assign (wszPath);
				if (strDataPath.at (strDataPath.length () - 1) != _T('\\'))
				{
					strDataPath.append (_T("\\"));
				}
				strDataPath.append (_T("System32\\Drivers\\BDArKit.dat"));
			}
			else
			{
				strDataPath.assign (argv[2]);
			}
			ParseBDArKitRebootFile(strDataPath.c_str ());
			break;
		}

		pfnGetBDKitInterface pKitFace = (pfnGetBDKitInterface)GetProcAddress (hInstance, "GetBDKitInterface");
		if ( pKitFace == NULL )
		{
			break;
		}

		IKernelUtils* pSysKit = pKitFace ();

		HRESULT	hr = pSysKit->Init (NULL);
		if ( hr != ERROR_SUCCESS )
		{
			printf("Init Kernel Utils Failed %p [%u]\r\n", hr, GetLastError());
			break;
		}

		printf("Init Kernel Utils Success\r\n");

		if ( wcscmp (argv[1], L"df") == 0 )
		{
			BOOL bPrevent = FALSE;
			if ( argv[3] == NULL )
			{
				bPrevent = TRUE;
			}
			else
			{
				bPrevent = BOOL(_ttoi(argv[3]));
			}
			pSysKit->DeleteFileW (argv[2], bPrevent);

		} else if ( wcscmp (argv[1], L"cp") == 0 ) 
		{
			BOOL bRebootDelete = TRUE;
			if ( argv[3] == NULL ) 
			{
				bRebootDelete = TRUE;
			}
			else
			{
				bRebootDelete = BOOL(_ttoi(argv[3]));
			}
			pSysKit->DeleteFileOnRebootW (argv[2], bRebootDelete);

		} else if ( wcscmp (argv[1], L"cp") == 0 )
		{
			pSysKit->CopyFile (argv[2], argv[3], TRUE);
			printf("%d\r\n", GetLastError());

		} else if ( wcscmp (argv[1], L"mv") == 0 )
		{
			pSysKit->MoveFile (argv[2], argv[3]);

		} else if ( wcscmp(argv[1], L"kp") == 0 )
		{
			HANDLE hProcess = pSysKit->OpenProcess (PROCESS_TERMINATE, FALSE, _ttoi(argv[2]));
			if ( hProcess != NULL && hProcess != INVALID_HANDLE_VALUE )
			{
				pSysKit->TerminateProcess (hProcess, 0);
				CloseHandle (hProcess);
			}
			else
			{
				_tprintf(_T("OpenProcess Failed %x %u\r\n"), hProcess, GetLastError());
			}

		} else if ( wcscmp(argv[1], L"kpid") == 0 )
		{
			BOOL bResult = pSysKit->TerminateProcessId (_ttoi(argv[2]), 0);
			if ( bResult == FALSE )
			{
				_tprintf(_T("Terminate Process Id Failed %u\r\n"), GetLastError());
			}

		} else if ( wcscmp(argv[1], L"qf") == 0 )
		{
			WIN32_FIND_DATAW	win32Data = {0x00};
			_tprintf(_T("Query Directory:%s\r\n"), argv[2]);
			HANDLE hFile = pSysKit->FindFirstFileW (argv[2], &win32Data);
			if ( hFile != INVALID_HANDLE_VALUE )
			{
				_tprintf(_T("FILE:%s\r\n"), win32Data.cFileName);
				while(TRUE)
				{
					ZeroMemory(&win32Data, sizeof(win32Data));
					pSysKit->FindNextFileW (hFile, &win32Data);
					if ( GetLastError() == ERROR_NO_MORE_FILES)
					{
						break;
					}
					_tprintf(_T("FILE[%u]:%s\r\n"), GetLastError(), win32Data.cFileName);
				}
			}
			else
			{
				printf("Find Filed Failed %u\r\n", GetLastError());
			}

			pSysKit->FindClose (hFile);

		} else if ( wcscmp (argv[1], L"rf") == 0 )
		{
			HANDLE hFile = pSysKit->CreateFileW (
				argv[2],
				FILE_GENERIC_READ&~SYNCHRONIZE,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_SEQUENTIAL_SCAN|FILE_FLAG_OVERLAPPED,
				NULL);
			if ( hFile != INVALID_HANDLE_VALUE )
			{
				// TODO read file
				CHAR		buffer[1024]	= {0x00};
				OVERLAPPED	overLap			= {0x00};
				ULONG		ulBytesToRead	= 0;

				overLap.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
				overLap.Offset=0;
				pSysKit->ReadFile (hFile, buffer, sizeof(buffer), &ulBytesToRead, &overLap);
				if ( GetLastError() == ERROR_IO_PENDING )
				{
					if(WaitForSingleObject(overLap.hEvent,2000)!=WAIT_OBJECT_0)
						printf ("waitfor failed\r\n");
					DWORD	byTransfer = 0;
					if ( !GetOverlappedResult(hFile,&overLap,&byTransfer, FALSE) )
						printf ("GetOverlapped Failed\r\n");
				}
				ResetEvent(overLap.hEvent);
				printf("[%u]%s\r\n", ulBytesToRead, buffer);
				pSysKit->CloseHandle (hFile);
				CloseHandle (overLap.hEvent);
			}

		} else if ( wcscmp (argv[1], L"wf") == 0 )
		{
			HANDLE hFile = pSysKit->CreateFileW (
				argv[2],
				FILE_GENERIC_READ&~SYNCHRONIZE,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_SEQUENTIAL_SCAN|FILE_FLAG_OVERLAPPED,
				NULL);
			if ( hFile != INVALID_HANDLE_VALUE )
			{
				TCHAR		buffer[1024]	= {0x00};
				OVERLAPPED	overLap			= {0x00};
				ULONG		ulBytesToWrite	= 0;

				if (argv[3] != NULL)
				{
					memcpy(buffer, argv[3], _tcslen(argv[3]) * sizeof(TCHAR));
				}
				else
				{
#define TEST_STRING _T("This is test string!")
					memcpy(buffer, TEST_STRING, _tcslen(TEST_STRING) * sizeof(TCHAR));
				}
				overLap.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
				overLap.Offset = 0;
				pSysKit->WriteFile (hFile, buffer, (_tcslen (buffer) + sizeof(TCHAR)) * sizeof(TCHAR), &ulBytesToWrite, &overLap);
				if ( GetLastError() == ERROR_IO_PENDING )
				{
					if (WaitForSingleObject (overLap.hEvent, 2000) != WAIT_OBJECT_0 )
						printf("waitfor failed\r\n");
					DWORD byTransfer = 0;
					if ( !GetOverlappedResult (hFile, &overLap, &byTransfer, FALSE))
						printf ("GetOverlapped Failed\r\n");
				}
				ResetEvent (overLap.hEvent);
				pSysKit->CloseHandle (hFile);
				CloseHandle (overLap.hEvent);
			}

		} else if ( wcscmp (argv[1], L"dfb") == 0 )
		{
			BOOL bAdd = FALSE;
			if ( argv[3] == NULL )
			{
				bAdd = TRUE;
			}
			else
			{
				bAdd = BOOL(_ttoi(argv[3]));
			}
			pSysKit->DeleteFileOnRebootW (argv[2], bAdd);

		} else if ( wcscmp (argv[1], L"cpb") == 0 )
		{
			BOOL bAdd = FALSE;
			if ( argv[4] == NULL )
			{
				bAdd = TRUE;
			}
			else
			{
				bAdd = BOOL(_ttoi(argv[4]));
			}
			pSysKit->CopyFileOnReboot (argv[2], argv[3], bAdd);
		}

		pSysKit->UnInit (NULL);

	} while (FALSE);

	if ( hInstance != NULL )
	{
		FreeLibrary (hInstance);
	}

	//system ("pause");

	return 0;
}