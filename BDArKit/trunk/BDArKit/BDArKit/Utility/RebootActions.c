#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "BDArKit\KitHelper.h"
#include "Utility.h"
#include "BDKitUtils.h"
#include "BDKitHandler.h"
#include "RebootActions.h"


typedef struct
{
	DWORD		Signature;
	DWORD		ulSecNums;
	ULONG64		ulFirstOffset; /* 这里我们需要统一下64和32位下的文件格式, 方便应用层调试解析*/
} DataFileHeader, *PDataFileHeader;

typedef struct
{
	ULONG					Tag;
	REBOOT_ACTION_CALLBACK	pfnHandler;
	BOOLEAN					bAsyncWorking;
	PVOID					pData;
	ULONG					ulLength;
	PVOID					pContext;
	PIO_WORKITEM			pWorkItem;
} REGISTER_CALLBACK;

typedef struct
{
	ULONG	Tag;
	BYTE	pData[0];
} SEC_NODE, *PSEC_NODE;

static UNICODE_STRING strRebootActionsFile =
RTL_CONSTANT_STRING(L"\\SystemRoot\\System32\\drivers\\" BDArKit_DRIVER_NAMEW L".dat");
static ULONG				g_bRebootIsInitialize	= FALSE;
static PDataFileHeader		g_DataFilePoint			= NULL;
static ERESOURCE			g_FileLock				= {0x00};
static CONST ULONG			g_DataSignature			= 'BDSD';
static CONST ULONG			g_SecSignature			= 'BDSS';
static REGISTER_CALLBACK	g_RegisterInfo[]		=
{
	{ REBOOT_DELETE_FILE_TAG, &BDKitRebootDeleteFileHandler, FALSE, NULL, 0, NULL, NULL },
	{ REBOOT_RECOVER_FILE_TAG,&BDKitRebootFileRecoverHandler, FALSE, NULL, 0, NULL, NULL},
	{ 0xFFFFFFFF, NULL, FALSE, NULL, 0, NULL, NULL }
};

BOOLEAN detectReboot(VOID)
{
	return TRUE;
}

NTSTATUS FixDataOffset(
					   __in PDataFileHeader pDataHeader, 
					   __in ULONG			ulLength,
					   __in ULONG_PTR		ulBaseOffset,
					   __in ULONG_PTR		ulNewBaseOffset
					   )
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	PSectionHeader	pSecHeader	= NULL;
	ULONG			ulSecNums	= 0;

	do 
	{
		__try
		{
			BDKit_If_Not_Break_With_Reason(pDataHeader->Signature == g_DataSignature, STATUS_FILE_INVALID);

			if ( ulBaseOffset == 0 )
			{
				pSecHeader	= (PSectionHeader)(pDataHeader->ulFirstOffset + (ULONG_PTR)pDataHeader);

			} else if ( ulNewBaseOffset == 0 )
			{
				pSecHeader	= (PSectionHeader)(pDataHeader->ulFirstOffset);

			} else
			{
				pSecHeader	= (PSectionHeader)(pDataHeader->ulFirstOffset - ulBaseOffset + ulNewBaseOffset);
			}

			pDataHeader->ulFirstOffset	= (ULONG_PTR)pDataHeader->ulFirstOffset - ulBaseOffset + ulNewBaseOffset;
			ulSecNums = pDataHeader->ulSecNums;
			while(ulSecNums > 0
				&& pSecHeader != NULL
				&& pSecHeader > (PSectionHeader)(pDataHeader)
				&& pSecHeader < (PSectionHeader)((ULONG_PTR)pDataHeader + ulLength))
			{
				PSectionHeader	pNextSecHeader	= NULL;
				BDKit_If_Not_Break_With_Reason(pSecHeader->Signature == g_SecSignature, STATUS_FILE_INVALID);
				if ( ulBaseOffset == 0 )
				{
					pNextSecHeader	= (PSectionHeader)(pSecHeader->ulNextOffset + (ULONG_PTR)pDataHeader);

				} else if ( ulNewBaseOffset == 0 )
				{
					pNextSecHeader	= (PSectionHeader)(pSecHeader->ulNextOffset);

				} else
				{
					pNextSecHeader	= (PSectionHeader)(pSecHeader->ulNextOffset - ulBaseOffset + ulNewBaseOffset);
				}
				pSecHeader->ulNextOffset = pSecHeader->ulNextOffset - ulBaseOffset + ulNewBaseOffset;
				pSecHeader = pNextSecHeader;
				--ulSecNums;
			}
			BDKit_If_Not_Break_With_Reason(ulSecNums == 0, STATUS_FILE_INVALID);

			nsStatus = STATUS_SUCCESS;
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = STATUS_INVALID_FILE_FOR_SECTION;
		}

	} while (FALSE);

	return nsStatus;
}

