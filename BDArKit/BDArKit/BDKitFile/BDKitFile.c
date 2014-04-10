#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "BDArKit\KitHelper.h"
#include "DriverHelper.h"
#include "Utility.h"
#include "NtosKrnl.h"
#include "BDKitUtils.h"
#include "BDKitHandler.h"


#define DEFAULT_FILE_COPY_ALLOCATION	4 * 1024


BOOLEAN FileIsExist (__in CONST PWCHAR wszFileFullPath)
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	OBJECT_ATTRIBUTES	oa			= {0x00};	
	UNICODE_STRING		uszFilePath	= {0x00};
	IO_STATUS_BLOCK		IoStatus	= {0x00};
	HANDLE				hFile		= NULL;
	BOOLEAN				bExist		= FALSE;

	do 
	{
		RtlInitUnicodeString (&uszFilePath, wszFileFullPath);

		InitializeObjectAttributes(
			&oa,
			&uszFilePath,
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
			NULL,
			NULL
			);

		nsStatus = BDKitCreateFileByObject (
			&hFile,
			SYNCHRONIZE,
			&oa,
			&IoStatus,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_OPEN,
			FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			0,
			CreateFileTypeNone,
			NULL,
			IO_NO_PARAMETER_CHECKING | IO_CHECK_CREATE_PARAMETERS,
			IOP_CREATE_IGNORE_SHARE_ACCESS_CHECK,
			NULL
			);

		bExist = NT_SUCCESS(nsStatus) || nsStatus == STATUS_DELETE_PENDING;
	} while (FALSE);

	BDKitCloseHandle(hFile);

	return bExist;
}

NTSTATUS BDKitFocusCreateFile(
							  __in_bcount(ulInBufferSize)	PVOID pInBuffer,
							  __in		ULONG			ulInBufferSize,
							  __out_bcount(ulOutBufferSize) PVOID pOutBuffer,
							  __in		ULONG			ulOutBufferSize,
							  __out		PULONG			information,
							  __in		ULONG			level
							  )
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	FILE_CREATE_IN*		pInData		= (FILE_CREATE_IN*)pInBuffer;
	FILE_CREATE_OUT*	pOutData	= (FILE_CREATE_OUT*)pOutBuffer;
	OBJECT_ATTRIBUTES	oa			= {0x00};
	UNICODE_STRING		uszFileName	= {0x00};
	IO_STATUS_BLOCK		IoStatus	= {0x00};
	HANDLE				hKeFile		= NULL;
	HANDLE				hUserFile	= NULL;

	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);

	RtlInitUnicodeString (&uszFileName, pInData->fileInfo.cFilePath);
	InitializeObjectAttributes(&oa, &uszFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

	do 
	{
		switch(level)
		{
		case emNormalLevel:
			{
				nsStatus = BDKitCreateFileByAPI (
					&hKeFile,
					pInData->AccessMask,
					&oa,
					&IoStatus,
					pInData->FileAttributes,
					pInData->ShareAccess,
					pInData->CreateDisposition,
					pInData->CreateOptions
					);
				BDKit_If_Not_Break(!NT_SUCCESS(nsStatus) && hKeFile == NULL);
			}

		case emMiddleLevel:
			{
				nsStatus = BDKitCreateFileByObject (
					&hKeFile,
					pInData->AccessMask,
					&oa,
					&IoStatus,
					NULL,
					pInData->FileAttributes,
					pInData->ShareAccess,
					pInData->CreateDisposition,
					pInData->CreateOptions,
					NULL,
					0,
					CreateFileTypeNone,
					NULL,
					IO_NO_PARAMETER_CHECKING | IO_CHECK_CREATE_PARAMETERS,
					IOP_CREATE_IGNORE_SHARE_ACCESS_CHECK,
					NULL
					);
				BDKit_If_Not_Break(!NT_SUCCESS(nsStatus) && hKeFile == NULL);
			}

		case emFocusLevel:
			{
				//nsStatus = BDKitCreateFileByDupHandle (
				//	&hKeFile,
				//	pInData->AccessMask,
				//	&oa,
				//	&IoStatus,
				//	pInData->FileAttributes,
				//	pInData->ShareAccess,
				//	pInData->CreateDisposition,
				//	pInData->CreateOptions
				//	);
			}

		case emPowerfulLevel:
			{
				// TODO		
			}
			break;

		default:
			{
				// TODO
			}
			break;
		}
	} while (FALSE);

	if ( hKeFile != NULL )
	{
		PFILE_OBJECT pFileObject = NULL;
		nsStatus = ObReferenceObjectByHandle (
			hKeFile,
			pInData->AccessMask,
			*IoFileObjectType,
			KernelMode,
			&pFileObject,
			NULL
			);
		if ( NT_SUCCESS(nsStatus) )
		{
			nsStatus = ObOpenObjectByPointer(
				pFileObject,
				0,
				NULL,
				pInData->AccessMask,
				*IoFileObjectType,
				UserMode,
				&hUserFile
				);
		}
		BDKitCloseHandle(hKeFile);
		BDKitCloseObject(pFileObject);

		*information = sizeof(FILE_CREATE_OUT);
	}

	pOutData->hFile = hUserFile;

	return nsStatus;
}

