#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "BDArKit\KitHelper.h"
#include "Utility.h"
#include "RebootActions.h"
#include "BDKitUtils.h"
#include "BDKitHandler.h"


typedef struct
{
	ULONG	ulSrcLength;
	ULONG	ulDstLength;
	BYTE	pData[0];
} FILE_RECOVER_NODE, *PFILE_RECOVER_NODE;

NTSTATUS BDKitAddRebootDeleteFile( __in PUNICODE_STRING puszFileFullPath )
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	PVOID		pUsrData	= NULL;
	ULONG		ulUsrLength	= 0;
	BOOLEAN		bNeedFree	= FALSE;

	do 
	{
		BDKit_If_Not_Break_With_Reason(puszFileFullPath != NULL, STATUS_INVALID_PARAMETER);

		if ( puszFileFullPath->Buffer[puszFileFullPath->Length / 2 - 1] == L'\0' )
		{
			pUsrData = puszFileFullPath->Buffer;
			ulUsrLength = puszFileFullPath->Length;

		} else if ( puszFileFullPath->Length + sizeof(WCHAR) <= puszFileFullPath->MaximumLength )
		{
			puszFileFullPath->Buffer[puszFileFullPath->Length / 2] = L'\0';
			pUsrData = puszFileFullPath->Buffer;
			ulUsrLength = puszFileFullPath->Length + sizeof(WCHAR);

		} else
		{
			BDKitAllocateNonpagePool(pUsrData, puszFileFullPath->Length + sizeof(WCHAR));
			BDKit_If_Not_Break_With_Reason(pUsrData != NULL, STATUS_INSUFF_SERVER_RESOURCES);
			RtlCopyMemory(pUsrData, puszFileFullPath->Buffer, puszFileFullPath->Length);
			ulUsrLength = puszFileFullPath->Length + sizeof(WCHAR);
			bNeedFree = TRUE;
		}

		nsStatus = BDKitAppendDataToFile (REBOOT_DELETE_FILE_TAG, pUsrData, ulUsrLength);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKit_If_DoAction(bNeedFree == TRUE, BDKitFreePool(pUsrData));

	return nsStatus;
}

NTSTATUS BDKitRemoveRebootDeleteFile( __in PUNICODE_STRING puszFileFullPath )
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	PVOID		pUsrData	= NULL;
	ULONG		ulUsrLength	= 0;
	BOOLEAN		bNeedFree	= FALSE;

	do 
	{
		BDKit_If_Not_Break_With_Reason(puszFileFullPath != NULL, STATUS_INVALID_PARAMETER);

		if ( puszFileFullPath->Buffer[puszFileFullPath->Length / 2 - 1] == L'\0' )
		{
			pUsrData = puszFileFullPath->Buffer;
			ulUsrLength = puszFileFullPath->Length;

		} else if ( puszFileFullPath->Length + sizeof(WCHAR) <= puszFileFullPath->MaximumLength )
		{
			puszFileFullPath->Buffer[puszFileFullPath->Length / 2] = L'\0';
			pUsrData = puszFileFullPath->Buffer;
			ulUsrLength = puszFileFullPath->Length + sizeof(WCHAR);

		} else
		{
			BDKitAllocateNonpagePool(pUsrData, puszFileFullPath->Length + sizeof(WCHAR));
			BDKit_If_Not_Break_With_Reason(pUsrData != NULL, STATUS_INSUFF_SERVER_RESOURCES);
			RtlCopyMemory(pUsrData, puszFileFullPath->Buffer, puszFileFullPath->Length);
			ulUsrLength = puszFileFullPath->Length + sizeof(WCHAR);
			bNeedFree = TRUE;
		}

		nsStatus = BDKitRemoveDataFromFile (REBOOT_DELETE_FILE_TAG, pUsrData, ulUsrLength);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKit_If_DoAction(bNeedFree == TRUE, BDKitFreePool(pUsrData));

	return nsStatus;
}

VOID BDKitRebootDeleteFileHandler( __in PVOID pData, __in ULONG ulLength, __in PVOID pContext )
{
	UNICODE_STRING	uszFileFullPath	= {0x00};
	PCWSTR	pFilePath	= (PCWSTR)pData;

	UNREFERENCED_PARAMETER(pContext);

	do 
	{
		RtlInitUnicodeString (&uszFileFullPath, pFilePath);
		BDKit_If_Not_Break(uszFileFullPath.Length <= ulLength);

		BDKitFocusDeleteFile(
			&uszFileFullPath,
			sizeof(UNICODE_STRING) + uszFileFullPath.MaximumLength,
			NULL,
			0,
			NULL,
			emNormalLevel
			);
	} while (FALSE);
}

