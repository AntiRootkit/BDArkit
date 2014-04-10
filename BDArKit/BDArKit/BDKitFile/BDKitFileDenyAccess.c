#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "DriverHelper.h"
#include "Utility.h"
#include "List.h"
#include "BDKitUtils.h"
#include "miniFilter.h"


static BOOLEAN	g_bAddedCallback	= FALSE;
static HANDLE	g_DenyFileHashList	= NULL;

NTSTATUS AddDenyFileAccessByCreateFile(__in PUNICODE_STRING puszFileFullPath)
{
	NTSTATUS			nsStatus			= STATUS_UNSUCCESSFUL;
	IO_STATUS_BLOCK		IoStatus			= {0x00};
	HANDLE				hKeFile				= NULL;
	OBJECT_ATTRIBUTES	oa					= {0x00};
	FILE_BASIC_INFORMATION	FileAttributes	= {0x00};

	do 
	{
		InitializeObjectAttributes(
			&oa, 
			puszFileFullPath, 
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 
			NULL, 
			NULL
			);
		nsStatus = ZwCreateFile(
			&hKeFile,
			GENERIC_READ,
			&oa,
			&IoStatus,
			NULL,
			FILE_ATTRIBUTE_SYSTEM,
			0,
			FILE_OPEN_IF,
			FILE_SYNCHRONOUS_IO_NONALERT | FILE_DELETE_ON_CLOSE,
			NULL,
			0);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && hKeFile != NULL);

		nsStatus = ZwQueryInformationFile (hKeFile, &IoStatus, &FileAttributes, sizeof(FileAttributes), FileBasicInformation);
		FileAttributes.FileAttributes = FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN;
		nsStatus = ZwSetInformationFile (hKeFile, &IoStatus, &FileAttributes, sizeof(FileAttributes), FileBasicInformation);

	} while (FALSE);

	//BDKitCloseHandle(hKeFile);

	if ( puszFileFullPath->Buffer != NULL )
	{
		PWSTR	pszFilePath = NULL;
		BDKitAllocateNonpagePool(pszFilePath, puszFileFullPath->Length + sizeof(WCHAR));
		RtlCopyMemory(pszFilePath, puszFileFullPath->Buffer, puszFileFullPath->Length);
		BDKitAddDeleteFileList (pszFilePath);
	}

	return nsStatus;
}

BOOLEAN findDenyFilePath(PVOID pNode1, PVOID pNode2)
{
	return !RtlCompareUnicodeString ((PUNICODE_STRING)pNode1, (PUNICODE_STRING)pNode2, TRUE);	
}

BOOLEAN removeDenyFilePath(PVOID pNode1, PVOID pNode2)
{
	if ( RtlCompareUnicodeString ((PUNICODE_STRING)pNode1, (PUNICODE_STRING)pNode2, TRUE) == 0 )
	{
		BDKitFreePool(pNode1);
		return TRUE;
	}

	return FALSE;
}

VOID freeDenyListNode(PVOID pNode)
{
	BDKitFreePool(pNode);
}

