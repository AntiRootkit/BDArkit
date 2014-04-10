#pragma once


class IKernelUtils
{
public:

	virtual ULONG STDMETHODCALLTYPE AddRef(void){ return 0;}
	virtual ULONG STDMETHODCALLTYPE Release(void) { return 0;}
	virtual HRESULT STDMETHODCALLTYPE Init(void* pvContext = NULL) {return KernelInitialize();}
	virtual HRESULT STDMETHODCALLTYPE UnInit(void* pvContext = NULL) 
	{
		BOOL bNeedUninstall = FALSE;
		if ( pvContext != NULL )
		{
			PBOOL pbContext = static_cast<PBOOL>(pvContext);
			bNeedUninstall  = *pbContext;
		}
		return KernelUnInitialize(bNeedUninstall);
	}

	// File operations
	virtual HANDLE WINAPI CreateFileW(
		__in          LPCWSTR lpFileName,
		__in          DWORD dwDesiredAccess,
		__in          DWORD dwShareMode,
		__in          LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		__in          DWORD dwCreationDisposition,
		__in          DWORD dwFlagsAndAttributes,
		__in          HANDLE hTemplateFile
		) = 0;
	virtual BOOL WINAPI ReadFile(
		__in          HANDLE hFile,
		__out         LPVOID lpBuffer,
		__in          DWORD nNumberOfBytesToRead,
		__out         LPDWORD lpNumberOfBytesRead,
		__in          LPOVERLAPPED lpOverlapped
		) = 0;
	virtual BOOL WINAPI WriteFile(
		__in          HANDLE hFile,
		__in          LPCVOID lpBuffer,
		__in          DWORD nNumberOfBytesToWrite,
		__out         LPDWORD lpNumberOfBytesWritten,
		__in          LPOVERLAPPED lpOverlapped
		) = 0;
	virtual HANDLE WINAPI FindFirstFileW(
		__in          LPCWSTR lpFileName,
		__out         LPWIN32_FIND_DATA lpFindFileData
		) = 0;
	virtual BOOL WINAPI FindNextFileW(
		__in          HANDLE hFindFile,
		__out         LPWIN32_FIND_DATA lpFindFileData
		) = 0;
	virtual HANDLE WINAPI FindFirstFileExW(
		__in          LPCWSTR lpFileName,
		__in          FINDEX_INFO_LEVELS fInfoLevelId,
		__out         LPVOID lpFindFileData,
		__in          FINDEX_SEARCH_OPS fSearchOp,
		__in          LPVOID lpSearchFilter,
		__in		  DWORD dwAdditionalFlags
		) = 0;
	virtual BOOL WINAPI FindClose(
		__in		  HANDLE hFindFile
		) = 0;
	virtual BOOL WINAPI CloseHandle(
		__in          HANDLE hObject
		) = 0;
	virtual BOOL WINAPI DeleteFileW(
		__in          LPCWSTR lpFileName,
		__in		  BOOL	  bPreventCreateFile  = FALSE 
		) = 0;
	virtual BOOL WINAPI DeleteFileOnRebootW(
		__in		  LPCWSTR lpFileName,
		__in		  BOOL	  bDeleteOnReboot  = TRUE 
		) = 0;

	// Process operations
	virtual HANDLE WINAPI OpenProcess(
		__in          DWORD dwDesiredAccess,
		__in          BOOL bInheritHandle,
		__in          DWORD dwProcessId
		) = 0;
	virtual BOOL WINAPI TerminateProcess(
		__in          HANDLE hProcess,
		__in          UINT uExitCode
		) = 0;
	virtual BOOL WINAPI TerminateProcessId(
		__in          DWORD dwProcessId,
		__in          UINT uExitCode
		) = 0;

