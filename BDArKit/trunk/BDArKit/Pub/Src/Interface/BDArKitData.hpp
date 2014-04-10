#pragma once


#include <Windows.h>
#include <string>
#include "Utils\File.hpp"

typedef std::wstring tstring;
typedef struct
{
	DWORD		Signature;
	DWORD		ulSecNums;
	ULONG64		ulFirstOffset;
} DataFileHeader, *PDataFileHeader;

#pragma warning(push)
#pragma warning(disable:4200)
typedef struct
{
	ULONG	Tag;
	BYTE	pData[0];
} SEC_NODE, *PSEC_NODE;

typedef struct
{
	DWORD		Signature;
	ULONG64		ulNextOffset;
	ULONG		ulLength;
	UCHAR		Data[0];
} SectionHeader, *PSectionHeader;
#pragma warning(pop)

//tstring strRebootActionsFile = tstring(_T("\\SystemRoot\\System32\\drivers\\BDArKit.dat"));
static PDataFileHeader		g_DataFilePoint			= NULL;
static CONST ULONG			g_DataSignature			= 'BDSD';
static CONST ULONG			g_SecSignature			= 'BDSS';

#define BDKit_If_DoAction(condition, action)	\
{												\
	if ( (condition) ) { action; }				\
}
#define BDKit_If_Not_Break_With_Reason(condition, reason)	\
{															\
	if ( !(condition) ) { nsStatus = reason; break; }		\
}
#define BDKit_If_Not_Break(condition)			\
{												\
	BDKit_If_DoAction( !(condition), break)		\
}
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
//typedef ULONG	NTSTATUS;

#define STATUS_SUCCESS                   ((NTSTATUS)0x00000000L)
#define STATUS_UNSUCCESSFUL              ((NTSTATUS)0xC0000001L)
#define STATUS_FILE_INVALID              ((NTSTATUS)0xC0000098L)
#define STATUS_INVALID_FILE_FOR_SECTION  ((NTSTATUS)0xC0000020L)
#define STATUS_INVALID_PARAMETER         ((NTSTATUS)0xC000000DL)
#define STATUS_INSUFF_SERVER_RESOURCES   ((NTSTATUS)0xC0000205L)

#define RtlCompareMemory memcmp

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