NTSTATUS LoadAndValid(__out PDataFileHeader* ppData, __out PULONG pulLength)
{
	NTSTATUS					nsStatus	= STATUS_UNSUCCESSFUL;
	HANDLE						hDatFile	= NULL;
	PDataFileHeader				pData		= NULL;
	IO_STATUS_BLOCK				IoStatus	= {0x00};
	OBJECT_ATTRIBUTES			oa			= {0x00};
	FILE_STANDARD_INFORMATION	fileInfo	= {0x00};
	LARGE_INTEGER				llOrgSize	= {0x00};

	do 
	{
		InitializeObjectAttributes(&oa, &strRebootActionsFile, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
		nsStatus = ZwCreateFile (
			&hDatFile,
			FILE_ALL_ACCESS,
			&oa,
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

		nsStatus = ZwQueryInformationFile (
			hDatFile,
			&IoStatus,
			&fileInfo,
			sizeof(fileInfo),
			FileStandardInformation
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		llOrgSize.QuadPart = fileInfo.EndOfFile.QuadPart;
		BDKit_If_Not_Break_With_Reason(llOrgSize.HighPart == 0, STATUS_FILE_TOO_LARGE);

		BDKitAllocateNonpagePool(pData, llOrgSize.LowPart);
		BDKit_If_Not_Break_With_Reason(pData != NULL, STATUS_INSUFF_SERVER_RESOURCES);

		nsStatus = ZwReadFile (
			hDatFile,
			NULL,
			NULL,
			NULL,
			&IoStatus,
			pData,
			llOrgSize.LowPart,
			NULL,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && pData->Signature == g_DataSignature);

		// 这里提前删除, 防止修复Offset出错导致不停重启蓝屏
		BDKitCloseHandle(hDatFile);
		BDKitFocusDeleteFile(
			&strRebootActionsFile,
			sizeof(UNICODE_STRING) + strRebootActionsFile.MaximumLength,
			NULL,
			0,
			NULL,
			emNormalLevel
			);

		nsStatus = FixDataOffset (pData, llOrgSize.LowPart, 0, (ULONG_PTR)pData);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKitCloseHandle(hDatFile);
	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), BDKitFreePool(pData));
	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), llOrgSize.QuadPart	= 0);
	BDKitFocusDeleteFile(
		&strRebootActionsFile,
		sizeof(UNICODE_STRING) + strRebootActionsFile.MaximumLength,
		NULL,
		0,
		NULL,
		emNormalLevel
		);

	*ppData = pData;
	*pulLength = llOrgSize.LowPart;

	return nsStatus;
}

