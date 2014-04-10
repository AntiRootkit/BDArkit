#pragma once


#include "stdafx.h"
#include "tstring.h"
#include <WinIoCtl.h>
#include "BDArKit\KitHelper.h"
#include "FileStruct.h"
#include "Utility.h"
#include "KernelUtils.h"


#define FILE_OPEN_REMOTE_INSTANCE				0x00000400
#define FILE_SUPERSEDE							0x00000000
#define FILE_OPEN								0x00000001
#define FILE_CREATE								0x00000002
#define FILE_OPEN_IF							0x00000003
#define FILE_OVERWRITE							0x00000004
#define FILE_OVERWRITE_IF						0x00000005
#define FILE_MAXIMUM_DISPOSITION				0x00000005
#define FILE_SYNCHRONOUS_IO_ALERT               0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT            0x00000020
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_CREATE_TREE_CONNECTION             0x00000080
#define FILE_DIRECTORY_FILE                     0x00000001
#define FILE_WRITE_THROUGH                      0x00000002
#define FILE_SEQUENTIAL_ONLY                    0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING          0x00000008
#define FILE_COMPLETE_IF_OPLOCKED               0x00000100
#define FILE_NO_EA_KNOWLEDGE                    0x00000200
#define FILE_OPEN_FOR_RECOVERY                  0x00000400
#define FILE_RANDOM_ACCESS                      0x00000800
#define FILE_DELETE_ON_CLOSE                    0x00001000
#define FILE_OPEN_BY_FILE_ID                    0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT             0x00004000
#define FILE_NO_COMPRESSION                     0x00008000
#define FILE_RESERVE_OPFILTER                   0x00100000
#define FILE_OPEN_REPARSE_POINT                 0x00200000
#define FILE_OPEN_NO_RECALL                     0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY          0x00800000
#define FILE_ATTRIBUTE_VALID_FLAGS				0x00007fb7
#define FILE_ATTRIBUTE_VALID_SET_FLAGS			0x000031a7



CKernelUtils::CKernelUtils()
: m_hDriver(INVALID_HANDLE_VALUE)
, m_pfnRtlNtStatusToDosError(NULL)
, m_bIsRunning(FALSE)
, m_InstanceCount(0)
{
	InitializeCriticalSection (&m_cs);
}

CKernelUtils::~CKernelUtils() throw()
{
	DeleteCriticalSection (&m_cs);
}

HRESULT CKernelUtils::KernelInitialize()
{

	TCHAR	wszPath[MAX_PATH] = {_T("\0")};
	DWORD	dwResId = OSInfo::Is32Bit() ? IDR_DRIVER_32 : IDR_DRIVER_64;
	tstring strDriverFileName;
	DWORD	dwError = ERROR_SUCCESS;
	HRESULT	hr = E_FAIL;

	do	
	{
		EnterCriticalSection (&m_cs);		

		if ( m_hDriver != NULL && m_hDriver != INVALID_HANDLE_VALUE)
		{
			break;
		}

		if ( dwResId == IDR_DRIVER_32 )
		{
			strDriverFileName.assign(BDArKit_DRIVER_FILENAMEW);

		} else if ( dwResId == IDR_DRIVER_64 )
		{
			strDriverFileName.assign(BDArKit_DRIVER_FILENAME64W);
		}

		GetModuleFileName(g_DllInstance, wszPath, MAX_PATH);
		m_strOrigDriverPath.assign (wszPath);
		m_strOrigDriverPath.erase (m_strOrigDriverPath.rfind (L'\\'), m_strOrigDriverPath.length () - 1);

		int upFolder = -1;
		if ((upFolder = m_strOrigDriverPath.rfind (L'\\')) > 0)
		{
			// has exist parent dir
			m_strOrigDriverPath.erase (upFolder, m_strOrigDriverPath.length () - 1);
		}

		if (m_strOrigDriverPath.at (m_strOrigDriverPath.length () - 1) != _T('\\'))
		{
			m_strOrigDriverPath.append (_T("\\"));
		}

		if (upFolder > 0)
		{
			m_strOrigDriverPath.append (_T("drivers\\"));
		}

		m_strOrigDriverPath.append (strDriverFileName);

		WIN32_FILE_ATTRIBUTE_DATA Data;
		if (GetFileAttributesEx(m_strOrigDriverPath.c_str(), GetFileExInfoStandard, &Data) == FALSE
			&& GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			m_strOrigDriverPath.clear();
			m_strOrigDriverPath.assign (wszPath);
			m_strOrigDriverPath.erase (m_strOrigDriverPath.rfind (L'\\'), m_strOrigDriverPath.length () - 1);
			m_strOrigDriverPath.append (_T("\\"));
			m_strOrigDriverPath.append(strDriverFileName);
		}

		memset(wszPath, 0, sizeof(wszPath));
		GetWindowsDirectory(wszPath, MAX_PATH);
		m_strDriverPath.assign (wszPath);
		if (m_strDriverPath.at (m_strDriverPath.length () - 1) != _T('\\'))
		{
			m_strDriverPath.append (_T("\\"));
		}
		m_strDriverPath.append (_T("System32\\Drivers\\"));
		m_strDriverPath.append (strDriverFileName);

		m_strDriverName.assign (BDArKit_DRIVER_NAMEW);

		dwError = initialize ();
	} while(FALSE);

	if ( dwError == ERROR_SUCCESS )
	{
		hr = S_OK;
		InterlockedIncrement(&m_InstanceCount);
	}

	SetLastError(dwError);
	LeaveCriticalSection (&m_cs);

	return hr;
}

HRESULT CKernelUtils::KernelUnInitialize( BOOL bUnInstall /*= FALSE*/ )
{
	if ( m_hDriver != NULL && m_hDriver != INVALID_HANDLE_VALUE)
	{
		if ( InterlockedDecrement(&m_InstanceCount) == 0 &&
			bUnInstall == TRUE )
		{
			uninitialize ();
		}
	}

	return S_OK;
}

BOOL CKernelUtils::IsDriverRunning()
{
	return m_bIsRunning == TRUE && m_hDriver != NULL && m_hDriver != INVALID_HANDLE_VALUE;
}