	// Registry operations
	virtual LONG WINAPI RegCreateKeyW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey,
		__out         PHKEY phkResult
		) = 0;
	virtual LONG WINAPI RegCreateKeyExW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey,
		DWORD Reserved,
		__in          LPTSTR lpClass,
		__in          DWORD dwOptions,
		__in          REGSAM samDesired,
		__in          LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		__out         PHKEY phkResult,
		__out         LPDWORD lpdwDisposition
		) = 0;
	virtual LONG WINAPI RegOpenKeyW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey,
		__out         PHKEY phkResult
		) = 0;
	virtual LONG WINAPI RegOpenKeyExW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey,
		DWORD ulOptions,
		__in          REGSAM samDesired,
		__out         PHKEY phkResult
		) = 0;
	virtual LONG WINAPI RegCloseKey(
		__in          HKEY hKey
		) = 0;
	virtual LONG WINAPI RegDeleteKeyW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey
		) = 0;
	virtual LONG WINAPI RegDeleteKeyExW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey,
		__in          REGSAM samDesired,
		DWORD Reserved
		) = 0;
	virtual LONG WINAPI RegDeleteValueW(
		__in          HKEY hKey,
		__in          LPCWSTR lpValueName
		) = 0;
	virtual LONG WINAPI RegSetValueExW(
		__in          HKEY hKey,
		__in          LPCWSTR lpValueName,
		DWORD Reserved,
		__in          DWORD dwType,
		__in          const BYTE* lpData,
		__in          DWORD cbData
		) = 0;
	virtual LONG WINAPI RegQueryValueExW(
		__in          HKEY hKey,
		__in          LPCWSTR lpValueName,
		LPDWORD lpReserved,
		__out         LPDWORD lpType,
		__out         LPBYTE lpData,
		__out		  LPDWORD lpcbData
		) = 0;
	virtual LONG WINAPI RegEnumKeyW(
		__in          HKEY hKey,
		__in          DWORD dwIndex,
		__out         LPWSTR lpName,
		__in          DWORD cchName
		) = 0;
	virtual LONG WINAPI RegEnumKeyExW(
		__in          HKEY hKey,
		__in          DWORD dwIndex,
		__out         LPWSTR lpName,
		__out		  LPDWORD lpcName,
		LPDWORD lpReserved,
		__out		  LPWSTR lpClass,
		__out		  LPDWORD lpcClass,
		__out         PFILETIME lpftLastWriteTime
		) = 0;
	virtual LONG WINAPI RegEnumValueW(
		__in          HKEY hKey,
		__in          DWORD dwIndex,
		__out         LPWSTR lpValueName,
		__out		  LPDWORD lpcchValueName,
		LPDWORD lpReserved,
		__out         LPDWORD lpType,
		__out         LPBYTE lpData,
		__out		  LPDWORD lpcbData
		) = 0;

		// Objects operation
		virtual LONG WINAPI ZwQueryObject (
		__in		  HANDLE ObjectHandle,
		__in		  DWORD ObjectInformationClass,
		__out		  PVOID ObjectInformation,
		__in		  ULONG Length,
		__out		  PULONG ResultLength
		) = 0;

		virtual BOOL WINAPI CopyFileEx( 
			__in		  LPCTSTR lpExistingFileName, 
			__in		  LPCTSTR lpNewFileName, 
			__in		  LPPROGRESS_ROUTINE lpProgressRoutine, 
			__in		  LPVOID lpData, 
			__in		  LPBOOL pbCancel, 
			__in		  DWORD dwCopyFlags 
			) = 0;
		virtual BOOL WINAPI MoveFileEx(
			__in          LPCTSTR lpExistingFileName,
			__in          LPCTSTR lpNewFileName,
			__in          DWORD dwFlags
			) = 0;
		virtual BOOL WINAPI CopyFile(
			__in          LPCTSTR lpExistingFileName,
			__in          LPCTSTR lpNewFileName,
			__in          BOOL bFailIfExists
			) = 0;
		virtual BOOL WINAPI MoveFile(
			__in          LPCTSTR lpExistingFileName,
			__in          LPCTSTR lpNewFileName
			) = 0;
		virtual BOOL WINAPI LockFile(
			__in		  LPCTSTR lpFileName) = 0;
		virtual BOOL WINAPI unLockFile(
			__in		  LPCTSTR lpFileName) = 0;
		virtual BOOL WINAPI CopyFileOnReboot(
			__in		  LPCTSTR lpExistingFileName, 
			__in		  LPCTSTR lpNewFileName,
			__in		  BOOL	  bCopyOnReboot = TRUE
			) = 0;

protected:

	virtual HRESULT KernelInitialize() = 0;
	virtual HRESULT KernelUnInitialize(BOOL bNeedUninstall) = 0;
};

#ifdef BDKITDYNAMIC
#define DLL_DEFINE __declspec(dllexport)
#else
#define DLL_DEFINE __declspec(dllimport)
#endif

typedef IKernelUtils* (*pfnGetBDKitInterface) (VOID);
extern "C" DLL_DEFINE IKernelUtils* GetBDKitInterface(VOID);