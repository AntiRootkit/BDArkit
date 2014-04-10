#pragma once


#include "BDKitUtils\BDKitUtils.h"
#include "tstring.h"


class CKernelUtils : public IKernelUtils
{
public:

	CKernelUtils();
	~CKernelUtils() throw();

	virtual HANDLE WINAPI CreateFileW(
		__in          LPCWSTR lpFileName,
		__in          DWORD dwDesiredAccess,
		__in          DWORD dwShareMode,
		__in          LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		__in          DWORD dwCreationDisposition,
		__in          DWORD dwFlagsAndAttributes,
		__in          HANDLE hTemplateFile
		);
	virtual BOOL WINAPI ReadFile(
		__in          HANDLE hFile,
		__out         LPVOID lpBuffer,
		__in          DWORD nNumberOfBytesToRead,
		__out         LPDWORD lpNumberOfBytesRead,
		__in          LPOVERLAPPED lpOverlapped
		);
	virtual BOOL WINAPI WriteFile(
		__in          HANDLE hFile,
		__in          LPCVOID lpBuffer,
		__in          DWORD nNumberOfBytesToWrite,
		__out         LPDWORD lpNumberOfBytesWritten,
		__in          LPOVERLAPPED lpOverlapped
		);
	virtual HANDLE WINAPI FindFirstFileW(
		__in          LPCWSTR lpFileName,
		__out         LPWIN32_FIND_DATA lpFindFileData
		);
	virtual BOOL WINAPI FindNextFileW(
		__in          HANDLE hFindFile,
		__out         LPWIN32_FIND_DATA lpFindFileData
		);
	virtual HANDLE WINAPI FindFirstFileExW(
		__in          LPCWSTR lpFileName,
		__in          FINDEX_INFO_LEVELS fInfoLevelId,
		__out         LPVOID lpFindFileData,
		__in          FINDEX_SEARCH_OPS fSearchOp,
		__in          LPVOID lpSearchFilter,
		__in		  DWORD dwAdditionalFlags
		);
	virtual BOOL WINAPI FindClose(
		__in		  HANDLE hFindFile
		);
	virtual BOOL WINAPI CloseHandle(
		__in          HANDLE hObject
		);
	virtual BOOL WINAPI DeleteFileW(
		__in          LPCWSTR lpFileName,
		__in		  BOOL	  bPreventCreateFile = FALSE
		);
	virtual BOOL WINAPI DeleteFileOnRebootW(
		__in		  LPCWSTR lpFileName,
		__in		  BOOL	  bDeleteOnReboot = TRUE 
		);