HANDLE WINAPI CKernelUtils::CreateFileW( 
										__in LPCWSTR lpFileName, 
										__in DWORD dwDesiredAccess, 
										__in DWORD dwShareMode, 
										__in LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
										__in DWORD dwCreationDisposition, 
										__in DWORD dwFlagsAndAttributes, 
										__in HANDLE hTemplateFile 
										)
{
	if ( lpFileName == NULL )
	{
		SetLastError (ERROR_PATH_NOT_FOUND);
		return INVALID_HANDLE_VALUE;
	}

	if ( lpSecurityAttributes != NULL || hTemplateFile != NULL )
	{
		SetLastError (ERROR_NOT_SUPPORTED);
		return INVALID_HANDLE_VALUE;
	}

	switch (dwCreationDisposition)
	{
	case CREATE_NEW:
		dwCreationDisposition = FILE_CREATE;
		break;

	case CREATE_ALWAYS:
		dwCreationDisposition = FILE_OVERWRITE_IF;
		break;

	case OPEN_EXISTING:
		dwCreationDisposition = FILE_OPEN;
		break;

	case OPEN_ALWAYS:
		dwCreationDisposition = FILE_OPEN_IF;
		break;

	case TRUNCATE_EXISTING:
		dwCreationDisposition = FILE_OVERWRITE;
		break;

	default:
		SetLastError(ERROR_INVALID_PARAMETER);
		return (INVALID_HANDLE_VALUE);
	}

	ULONG FileAttributes, Flags = 0;
	if (!(dwFlagsAndAttributes & FILE_FLAG_OVERLAPPED))
		Flags |= FILE_SYNCHRONOUS_IO_NONALERT;

	if(dwFlagsAndAttributes & FILE_FLAG_WRITE_THROUGH)
		Flags |= FILE_WRITE_THROUGH;

	if(dwFlagsAndAttributes & FILE_FLAG_NO_BUFFERING)
		Flags |= FILE_NO_INTERMEDIATE_BUFFERING;

	if(dwFlagsAndAttributes & FILE_FLAG_RANDOM_ACCESS)
		Flags |= FILE_RANDOM_ACCESS;

	if(dwFlagsAndAttributes & FILE_FLAG_SEQUENTIAL_SCAN)
		Flags |= FILE_SEQUENTIAL_ONLY;

	if(dwFlagsAndAttributes & FILE_FLAG_DELETE_ON_CLOSE)
		Flags |= FILE_DELETE_ON_CLOSE;

	if(dwFlagsAndAttributes & FILE_FLAG_BACKUP_SEMANTICS)
	{
		if(dwDesiredAccess & GENERIC_ALL)
			Flags |= FILE_OPEN_FOR_BACKUP_INTENT | FILE_OPEN_REMOTE_INSTANCE;
		else
		{
			if(dwDesiredAccess & GENERIC_READ)
				Flags |= FILE_OPEN_FOR_BACKUP_INTENT;

			if(dwDesiredAccess & GENERIC_WRITE)
				Flags |= FILE_OPEN_REMOTE_INSTANCE;
		}
	}
	else
		Flags |= FILE_NON_DIRECTORY_FILE;

	if(dwFlagsAndAttributes & FILE_FLAG_OPEN_REPARSE_POINT)
		Flags |= FILE_OPEN_REPARSE_POINT;

	if(dwFlagsAndAttributes & FILE_FLAG_OPEN_NO_RECALL)
		Flags |= FILE_OPEN_NO_RECALL;

	FileAttributes = (dwFlagsAndAttributes & (FILE_ATTRIBUTE_VALID_FLAGS & ~FILE_ATTRIBUTE_DIRECTORY));

	/* handle may always be waited on and querying attributes are always allowed */
	dwDesiredAccess |= SYNCHRONIZE | FILE_READ_ATTRIBUTES;

	FILE_CREATE_IN	inData = {0x00};
	FILE_CREATE_OUT	outData= {0x00};
	outData.hFile = INVALID_HANDLE_VALUE;

	if (wcslen (lpFileName) * sizeof(WCHAR) > sizeof(inData.fileInfo.cFilePath))
	{
		SetLastError (ERROR_FILENAME_EXCED_RANGE);
		return INVALID_HANDLE_VALUE;
	}

	inData.AccessMask			= dwDesiredAccess;
	inData.ShareAccess			= dwShareMode;
	inData.CreateDisposition	= dwCreationDisposition;
	inData.FileAttributes		= FileAttributes;
	inData.CreateOptions		= Flags;
	wcscpy_s (inData.fileInfo.cFilePath, lpFileName);

	DWORD dwError = doIOCTL (
		IOCTL_CREATE_FILE,
		&inData,
		sizeof(inData),
		&outData,
		sizeof(outData)
		);
	SetLastError (dwError);

	return Handle64ToHandle(outData.hFile);
}