NTSTATUS BDKitFocusReadFile(
							__in_bcount(ulInBufferSize)		PVOID pInBuffer,
							__in		ULONG			ulInBufferSize,
							__out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
							__in		ULONG			ulOutBufferSize,
							__out		PULONG			information,
							__in		ULONG			level
							)
{
	NTSTATUS		nsStatus= STATUS_UNSUCCESSFUL;
	IO_STATUS_BLOCK	IoStatus= {0x00};
	_ULONG			Readed	= 0;
	PVOID			Buffer	= NULL;
	FILE_READ_IN*	pInData = (FILE_READ_IN*)pInBuffer;
	FILE_READ_OUT*	pOutData= (FILE_READ_OUT*)pOutBuffer;

	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);
	UNREFERENCED_PARAMETER(level);

	nsStatus = BDKitReadFile (
		pInData->Handle,
		pInData->Event,
		NULL,
		NULL,
		&IoStatus,
		pInData->Buffer,
		pInData->nNumberBytesOfToRead,
		&pInData->Offset,
		NULL
		);
	Readed = IoStatus.Information;
	Buffer = pInData->Buffer;
	pOutData->Buffer = Buffer;
	pOutData->nNumberBytesOfRead	= pInData->nNumberBytesOfRead;
	*pOutData->nNumberBytesOfRead	= (ULONG)Readed;

	*information = sizeof(FILE_READ_OUT);

	return nsStatus;
}

NTSTATUS BDKitFocusWriteFile(
							 __in_bcount(ulInBufferSize)	PVOID pInBuffer,
							 __in		ULONG			ulInBufferSize,
							 __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
							 __in		ULONG			ulOutBufferSize,
							 __out		PULONG			information,
							 __in		ULONG			level
							 )
{
	NTSTATUS		nsStatus= STATUS_UNSUCCESSFUL;
	IO_STATUS_BLOCK	IoStatus= {0x00};
	_ULONG			Writed	= 0;
	FILE_WRITE_IN*	pInData = (FILE_WRITE_IN*)pInBuffer;
	FILE_WRITE_OUT*	pOutData= (FILE_WRITE_OUT*)pOutBuffer;

	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);
	UNREFERENCED_PARAMETER(level);

	nsStatus = BDKitWriteFile  (
		pInData->Handle,
		pInData->Event,
		NULL,
		NULL,
		&IoStatus,
		pInData->Buffer,
		pInData->nNumberBytesOfToWrite,
		&pInData->Offset,
		NULL
		);
	Writed = IoStatus.Information;
	pOutData->nNumberBytesOfWrite	= pInData->nNumberBytesOfWrite;
	*pOutData->nNumberBytesOfWrite	= (ULONG)Writed;

	*information = sizeof(FILE_WRITE_OUT);

	return nsStatus;
}

NTSTATUS BDKitFocusCloseFile(
							 __in_bcount(ulInBufferSize)	PVOID pInBuffer,
							 __in		ULONG			ulInBufferSize,
							 __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
							 __in		ULONG			ulOutBufferSize,
							 __out		PULONG			information,
							 __in		ULONG			level
							 )
{
	HANDLE hFile = (HANDLE)pInBuffer;

	UNREFERENCED_PARAMETER(ulInBufferSize);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(information);
	UNREFERENCED_PARAMETER(level);

	return BDKitCloseFile (hFile);
}

