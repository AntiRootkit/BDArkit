#pragma once


#include <WINDOWS.H>
#include "Utils\String.hpp"
#include "BDArKit\KitHelper.h"


#pragma comment(lib, "Advapi32.lib")


namespace SystemTools
{
#define SERVICE_CONTROL_START	0x00000000
	DWORD ControlService(tstring strServiceName, ULONG ulServiceControl)
	{
		SC_HANDLE	schSCManger	= NULL;
		SC_HANDLE	schService	= NULL;

		if (strServiceName.empty())
		{
			return ERROR_INVALID_PARAMETER;
		}

		DWORD	dwError = ERROR_SUCCESS;
		do 
		{
			schSCManger = OpenSCManager(
				NULL,
				NULL,
				SC_MANAGER_ALL_ACCESS);
			if (schSCManger == NULL)
			{
				dwError = GetLastError();
				break;
			}

			schService = OpenService(schSCManger, strServiceName.c_str(), SERVICE_ALL_ACCESS);
			if (schService == NULL)
			{
				dwError = GetLastError();
				break;
			}

			BOOL bResult = FALSE;
			if (ulServiceControl != SERVICE_CONTROL_START)
			{
				SERVICE_STATUS	serviceStatus = {0x00};
				bResult = ControlService (schService, ulServiceControl, &serviceStatus);
				if (bResult == FALSE)
				{
					dwError = GetLastError();
					if (dwError != ERROR_SERVICE_NOT_ACTIVE &&
						dwError != ERROR_SHUTDOWN_IN_PROGRESS)
					{
						break;
					}
					dwError = ERROR_SUCCESS;
				}
			}
			else
			{
				bResult = StartService(schService, 0, NULL);
				if (bResult == FALSE)
				{
					dwError = GetLastError();
					if (dwError != ERROR_SERVICE_ALREADY_RUNNING)
					{
						break;
					}
					dwError = ERROR_SUCCESS;
				}
			}

		} while (FALSE);

		if (schService != NULL)
		{
			CloseServiceHandle (schService);
		}
		if (schSCManger != NULL)
		{
			CloseServiceHandle (schSCManger);
		}

		return dwError;
	}
	
	BOOL EnableDriverMode(tstring DriverName, ULONG ulFlag, BOOL bCombine)
	{
		CONST TCHAR ServiceKey[]	= _T("SYSTEM\\CurrentControlSet\\services\\");
		HKEY		hKey			= NULL;
		LONG		lResult			= ERROR_FUNCTION_FAILED;
		tstring		DriverKey;

		DriverKey.assign (ServiceKey);
		DriverKey += DriverName;

		do 
		{
			lResult = RegOpenKeyEx(
				HKEY_LOCAL_MACHINE,
				DriverKey.c_str (),
				0,
				KEY_ALL_ACCESS,
				&hKey);
			if (lResult != ERROR_SUCCESS) {
				break;
			}

			DWORD dwFlag = 0;
			DWORD dwType = 0;
			DWORD cbData = sizeof(dwFlag);
			lResult = RegQueryValueEx(hKey, BDArKit_Debug_Mode, 0, &dwType, reinterpret_cast<LPBYTE>(&dwFlag), &cbData);
			if (bCombine == TRUE) {
				dwFlag |= ulFlag;
			} else {
				dwFlag = ulFlag;
			}

			lResult = RegSetValueEx(hKey, BDArKit_Debug_Mode, 0, REG_DWORD, reinterpret_cast<LPBYTE>(&dwFlag), sizeof(dwFlag));
			if (lResult != ERROR_SUCCESS) {
				break;
			}
		} while (FALSE);

		if (hKey != NULL) {
			RegCloseKey(hKey);
			hKey = NULL;
		}

		return lResult == ERROR_SUCCESS? TRUE : FALSE;
	}
};