BOOL WINAPI CKernelUtils::ReadFile(
								   __in HANDLE hFile, 
								   __out LPVOID lpBuffer, 
								   __in DWORD nNumberOfBytesToRead, 
								   __out LPDWORD lpNumberOfBytesRead, 
								   __in LPOVERLAPPED lpOverlapped 
								   )
{
	DWORD			dwError	= ERROR_SUCCESS;
	FILE_READ_IN	inData	= {0x00};
	FILE_READ_OUT	outData = {0x00};

	if (lpNumberOfBytesRead != NULL)
	{
		*lpNumberOfBytesRead = 0;
	}

	if (!nNumberOfBytesToRead)
	{
		return TRUE;
	}

	if (lpOverlapped != NULL)
	{
		LARGE_INTEGER Offset;
		PVOID ApcContext;

		Offset.u.LowPart = lpOverlapped->Offset;
		Offset.u.HighPart = lpOverlapped->OffsetHigh;
		lpOverlapped->Internal = STATUS_PENDING;
		ApcContext = (((ULONG_PTR)lpOverlapped->hEvent & 0x1) ? NULL : lpOverlapped);

		inData.ApcContext	= ApcContext;
		inData.Event		= lpOverlapped->hEvent;
		inData.Handle		= hFile;
		inData.Offset		= Offset;
		inData.nNumberBytesOfToRead	= nNumberOfBytesToRead;
		inData.Buffer		= lpBuffer;
		inData.nNumberBytesOfRead	= lpNumberOfBytesRead;
		dwError = doIOCTL (
			IOCTL_READ_FILE,
			&inData,
			sizeof(inData),
			&outData,
			sizeof(outData)
			);

		if (dwError != ERROR_SUCCESS || dwError == ERROR_IO_PENDING)
		{
			if (dwError == ERROR_HANDLE_EOF &&
				lpNumberOfBytesRead != NULL)
			{
				*lpNumberOfBytesRead = 0;
			}

			SetLastError(dwError);
			return FALSE;
		}

		if (lpNumberOfBytesRead != NULL)
		{
			*lpNumberOfBytesRead = lpOverlapped->InternalHigh;
		}
	}
	else
	{
		inData.ApcContext	= NULL;
		inData.Event		= NULL;
		inData.Handle		= hFile;
		inData.nNumberBytesOfToRead	= nNumberOfBytesToRead;
		inData.Buffer		= lpBuffer;
		inData.nNumberBytesOfRead	= lpNumberOfBytesRead;
		dwError = doIOCTL (
			IOCTL_READ_FILE,
			&inData,
			sizeof(inData),
			&outData,
			sizeof(outData)
			);
		if (dwError == ERROR_HANDLE_EOF)
		{
			*lpNumberOfBytesRead = 0;

		} else if (dwError == ERROR_SUCCESS)
		{
			*lpNumberOfBytesRead = *outData.nNumberBytesOfRead;
		}
		else
		{
			SetLastError(dwError);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL WINAPI CKernelUtils::WriteFile( 
									__in HANDLE hFile, 
									__in LPCVOID lpBuffer, 
									__in DWORD nNumberOfBytesToWrite, 
									__out LPDWORD lpNumberOfBytesWritten, 
									__in LPOVERLAPPED lpOverlapped 
									)
{
	DWORD			dwError	= ERROR_SUCCESS;
	FILE_WRITE_IN	inData	= {0x00};
	FILE_WRITE_OUT	outData = {0x00};

	if (lpNumberOfBytesWritten != NULL)
	{
		*lpNumberOfBytesWritten = 0;
	}

	if (!nNumberOfBytesToWrite)
	{
		return TRUE;
	}

	if (lpOverlapped != NULL)
	{
		LARGE_INTEGER Offset;
		PVOID ApcContext;

		Offset.u.LowPart = lpOverlapped->Offset;
		Offset.u.HighPart = lpOverlapped->OffsetHigh;
		lpOverlapped->Internal = STATUS_PENDING;
		ApcContext = (((ULONG_PTR)lpOverlapped->hEvent & 0x1) ? NULL : lpOverlapped);

		inData.ApcContext	= ApcContext;
		inData.Event		= lpOverlapped->hEvent;
		inData.Handle		= hFile;
		inData.Offset		= Offset;
		inData.nNumberBytesOfToWrite= nNumberOfBytesToWrite;
		inData.Buffer		= const_cast<LPVOID>(lpBuffer);
		inData.nNumberBytesOfWrite	= lpNumberOfBytesWritten;
		dwError = doIOCTL (
			IOCTL_WRITE_FILE,
			&inData,
			sizeof(inData),
			&outData,
			sizeof(outData)
			);

		if (dwError != ERROR_SUCCESS || dwError == ERROR_IO_PENDING)
		{
			if (dwError == ERROR_HANDLE_EOF &&
				lpNumberOfBytesWritten != NULL)
			{
				*lpNumberOfBytesWritten = 0;
			}

			SetLastError(dwError);
			return FALSE;
		}

		if (lpNumberOfBytesWritten != NULL)
		{
			*lpNumberOfBytesWritten = lpOverlapped->InternalHigh;
		}
	}
	else
	{
		inData.ApcContext	= NULL;
		inData.Event		= NULL;
		inData.Handle		= hFile;
		inData.nNumberBytesOfToWrite	= nNumberOfBytesToWrite;
		inData.Buffer		= const_cast<LPVOID>(lpBuffer);
		inData.nNumberBytesOfWrite	= lpNumberOfBytesWritten;
		dwError = doIOCTL (
			IOCTL_WRITE_FILE,
			&inData,
			sizeof(inData),
			&outData,
			sizeof(outData)
			);
		if (dwError == ERROR_HANDLE_EOF)
		{
			*lpNumberOfBytesWritten = 0;

		} else if (dwError == ERROR_SUCCESS)
		{
			*lpNumberOfBytesWritten = *outData.nNumberBytesOfWrite;
		}
		else
		{
			SetLastError(dwError);
			return FALSE;
		}
	}

	return TRUE;
}

HANDLE WINAPI CKernelUtils::FindFirstFileW( 
	__in LPCWSTR lpFileName, 
	__out LPWIN32_FIND_DATA lpFindFileData 
	)
{
	if ( lpFileName == NULL || lpFindFileData == NULL )
	{
		return INVALID_HANDLE_VALUE;
	}

	return this->FindFirstFileExW(
		lpFileName,
		FindExInfoStandard,
		lpFindFileData,
		FindExSearchNameMatch,
		NULL, 
		0
		);
}

HANDLE WINAPI CKernelUtils::FindFirstFileExW( 
	__in LPCWSTR lpFileName, 
	__in FINDEX_INFO_LEVELS fInfoLevelId, 
	__out LPVOID lpFindFileData, 
	__in FINDEX_SEARCH_OPS fSearchOp, 
	__in LPVOID lpSearchFilter, 
	__in DWORD dwAdditionalFlags 
	)
{
	PFIND_DATA_HANDLE FindDataHandle = static_cast<PFIND_DATA_HANDLE>(INVALID_HANDLE_VALUE);

	do
	{
		if (fInfoLevelId != FindExInfoStandard ||
			fSearchOp == FindExSearchLimitToDevices ||
			dwAdditionalFlags & ~FIND_FIRST_EX_CASE_SENSITIVE /* only supported flag for now */)
		{
			SetLastError(fSearchOp == FindExSearchLimitToDevices
				? ERROR_NOT_SUPPORTED
				: ERROR_INVALID_PARAMETER);
			return INVALID_HANDLE_VALUE;
		}

		if ((fSearchOp != FindExSearchNameMatch &&
			fSearchOp != FindExSearchLimitToDirectories) ||
			lpSearchFilter != NULL)
		{
			SetLastError(ERROR_NOT_SUPPORTED);
			return INVALID_HANDLE_VALUE;
		}

		tstring strFilePath;
		tstring	strFilePattern;
		strFilePath.assign (lpFileName);

		int nPos = strFilePath.rfind (L"\\");
		if ( nPos == strFilePath.length () - 1)
		{
			strFilePattern.assign (L"*");
		}
		else if ( nPos != std::string::npos)
		{
			strFilePattern.assign (strFilePath.substr (nPos + 1, strFilePath.length () - 1));
			strFilePath.erase (nPos + 1, strFilePath.length () - 1);
		}
		else
		{
			SetLastError (ERROR_INVALID_PARAMETER);
			break;
		}

		FIND_FIRST_FILE_IN	inData	= {0x00};
		if (strFilePath.length () >= sizeof(inData.szDirectory) ||
			strFilePattern.length () >= sizeof(inData.szFilePattern))
		{
			SetLastError (ERROR_BAD_LENGTH);
			break;
		}

		wcscpy_s (inData.szDirectory, strFilePath.c_str ());
		wcscpy_s (inData.szFilePattern, strFilePattern.c_str ());

		FIND_FIRST_FILE_OUT	outData	= {0x00};
		DWORD dwError = doIOCTL (
			IOCTL_FIND_FIRST_FILE,
			&inData,
			sizeof(inData),
			&outData,
			sizeof(outData)
			);
		if ( dwError != ERROR_SUCCESS )
		{
			SetLastError (dwError);
			break;
		}

		FindDataHandle = new FIND_DATA_HANDLE;
		if ( FindDataHandle == NULL )
		{
			FindDataHandle = static_cast<PFIND_DATA_HANDLE>(INVALID_HANDLE_VALUE);
			SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			break;
		}

		FindDataHandle->Type						= FindFile;
		FindDataHandle->FindFileData.Handle			= Handle64ToHandle(outData.hDirectory);
		FindDataHandle->FindFileData.InfoLevel		= fInfoLevelId;
		FindDataHandle->FindFileData.SearchOp		= fSearchOp;
		FindDataHandle->FindFileData.DirectoryPath	= strFilePath;
		FindDataHandle->FindFileData.FilePattern	= strFilePattern;

		LPWIN32_FIND_DATA lpWin32Data = (LPWIN32_FIND_DATA)lpFindFileData;
		RtlZeroMemory(lpWin32Data, sizeof(WIN32_FIND_DATAW));
		lpWin32Data->dwFileAttributes	= outData.dwFileAttributes;
		lpWin32Data->nFileSizeHigh		= outData.nFileSizeHigh;
		lpWin32Data->nFileSizeLow		= outData.nFileSizeLow;
		lpWin32Data->ftCreationTime		= File::fileTimeFromLargeInteger (outData.ftCreationTime);
		lpWin32Data->ftLastAccessTime	= File::fileTimeFromLargeInteger (outData.ftLastAccessTime);
		lpWin32Data->ftLastWriteTime	= File::fileTimeFromLargeInteger (outData.ftLastWriteTime);
		wcscpy_s (lpWin32Data->cFileName, outData.cFileName);

		SetLastError (ERROR_SUCCESS);
	} while(FALSE); 

	return static_cast<HANDLE>(FindDataHandle);
}

BOOL WINAPI CKernelUtils::FindNextFileW( 
										__in HANDLE hFindFile, 
										__out LPWIN32_FIND_DATA lpFindFileData 
										)
{
	BOOL bFindFile	= FALSE;
	DWORD dwError	= ERROR_SUCCESS;

	do 
	{
		PFIND_DATA_HANDLE FindDataHandle = (PFIND_DATA_HANDLE)hFindFile;

		if (hFindFile == NULL || 
			hFindFile == INVALID_HANDLE_VALUE ||
			FindDataHandle->Type != FindFile)
		{
			dwError = ERROR_INVALID_HANDLE;
			break;
		}

		FIND_NEXT_FILE_IN	inData = {0x00};
		inData.hDirectory = FindDataHandle->FindFileData.Handle;
		wcscpy_s (inData.szFilePattern, FindDataHandle->FindFileData.FilePattern.c_str ());

		FIND_NEXT_FILE_OUT	outData= {0x00};
		dwError = doIOCTL (
			IOCTL_FIND_NEXT_FILE,
			&inData,
			sizeof(inData),
			&outData,
			sizeof(outData));
		if ( dwError != ERROR_SUCCESS )
		{
			break;
		}

		LPWIN32_FIND_DATA lpWin32Data = (LPWIN32_FIND_DATA)lpFindFileData;
		RtlZeroMemory(lpWin32Data, sizeof(WIN32_FIND_DATAW));
		lpWin32Data->dwFileAttributes	= outData.dwFileAttributes;
		lpWin32Data->nFileSizeHigh		= outData.nFileSizeHigh;
		lpWin32Data->nFileSizeLow		= outData.nFileSizeLow;
		lpWin32Data->ftCreationTime		= File::fileTimeFromLargeInteger (outData.ftCreationTime);
		lpWin32Data->ftLastAccessTime	= File::fileTimeFromLargeInteger (outData.ftLastAccessTime);
		lpWin32Data->ftLastWriteTime	= File::fileTimeFromLargeInteger (outData.ftLastWriteTime);
		wcscpy_s (lpWin32Data->cFileName, outData.cFileName);

		bFindFile = TRUE;
	} while(FALSE);

	SetLastError (dwError);
	return bFindFile;
}

BOOL WINAPI CKernelUtils::FindClose( __in HANDLE hFindFile )
{
	if (hFindFile == NULL ||
		hFindFile == INVALID_HANDLE_VALUE)
	{
		SetLastError (ERROR_INVALID_HANDLE);
		return FALSE;
	}

	PFIND_DATA_HANDLE FindDataHandle = (PFIND_DATA_HANDLE)hFindFile;
	if (FindDataHandle->Type != FindFile ||
		FindDataHandle->FindFileData.Handle == NULL ||
		FindDataHandle->FindFileData.Handle == INVALID_HANDLE_VALUE)
	{
		SetLastError (ERROR_INVALID_HANDLE_STATE);
		return FALSE;
	}

	CloseHandle (FindDataHandle->FindFileData.Handle);
	free(FindDataHandle);

	return TRUE;
}

BOOL WINAPI CKernelUtils::CloseHandle(
									  __in          HANDLE hObject
									  )
{
	FILE_CLOSE_IN	inData = {0x00};
	DWORD			dwError= ERROR_SUCCESS;

	do 
	{
		if ( hObject == NULL || hObject == INVALID_HANDLE_VALUE)
		{
			dwError = ERROR_INVALID_HANDLE;
			break;
		}

		if (((DWORD)hObject & 0x80000000) == 0x80000000) {
			return ::CloseHandle (hObject);
		}

		inData.hFile = hObject;
		dwError = doIOCTL (
			IOCTL_CLOSE_FILE,
			&inData,
			sizeof(inData),
			NULL,
			0);
	} while (FALSE);

	return dwError == ERROR_SUCCESS;
}

BOOL WINAPI CKernelUtils::DeleteFileW( __in LPCWSTR lpFileName, __in BOOL bPreventCreateFile /*= FALSE*/)
{
	DELETE_FILE_IN	DeleteData		= {0x00};
	DWORD			dwError			= 0;
	tstring			strFilePath(lpFileName);

	do 
	{
		if (strFilePath.length () >= sizeof(DeleteData.fileInfo.cFilePath))
		{
			dwError = ERROR_FILENAME_EXCED_RANGE;
			break;
		}

		DeleteData.DoFileAction = DO_OnceAction;
		DeleteData.DoFileAction |= bPreventCreateFile? DO_DenyAction : DO_PassAction;
		ZeroMemory(DeleteData.fileInfo.cFilePath, sizeof(DeleteData.fileInfo.cFilePath));
		wcscat_s (DeleteData.fileInfo.cFilePath, strFilePath.c_str ());

		dwError = doIOCTL (
			IOCTL_DELETE_FILE,
			&DeleteData,
			sizeof(DeleteData),
			NULL,
			0);
	} while (FALSE);

	SetLastError (dwError);
	return dwError == ERROR_SUCCESS;
}

BOOL WINAPI CKernelUtils::DeleteFileOnRebootW(__in LPCWSTR lpFileName, __in BOOL bDeleteOnReboot)
{
	DELETE_FILE_IN	DeleteData		= {0x00};
	DWORD			dwError			= 0;
	tstring			strFilePath(lpFileName);

	do 
	{
		if (strFilePath.length () >= sizeof(DeleteData.fileInfo.cFilePath) / sizeof(TCHAR))
		{
			dwError = ERROR_FILENAME_EXCED_RANGE;
			break;
		}

		DeleteData.DoFileAction = bDeleteOnReboot? DO_RebootAction : DO_RemoveAction;
		ZeroMemory(DeleteData.fileInfo.cFilePath, sizeof(DeleteData.fileInfo.cFilePath));
		wcscat_s (DeleteData.fileInfo.cFilePath, strFilePath.c_str ());

		dwError = doIOCTL (
			IOCTL_DELETE_FILE,
			&DeleteData,
			sizeof(DeleteData),
			NULL,
			0);
	} while (FALSE);

	SetLastError (dwError);
	return dwError == ERROR_SUCCESS;
}

LONG WINAPI CKernelUtils::ZwQueryObject (
	__in		  HANDLE ObjectHandle,
	__in		  DWORD ObjectInformationClass,
	__out		  PVOID ObjectInformation,
	__in		  ULONG Length,
	__out		  PULONG ResultLength
	)
{
	QUERY_OBJECT_IN	InData	= {0x00};

	InData.hObjectHandle			= ObjectHandle;
	InData.ObjectInformationClass	= static_cast<OBJECT_INFO_CLASS>(ObjectInformationClass);
	InData.Length					= Length;
	InData.ObjectInformation		= ObjectInformation;
	InData.ResultLength			= ResultLength;

	DWORD	dwError	= ERROR_SUCCESS;

	do 
	{
		if ( (ObjectHandle == NULL || ObjectHandle == INVALID_HANDLE_VALUE) &&
			(ObjectInformationClass < ObjectBasicInfo || ObjectInformationClass > ObjectProtectionInfo))
		{
			dwError = ERROR_INVALID_HANDLE;
			break;
		}

		dwError = doIOCTL (
			IOCTL_QUERY_OBJECT,
			&InData,
			sizeof(InData),
			NULL,
			0	
			);
	} while (FALSE);

	return dwError;
}

HANDLE WINAPI CKernelUtils::OpenProcess(
										__in DWORD dwDesiredAccess,
										__in BOOL bInheritHandle,
										__in DWORD dwProcessId
										)
{
	OPEN_PROCESS_IN		InData	= {0x00};
	OPEN_PROCESS_OUT	OutData	= {0x00};
	OutData.hProcess = reinterpret_cast<HANDLE>(NULL);

	DWORD	dwError = ERROR_SUCCESS;

	do 
	{
		if ( dwProcessId == 0 )
		{
			dwError = ERROR_INVALID_PARAMETER;
			break;
		}

		InData.CliendId.UniqueProcess	= UlongToHandle(dwProcessId);
		InData.CliendId.UniqueThread	= 0;
		InData.dwDesiredAccess			= dwDesiredAccess;
		InData.bInheritHandle			= bInheritHandle;
		dwError = doIOCTL (
			IOCTL_OPEN_PROCESS,
			&InData,
			sizeof(InData),
			&OutData,
			sizeof(OutData)
			);
	} while (FALSE);

	SetLastError (dwError);
	return Handle64ToHandle(OutData.hProcess);
}

BOOL WINAPI CKernelUtils::TerminateProcess( __in HANDLE hProcess, __in UINT uExitCode )
{
	KILL_PROCESS_IN	inData	= {0x00};
	DWORD			dwError	= 0;

	do 
	{
		if ( hProcess == NULL || hProcess == INVALID_HANDLE_VALUE )
		{
			dwError = ERROR_INVALID_HANDLE;
			break;
		}

		inData.hProcess	= hProcess;
		inData.ExitCode	= uExitCode;
		dwError = doIOCTL (
			IOCTL_KILL_PROCESS,
			&inData,
			sizeof(inData),
			NULL,
			0);
	} while (FALSE);

	SetLastError (dwError);
	return dwError == ERROR_SUCCESS;
}

BOOL WINAPI CKernelUtils::TerminateProcessId( __in DWORD dwProcessId, __in UINT uExitCode )
{
	KILL_PROCESSID_IN	inData	= {0x00};
	DWORD				dwError	= 0;

	do 
	{
		if ( dwProcessId == 0 || dwProcessId == 4 )
		{
			dwError = ERROR_INVALID_PARAMETER;
			break;
		}

		inData.dwProcessId	= dwProcessId;
		inData.ExitCode	= uExitCode;
		dwError = doIOCTL (
			IOCTL_KILL_PROCESSID,
			&inData,
			sizeof(inData),
			NULL,
			0);
	} while (FALSE);

	SetLastError (dwError);
	return dwError == ERROR_SUCCESS;
}

LONG WINAPI CKernelUtils::RegCreateKeyW(
										__in          HKEY hKey,
										__in          LPCWSTR lpSubKey,
										__out         PHKEY phkResult
								  )
{
	return ::RegCreateKeyW (
		hKey,
		lpSubKey,
		phkResult
		);
}

LONG WINAPI CKernelUtils::RegCreateKeyExW(
	__in          HKEY hKey,
	__in          LPCWSTR lpSubKey,
	DWORD Reserved,
	__in          LPWSTR lpClass,
	__in          DWORD dwOptions,
	__in          REGSAM samDesired,
	__in          LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	__out         PHKEY phkResult,
	__out         LPDWORD lpdwDisposition
	)
{
	return ::RegCreateKeyExW (
		hKey,
		lpSubKey,
		Reserved,
		lpClass,
		dwOptions,
		samDesired,
		lpSecurityAttributes,
		phkResult,
		lpdwDisposition
		);
}

LONG WINAPI CKernelUtils::RegOpenKeyW(
									  __in          HKEY hKey,
									  __in          LPCWSTR lpSubKey,
									  __out         PHKEY phkResult
									  )
{
	return ::RegOpenKeyW (
		hKey,
		lpSubKey,
		phkResult
		);
}

LONG WINAPI CKernelUtils::RegOpenKeyExW(
										__in          HKEY hKey,
										__in          LPCWSTR lpSubKey,
										DWORD ulOptions,
										__in          REGSAM samDesired,
										__out         PHKEY phkResult
								  )
{
	return ::RegOpenKeyExW(
		hKey,
		lpSubKey,
		ulOptions,
		samDesired,
		phkResult
		);
}

LONG WINAPI CKernelUtils::RegCloseKey( __in HKEY hKey )
{
	return ::RegCloseKey (hKey);
}

LONG WINAPI CKernelUtils::RegDeleteKeyW( __in HKEY hKey, __in LPCWSTR lpSubKey )
{
	return ::RegDeleteKeyW (hKey, lpSubKey);
}

LONG WINAPI CKernelUtils::RegDeleteKeyExW(
	__in          HKEY		hKey,
	__in          LPCWSTR	lpSubKey,
	__in          REGSAM	samDesired,
	DWORD Reserved
	)
{
	//return ::RegDeleteKeyExW (hKey, lpSubKey, samDesired, Reserved);
	return 0;
}

LONG WINAPI CKernelUtils::RegDeleteValueW(
	__in          HKEY hKey,
	__in          LPCWSTR lpValueName
	)
{
	return ::RegDeleteValueW (hKey, lpValueName);
}

LONG WINAPI CKernelUtils::RegSetValueExW(
	__in          HKEY hKey,
	__in          LPCWSTR lpValueName,
	DWORD Reserved,
	__in          DWORD dwType,
	__in          const BYTE* lpData,
	__in          DWORD cbData
	)
{
	return ::RegSetValueExW (
		hKey,
		lpValueName,
		Reserved,
		dwType,
		lpData,
		cbData
		);
}

LONG WINAPI CKernelUtils::RegQueryValueExW(
	__in          HKEY hKey,
	__in          LPCWSTR lpValueName,
	LPDWORD lpReserved,
	__out         LPDWORD lpType,
	__out         LPBYTE lpData,
	__out		   LPDWORD lpcbData
	)
{
	return RegQueryValueExW (
		hKey,
		lpValueName,
		lpReserved,
		lpType,
		lpData,
		lpcbData
		);
}

LONG WINAPI CKernelUtils::RegEnumKeyW(
									  __in          HKEY hKey,
									  __in          DWORD dwIndex,
									  __out         LPWSTR lpName,
									  __in          DWORD cchName
									  )
{
	return ::RegEnumKeyW (
		hKey,
		dwIndex,
		lpName,
		cchName
		);
}

LONG WINAPI CKernelUtils::RegEnumKeyExW(
										__in			HKEY hKey,
										__in			DWORD dwIndex,
										__out			LPWSTR lpName,
										__out			LPDWORD lpcName,
										LPDWORD lpReserved,
										__out			LPWSTR lpClass,
										__out			LPDWORD lpcClass,
										__out			PFILETIME lpftLastWriteTime
								  )
{
	return ::RegEnumKeyExW (
		hKey,
		dwIndex,
		lpName,
		lpcName,
		lpReserved,
		lpClass,
		lpcClass,
		lpftLastWriteTime
		);
}
LONG WINAPI CKernelUtils::RegEnumValueW(
										__in			HKEY hKey,
										__in			DWORD dwIndex,
										__out			LPWSTR lpValueName,
										__out			LPDWORD lpcchValueName,
										LPDWORD	lpReserved,
										__out			LPDWORD lpType,
										__out			LPBYTE lpData,
										__out			LPDWORD lpcbData
								  )
{
	return ::RegEnumValueW(
		hKey,
		dwIndex,
		lpValueName,
		lpcchValueName,
		lpReserved,
		lpType,
		lpData,
		lpcbData
		);
}

BOOL WINAPI CKernelUtils::CopyFileEx( 
									 __in LPCTSTR	lpExistingFileName, 
									 __in LPCTSTR	lpNewFileName,
									 __in LPPROGRESS_ROUTINE lpProgressRoutine,
									 __in LPVOID	lpData,
									 __in LPBOOL	pbCancel,
									 __in DWORD		dwCopyFlags
									 )
{
#if 0
	BOOL			bIsOk			= FALSE;
	BOOL			bIsCancel		= FALSE;
	HANDLE			hFileNew		= INVALID_HANDLE_VALUE;
	HANDLE			hFileCurrent	= INVALID_HANDLE_VALUE;
	PVOID			pBuffer			= NULL;
	LARGE_INTEGER	ullFileSize		= {0x00};
	ULONG			ulBufLen		= 0;
	ULONG			ulActLen		= 0;
	LONGLONG		ullOffset		= 0;
	WIN32_FILE_ATTRIBUTE_DATA stStandard = {0x00};	

	do 
	{
		if (lpExistingFileName==NULL||lpNewFileName==NULL)
		{
			SetLastError(ERROR_INVALID_PARAMETER);
			break;
		}
		if(lpProgressRoutine != NULL || lpData != NULL)
		{
			SetLastError(ERROR_INVALID_PARAMETER);
			break;
		}
		//只支持COPY_FILE_FAIL_IF_EXISTS参数
		if((dwCopyFlags&~COPY_FILE_FAIL_IF_EXISTS) != 0)
		{
			SetLastError(ERROR_INVALID_PARAMETER);
			break;
		}

		if (GetFileAttributesEx(lpExistingFileName, GetFileExInfoStandard, &stStandard) == FALSE)
		{
			break;
		}

		hFileCurrent = this->CreateFileW(
			lpExistingFileName,
			FILE_READ_ATTRIBUTES,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if(hFileCurrent == INVALID_HANDLE_VALUE)
		{
			break;
		}

		hFileNew = this->CreateFileW(
			lpNewFileName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			((dwCopyFlags & COPY_FILE_FAIL_IF_EXISTS)? CREATE_NEW : CREATE_ALWAYS),
			stStandard.dwFileAttributes,
			NULL
			);
		if(hFileNew == INVALID_HANDLE_VALUE)
		{
			break;
		}

		GetFileSizeEx (hFileCurrent, &ullFileSize);	
		if(ullFileSize.QuadPart < 8*1024)
		{
			ulBufLen = 8*1024;
		}
		else if(ullFileSize.QuadPart < 16*1024)
		{
			ulBufLen = 16*1024;
		}
		else if(ullFileSize.QuadPart < 32 * 1024)
		{
			ulBufLen = 32*1024;
		}
		else 
		{
			ulBufLen = 64*1024;
		}

		while(ulBufLen >= (8 * 1024))
		{
			pBuffer = HeapAlloc (GetProcessHeap (), HEAP_ZERO_MEMORY, ulBufLen);
			if (pBuffer == NULL)
			{
				ulBufLen /= 2;
				continue;
			}
			break;
		}
		if (ulBufLen < (8 * 1024))
		{
			break;
		}

		ullOffset = 0;
		while(ullOffset < ullFileSize.QuadPart && bIsCancel == FALSE)
		{
			memset(pBuffer, 0x00, ulBufLen);
			if (this->ReadFile (hFileCurrent, pBuffer, ulBufLen, &ulActLen, NULL) == FALSE)
			{
				break;
			}
			if(ulActLen == 0)
			{
				break;
			}
			if (this->WriteFile (hFileNew, pBuffer, ulActLen, &ulActLen, NULL) == FALSE)
			{
				break;
			}

			ullOffset += ulBufLen;
			if(pbCancel != NULL )
			{
				bIsCancel = (BOOL)*pbCancel;
			}
		}

		if(ullOffset < ullFileSize.QuadPart)
		{
			break;
		}

		if (SetEndOfFile (hFileNew) == FALSE)
		{
			break;
		}

		bIsOk = TRUE;
	} while (FALSE);

	if(pBuffer != NULL )
	{
		HeapFree(GetProcessHeap(), 0, pBuffer);
		pBuffer = NULL;
	}

	if(hFileNew != INVALID_HANDLE_VALUE)
	{
		this->CloseHandle (hFileNew);
		hFileNew = INVALID_HANDLE_VALUE;
	}

	if(hFileCurrent != INVALID_HANDLE_VALUE)
	{
		this->CloseHandle (hFileCurrent);
		hFileCurrent = INVALID_HANDLE_VALUE;
	}

	return bIsOk;
#else
	BOOL			bIsOk			= FALSE;
	BOOL			bIsCancel		= FALSE;
	COPY_FILE_IN	CopyData;
	DWORD			dwError			= 0;
	tstring			strSrcFile;
	tstring			strDstFile;

	if (lpExistingFileName == NULL || lpNewFileName == NULL)
	{
		return FALSE;
	}

	strSrcFile.assign(lpExistingFileName);
	strDstFile.assign(lpNewFileName);
	wcscat_s (CopyData.fileSrcInfo.cFilePath, strSrcFile.c_str ());
	wcscat_s (CopyData.fileDstInfo.cFilePath, strDstFile.c_str ());

	CopyData.DoFileAction= DO_OnceAction;
	CopyData.dwCopyFlags = dwCopyFlags;
	CopyData.lpData		 = lpData;
	CopyData.pbCancel	 = pbCancel == NULL? &bIsCancel : pbCancel;
	CopyData.pProgressRoutine	= lpProgressRoutine;

	dwError = doIOCTL (
		IOCTL_COPY_FILE,
		&CopyData,
		sizeof(CopyData),
		NULL,
		0);

	SetLastError (dwError);
	return dwError == ERROR_SUCCESS;
#endif
}

BOOL WINAPI CKernelUtils::CopyFileOnReboot( 
	__in LPCTSTR lpExistingFileName, 
	__in LPCTSTR lpNewFileName, 
	__in BOOL bCopyOnReboot /*= TRUE */ )
{
	BOOL			bIsOk			= FALSE;
	BOOL			bIsCancel		= FALSE;
	COPY_FILE_IN	CopyData;
	DWORD			dwError			= 0;
	tstring			strSrcFile;
	tstring			strDstFile;

	if (lpExistingFileName == NULL || lpNewFileName == NULL)
	{
		return FALSE;
	}

	strSrcFile.assign(lpExistingFileName);
	strDstFile.assign(lpNewFileName);
	ZeroMemory(CopyData.fileSrcInfo.cFilePath, sizeof(CopyData.fileSrcInfo.cFilePath));
	ZeroMemory(CopyData.fileDstInfo.cFilePath, sizeof(CopyData.fileDstInfo.cFilePath));
	wcscat_s (CopyData.fileSrcInfo.cFilePath, strSrcFile.c_str ());
	wcscat_s (CopyData.fileDstInfo.cFilePath, strDstFile.c_str ());

	CopyData.DoFileAction= bCopyOnReboot? DO_RebootAction : DO_RemoveAction;
	CopyData.dwCopyFlags = 0;
	CopyData.lpData		 = NULL;
	CopyData.pbCancel	 = NULL;
	CopyData.pProgressRoutine	= NULL;

	dwError = doIOCTL (
		IOCTL_COPY_FILE,
		&CopyData,
		sizeof(CopyData),
		NULL,
		0);

	SetLastError (dwError);
	return dwError == ERROR_SUCCESS;
}

BOOL WINAPI CKernelUtils::MoveFileEx(
									 __in          LPCTSTR lpExistingFileName,
									 __in          LPCTSTR lpNewFileName,
									 __in          DWORD dwFlags
									 )
{
	if (this->CopyFile (lpExistingFileName, lpNewFileName, TRUE) == FALSE)
	{
		return FALSE;
	}

	return this->DeleteFileW (lpExistingFileName, FALSE);
}

BOOL WINAPI CKernelUtils::CopyFile(
								   __in          LPCTSTR lpExistingFileName,
								   __in          LPCTSTR lpNewFileName,
								   __in          BOOL bFailIfExists
								   )
{
	return this->CopyFileEx (lpExistingFileName, lpNewFileName, NULL, NULL, NULL, bFailIfExists? COPY_FILE_FAIL_IF_EXISTS : 0);
}

BOOL WINAPI CKernelUtils::MoveFile( 
								   __in LPCTSTR lpExistingFileName, 
								   __in LPCTSTR lpNewFileName 
								   )
{
	return this->MoveFileEx (lpExistingFileName, lpNewFileName, 0);
}

BOOL WINAPI CKernelUtils::LockFile( __in LPCTSTR lpFileName )
{
	LOCK_FILE_IN	LockData		= {0x00};
	DWORD			dwError			= 0;
	tstring			strFilePath(lpFileName);

	do 
	{
		if (strFilePath.length () >= sizeof(LockData.fileInfo.cFilePath))
		{
			dwError = ERROR_FILENAME_EXCED_RANGE;
			break;
		}

		wcscat_s (LockData.fileInfo.cFilePath, strFilePath.c_str ());

		dwError = doIOCTL (
			IOCTL_LOCK_FILE,
			&LockData,
			sizeof(LockData),
			NULL,
			0);
	} while (FALSE);

	SetLastError (dwError);
	return dwError == ERROR_SUCCESS;
}

BOOL WINAPI CKernelUtils::unLockFile( __in LPCTSTR lpFileName )
{
	UNLOCK_FILE_IN	unLockData		= {0x00};
	DWORD			dwError			= 0;
	tstring			strFilePath(lpFileName);

	do 
	{
		if (strFilePath.length () >= sizeof(unLockData.fileInfo.cFilePath))
		{
			dwError = ERROR_FILENAME_EXCED_RANGE;
			break;
		}

		wcscat_s (unLockData.fileInfo.cFilePath, strFilePath.c_str ());

		dwError = doIOCTL (
			IOCTL_UNLOCK_FILE,
			&unLockData,
			sizeof(unLockData),
			NULL,
			0);
	} while (FALSE);

	SetLastError (dwError);
	return dwError == ERROR_SUCCESS;
}

DWORD CKernelUtils::doIOCTL(
							__in DWORD dwIoControlCode,
							__in LPVOID lpInBuffer, 
							__in DWORD nInBufferSize, 
							__out LPVOID lpOutBuffer, 
							__out DWORD nOutBufferSize, 
							__out LPDWORD lpBytesReturned /*= NULL */ 
							)
{
	if ( m_hDriver == NULL || m_hDriver == INVALID_HANDLE_VALUE )
	{
		DWORD nRes( const_cast<CKernelUtils*>(this)->initialize() );
		if ( nRes != ERROR_SUCCESS )
		{
			// TODO
			return nRes;
		}
	}

	SetLastError (ERROR_SUCCESS);

	DWORD BytesReturned = 0;
	DeviceIoControl(
		m_hDriver, 
		dwIoControlCode,
		lpInBuffer, nInBufferSize,
		lpOutBuffer, nOutBufferSize,
		lpBytesReturned == NULL? &BytesReturned : lpBytesReturned, 
		NULL);

	BytesReturned = lpBytesReturned == NULL? BytesReturned : *lpBytesReturned;
	DWORD dwResult = ERROR_SUCCESS;
	if ( lpOutBuffer != NULL && BytesReturned > 0 )
	{
		dwResult = NtStatusToDosError (((PCOMMON_HEAD_OUT)lpOutBuffer)->IoStatus.Status);
	}
	else
	{
		dwResult = GetLastError();
	}

	return dwResult;
}

DWORD CKernelUtils::initialize()
{
	DWORD nRes = openDriver(BDArKit_R3_SYMBOL_LINK);
	if( nRes == ERROR_SUCCESS || nRes == ERROR_ACCESS_DENIED)
		return nRes;

	uninitialize ();

	nRes = installDriver(m_strDriverPath.c_str (), m_strDriverName.c_str ());
	if( nRes != ERROR_SUCCESS )
	{
		return nRes;
	}

	// Try again to open the driver
	nRes = openDriver(BDArKit_R3_SYMBOL_LINK);
	if( nRes == ERROR_SUCCESS )
	{
		return nRes;
	}

	// If still failed, try uninstall driver
	uninstallDriver (m_strDriverName.c_str ());

	return nRes;
}

DWORD CKernelUtils::uninitialize()
{
	DWORD dwError = ERROR_SUCCESS;

	if ( m_strDriverName.length () != 0)
	{
		dwError = uninstallDriver (m_strDriverName.c_str ());
		::DeleteFileW(m_strDriverPath.c_str ());
	}

	return dwError;
}

DWORD CKernelUtils::openDriver( LPCTSTR lpSymbolLink )
{
	if( m_hDriver != NULL && m_hDriver != INVALID_HANDLE_VALUE)
		return ERROR_SUCCESS;

	HANDLE h( ::CreateFile(
		lpSymbolLink,
		FILE_ALL_ACCESS,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL) );

	if( h == INVALID_HANDLE_VALUE )
		return GetLastError();

	m_hDriver = h;

	return ERROR_SUCCESS;
}

DWORD CKernelUtils::installDriver( LPCTSTR lpDriverPath, LPCTSTR lpDriverName )
{
	DWORD	dwError = ERROR_SUCCESS;

	DWORD dwResId = OSInfo::Is32Bit() ? IDR_DRIVER_32 : IDR_DRIVER_64;

	if ( dwResId == IDR_DRIVER_64 )
	{
		File::CWow64RedirDisable wow64Disable(TRUE);
		::DeleteFileW(lpDriverPath);
		::CopyFile(m_strOrigDriverPath.c_str (), lpDriverPath, FALSE);
	}
	else
	{
		::DeleteFileW(lpDriverPath);
		::CopyFile(m_strOrigDriverPath.c_str (), lpDriverPath, FALSE);
	}

	SC_HANDLE hService = NULL;
	SC_HANDLE hSCM = NULL;

	do 
	{
		hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if( !hSCM )
		{
			dwError = GetLastError();
			break;
		}

		hService = OpenService(hSCM, lpDriverName, SERVICE_ALL_ACCESS);
		if( !hService )
		{
			// Couldn't open service (probably doesn't exist). Create it
			hService = CreateService(
				hSCM,
				lpDriverName,
				L"BAIDU Ark Kit Service",
				SERVICE_ALL_ACCESS,
				SERVICE_KERNEL_DRIVER,
				SERVICE_AUTO_START,
				SERVICE_ERROR_NORMAL,
				lpDriverPath,
				NULL, NULL, NULL, NULL, NULL);
			if( !hService )
			{
				dwError = GetLastError();
				break;
			}
		}

	} while (FALSE);

	if ( hService )
	{
		BOOL bStarted = FALSE;
		bStarted = StartService(hService, 0, NULL);
		dwError = GetLastError();
		if ((bStarted == FALSE &&
			(dwError == ERROR_SERVICE_ALREADY_RUNNING ||
			dwError == ERROR_SERVICE_DISABLED)) ||
			bStarted == TRUE)
		{
			m_bIsRunning = TRUE;
			dwError = ERROR_SUCCESS;

		} else
		{
			m_bIsRunning = FALSE;
		}

		CloseServiceHandle (hService);
	}

	if ( hSCM )
	{
		CloseServiceHandle (hSCM);
	}

	return dwError;
}

DWORD CKernelUtils::uninstallDriver( LPCTSTR lpDriverName )
{
	if ( m_hDriver != NULL && m_hDriver != INVALID_HANDLE_VALUE)
	{
		::CloseHandle (m_hDriver);
		m_hDriver = INVALID_HANDLE_VALUE;
	}

	DWORD			dwError			= ERROR_SUCCESS;
	SC_HANDLE		schSCManager	= NULL;
	SC_HANDLE		schService		= NULL;

	do 
	{
		if ( lpDriverName == NULL )
		{
			dwError = ERROR_INVALID_PARAMETER;
			break;
		}

		schSCManager = OpenSCManager(	
			NULL,                 // machine (NULL == local)
			NULL,                 // database (NULL == default)
			SC_MANAGER_ALL_ACCESS // access required
			);
		if ( schSCManager == NULL)
		{
			dwError = GetLastError();
			break;
		}

		schService = OpenService( schSCManager, lpDriverName, SERVICE_ALL_ACCESS );
		if (schService == NULL)
		{
			if (GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST)
			{
				m_bIsRunning = FALSE;
			}
			break;
		}

		SERVICE_STATUS  serviceStatus	= {0x00};
		BOOL bResult = FALSE;
		bResult = ControlService( schService, SERVICE_CONTROL_STOP, &serviceStatus );
		if (bResult == FALSE)
		{
			dwError = GetLastError();
			if (dwError == ERROR_SERVICE_NOT_ACTIVE ||dwError == ERROR_SHUTDOWN_IN_PROGRESS)
			{
				m_bIsRunning = FALSE;
			}
		}

		m_bIsRunning = FALSE;

		bResult = DeleteService( schService );
		if (bResult == FALSE)
		{
			dwError = GetLastError();
			break;
		}
	} while (FALSE);

	if (schService != NULL)
	{
		CloseServiceHandle( schService );
	}

	if (schSCManager != NULL)
	{
		CloseServiceHandle( schSCManager );
	}

	return dwError;
}

ULONG CKernelUtils::NtStatusToDosError( NTSTATUS Status )
{
	if ( m_pfnRtlNtStatusToDosError == NULL )
	{
		m_pfnRtlNtStatusToDosError = 
			(_pfnRtlNtStatusToDosError)GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "RtlNtStatusToDosError");
	}

	if ( m_pfnRtlNtStatusToDosError != NULL )
	{
		return m_pfnRtlNtStatusToDosError(Status);
	}

	return Status;
}

ULONG CKernelUtils::RtlIsDosDeviceName_U( PCWSTR DosFileName )
{
	if ( m_pfnRtlIsDosDeviceName_U == NULL )
	{
		m_pfnRtlIsDosDeviceName_U = 
			(_pfnRtlIsDosDeviceName_U)GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "RtlIsDosDeviceName_U");
	}

	if ( m_pfnRtlIsDosDeviceName_U == NULL )
	{
		SetLastError(ERROR_INVALID_FUNCTION);
		return 0; 
	}

	return m_pfnRtlIsDosDeviceName_U(DosFileName);
}


//VOID CKernelUtils::RtlInitUnicodeString_U( PUNICODE_STRING NtFileName, PCWSTR DosFileName )
//{
//	if ( m_pfnRtlInitUnicodeString == NULL )
//	{
//		m_pfnRtlInitUnicodeString = 
//			(_pfnRtlInitUnicodeString)GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "RtlInitUnicodeString");
//	}
//
//	if ( m_pfnRtlInitUnicodeString == NULL )
//	{
//		SetLastError(ERROR_INVALID_FUNCTION);
//		return ; 
//	}
//
//	m_pfnRtlInitUnicodeString(NtFileName,DosFileName);
//}

//BOOL CKernelUtils::RtlDosPathNameToNtPathName_U(
//	PCWSTR DosFileName,
//	PUNICODE_STRING NtFileName,
//	PWSTR *FilePart,
//	PVOID Reserved
//	)
//{
//	if ( m_pfnRtlDosPathNameToNtPathName_U == NULL )
//	{
//		m_pfnRtlDosPathNameToNtPathName_U = 
//			(_pfnRtlDosPathNameToNtPathName_U)GetProcAddress (GetModuleHandle(_T("ntdll.dll")), "RtlDosPathNameToNtPathName_U");
//	}
//
//	if (m_pfnRtlDosPathNameToNtPathName_U == NULL)
//	{
//		SetLastError(ERROR_INVALID_FUNCTION);
//		return FALSE;
//	}
//
//	if (!m_pfnRtlDosPathNameToNtPathName_U(
//		DosFileName,
//		NtFileName,
//		FilePart,
//		Reserved))
//	{
//		SetLastError(ERROR_PATH_NOT_FOUND);
//		return FALSE;
//	}
//
//	return TRUE;
//}