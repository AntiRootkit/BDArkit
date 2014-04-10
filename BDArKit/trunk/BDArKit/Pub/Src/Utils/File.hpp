#pragma once


#include <WINDOWS.H>
#include <shlwapi.h>
#include "process.hpp"


#pragma comment(lib, "shlwapi.lib")

extern LPCTSTR	g_TestRunCommand;

namespace FileTools
{
	enum FileLockType
	{
		emAddFileRef	= 0,
		emLockFile		= 1,
		emRunFile		= 2,
		emNoShareFile	= 3,
		emEndLockFile,
	};

	DWORD FileIsExist(tstring strFilePath)
	{
		HANDLE hFile = NULL;

		hFile = CreateFile (
			strFilePath.c_str (), 
			FILE_READ_DATA,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if ( hFile == INVALID_HANDLE_VALUE || hFile == NULL )
		{
			return ERROR_FILE_NOT_FOUND;
		}
		else
		{
			CloseHandle (hFile);
			return ERROR_ALREADY_EXISTS;
		}
	}

	DWORD TryToCreateTestFile(tstring strFilePath)
	{
		HANDLE hFile = CreateFile(
			strFilePath.c_str(),
			FILE_READ_DATA,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if ( hFile != INVALID_HANDLE_VALUE )
		{
			CloseHandle (hFile);
			return ERROR_SUCCESS;
		}

		return GetLastError();
	}

	template<size_t nSize>  
	inline void ModifyPathSpec( TCHAR (&szDst)[nSize], BOOL  bAddSpec )  
	{  
		int nLen = lstrlen( szDst );  

		TCHAR  ch  = szDst[ nLen - 1 ];  
		if( ( ch == _T('\\') ) )  
		{  
			if( !bAddSpec )  
			{  
				szDst[ nLen - 1 ] = _T('\0');  
			}  
		}  
		else  
		{  
			if( bAddSpec )  
			{  
				szDst[ nLen ] = _T('\\');  
				szDst[ nLen + 1 ] = _T('\0');  
			}  
		}  
	} 

	BOOL  CreateDirectoryNested(tstring lpszDir)  
	{  
		if( ::PathIsDirectory(lpszDir.c_str ()) ) return TRUE;  

		TCHAR   szPreDir[ MAX_PATH ];  
		_tcscpy_s(szPreDir, lpszDir.c_str ());

		//确保路径末尾没有反斜杠  
		ModifyPathSpec( szPreDir, FALSE );  

		//获取上级目录  
		BOOL  bGetPreDir  = ::PathRemoveFileSpec( szPreDir );  
		if( !bGetPreDir ) return FALSE;  

		//如果上级目录不存在,则递归创建上级目录  
		if( !::PathIsDirectory(szPreDir) )  
		{  
			CreateDirectoryNested(szPreDir);  
		}  

		return ::CreateDirectory(lpszDir.c_str (), NULL);  
	}

	BOOL RemoveDirecotryNested(tstring szFileDir)
	{
		tstring strDir = szFileDir;

		if (strDir.at(strDir.length()-1) != '\\')
		{
			strDir += '\\';

		}

		WIN32_FIND_DATA wfd;
		HANDLE hFind = FindFirstFile((strDir + _T("*.*")).c_str(), &wfd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		do
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{

				if (_tcsicmp(wfd.cFileName, _T(".")) != 0 &&
					_tcsicmp(wfd.cFileName, _T("..")) != 0)
				{
					RemoveDirecotryNested((strDir + wfd.cFileName).c_str());
				}
			}
			else
			{
				DeleteFile( (strDir + wfd.cFileName).c_str());
			}
		}
		while (FindNextFile(hFind, &wfd));

		FindClose(hFind);
		::RemoveDirectory(szFileDir.c_str ());

		return TRUE;
	} 

	BOOL LockItFile(tstring strFilePath, FileLockType lockType)
	{
		HANDLE hFile = CreateFile(
			strFilePath.c_str (),
			FILE_READ_DATA,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if ( hFile == INVALID_HANDLE_VALUE )
		{
			return FALSE;
		}

		switch(lockType)
		{
		case emAddFileRef:
			{
				for (ULONG ulLockNums = 0; ulLockNums < 100; ++ulLockNums)
				{
					CreateFile(
						strFilePath.c_str (),
						FILE_READ_DATA,
						FILE_SHARE_READ,
						NULL,
						OPEN_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL
						);
				}
			}break;

		case emLockFile:
			{
				LockFile (hFile,0, 0, GetFileSize(hFile, NULL), 0);
			} break;

		case emNoShareFile:
			{
				CreateFile(
					strFilePath.c_str (),
					FILE_READ_DATA,
					0,
					NULL,
					OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL
					);
			}break;
		case emRunFile:
			{
				PTSTR		pCmdline	= NULL;
				STARTUPINFO si			= {sizeof(si)};
				PROCESS_INFORMATION	pi	= {0x00};

				si.dwFlags = STARTF_USESHOWWINDOW;   
				si.wShowWindow = TRUE;

				ProcessTools::RunWithProcess (strFilePath, tstring(g_TestRunCommand));
			}
		}

		CloseHandle (hFile);

		return TRUE;
	}

	class CWow64RedirDisable
	{
	public:
		CWow64RedirDisable(bool bAutoDisable = false);
		~CWow64RedirDisable();

		bool DisableRedirection();
		bool Revert();

	private:
		PVOID m_OldValue;
	};

	static FILETIME fileTimeFromLargeInteger(const LARGE_INTEGER& llValue)
	{
		FILETIME ft = { llValue.LowPart, llValue.HighPart };
		return ft;
	}

	static void* GetProcAddress( LPCWSTR szLoadedModuleName, LPCSTR szProcName )
	{
		HMODULE h( GetModuleHandle(szLoadedModuleName) );
		if( h == NULL )
			return h;

		return ::GetProcAddress( h, szProcName );
	}

#define DEFINE_LOADED_PROC( ModuleName, RetType, CallType, ProcName, Params )  \
	typedef RetType (CallType *ProcName##Type) Params;      \
	static const ProcName##Type p##ProcName( reinterpret_cast<ProcName##Type>(GetProcAddress(ModuleName,#ProcName)) )

	DEFINE_LOADED_PROC( L"kernel32.dll", BOOL, WINAPI, IsWow64Process, (HANDLE,PBOOL) );

	DEFINE_LOADED_PROC( L"kernel32.dll", BOOL, WINAPI, Wow64DisableWow64FsRedirection, (PVOID*) );
	DEFINE_LOADED_PROC( L"kernel32.dll", BOOL, WINAPI, Wow64RevertWow64FsRedirection, (PVOID) );

	CWow64RedirDisable::CWow64RedirDisable( bool bAutoDisable /*= false*/ )
		: m_OldValue(NULL)
	{
		if( bAutoDisable )
			DisableRedirection();
	}

	CWow64RedirDisable::~CWow64RedirDisable()
	{
		Revert();
	}

	bool CWow64RedirDisable::DisableRedirection()
	{
		if( pWow64DisableWow64FsRedirection == NULL )
			return false;
		return pWow64DisableWow64FsRedirection(&m_OldValue) != FALSE;
	}

	bool CWow64RedirDisable::Revert()
	{
		if( pWow64RevertWow64FsRedirection == NULL )
			return false;
		return pWow64RevertWow64FsRedirection(m_OldValue) != FALSE;
	}

#define IDR_DRIVER_32                   10100
#define IDR_DRIVER_64                   10101

	BOOL Is32Bit();
	BOOL Is64Bit();

	BOOL Is32Bit()
	{
		return !Is64Bit ();
	}

	BOOL Is64Bit()
	{
#ifdef _WIN64
		return TRUE;
#else
		static bool g_bWow64Initialized( false );
		static bool g_bIs64Bit( false );

		if( g_bWow64Initialized )
			return g_bIs64Bit;

		if( pIsWow64Process == NULL )
			return FALSE;

		BOOL bIsWow64( FALSE );
		if( pIsWow64Process(GetCurrentProcess(),&bIsWow64) == FALSE )
			return FALSE;

		g_bIs64Bit = bIsWow64 != FALSE;
		g_bWow64Initialized = true;
		return g_bIs64Bit;
#endif
	}
};