NTSTATUS BDKitFocusDeleteFile(
							  __in_bcount(ulInBufferSize)	PVOID pInBuffer,
							  __in		ULONG			ulInBufferSize,
							  __out_bcount(ulOutBufferSize) PVOID pOutBuffer,
							  __in		ULONG			ulOutBufferSize,
							  __out		PULONG			information,
							  __in		ULONG			level
							  )
{
	NTSTATUS			nsStatus		= STATUS_UNSUCCESSFUL;
	PUNICODE_STRING		puszFileFullPath= (PUNICODE_STRING)pInBuffer;

	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);
	UNREFERENCED_PARAMETER(information);

	do 
	{
		switch(level)
		{
		case emNormalLevel:
			{
				nsStatus = BDKitDeleteFileByAPI (puszFileFullPath);	
				BDKit_If_Not_Break_With_Reason(FileIsExist (puszFileFullPath->Buffer), STATUS_SUCCESS);	
			}

		case emMiddleLevel:
			{
				BDKitDerefHandleByCloseHandle (puszFileFullPath);
				nsStatus = BDKitDeleteFileByIRP (puszFileFullPath);
				BDKit_If_Not_Break_With_Reason(FileIsExist (puszFileFullPath->Buffer), STATUS_SUCCESS);	
			}

		case emFocusLevel:
			{
				nsStatus = BDKitDeleteFileByClearSection (puszFileFullPath);
				BDKit_If_Not_Break_With_Reason(FileIsExist (puszFileFullPath->Buffer), STATUS_SUCCESS);
			}

		case emPowerfulLevel:
			{
				//nsStatus = BDKitDeleteFileByXCB (puszFileFullPath);
				//BDKit_If_Not_Break_With_Reason(FileIsExist (puszFileFullPath->Buffer), STATUS_SUCCESS);
			}
			break;

		default:
			{
				// TODO
			}
			break;
		}
	} while (FALSE);

	return nsStatus;
}

NTSTATUS BDKitAddDeleteFileOnReboot(
								 __in_bcount(ulInBufferSize)	PVOID pInBuffer,
								 __in		ULONG			ulInBufferSize,
								 __out_bcount(ulOutBufferSize) PVOID pOutBuffer,
								 __in		ULONG			ulOutBufferSize,
								 __out		PULONG			information,
								 __in		ULONG			level
								)
{
	NTSTATUS			nsStatus		= STATUS_UNSUCCESSFUL;
	PUNICODE_STRING		puszFileFullPath= (PUNICODE_STRING)pInBuffer;

	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);
	UNREFERENCED_PARAMETER(information);
	UNREFERENCED_PARAMETER(level);

	nsStatus = BDKitAddRebootDeleteFile (puszFileFullPath);

	return nsStatus;
}

NTSTATUS BDKitRemoveDeleteFileOnReboot(
									   __in_bcount(ulInBufferSize)	PVOID pInBuffer,
									   __in		ULONG			ulInBufferSize,
									   __out_bcount(ulOutBufferSize) PVOID pOutBuffer,
									   __in		ULONG			ulOutBufferSize,
									   __out		PULONG			information,
									   __in		ULONG			level
									   )
{
	NTSTATUS			nsStatus		= STATUS_UNSUCCESSFUL;
	PUNICODE_STRING		puszFileFullPath= (PUNICODE_STRING)pInBuffer;

	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);
	UNREFERENCED_PARAMETER(information);
	UNREFERENCED_PARAMETER(level);

	nsStatus = BDKitRemoveRebootDeleteFile (puszFileFullPath);

	return nsStatus;
}

NTSTATUS BDKitAddRecoverFileOnReboot(
									 __in_bcount(ulInBufferSize)	PVOID pInBuffer,
									 __in		ULONG			ulInBufferSize,
									 __out_bcount(ulOutBufferSize) PVOID pOutBuffer,
									 __in		ULONG			ulOutBufferSize,
									 __out		PULONG			information,
									 __in		ULONG			level
									 )
{
	NTSTATUS			nsStatus = STATUS_UNSUCCESSFUL;
	PCOPY_FILE_EX_PROXY	pCpyFile = (PCOPY_FILE_EX_PROXY)pInBuffer;

	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);
	UNREFERENCED_PARAMETER(information);
	UNREFERENCED_PARAMETER(level);

	nsStatus = BDKitAddRecoverFile (pCpyFile->pExistingFileName, pCpyFile->pNewFileName);

	return nsStatus;
}

