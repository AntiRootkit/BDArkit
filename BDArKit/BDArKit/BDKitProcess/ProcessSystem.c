#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "BDArKit\KitHelper.h"
#include "DriverHelper.h"
#include "BDKitUtils.h"
#include "Utility.h"
//#include "BDKitProcess.h"

#define PROCESS_TERMINATE                  (0x0001)  
#define PROCESS_CREATE_THREAD              (0x0002)  
#define PROCESS_SET_SESSIONID              (0x0004)  
#define PROCESS_VM_OPERATION               (0x0008)  
#define PROCESS_VM_READ                    (0x0010)  
#define PROCESS_VM_WRITE                   (0x0020)  
#define PROCESS_DUP_HANDLE                 (0x0040)  
#define PROCESS_CREATE_PROCESS             (0x0080)  
#define PROCESS_SET_QUOTA                  (0x0100)  
#define PROCESS_SET_INFORMATION            (0x0200)  
#define PROCESS_QUERY_INFORMATION          (0x0400)  
#define PROCESS_SUSPEND_RESUME             (0x0800)  
#define PROCESS_QUERY_LIMITED_INFORMATION  (0x1000)

typedef struct _IMAGE_NAME_INFO
{
	UNICODE_STRING Name;
	CHAR Buffer[1];
}IMAGE_NAME_INFO,*PIMAGE_NAME_INFO;

//static PROCESS_CONTEXT	g_processContext;


NTSTATUS BDKitInitProcessContext(VOID)
{
	return STATUS_NOT_SUPPORTED;
}

NTSTATUS BDKitGetProcessImageNameByEProcess_s( 
	__in PEPROCESS EProcess, 
	__out LPWSTR lpszImageName, 
	__in ULONG Length 
	)
{
	NTSTATUS			nsStatus		= STATUS_INVALID_PARAMETER;
	ULONG				ReturnLength	= 0;
	HANDLE				hProcessHandle	= NULL;
	PIMAGE_NAME_INFO	pImageNameInfo	= NULL;
	PEPROCESS			pCurEProcess	= NULL;

	do 
	{
		BDKit_If_Not_Break_With_Reason(EProcess != NULL && lpszImageName != NULL && Length != 0,
			STATUS_INVALID_PARAMETER);

		pCurEProcess = EProcess;
		ObReferenceObject(pCurEProcess);

		nsStatus = ObOpenObjectByPointer(
			EProcess,
			OBJ_KERNEL_HANDLE,
			NULL,
			PROCESS_QUERY_INFORMATION,
			*PsProcessType,
			KernelMode,
			&hProcessHandle
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = BDKitGetProcessInfo(hProcessHandle, ProcessImageFileName, &pImageNameInfo, &ReturnLength);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		BDKit_If_Not_Break_With_Reason(pImageNameInfo->Name.Length < Length, STATUS_INFO_LENGTH_MISMATCH);

		RtlCopyMemory(lpszImageName,pImageNameInfo->Name.Buffer,pImageNameInfo->Name.Length);

		nsStatus = STATUS_SUCCESS;

	} while (FALSE);

	BDKitFreePool(pImageNameInfo);
	BDKitCloseHandle(hProcessHandle);
	BDKitCloseObject(pCurEProcess);

	return nsStatus;	
}