NTSTATUS LoadAndValid(__in tstring strFilePath, __out PDataFileHeader* ppData, __out PULONG pulLength, BOOL bDelete = TRUE)
{
	NTSTATUS					nsStatus	= STATUS_UNSUCCESSFUL;
	HANDLE						hDatFile	= NULL;
	PDataFileHeader				pData		= NULL;
	LARGE_INTEGER				llOrgSize	= {0x00};

	do 
	{
#ifdef _DDK_BUILD
		IO_STATUS_BLOCK				IoStatus	= {0x00};
		OBJECT_ATTRIBUTES			oa			= {0x00};
		FILE_STANDARD_INFORMATION	fileInfo	= {0x00};
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
#else
		hDatFile = ::CreateFile(
			strFilePath.c_str (),
			GENERIC_ALL,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		BDKit_If_Not_Break(hDatFile != INVALID_HANDLE_VALUE);
#endif

#ifdef _DDK_BUILD
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
#else
		DWORD dwFileSizeHigh = 0;
		llOrgSize.QuadPart = GetFileSize(hDatFile, &dwFileSizeHigh);
		pData = (PDataFileHeader)new BYTE[llOrgSize.LowPart];
		BDKit_If_Not_Break(pData != NULL);
		ZeroMemory(pData, llOrgSize.LowPart);
#endif

#ifdef _DDK_BUILD
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
#else
		DWORD dwBytesToRead = 0;
		ReadFile (hDatFile, pData, llOrgSize.LowPart, &dwBytesToRead, NULL);
#endif
		nsStatus = FixDataOffset (pData, llOrgSize.LowPart, 0, (ULONG_PTR)pData);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

#ifdef _DDK_BUILD
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
#else
	CloseHandle (hDatFile);
	if (nsStatus != STATUS_SUCCESS && pData != NULL) 
	{
		delete[] pData;
		pData = NULL;
	}
	if (bDelete) {
		DeleteFile(strFilePath.c_str ());
	}
#endif

	*ppData = pData;
	*pulLength = llOrgSize.LowPart;

	return nsStatus;
}

NTSTATUS StoreAndValid(__in tstring strFilePath, __in PDataFileHeader pData, __in ULONG ulLength)
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	HANDLE				hDatFile	= NULL;
#ifdef _DDK_BUILD
	OBJECT_ATTRIBUTES	oa			= {0x00};
	IO_STATUS_BLOCK		IoStatus	= {0x00};
#endif

	do 
	{
		nsStatus = FixDataOffset (pData, ulLength, (ULONG_PTR)pData, 0);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

#ifdef _DDK_BUILD
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
#else
		hDatFile = CreateFile(
			strFilePath.c_str (),
			GENERIC_ALL,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		BDKit_If_Not_Break(hDatFile != INVALID_HANDLE_VALUE);

		DWORD dwBytesToRead = 0;
		BOOL bResult = WriteFile (hDatFile, pData, ulLength, &dwBytesToRead, NULL);
		BDKit_If_Not_Break(bResult == TRUE);
#endif

	} while (FALSE);

#ifdef _DDK_BUILD
	BDKitCloseHandle(hDatFile);
#else
	CloseHandle (hDatFile);
#endif

	return nsStatus;
}

NTSTATUS BDKitAppendDataToFile(tstring strFilePath, ULONG Tag, __in PVOID pUsrData, __in ULONG ulUsrLength)
{
	NTSTATUS		nsStatus		= STATUS_UNSUCCESSFUL;
	PDataFileHeader	pDataHeader		= NULL;
	PDataFileHeader	pNewDataHeader	= NULL;
	PSectionHeader	pSecHeader		= NULL;
	PSEC_NODE		pNode			= NULL;
	ULONG			ulLength		= 0;
	ULONG			ulNewLength		= 0;
	BOOLEAN			bFoundNode		= FALSE;

#ifdef _DDK_BUILD
	BDKit_If_Not_Return(g_bRebootIsInitialize == TRUE, STATUS_FLT_NOT_INITIALIZED);
#endif

	do 
	{
#ifdef _DDK_BUILD
		FltAcquireResourceExclusive(&g_FileLock);
#endif

		BDKit_If_Not_Break_With_Reason(pUsrData != NULL && ulUsrLength > 0, STATUS_INVALID_PARAMETER);

		LoadAndValid (strFilePath, &pDataHeader, &ulLength);
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
						&&	RtlCompareMemory (pNode->pData, pUsrData, ulUsrLength) == 0)
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
			nsStatus = StoreAndValid (strFilePath, pDataHeader, ulLength);
			break;
		}

		ulNewLength += sizeof(SectionHeader);
		ulNewLength += sizeof(SEC_NODE);
		ulNewLength += ulUsrLength;
#ifdef _DDK_BUILD
		BDKitAllocateNonpagePool(pNewDataHeader, ulNewLength);
#else
		pNewDataHeader = (PDataFileHeader)new BYTE[ulNewLength];
		memset(pNewDataHeader, 0, ulNewLength);
#endif
		
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

		nsStatus = StoreAndValid (strFilePath, pNewDataHeader, ulNewLength);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

#ifdef _DDK_BUILD
	FltReleaseResource (&g_FileLock);
	BDKitFreePool(pDataHeader);
	BDKitFreePool(pNewDataHeader);
#else
	delete[] pDataHeader;
	delete[] pNewDataHeader;
#endif


	return nsStatus;
}


NTSTATUS BDKitRemoveDataFromFile(__in tstring strFilePath, __in ULONG Tag, __in PVOID pUsrData, __in ULONG ulUsrLength)
{
	NTSTATUS		nsStatus		= STATUS_UNSUCCESSFUL;
	PDataFileHeader	pDataHeader		= NULL;
	PSectionHeader	pSecHeader		= NULL;
	PSectionHeader	pNewSecHeader	= NULL;
	PDataFileHeader	pNewDataHeader	= NULL;
	ULONG			ulLength		= 0;
	ULONG			ulNewLength		= 0;
	BOOLEAN			bFoundNode		= FALSE;

#ifdef _DDK_BUILD
	BDKit_If_Not_Return(g_bRebootIsInitialize == TRUE, STATUS_FLT_NOT_INITIALIZED);
#endif

	do 
	{
#ifdef _DDK_BUILD
		FltAcquireResourceExclusive(&g_FileLock);
#endif

		BDKit_If_Not_Break_With_Reason(pUsrData != NULL && ulUsrLength > 0, STATUS_INVALID_PARAMETER);

		LoadAndValid (strFilePath, &pDataHeader, &ulLength);
		BDKit_If_Not_Break_With_Reason(pDataHeader != NULL, STATUS_FILE_INVALID);

		ulNewLength = ulLength - sizeof(SectionHeader) - ulUsrLength - sizeof(SEC_NODE);
		BDKit_If_Not_Break_With_Reason(ulUsrLength < (ulLength - sizeof(SectionHeader) - sizeof(SEC_NODE)), STATUS_INVALID_PARAMETER);

#ifdef _DDK_BUILD
		BDKitAllocateNonpagePool(pNewDataHeader, ulNewLength);
#else
		pNewDataHeader = (PDataFileHeader)new BYTE[ulNewLength];
		memset (pNewDataHeader, 0, ulNewLength);
#endif
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
				&&	RtlCompareMemory (pNode->pData, pUsrData, ulUsrLength) == 0 )
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
		nsStatus = StoreAndValid (strFilePath, pNewDataHeader, ulNewLength);
	}
	else if ( bFoundNode == FALSE && pDataHeader != NULL )
	{
		nsStatus = StoreAndValid (strFilePath, pDataHeader, ulLength);
	}

#ifdef _DDK_BUILD
	FltReleaseResource (&g_FileLock);
	BDKitFreePool(pDataHeader);
	BDKitFreePool(pNewDataHeader);
#else
	if (pDataHeader != NULL) {
		delete[] pDataHeader;
	}
	if (pNewDataHeader != NULL) {
		delete[] pNewDataHeader;
	}
#endif

	return nsStatus;
}

VOID ParseBDArKitRebootFile(tstring strFilePath)
{
	PDataFileHeader pDataHeader = NULL;
	ULONG			ulBytesToRead = 0;

	FileTools::CWow64RedirDisable autoRedirDisable(TRUE);
	
	if (LoadAndValid (strFilePath, &pDataHeader, &ulBytesToRead, FALSE) == STATUS_SUCCESS) {
		PSectionHeader pSecHeader = NULL;
		pSecHeader = (PSectionHeader) pDataHeader->ulFirstOffset;
		while(pSecHeader->ulNextOffset != 0)
		{
			UCHAR* result = new UCHAR[pSecHeader->ulLength+2];
			ZeroMemory(result,pSecHeader->ulLength+2);
			memcpy(result,(void*)pSecHeader->Data,pSecHeader->ulLength);
			_tprintf(_T("[%u]%s\n"), ((PSEC_NODE)result)->Tag, ((PSEC_NODE)result)->pData);
			delete[] result;
			pSecHeader = (PSectionHeader)pSecHeader->ulNextOffset;
		}
	}
	else
	{
		_tprintf(_T("File %s is Invalid, May be missing! Check of reason is %u\r\n"), strFilePath.c_str(), GetLastError());
	}
}