NTSTATUS BDKitRemoveRecoverFileOnReboot(
										__in_bcount(ulInBufferSize)	PVOID pInBuffer,
										__in		ULONG			ulInBufferSize,
										__out_bcount(ulOutBufferSize) PVOID pOutBuffer,
										__in		ULONG			ulOutBufferSize,
										__out		PULONG			information,
										__in		ULONG			level
										)
{
	NTSTATUS			nsStatus = STATUS_UNSUCCESSFUL;
	PCOPY_FILE_EX_PROXY	pCpyFile = (PCOPY_FILE_EX_PROXY)pInBuffer;

	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);
	UNREFERENCED_PARAMETER(information);
	UNREFERENCED_PARAMETER(level);

	nsStatus = BDKitRemoveRecoverFile (pCpyFile->pExistingFileName, pCpyFile->pNewFileName);

	return nsStatus;
}

NTSTATUS BDKitDenyFileCreate(
							 __in_bcount(ulInBufferSize)	PVOID pInBuffer,
							 __in		ULONG			ulInBufferSize,
							 __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
							 __in		ULONG			ulOutBufferSize,
							 __out		PULONG			information,
							 __in		ULONG			level
							 )
{
	NTSTATUS			nsStatus			= STATUS_UNSUCCESSFUL;
	PUNICODE_STRING		puszFileLockPath	= (PUNICODE_STRING)pInBuffer;

	UNREFERENCED_PARAMETER(level);
	UNREFERENCED_PARAMETER(information);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);

	nsStatus = BDKitAddFileDenyAccess (puszFileLockPath);

	return nsStatus;
}

NTSTATUS BDKitRemoveDenyFileCreate(
								   __in_bcount(ulInBufferSize)	PVOID pInBuffer,
								   __in		ULONG			ulInBufferSize,
								   __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
								   __in		ULONG			ulOutBufferSize,
								   __out		PULONG			information,
								   __in		ULONG			level
								   )
{
	NTSTATUS			nsStatus			= STATUS_UNSUCCESSFUL;
	PUNICODE_STRING		puszFileLockPath	= (PUNICODE_STRING)pInBuffer;

	UNREFERENCED_PARAMETER(level);
	UNREFERENCED_PARAMETER(information);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);

	nsStatus = BDKitRemoveFileDenyAccess (puszFileLockPath);

	return nsStatus;
}

NTSTATUS BDKitFocusLockFile(
							__in_bcount(ulInBufferSize) PVOID pInBuffer,
							__in		ULONG			ulInBufferSize,
							__out_bcount(ulOutBufferSize) PVOID pOutBuffer,
							__in		ULONG			ulOutBufferSize,
							__out		PULONG			information,
							__in ULONG level
							)
{
	NTSTATUS nsStatus = STATUS_UNSUCCESSFUL;

	do 
	{
		switch(level)
		{
		case emNormalLevel:
			{

				nsStatus = BDKitDenyFileCreate(
					pInBuffer,
					ulInBufferSize,
					pOutBuffer,
					ulOutBufferSize,
					information,
					emNormalLevel
					);
			}

		case emMiddleLevel:
			{
				// TODO
			}

		case emFocusLevel:
			{
				// TODO
			}

		case emPowerfulLevel:
			{
				// TODO		
			}
			break;

		default:
			{
				// TODO
			}
			break;
		}
	} while (FALSE);

	return nsStatus;
}

NTSTATUS BDKitFocusUnlockFile(
							  __in_bcount(ulInBufferSize) PVOID pInBuffer,
							  __in		ULONG			ulInBufferSize,
							  __out_bcount(ulOutBufferSize) PVOID pOutBuffer,
							  __in		ULONG			ulOutBufferSize,
							  __out		PULONG			information,
							  __in ULONG level
							  )
{
	NTSTATUS nsStatus = STATUS_UNSUCCESSFUL;

	do 
	{
		switch(level)
		{
		case emNormalLevel:
			{
				nsStatus = BDKitRemoveDenyFileCreate(
					pInBuffer,
					ulInBufferSize,
					pOutBuffer,
					ulOutBufferSize,
					information,
					emNormalLevel
					);
			}

		case emMiddleLevel:
			{
				// TODO
			}

		case emFocusLevel:
			{
				// TODO
			}

		case emPowerfulLevel:
			{
				// TODO		
			}
			break;

		default:
			{
				// TODO
			}
			break;
		}
	} while (FALSE);

	return nsStatus;
}