	virtual HANDLE WINAPI OpenProcess(
		__in          DWORD dwDesiredAccess,
		__in          BOOL bInheritHandle,
		__in          DWORD dwProcessId
		);
	virtual BOOL WINAPI TerminateProcess(
		__in          HANDLE hProcess,
		__in          UINT uExitCode
		);
	virtual BOOL WINAPI TerminateProcessId(
		__in          DWORD dwProcessId,
		__in          UINT uExitCode
		);
	virtual LONG WINAPI RegCreateKeyW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey,
		__out         PHKEY phkResult
		);
	virtual LONG WINAPI RegCreateKeyExW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey,
		DWORD Reserved,
		__in          LPWSTR lpClass,
		__in          DWORD dwOptions,
		__in          REGSAM samDesired,
		__in          LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		__out         PHKEY phkResult,
		__out         LPDWORD lpdwDisposition
		);
	virtual LONG WINAPI RegOpenKeyW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey,
		__out         PHKEY phkResult
		);
	virtual LONG WINAPI RegOpenKeyExW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey,
		DWORD ulOptions,
		__in          REGSAM samDesired,
		__out         PHKEY phkResult
		);
	virtual LONG WINAPI RegCloseKey(
		__in          HKEY hKey
		);
	virtual LONG WINAPI RegDeleteKeyW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey
		);
	virtual LONG WINAPI RegDeleteKeyExW(
		__in          HKEY hKey,
		__in          LPCWSTR lpSubKey,
		__in          REGSAM samDesired,
		DWORD Reserved
		);
	virtual LONG WINAPI RegDeleteValueW(
		__in          HKEY hKey,
		__in          LPCWSTR lpValueName
		);
	virtual LONG WINAPI RegSetValueExW(
		__in          HKEY hKey,
		__in          LPCWSTR lpValueName,
		DWORD Reserved,
		__in          DWORD dwType,
		__in          const BYTE* lpData,
		__in          DWORD cbData
		);
	virtual LONG WINAPI RegQueryValueExW(
		__in          HKEY hKey,
		__in          LPCWSTR lpValueName,
		LPDWORD lpReserved,
		__out         LPDWORD lpType,
		__out         LPBYTE lpData,
		__out		  LPDWORD lpcbData
		);
	virtual LONG WINAPI RegEnumKeyW(
		__in          HKEY hKey,
		__in          DWORD dwIndex,
		__out         LPWSTR lpName,
		__in          DWORD cchName
		);
	virtual LONG WINAPI RegEnumKeyExW(
		__in          HKEY hKey,
		__in          DWORD dwIndex,
		__out         LPWSTR lpName,
		__out		  LPDWORD lpcName,
		LPDWORD lpReserved,
		__out		  LPWSTR lpClass,
		__out		  LPDWORD lpcClass,
		__out         PFILETIME lpftLastWriteTime
		);
	virtual LONG WINAPI RegEnumValueW(
		__in          HKEY hKey,
		__in          DWORD dwIndex,
		__out         LPWSTR lpValueName,
		__out		  LPDWORD lpcchValueName,
		LPDWORD lpReserved,
		__out         LPDWORD lpType,
		__out         LPBYTE lpData,
		__out		  LPDWORD lpcbData
		);
	virtual LONG WINAPI ZwQueryObject (
		__in		  HANDLE ObjectHandle,
		__in		  DWORD ObjectInformationClass,
		__out		  PVOID ObjectInformation,
		__in		  ULONG Length,
		__out		  PULONG ResultLength
		);
	virtual BOOL WINAPI CopyFileEx( 
		__in		  LPCTSTR lpExistingFileName, 
		__in		  LPCTSTR lpNewFileName, 
		__in		  LPPROGRESS_ROUTINE lpProgressRoutine, 
		__in		  LPVOID lpData, 
		__in		  LPBOOL pbCancel, 
		__in		  DWORD dwCopyFlags 
		);
	virtual BOOL WINAPI MoveFileEx(
		__in          LPCTSTR lpExistingFileName,
		__in          LPCTSTR lpNewFileName,
		__in          DWORD dwFlags
		);
	virtual BOOL WINAPI CopyFile(
		__in          LPCTSTR lpExistingFileName,
		__in          LPCTSTR lpNewFileName,
		__in          BOOL bFailIfExists
		);
	virtual BOOL WINAPI MoveFile(
		__in          LPCTSTR lpExistingFileName,
		__in          LPCTSTR lpNewFileName
		);
	virtual BOOL WINAPI CopyFileOnReboot(
		__in		  LPCTSTR lpExistingFileName, 
		__in		  LPCTSTR lpNewFileName,
		__in		  BOOL	  bCopyOnReboot = TRUE
		);
	virtual BOOL WINAPI LockFile(
		__in		  LPCTSTR lpFileName);
	virtual BOOL WINAPI unLockFile(
		__in		  LPCTSTR lpFileName);
protected:

	virtual HRESULT KernelInitialize();
	virtual HRESULT KernelUnInitialize(BOOL bUnInstall);

private:

	typedef ULONG	NTSTATUS;

	virtual BOOL IsDriverRunning();
	DWORD doIOCTL(
		__in	DWORD dwIoControlCode,
		__in	LPVOID lpInBuffer,
		__in	DWORD nInBufferSize,
		__out	LPVOID lpOutBuffer,
		__out	DWORD nOutBufferSize,
		__out	LPDWORD lpBytesReturned = NULL
		);

	DWORD initialize();
	DWORD uninitialize();
	DWORD openDriver(LPCTSTR lpSymbolLink);
	DWORD installDriver(LPCTSTR lpDriverPath, LPCTSTR lpDriverName);
	DWORD uninstallDriver(LPCTSTR lpDriverName);
	ULONG NtStatusToDosError(NTSTATUS Status);
	//VOID RtlInitUnicodeString_U( PUNICODE_STRING NtFileName, PCWSTR DosFileName );
	ULONG RtlIsDosDeviceName_U( PCWSTR DosFileName );
	//BOOL RtlDosPathNameToNtPathName_U( PCWSTR DosFileName, PUNICODE_STRING NtFileName, PWSTR *FilePart, PVOID Reserved );

	typedef ULONG (WINAPI *_pfnRtlNtStatusToDosError) (NTSTATUS Status);
	_pfnRtlNtStatusToDosError	m_pfnRtlNtStatusToDosError;
	typedef LONG  (WINAPI *_pfnRtlIsDosDeviceName_U)	(PCWSTR DosFileName);
	_pfnRtlIsDosDeviceName_U	m_pfnRtlIsDosDeviceName_U;
	//typedef VOID  (WINAPI *_pfnRtlInitUnicodeString) (PUNICODE_STRING, PCWSTR);
	//_pfnRtlInitUnicodeString	m_pfnRtlInitUnicodeString;
	//typedef BOOLEAN (WINAPI *_pfnRtlDosPathNameToNtPathName_U)(PCWSTR, PUNICODE_STRING, PWSTR*, PVOID);
	//_pfnRtlDosPathNameToNtPathName_U	m_pfnRtlDosPathNameToNtPathName_U;

	volatile LONG		m_InstanceCount;
	CRITICAL_SECTION	m_cs;
	tstring				m_strDriverPath;
	tstring				m_strDriverName;
	tstring				m_strOrigDriverPath;
	BOOL				m_bIsRunning;
	HANDLE				m_hDriver;
};