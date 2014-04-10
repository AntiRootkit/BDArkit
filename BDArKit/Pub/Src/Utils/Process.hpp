#pragma once


#include <WINDOWS.H>
#include <Tlhelp32.h>
#include "Utils/String.hpp"


namespace ProcessTools
{
	BOOL KillProcessByName(tstring& strProcessName)
	{
		PROCESSENTRY32  pe	= {sizeof(pe)};
		HANDLE			hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		Process32First(hSnapshot, &pe);

		do{
			if( _tcsicmp(pe.szExeFile,strProcessName.c_str())==0 )
			{
				HANDLE hProcess = OpenProcess( PROCESS_TERMINATE , FALSE , pe.th32ProcessID );
				if ( hProcess != NULL )
				{
					TerminateProcess(hProcess, EXIT_SUCCESS);
					CloseHandle( hProcess );
				}
					
				break;
			}
		}while(Process32Next(hSnapshot, &pe));

		return TRUE;
	}

	BOOL RunWithProcess(tstring& strImagePath, tstring& strCommand)
	{
		PTSTR		pCmdline	= NULL;
		STARTUPINFO si			= {sizeof(si)};
		PROCESS_INFORMATION	pi	= {0x00};
		tstring		strRunImage;

		si.dwFlags = STARTF_USESHOWWINDOW;   
		si.wShowWindow = FALSE;
		strRunImage = strImagePath + tstring(_T(" ")) + strCommand;
		return CreateProcess(
			NULL, 
			const_cast<LPTSTR>(strRunImage.c_str ()), 
			NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	}

	DWORD GetProcessIDByName(tstring processName)
	{
		DWORD dwProcID = -1;
		do 
		{
			HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hProcSnap == INVALID_HANDLE_VALUE) {
				break;
			}

			PROCESSENTRY32 pe32= {0x00};
			pe32.dwSize = sizeof(PROCESSENTRY32);
			BOOL bFlags = Process32First(hProcSnap, &pe32);
			while(bFlags)
			{
				if (processName.compare(pe32.szExeFile) == 0) {
					dwProcID = pe32.th32ProcessID;
					break;
				}
				bFlags = Process32Next(hProcSnap, &pe32);
			}
			if (hProcSnap != INVALID_HANDLE_VALUE) {
				CloseHandle (hProcSnap);
				hProcSnap = NULL;
			}
		} while (FALSE);

		return dwProcID;
	}
};