#define COPY_FILE_FAIL_IF_EXISTS              0x00000001
NTSTATUS BDKitFocusCopyFile(
							__in_bcount(ulInBufferSize)		PVOID pInBuffer,
							__in		ULONG			ulInBufferSize,
							__out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
							__in		ULONG			ulOutBufferSize,
							__out		PULONG			information,
							__in		ULONG			level
							)
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	HANDLE		hSrcFile	= NULL;
	HANDLE		hDstFile	= NULL;
	PVOID		pBuffer		= NULL;
	BOOL		bIsCancel		= FALSE;
	LONGLONG	ullOffset	= 0;
	OBJECT_ATTRIBUTES	oaS	= {0x00};
	OBJECT_ATTRIBUTES	oaD	= {0x00};
	IO_STATUS_BLOCK	IoStatus= {0x00};
	LARGE_INTEGER		llOrgSize	= {0x00};
	FILE_STANDARD_INFORMATION	fileInfo	= {0x00};
	PCOPY_FILE_EX_PROXY	pCpyFile = (PCOPY_FILE_EX_PROXY)pInBuffer;

	do 
	{
		BDKit_If_Not_Break_With_Reason(pCpyFile != NULL, STATUS_INVALID_PARAMETER);
		BDKit_If_Not_Break_With_Reason((pCpyFile->dwCopyFlags&~COPY_FILE_FAIL_IF_EXISTS) == 0, STATUS_INVALID_PARAMETER);

		InitializeObjectAttributes(&oaS, pCpyFile->pExistingFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
		InitializeObjectAttributes(&oaD, pCpyFile->pNewFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

		// TODO DELETE FILE
		BDKitFocusDeleteFile (
			pCpyFile->pNewFileName,
			sizeof(UNICODE_STRING) + pCpyFile->pNewFileName->MaximumLength,
			NULL,
			0,
			NULL,
			emNormalLevel
			);

		nsStatus = ZwCreateFile (
			&hSrcFile,
			GENERIC_READ,
			&oaS,
			&IoStatus,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ,
			FILE_OPEN,
			FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			0
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ZwCreateFile (
			&hDstFile,
			GENERIC_WRITE,
			&oaD,
			&IoStatus,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ,
			FILE_CREATE,
			FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			0
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
		
		nsStatus = ZwQueryInformationFile (
			hSrcFile,
			&IoStatus,
			&fileInfo,
			sizeof(fileInfo),
			FileStandardInformation
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		llOrgSize.QuadPart = fileInfo.EndOfFile.QuadPart;
		BDKit_If_Not_Break_With_Reason(llOrgSize.HighPart == 0, STATUS_FILE_TOO_LARGE);

		BDKitAllocateNonpagePool(pBuffer, DEFAULT_FILE_COPY_ALLOCATION);
		BDKit_If_Not_Break_With_Reason(pBuffer != NULL, STATUS_INSUFF_SERVER_RESOURCES);

		ullOffset = 0;
		while(ullOffset < llOrgSize.QuadPart && bIsCancel == FALSE)
		{
			RtlZeroMemory(pBuffer, DEFAULT_FILE_COPY_ALLOCATION);
			nsStatus = ZwReadFile (
				hSrcFile,
				NULL,
				NULL,
				NULL,
				&IoStatus,
				pBuffer,
				DEFAULT_FILE_COPY_ALLOCATION,
				NULL,
				NULL
				);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
			BDKit_If_Not_Break(IoStatus.Information != 0);

			nsStatus = ZwWriteFile (
				hDstFile,
				NULL,
				NULL,
				NULL,
				&IoStatus,
				pBuffer,
				(ULONG)IoStatus.Information,
				NULL,
				NULL
				);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
			ullOffset += IoStatus.Information;

			if (pCpyFile->pbCancel != NULL)
			{
				bIsCancel = *pCpyFile->pbCancel;
			}
		}

		BDKit_If_DoAction(ullOffset == llOrgSize.QuadPart, STATUS_SUCCESS);
	} while (FALSE);

	BDKitCloseHandle(hDstFile);
	BDKitCloseHandle(hSrcFile);
	BDKitFreePool(pBuffer);

	return nsStatus;
}

NTSTATUS BDKitFindFirstFile(
							__in_bcount(ulInBufferSize)		PVOID pInBuffer,
							__in		ULONG			ulInBufferSize,
							__out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
							__in		ULONG			ulOutBufferSize,
							__out		PULONG			information,
							__in		ULONG			level
							)

{
	NTSTATUS				nsStatus				= STATUS_UNSUCCESSFUL;
	FIND_FIRST_FILE_IN*		pQueryDirectory			= (FIND_FIRST_FILE_IN*)pInBuffer;
	FIND_FIRST_FILE_OUT*	pResultData				= (FIND_FIRST_FILE_OUT*)pOutBuffer;
	OBJECT_ATTRIBUTES		oa						= {0x00};
	IO_STATUS_BLOCK			IoStatus				= {0x00};
	UNICODE_STRING			uszDirectory			= {0x00};
	UNICODE_STRING			uszFilePattern			= {0x00};
	ULONG					ulDirInfoLen			= 0;
	HANDLE					hDirectory				= NULL;
	HANDLE					hUserHandle				= NULL;
	PFILE_OBJECT			pDirectoryObject		= NULL;
	PFILE_DIRECTORY_INFORMATION	pDirInfo			= NULL;

	UNREFERENCED_PARAMETER(level);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);

	do 
	{
		BDKit_If_Not_Break(pInBuffer != NULL && pOutBuffer != NULL && information != NULL);

		RtlInitUnicodeString (&uszDirectory, pQueryDirectory->szDirectory);
		RtlInitUnicodeString (&uszFilePattern, pQueryDirectory->szFilePattern);

		InitializeObjectAttributes(
			&oa, 
			&uszDirectory, 
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 
			NULL, 
			NULL
			);
		nsStatus = BDKitCreateFileByObject (
			&hDirectory,
			SYNCHRONIZE | FILE_LIST_DIRECTORY,
			&oa,
			&IoStatus,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_OPEN,
			FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			0,
			CreateFileTypeNone,
			NULL,
			IO_NO_PARAMETER_CHECKING | IO_CHECK_CREATE_PARAMETERS,
			IOP_CREATE_IGNORE_SHARE_ACCESS_CHECK,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		ulDirInfoLen = sizeof(FILE_DIRECTORY_INFORMATION) + sizeof(pResultData->cFileName);
		BDKitAllocateNonpagePool(pDirInfo, ulDirInfoLen);
		BDKit_If_Not_Break(pDirInfo != NULL);

		nsStatus = BDKitQueryDirectoryFile (
			hDirectory,
			NULL,
			NULL,
			NULL,
			&IoStatus,
			pDirInfo,
			ulDirInfoLen,
			FileDirectoryInformation,
			TRUE,
			uszFilePattern.Length == 0? NULL : &uszFilePattern,
			TRUE
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ObReferenceObjectByHandle(
			hDirectory,
			FILE_LIST_DIRECTORY | SYNCHRONIZE,
			*IoFileObjectType,
			KernelMode,
			&pDirectoryObject,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ObOpenObjectByPointer(
			pDirectoryObject,
			0,
			NULL,
			FILE_LIST_DIRECTORY | SYNCHRONIZE,
			*IoFileObjectType,
			UserMode,
			&hUserHandle
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		BDKit_If_Not_Break_With_Reason(sizeof(pResultData->cFileName) >= pDirInfo->FileNameLength,
			STATUS_INFO_LENGTH_MISMATCH);

		pResultData->hDirectory			= hUserHandle;
		pResultData->dwFileAttributes	= pDirInfo->FileAttributes;
		pResultData->ftCreationTime		= pDirInfo->CreationTime;
		pResultData->ftLastWriteTime	= pDirInfo->LastWriteTime;
		pResultData->ftLastAccessTime	= pDirInfo->LastAccessTime;
		pResultData->nFileSizeHigh		= pDirInfo->AllocationSize.HighPart;
		pResultData->nFileSizeLow		= pDirInfo->AllocationSize.LowPart;

		RtlZeroMemory(pResultData->cFileName, sizeof(pResultData->cFileName));
		RtlCopyMemory(
			pResultData->cFileName,
			pDirInfo->FileName,
			pDirInfo->FileNameLength);

		*information = sizeof(FIND_FIRST_FILE_OUT);

	} while (FALSE);

	BDKitFreePool(pDirInfo);
	BDKitCloseHandle(hDirectory);
	BDKitCloseObject(pDirectoryObject);

	return nsStatus;
}

NTSTATUS BDKitFindNextFile(
						   __in_bcount(ulInBufferSize)		PVOID pInBuffer,
						   __in		ULONG			ulInBufferSize,
						   __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
						   __in		ULONG			ulOutBufferSize,
						   __out	PULONG			information,
						   __in		ULONG			level
						   )

{
	NTSTATUS				nsStatus			= STATUS_UNSUCCESSFUL;
	FIND_NEXT_FILE_IN*		pQueryDirectory		= (FIND_NEXT_FILE_IN*)pInBuffer;
	FIND_NEXT_FILE_OUT*		pResultData			= (FIND_NEXT_FILE_OUT*)pOutBuffer;
	ULONG					ulDirInfoLen		= 0;
	IO_STATUS_BLOCK			IoStatus			= {0x00};
	UNICODE_STRING			uszFilePattern		= {0x00};
	PFILE_OBJECT			pDirectoryObject	= NULL;
	HANDLE					hKernelHandle		= NULL;
	PFILE_DIRECTORY_INFORMATION	pDirInfo		= NULL;

	UNREFERENCED_PARAMETER(level);
	UNREFERENCED_PARAMETER(ulInBufferSize);
	UNREFERENCED_PARAMETER(ulOutBufferSize);

	do 
	{
		BDKit_If_Not_Break(pInBuffer != NULL && pOutBuffer != NULL && information != NULL);

		RtlInitUnicodeString (&uszFilePattern, pQueryDirectory->szFilePattern);

		nsStatus = ObReferenceObjectByHandle(
			pQueryDirectory->hDirectory,
			FILE_LIST_DIRECTORY | SYNCHRONIZE,
			*IoFileObjectType,
			UserMode,
			&pDirectoryObject,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ObOpenObjectByPointer(
			pDirectoryObject,
			OBJ_KERNEL_HANDLE,
			NULL,
			FILE_LIST_DIRECTORY | SYNCHRONIZE,
			*IoFileObjectType,
			KernelMode,
			&hKernelHandle
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		ulDirInfoLen = sizeof(FILE_DIRECTORY_INFORMATION) + sizeof(pResultData->cFileName);
		BDKitAllocateNonpagePool(pDirInfo, ulDirInfoLen);
		BDKit_If_Not_Break(pDirInfo != NULL);

		nsStatus = BDKitQueryDirectoryFile (
			hKernelHandle,
			NULL,
			NULL,
			NULL,
			&IoStatus,
			pDirInfo,
			ulDirInfoLen,
			FileDirectoryInformation,
			TRUE,
			uszFilePattern.Length == 0? NULL : &uszFilePattern,
			FALSE	
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		pResultData->dwFileAttributes	= pDirInfo->FileAttributes;
		pResultData->ftCreationTime		= pDirInfo->CreationTime;
		pResultData->ftLastWriteTime	= pDirInfo->LastWriteTime;
		pResultData->ftLastAccessTime	= pDirInfo->LastAccessTime;
		pResultData->nFileSizeHigh		= pDirInfo->AllocationSize.HighPart;
		pResultData->nFileSizeLow		= pDirInfo->AllocationSize.LowPart;

		RtlZeroMemory(pResultData->cFileName, sizeof(pResultData->cFileName));
		RtlCopyMemory(
			pResultData->cFileName,
			pDirInfo->FileName,
			pDirInfo->FileNameLength);

		*information = sizeof(FIND_FIRST_FILE_OUT);
	} while (FALSE);

	BDKitFreePool(pDirInfo);
	BDKitCloseHandle(hKernelHandle);
	BDKitCloseObject(pDirectoryObject);

	return nsStatus;
}

NTSTATUS BDKitQueryObject(
						   __in_bcount(ulInBufferSize)		PVOID pInBuffer,
						   __in		ULONG			ulInBufferSize,
						   __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
						   __in		ULONG			ulOutBufferSize,
						   __out	PULONG			information,
						   __in		ULONG			level
						   )
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	QUERY_OBJECT_IN*	pInData		= (QUERY_OBJECT_IN*)pInBuffer;

	nsStatus = ZwQueryObject (
		pInData->hObjectHandle,
		pInData->ObjectInformationClass,
		pInData->ObjectInformation,
		pInData->Length,
		pInData->ResultLength
		);

	*information = sizeof(QUERY_OBJECT_IN);

	return nsStatus;
}