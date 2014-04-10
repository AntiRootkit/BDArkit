#pragma once


#include <WINDOWS.H>
#include <string.h>


static CONST TCHAR g_ProductRegPath[]	= _T("SOFTWARE\\Baidu\\BaiduSd\\");
static CONST TCHAR g_UtilsName[]		= _T("BDKitUtils.dll");

BOOL GetProductInstallPath(LPTSTR pProductPath, DWORD cbSize)
{
	LONG	errCode = 0;
	HKEY	hKey	= NULL;

	do 
	{
		errCode = RegOpenKeyExW(HKEY_LOCAL_MACHINE, g_ProductRegPath, 0, KEY_ALL_ACCESS, &hKey);
		if ( errCode != ERROR_SUCCESS )
		{
			break;
		}

		DWORD dwType = REG_SZ;
		DWORD dwBytesReturn = cbSize;
		errCode = RegQueryValueEx(hKey, _T("InstallDir"), NULL, &dwType, (LPBYTE)pProductPath, &dwBytesReturn);
		if ( errCode != ERROR_SUCCESS )
		{
			break;
		}

		_tcscat_s (pProductPath, cbSize, L"\\");

		dwType = REG_SZ;
		dwBytesReturn = cbSize - _tcslen(pProductPath) * sizeof(WCHAR);
		errCode = RegQueryValueEx(
			hKey, 
			_T("Version"), 
			NULL, 
			&dwType, 
			(LPBYTE)((LPTSTR)pProductPath + _tcslen(pProductPath)), 
			&dwBytesReturn);
		if ( errCode != ERROR_SUCCESS )
		{
			break;
		}


	} while (FALSE);

	CloseHandle (hKey);

	return errCode == ERROR_SUCCESS;
}

DWORD GetUtilsLocatorPath(LPTSTR pDllPath, ULONG cbSize)
{
	if (GetProductInstallPath(pDllPath, cbSize) == FALSE)
	{
		GetModuleFileName(NULL, pDllPath, cbSize);
		LPTSTR pMover = pDllPath + _tcslen (pDllPath);
		while(*pMover != L'\\' && pMover > pDllPath)
		{
			--pMover;
		}
		*pMover = L'\0';
	}
	else
	{
		_tcscat_s (pDllPath, cbSize, L"\\bdmantivirus");
	}

	if ( pDllPath[_tcslen (pDllPath) - 1] != L'\\' )
	{
		_tcscat_s (pDllPath, cbSize, L"\\");
	}
	_tcscat_s (pDllPath, cbSize, g_UtilsName);

	return ERROR_SUCCESS;
}