FLT_PREOP_CALLBACK_STATUS FLTAPI DenyCreateFileRoutine(
	__inout PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__deref_out_opt PVOID *CompletionContext
	)
{
	NTSTATUS nsStatus = FLT_PREOP_SUCCESS_NO_CALLBACK;
	PFLT_FILE_NAME_INFORMATION pFileName = NULL;

	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(CompletionContext);

	if (	Data->Iopb->MajorFunction == IRP_MJ_SET_INFORMATION 
		&&	Data->Iopb->Parameters.SetFileInformation.FileInformationClass == FileRenameInformation)
	{
		if (((FILE_RENAME_INFORMATION*)Data->Iopb->Parameters.SetFileInformation.InfoBuffer)->FileName != NULL)
		{
			UNICODE_STRING	uszFileNtPath		= {0x00};
			PUNICODE_STRING	puszStoreFilePath	= NULL;
			uszFileNtPath.Buffer = ((FILE_RENAME_INFORMATION*)Data->Iopb->Parameters.SetFileInformation.InfoBuffer)->FileName;
			uszFileNtPath.Length = (USHORT)((FILE_RENAME_INFORMATION*)Data->Iopb->Parameters.SetFileInformation.InfoBuffer)->FileNameLength;
			uszFileNtPath.MaximumLength = uszFileNtPath.Length;
			nsStatus = BDKitVolumeNameToDosName (&uszFileNtPath, &puszStoreFilePath);
			if (	NT_SUCCESS(nsStatus)
				&&	BDKitFindListNodeWithHandler (g_DenyFileHashList, puszStoreFilePath, findDenyFilePath) != NULL 
				&& !NT_SUCCESS(BDKitIsAuthorizedProcess(PsGetCurrentProcess())))
			{
				Data->IoStatus.Status = STATUS_ACCESS_DENIED;
				Data->IoStatus.Information = 0;
				nsStatus = FLT_PREOP_COMPLETE;
			}
			BDKitFreePool(puszStoreFilePath);
		}

	} else if ( Data->Iopb->MajorFunction == IRP_MJ_CREATE )
	{
		do 
		{
			nsStatus = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED, &pFileName);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

			if ( BDKitFindListNodeWithHandler(g_DenyFileHashList, &pFileName->Name, findDenyFilePath) != NULL )
			{
				Data->IoStatus.Status = STATUS_ACCESS_DENIED;
				Data->IoStatus.Information = 0;
				nsStatus = FLT_PREOP_COMPLETE;
			}
		} while (FALSE);

		BDKit_If_DoAction(pFileName != NULL, FltReleaseFileNameInformation (pFileName));

	} else if ( Data->Iopb->MajorFunction == IRP_MJ_UNREGISTER)
	{
		BDKitCloseListHandleWithHandler (g_DenyFileHashList, freeDenyListNode);
		g_DenyFileHashList = NULL;
	}

	return nsStatus;
}

NTSTATUS AddDenyFileAccessByFilter(__in PUNICODE_STRING puszFileFullPath)
{
	NTSTATUS		nsStatus			= STATUS_UNSUCCESSFUL;
	PUNICODE_STRING	puszStoreFilePath	= NULL;

	do 
	{
		if ( g_DenyFileHashList == NULL )
		{
			g_DenyFileHashList = BDKitInitializeList ('YNED');
			BDKit_If_Not_Break(g_DenyFileHashList != NULL);

			nsStatus = BDKitRegisterFileOperationCallback (DenyCreateFileRoutine);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
		}

		nsStatus = BDKitVolumeNameToDosName (puszFileFullPath, &puszStoreFilePath);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		BDKitInsertListNode (g_DenyFileHashList, puszStoreFilePath);

		nsStatus = STATUS_SUCCESS;
	} while (FALSE);

	return nsStatus;
}

NTSTATUS RemoveDenyFileAccessByFilter(__in PUNICODE_STRING puszFileFullPath)
{
	NTSTATUS		nsStatus			= STATUS_UNSUCCESSFUL;
	PUNICODE_STRING	puszStoreFilePath	= NULL;

	do 
	{
		BDKit_If_Not_Break(g_DenyFileHashList != NULL);

		nsStatus = BDKitVolumeNameToDosName (puszFileFullPath, &puszStoreFilePath);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = BDKitRemoveListNodeWithHandler (
			g_DenyFileHashList, 
			puszStoreFilePath, 
			removeDenyFilePath)? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
	} while (FALSE);

	BDKitFreePool(puszStoreFilePath);

	return nsStatus;
}

NTSTATUS BDKitAddFileDenyAccess( __in PUNICODE_STRING puszFileFullPath )
{
	if ( puszFileFullPath != NULL )
	{
		return AddDenyFileAccessByFilter (puszFileFullPath);
	}

	return STATUS_UNSUCCESSFUL;
}

NTSTATUS BDKitRemoveFileDenyAccess( __in PUNICODE_STRING puszFileFullPath )
{
	if ( puszFileFullPath != NULL && g_DenyFileHashList != NULL )
	{
		return RemoveDenyFileAccessByFilter (puszFileFullPath);
	}

	return STATUS_UNSUCCESSFUL;
}