NTSTATUS StoreAndValid(__in PDataFileHeader pData, __in ULONG ulLength)
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	HANDLE				hDatFile	= NULL;
	OBJECT_ATTRIBUTES	oa			= {0x00};
	IO_STATUS_BLOCK		IoStatus	= {0x00};

	do 
	{
		nsStatus = FixDataOffset (pData, ulLength, (ULONG_PTR)pData, 0);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		InitializeObjectAttributes(&oa, &strRebootActionsFile, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
		nsStatus = ZwCreateFile (
			&hDatFile,
			FILE_ALL_ACCESS,
			&oa,
			&IoStatus,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ,
			FILE_OPEN_IF,
			FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			0
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ZwWriteFile (
			hDatFile,
			NULL,
			NULL,
			NULL,
			&IoStatus,
			pData,
			ulLength,
			NULL,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKitCloseHandle(hDatFile);

	return nsStatus;
}

NTSTATUS BDKitAppendDataToFile(ULONG Tag, __in PVOID pUsrData, __in ULONG ulUsrLength)
{
	NTSTATUS		nsStatus		= STATUS_UNSUCCESSFUL;
	PDataFileHeader	pDataHeader		= NULL;
	PDataFileHeader	pNewDataHeader	= NULL;
	PSectionHeader	pSecHeader		= NULL;
	PSEC_NODE		pNode			= NULL;
	ULONG			ulLength		= 0;
	ULONG			ulNewLength		= 0;
	BOOLEAN			bFoundNode		= FALSE;

	BDKit_If_Not_Return(g_bRebootIsInitialize == TRUE, STATUS_FLT_NOT_INITIALIZED);

	do 
	{
		FltAcquireResourceExclusive(&g_FileLock);

		BDKit_If_Not_Break_With_Reason(pUsrData != NULL && ulUsrLength > 0, STATUS_INVALID_PARAMETER);

		LoadAndValid (&pDataHeader, &ulLength);
		if ( pDataHeader == NULL )
		{
			ulNewLength = sizeof(DataFileHeader) + sizeof(SectionHeader);
		}
		else
		{
			// 查询是否已经存在
			bFoundNode	= FALSE;
			pSecHeader	= (PSectionHeader)pDataHeader->ulFirstOffset;
			while(	pSecHeader != NULL
				&&	pSecHeader > (PSectionHeader)(pDataHeader)
				&&	pSecHeader < (PSectionHeader)((PUCHAR)pDataHeader + ulLength)
				)
			{
				PSEC_NODE pNode = NULL;				
				//UNICODE_STRING	uszOrigData		= {0x00};
				//UNICODE_STRING	uszExistData	= {0x00};

				BDKit_If_Not_Break_With_Reason(pSecHeader->Signature == g_SecSignature, STATUS_FILE_INVALID);
				
				if (	pSecHeader->ulLength == (ulUsrLength + sizeof(SEC_NODE))
					&&	pSecHeader->Data != NULL)
				{
					pNode = (PSEC_NODE)pSecHeader->Data;
					if (	pNode->Tag == Tag
						&&	RtlCompareMemory (pNode->pData, pUsrData, ulUsrLength) == ulUsrLength)
					{
						bFoundNode = TRUE;
						break;
					}
				}

				pSecHeader = (PSectionHeader)pSecHeader->ulNextOffset;
			}

			ulNewLength = ulLength;
		}

		if ( bFoundNode == TRUE )
		{
			nsStatus = StoreAndValid (pDataHeader, ulLength);
			break;
		}

		ulNewLength += sizeof(SectionHeader);
		ulNewLength += sizeof(SEC_NODE);
		ulNewLength += ulUsrLength;
		BDKitAllocateNonpagePool(pNewDataHeader, ulNewLength);
		BDKit_If_Not_Break_With_Reason(pNewDataHeader != NULL, STATUS_INSUFF_SERVER_RESOURCES);

		if ( pDataHeader == NULL )
		{
			pNewDataHeader->Signature	= g_DataSignature;
			pNewDataHeader->ulFirstOffset	= (ULONG_PTR)pNewDataHeader + sizeof(DataFileHeader);
			pSecHeader					= (PSectionHeader)pNewDataHeader->ulFirstOffset;
			pSecHeader->Signature		= g_SecSignature;
			pSecHeader->ulNextOffset	= 0;
		}
		else
		{
			RtlCopyMemory(pNewDataHeader, pDataHeader, ulLength);
			nsStatus = FixDataOffset (pNewDataHeader, ulNewLength, (ULONG_PTR)pDataHeader, (ULONG_PTR)pNewDataHeader);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
		}

		bFoundNode	= FALSE;
		pSecHeader	= (PSectionHeader)pNewDataHeader->ulFirstOffset;
		while(	pSecHeader != NULL
			&&	pSecHeader > (PSectionHeader)(pNewDataHeader)
			&&	pSecHeader < (PSectionHeader)((PUCHAR)pNewDataHeader + ulNewLength))
		{
			BDKit_If_Not_Break_With_Reason(pSecHeader->Signature == g_SecSignature, STATUS_FILE_INVALID);
			if ( pSecHeader->ulNextOffset == 0 )
			{
				bFoundNode = TRUE;
				break;
			}
			pSecHeader = (PSectionHeader)pSecHeader->ulNextOffset;
		}
		BDKit_If_Not_Break_With_Reason(bFoundNode == TRUE, STATUS_FILE_INVALID);

		++pNewDataHeader->ulSecNums;
		pSecHeader->Signature = g_SecSignature;
		pSecHeader->ulLength  = ulUsrLength + sizeof(SEC_NODE);
		pNode = (PSEC_NODE)pSecHeader->Data;
		pNode->Tag = Tag;
		RtlCopyMemory(pNode->pData, pUsrData, ulUsrLength);
		pSecHeader->ulNextOffset= (ULONG_PTR)pSecHeader->Data + pSecHeader->ulLength;

		// Fill End of Section Header
		pSecHeader = (PSectionHeader)pSecHeader->ulNextOffset;
		pSecHeader->Signature	= g_SecSignature;
		pSecHeader->ulLength	= 0;
		pSecHeader->ulNextOffset= 0;

		nsStatus = StoreAndValid (pNewDataHeader, ulNewLength);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	FltReleaseResource (&g_FileLock);
	BDKitFreePool(pDataHeader);
	BDKitFreePool(pNewDataHeader);

	return nsStatus;
}

NTSTATUS BDKitRemoveDataFromFile(__in ULONG Tag, __in PVOID pUsrData, __in ULONG ulUsrLength)
{
	NTSTATUS		nsStatus		= STATUS_UNSUCCESSFUL;
	PDataFileHeader	pDataHeader		= NULL;
	PSectionHeader	pSecHeader		= NULL;
	PSectionHeader	pNewSecHeader	= NULL;
	PDataFileHeader	pNewDataHeader	= NULL;
	ULONG			ulLength		= 0;
	ULONG			ulNewLength		= 0;
	BOOLEAN			bFoundNode		= FALSE;

	BDKit_If_Not_Return(g_bRebootIsInitialize == TRUE, STATUS_FLT_NOT_INITIALIZED);

	do 
	{
		FltAcquireResourceExclusive(&g_FileLock);

		BDKit_If_Not_Break_With_Reason(pUsrData != NULL && ulUsrLength > 0, STATUS_INVALID_PARAMETER);

		LoadAndValid (&pDataHeader, &ulLength);
		BDKit_If_Not_Break_With_Reason(pDataHeader != NULL, STATUS_FILE_INVALID);

		ulNewLength = ulLength - sizeof(SectionHeader) - ulUsrLength - sizeof(SEC_NODE);
		BDKit_If_Not_Break_With_Reason(ulUsrLength < (ulLength - sizeof(SectionHeader) - sizeof(SEC_NODE)), STATUS_INVALID_PARAMETER);

		BDKitAllocateNonpagePool(pNewDataHeader, ulNewLength);
		BDKit_If_Not_Break_With_Reason(pNewDataHeader != NULL, STATUS_INSUFF_SERVER_RESOURCES);

		pNewDataHeader->Signature	= g_DataSignature;
		pNewDataHeader->ulSecNums	= pDataHeader->ulSecNums - 1;
		pNewDataHeader->ulFirstOffset = ((ULONG_PTR)pNewDataHeader + sizeof(DataFileHeader));

		pSecHeader		= (PSectionHeader)pDataHeader->ulFirstOffset;
		pNewSecHeader	= (PSectionHeader)pNewDataHeader->ulFirstOffset;
		while(	pSecHeader > (PSectionHeader)(pDataHeader)
			&&	pSecHeader < (PSectionHeader)((PUCHAR)pDataHeader + ulLength)
			&&	pNewSecHeader > (PSectionHeader)(pNewDataHeader)
			&&	pNewSecHeader < (PSectionHeader)((PUCHAR)pNewDataHeader + ulNewLength)
			)
		{
			PSEC_NODE	pNode = NULL;
			
			BDKit_If_Not_Break_With_Reason(pSecHeader->Signature == g_SecSignature, STATUS_FILE_INVALID);

			pNode = (PSEC_NODE)pSecHeader->Data;
			if (	pSecHeader->ulLength == ulUsrLength + sizeof(SEC_NODE)
				&&	ulUsrLength != 0
				&& pNode->Tag == Tag
				&&	RtlCompareMemory (pNode->pData, pUsrData, ulUsrLength) == ulUsrLength )
			{
				bFoundNode = TRUE;
			}
			else
			{
				pNewSecHeader->Signature	= g_SecSignature;
				pNewSecHeader->ulLength		= pSecHeader->ulLength;
				if (	pSecHeader->ulLength != 0 
					&& ((ULONG_PTR)pNewSecHeader->Data + pNewSecHeader->ulLength) <= ((ULONG_PTR)pNewDataHeader + ulNewLength))
				{
					RtlCopyMemory(pNewSecHeader->Data, pSecHeader->Data, pSecHeader->ulLength);
				}
	
				pNewSecHeader->ulNextOffset	= 
					pSecHeader->ulNextOffset == 0? 
					0 : ((ULONG_PTR)pNewSecHeader + sizeof(SectionHeader) + pNewSecHeader->ulLength);
				pNewSecHeader = (PSectionHeader)pNewSecHeader->ulNextOffset;
			}
			pSecHeader = (PSectionHeader)pSecHeader->ulNextOffset;
		}

	} while (FALSE);

	if (	bFoundNode == TRUE 
		&&	pNewDataHeader != NULL
		&&	pNewDataHeader->ulSecNums > 0 )
	{
		nsStatus = StoreAndValid (pNewDataHeader, ulNewLength);
	}
	else if ( bFoundNode == FALSE && pDataHeader != NULL )
	{
		nsStatus = StoreAndValid (pDataHeader, ulLength);
	}

	FltReleaseResource (&g_FileLock);
	BDKitFreePool(pDataHeader);
	BDKitFreePool(pNewDataHeader);

	return nsStatus;
}

VOID AsyncExecWorkItem(__in PDEVICE_OBJECT DeviceObject, __in PVOID pContext)
{
	ULONG	ulIndex	= (ULONG)pContext;
	UNREFERENCED_PARAMETER(DeviceObject);
	g_RegisterInfo[ulIndex].pfnHandler(
		g_RegisterInfo[ulIndex].pData, 
		g_RegisterInfo[ulIndex].ulLength, 
		g_RegisterInfo[ulIndex].pContext);
	if ( g_RegisterInfo[ulIndex].pWorkItem != NULL )
	{
		IoFreeWorkItem (g_RegisterInfo[ulIndex].pWorkItem);
		g_RegisterInfo[ulIndex].pWorkItem = NULL;
	}
}

NTSTATUS BDKitExecuteRebootHandler(__in PDEVICE_OBJECT DeviceObject)
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	ULONG			ulIndex		= 0;
	ULONG			ulDataLength= 0;
	PDataFileHeader	pDataHeader	= NULL;
	PIO_WORKITEM	pWorkItem	= NULL;

	do 
	{
		nsStatus = ExInitializeResourceLite(&g_FileLock);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		g_bRebootIsInitialize = TRUE;
		BDKit_If_Not_Break_With_Reason(detectReboot () == TRUE, STATUS_INVALID_SERVER_STATE);

		nsStatus = LoadAndValid (&pDataHeader, &ulDataLength);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		g_DataFilePoint = pDataHeader;
		for (ulIndex = 0; ulIndex < (sizeof(g_RegisterInfo) / sizeof(g_RegisterInfo[0])); ++ulIndex)
		{
			PSectionHeader	pSecHeader	= (PSectionHeader)pDataHeader->ulFirstOffset;
			BDKit_If_DoAction(g_RegisterInfo[ulIndex].pfnHandler == NULL && pSecHeader != NULL, continue);

			while(pSecHeader != NULL && pSecHeader->ulLength != 0)
			{
				PSEC_NODE pNode = (PSEC_NODE)pSecHeader->Data;
				if (pNode->Tag == g_RegisterInfo[ulIndex].Tag)
				{
					if ( g_RegisterInfo[ulIndex].bAsyncWorking == TRUE )
					{
						pWorkItem = IoAllocateWorkItem (DeviceObject);
						BDKit_If_Not_Break_With_Reason(pWorkItem != NULL, STATUS_INSUFF_SERVER_RESOURCES);
						g_RegisterInfo[ulIndex].pData		= pNode->pData;
						g_RegisterInfo[ulIndex].ulLength	= pSecHeader->ulLength - sizeof(SEC_NODE);
						g_RegisterInfo[ulIndex].pWorkItem	= pWorkItem;
						IoQueueWorkItem (pWorkItem, AsyncExecWorkItem, DelayedWorkQueue, (PVOID)ulIndex);
					}
					else
					{
						g_RegisterInfo[ulIndex].pfnHandler(
							pNode->pData, 
							pSecHeader->ulLength - sizeof(SEC_NODE), 
							g_RegisterInfo[ulIndex].pContext
							);
					}
				}

				pSecHeader = (PSectionHeader)pSecHeader->ulNextOffset;
			}
		}
	} while (FALSE);

	return nsStatus;
}

NTSTATUS BDKitUnloadRebootHandler(VOID)
{
	NTSTATUS nsStatus = STATUS_SUCCESS;

	if ( g_bRebootIsInitialize == TRUE )
	{
		// 这里需要确保下所有的工作线程都已经退出, 由于没用到工作线程, 暂时先这么写, 以后再实现
		g_bRebootIsInitialize = FALSE;
		nsStatus = ExDeleteResourceLite(&g_FileLock);

		BDKitFreePool(g_DataFilePoint);
	}

	return STATUS_SUCCESS;
}