// 这里暂时先不考虑非系统分区
NTSTATUS BDKitAddRecoverFile( __in PUNICODE_STRING puszSrcFilePath, __in PUNICODE_STRING puszDstFilePath )
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	PVOID		pUsrData	= NULL;
	PVOID		pMover		= NULL;
	ULONG		ulUsrLength	= 0;
	ULONG		ulSrcLength	= 0;
	ULONG		ulDstLength	= 0;
	BOOLEAN		bNeedFree	= FALSE;
	PFILE_RECOVER_NODE	pNode	= NULL;

	do 
	{
		BDKit_If_Not_Break_With_Reason(puszSrcFilePath != NULL && puszDstFilePath != NULL, STATUS_INVALID_PARAMETER);

		ulSrcLength = puszSrcFilePath->Buffer[puszSrcFilePath->Length / sizeof(WCHAR) - 1] == L'\0'? 
			puszSrcFilePath->Length - sizeof(WCHAR) : puszSrcFilePath->Length;
		ulDstLength = puszDstFilePath->Buffer[puszDstFilePath->Length / sizeof(WCHAR) - 1] == L'\0'?
			puszDstFilePath->Length - sizeof(WCHAR) : puszDstFilePath->Length;
		ulUsrLength = ulSrcLength + ulDstLength + sizeof(WCHAR) * 2 + sizeof(FILE_RECOVER_NODE);

		BDKitAllocateNonpagePool(pUsrData, ulUsrLength);
		BDKit_If_Not_Break_With_Reason(pUsrData != NULL, STATUS_INSUFF_SERVER_RESOURCES);
		
		pNode = pUsrData;
		pNode->ulSrcLength = ulSrcLength + sizeof(WCHAR);
		pNode->ulDstLength = ulDstLength + sizeof(WCHAR);

		pMover = pNode->pData;
		RtlCopyMemory(pMover, puszSrcFilePath->Buffer, ulSrcLength);
		pMover = (PVOID)((ULONG_PTR)pMover + pNode->ulSrcLength);
		RtlCopyMemory(pMover, puszDstFilePath->Buffer, ulDstLength);

		nsStatus = BDKitAppendDataToFile (REBOOT_RECOVER_FILE_TAG, pUsrData, ulUsrLength);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKit_If_DoAction(bNeedFree == TRUE, BDKitFreePool(pUsrData));

	return nsStatus;
}

NTSTATUS BDKitRemoveRecoverFile( __in PUNICODE_STRING puszSrcFilePath, __in PUNICODE_STRING puszDstFilePath )
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	PVOID		pUsrData	= NULL;
	PVOID		pMover		= NULL;
	ULONG		ulUsrLength	= 0;
	ULONG		ulSrcLength	= 0;
	ULONG		ulDstLength	= 0;
	BOOLEAN		bNeedFree	= FALSE;
	PFILE_RECOVER_NODE	pNode	= NULL;

	do 
	{
		BDKit_If_Not_Break_With_Reason(puszSrcFilePath != NULL && puszDstFilePath != NULL, STATUS_INVALID_PARAMETER);

		ulSrcLength = puszSrcFilePath->Buffer[puszSrcFilePath->Length / sizeof(WCHAR) - 1] == L'\0'? 
			puszSrcFilePath->Length - sizeof(WCHAR) : puszSrcFilePath->Length;
		ulDstLength = puszDstFilePath->Buffer[puszDstFilePath->Length / sizeof(WCHAR) - 1] == L'\0'?
			puszDstFilePath->Length - sizeof(WCHAR) : puszDstFilePath->Length;
		ulUsrLength = ulSrcLength + ulDstLength + sizeof(WCHAR) * 2 + sizeof(FILE_RECOVER_NODE);

		BDKitAllocateNonpagePool(pUsrData, ulUsrLength);
		BDKit_If_Not_Break_With_Reason(pUsrData != NULL, STATUS_INSUFF_SERVER_RESOURCES);

		pNode = pUsrData;
		pNode->ulSrcLength = ulSrcLength + sizeof(WCHAR);
		pNode->ulDstLength = ulDstLength + sizeof(WCHAR);

		pMover = pNode->pData;
		RtlCopyMemory(pMover, puszSrcFilePath->Buffer, ulSrcLength);
		pMover = (PVOID)((ULONG_PTR)pMover + pNode->ulSrcLength);
		RtlCopyMemory(pMover, puszDstFilePath->Buffer, ulDstLength);

		nsStatus = BDKitRemoveDataFromFile (REBOOT_RECOVER_FILE_TAG, pUsrData, ulUsrLength);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKit_If_DoAction(bNeedFree == TRUE, BDKitFreePool(pUsrData));

	return nsStatus;
}

VOID BDKitRebootFileRecoverHandler( __in PVOID pData, __in ULONG ulLength, __in PVOID pContext )
{
	UNICODE_STRING	uszSrcFilePath	= {0x00};
	UNICODE_STRING	uszDstFilePath	= {0x00};
	PFILE_RECOVER_NODE	pNode	= (PFILE_RECOVER_NODE)pData;
	COPY_FILE_EX_PROXY	cpyFile	= {0x00};

	UNREFERENCED_PARAMETER(pContext);

	do 
	{
		BDKit_If_Not_Break(pData != NULL && ulLength != 0);
		BDKit_If_Not_Break((pNode->ulSrcLength + pNode->ulDstLength + sizeof(FILE_RECOVER_NODE)) == ulLength);
		RtlInitUnicodeString (&uszSrcFilePath, (PCWSTR)pNode->pData);
		RtlInitUnicodeString (&uszDstFilePath, (PCWSTR)(pNode->pData + pNode->ulSrcLength));

		cpyFile.pbCancel	= NULL;
		cpyFile.dwCopyFlags = 0;
		cpyFile.lpData		= NULL;
		cpyFile.pNewFileName= &uszDstFilePath;
		cpyFile.pExistingFileName	= &uszSrcFilePath;
		cpyFile.lpProgressRoutine	= NULL;

		BDKitFocusCopyFile(
			&cpyFile,
			sizeof(cpyFile),
			NULL,
			0,
			NULL,
			emNormalLevel);
	} while (FALSE);
}