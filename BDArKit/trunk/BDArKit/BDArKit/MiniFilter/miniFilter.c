#pragma once


#include "StdAfx.h"
#include <fltkernel.h>
#include "BDArKit\KitHelper.h"
#include "DriverHelper.h"
#include "Macro.h"
#include "List.h"
#include "miniFilter.h"


FLT_PREOP_CALLBACK_STATUS OnPreCallbackRoutine(
	__in PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in PVOID* CompletionContext
	);
FLT_CONTEXT_REGISTRATION g_FilterContexts[] = { {FLT_CONTEXT_END} };
FLT_OPERATION_REGISTRATION g_FilterOperations[] = {
	{ IRP_MJ_CREATE,			0, OnPreCallbackRoutine, NULL, NULL },
	{ IRP_MJ_SET_INFORMATION,	0, OnPreCallbackRoutine, NULL, NULL },
	{ IRP_MJ_OPERATION_END }
};
FLT_REGISTRATION g_FilterRegistration = {
	sizeof(FLT_REGISTRATION),                       // Size
	FLT_REGISTRATION_VERSION,                       // Version
#if DBG
	0,                                              // Flags
#else
	FLTFL_REGISTRATION_DO_NOT_SUPPORT_SERVICE_STOP, // Flags
#endif
	g_FilterContexts,                               // ContextRegistration
	g_FilterOperations,                             // OperationRegistration
	NULL,                                           // FilterUnloadCallback
	NULL,                                           // InstanceSetupCallback
	NULL,                                           // InstanceQueryTeardownCallback
	NULL,                                           // InstanceTeardownStartCallback
	NULL,                                           // InstanceTeardownCompleteCallback
	NULL,                                           // GenerateFileNameCallback
	NULL,                                           // NormalizeNameComponentCallback
#if FLT_MGR_LONGHORN
	NULL,                                           // NormalizeContextCleanupCallback
	NULL,                                           // TransactionNotificationCallback
	NULL                                            // NormalizeNameComponentExCallback
#else
	NULL
#endif
};

typedef struct _MiniFilterCallbackEntry_
{
	PFLT_PRE_OPERATION_CALLBACK pCallbackFunc;
} MiniFilterCallbackEntry, *PMiniFilterCallbackEntry;

typedef struct
{
	PWCHAR	pRegistryPath;
	PWCHAR	pValueName;
	PVOID	pValue;
	ULONG	ulRegType;
} REGISTRY_INFO, *PREGISTRY_INFO;

static volatile ULONG	g_bFilterIsInitialize	= FALSE;
static volatile HANDLE	g_CallBackList			= NULL;
static ERESOURCE		g_FilterLock			= {0x00};
static ULONG			g_ulFlags				= 0x00000000;
static PFLT_FILTER		g_miniFilter			= NULL;
static CONST REGISTRY_INFO	g_RegInfo[]			=
{
	{ NULL,									L"Group",				L"FSFilter Anti-Virus",		REG_SZ		},
	{ L"Instances",							L"DefaultInstance",		BDArKit_INSTANCE_NAMEW,		REG_SZ		},
	{ L"Instances\\"BDArKit_INSTANCE_NAMEW, L"Altitude",			BDArKit_Filter_ALTITUDE,	REG_SZ		},
	{ L"Instances\\"BDArKit_INSTANCE_NAMEW, L"Flags",				&g_ulFlags,					REG_DWORD	}
};

NTSTATUS installMinifilter(__in PUNICODE_STRING puszRegistry)
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	ULONG			ulFlags		= 0;
	ULONG			ulIndex		= 0;
	HANDLE			hDriverReg	= NULL;
	HANDLE			hRegKey		= NULL;
	UNICODE_STRING	uszRegKey	= {0x00};
	OBJECT_ATTRIBUTES	oa		= {0x00};
	RTL_QUERY_REGISTRY_TABLE QueryTable[] = 
	{
		{NULL, RTL_QUERY_REGISTRY_REQUIRED | RTL_QUERY_REGISTRY_DIRECT, L"Flags", &ulFlags, REG_DWORD, NULL, sizeof(ulFlags)},
		{NULL, 0, NULL, NULL, 0, NULL, 0}
	};

	do 
	{
		__try
		{
			// With one exception: If QueryRoutine returns STATUS_BUFFER_TOO_SMALL, the error code is ignored.
			nsStatus = RtlQueryRegistryValues(
				RTL_REGISTRY_SERVICES,
				BDArKit_DRIVER_NAMEW L"\\Instances\\" BDArKit_INSTANCE_NAMEW,
				QueryTable, 
				NULL, 
				NULL
				);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
		}

		BDKit_If_Not_Break_With_Reason(!NT_SUCCESS(nsStatus), STATUS_OBJECT_NAME_EXISTS);
		BDKit_If_Not_Break_With_Reason(nsStatus == STATUS_OBJECT_NAME_NOT_FOUND, STATUS_OBJECT_NAME_NOT_FOUND);

		InitializeObjectAttributes(&oa, puszRegistry, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
		nsStatus = ZwCreateKey (
			&hDriverReg,
			KEY_ALL_ACCESS,
			&oa,
			0,
			NULL,
			REG_OPTION_VOLATILE,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
		 
		for ( ulIndex = 0; ulIndex < sizeof(g_RegInfo) / sizeof(g_RegInfo[0]); ++ulIndex )
		{
			ULONG	ulValueLength = 0;

			RtlInitUnicodeString (&uszRegKey, g_RegInfo[ulIndex].pRegistryPath);
			InitializeObjectAttributes(&oa, &uszRegKey, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, hDriverReg, NULL);
			nsStatus = ZwCreateKey (
				&hRegKey,
				KEY_ALL_ACCESS,
				&oa,
				0,
				NULL,
				REG_OPTION_VOLATILE,
				NULL
				);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

			switch(g_RegInfo[ulIndex].ulRegType)
			{
			case REG_SZ:
				{
					ulValueLength = (wcslen (g_RegInfo[ulIndex].pValue) + 1) * sizeof(WCHAR);
				} break;

			case REG_DWORD:
				{
					ulValueLength = sizeof(DWORD);
				} break;

			default:
				{
					ulValueLength = 0;
				}
			}

			RtlInitUnicodeString (&uszRegKey, g_RegInfo[ulIndex].pValueName);
			nsStatus = ZwSetValueKey (
				hRegKey,
				&uszRegKey,
				0,
				g_RegInfo[ulIndex].ulRegType,
				g_RegInfo[ulIndex].pValue,
				ulValueLength
				);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

			BDKitCloseHandle(hRegKey);
		}

	} while (FALSE);

	BDKitCloseHandle(hRegKey);
	BDKitCloseHandle(hDriverReg);

	return nsStatus;
}

NTSTATUS initializeMinifilter(__in PDRIVER_OBJECT DriverObject)
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	PFLT_FILTER		pFilter		= NULL;
	PDRIVER_UNLOAD	OldDriverUnload = NULL;
	
	do 
	{
		// Filter manager resets the DriverUnload routine when we register. Save the
		// old value so we can replace it later
		OldDriverUnload = DriverObject->DriverUnload;

		nsStatus = FltRegisterFilter(
			DriverObject,
			&g_FilterRegistration, 
			&pFilter
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = FltStartFiltering(pFilter);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		g_miniFilter = pFilter;
	} while (FALSE);

	DriverObject->DriverUnload = OldDriverUnload;
	BDKit_If_DoAction(!NT_SUCCESS(nsStatus) && g_miniFilter != NULL, FltUnregisterFilter (pFilter));

	return nsStatus;
}

NTSTATUS BDKitRegisterFileOperationCallback(__in PFLT_PRE_OPERATION_CALLBACK pCallbackFunc)
{
	NTSTATUS nsStatus	= STATUS_UNSUCCESSFUL;

	FltAcquireResourceExclusive(&g_FilterLock);
	{
		do
		{
			PMiniFilterCallbackEntry pNewEntry = NULL;

			if( g_miniFilter == NULL )
			{
				nsStatus = installMinifilter(g_DriverRegistry);
				BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

				nsStatus = initializeMinifilter(g_DriverObject);
				BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
			}

			BDKitAllocateNonpagePool(pNewEntry, sizeof(MiniFilterCallbackEntry));
			BDKit_If_Not_Break_With_Reason(pNewEntry != NULL, STATUS_INSUFFICIENT_RESOURCES);

			pNewEntry->pCallbackFunc = pCallbackFunc;
			BDKitInsertListNode (g_CallBackList, pNewEntry);

			nsStatus = STATUS_SUCCESS;

		} while (FALSE);
	}
	FltReleaseResource (&g_FilterLock);

	return nsStatus;
}

BOOLEAN freeCallBackNode(
						 __in PVOID pNode1, 
						 __in PVOID pNode2
						 )
{
	PMiniFilterCallbackEntry	pPreCallBack= (PMiniFilterCallbackEntry)pNode1;
	PFLT_PRE_OPERATION_CALLBACK	pCallBack	= (PFLT_PRE_OPERATION_CALLBACK)pNode2;

	if (	pPreCallBack != NULL
		&&	pPreCallBack->pCallbackFunc == pCallBack)
	{
		FLT_CALLBACK_DATA		Data	= {0x00};
		FLT_IO_PARAMETER_BLOCK	Iopb	= {0x00};
		PFLT_IO_PARAMETER_BLOCK pIopb	= &Iopb;
		Iopb.MajorFunction = IRP_MJ_UNREGISTER;
#pragma warning( push )
#pragma warning (disable: 28132) //Taking the size of pointer pIopb: This will yield the size of a pointer (4 or 8), not the size of the object pointed to
		RtlCopyMemory(&Data.Iopb, &pIopb, sizeof(pIopb));
#pragma warning( pop ) 
		pCallBack(&Data, NULL, NULL);
		BDKitFreePool(pPreCallBack);
		return TRUE;
	}

	return FALSE;
}

VOID ClearCallBackNode(__in PVOID pNode)
{
	PMiniFilterCallbackEntry	pCallBackEntry = (PMiniFilterCallbackEntry)pNode;
	if (pCallBackEntry != NULL)
	{
		freeCallBackNode (pNode, pCallBackEntry->pCallbackFunc);
	}
}

NTSTATUS BDKitUnregisterFileOperationCallback(PFLT_PRE_OPERATION_CALLBACK pCallbackFunc)
{
	BOOLEAN		bFound		= FALSE;

	FltAcquireResourceExclusive(&g_FilterLock);
	{
		bFound = BDKitRemoveListNodeWithHandler (g_CallBackList, pCallbackFunc, freeCallBackNode);
	}
	FltReleaseResource(&g_FilterLock);

	return bFound? STATUS_SUCCESS : STATUS_OBJECT_NAME_NOT_FOUND;
}

NTSTATUS BDKitInitializeFilter( __in PDRIVER_OBJECT pDriverObject )
{
	NTSTATUS nsStatus = STATUS_UNSUCCESSFUL;

	UNREFERENCED_PARAMETER(pDriverObject);

	do 
	{
		nsStatus = ExInitializeResourceLite(&g_FilterLock);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		g_bFilterIsInitialize = TRUE;
		BDKit_If_DoAction(g_CallBackList == NULL, g_CallBackList = BDKitInitializeList ('TLIF'));
		BDKit_If_Not_Break_With_Reason(g_CallBackList != NULL, STATUS_INSUFFICIENT_RESOURCES);

		nsStatus = STATUS_SUCCESS;
	} while (FALSE);

	return nsStatus;
}

NTSTATUS BDKitUnInitializeFilter( VOID )
{
	BDKit_If_DoAction(g_miniFilter != NULL, FltUnregisterFilter (g_miniFilter));
	g_miniFilter = NULL;

	BDKitCloseListHandleWithHandler (g_CallBackList, ClearCallBackNode);
	g_CallBackList = NULL;

	if ( g_bFilterIsInitialize == TRUE )
	{
		g_bFilterIsInitialize = FALSE;
		ExDeleteResourceLite (&g_FilterLock);
	}

	return STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// Minifilter CallBack Operations //
//////////////////////////////////////////////////////////////////////////
typedef struct _FLT_PARAMETE_
{
	union
	{
		struct 
		{
			PFLT_CALLBACK_DATA		Data;
			PCFLT_RELATED_OBJECTS	FltObjects;
			PVOID*					CompletionContext;
		} FltCallback;
	} Parameters;
} FLT_PARAMETE, *PFLT_PARAMETE;

BOOLEAN doCallbackRoutine(PVOID pNode1, PVOID pNode2)
{
	PMiniFilterCallbackEntry	pPreCallBack= (PMiniFilterCallbackEntry)pNode1;
	PFLT_PARAMETE				pFltPara	= (PFLT_PARAMETE)pNode2;

	if ( pPreCallBack != NULL && pFltPara != NULL )
	{
		if (pPreCallBack->pCallbackFunc(
			pFltPara->Parameters.FltCallback.Data,
			pFltPara->Parameters.FltCallback.FltObjects,
			pFltPara->Parameters.FltCallback.CompletionContext
			) == FLT_PREOP_COMPLETE)
		{
			return TRUE;
		}

	}

	// Return FALSE make sure next callback can be executed
	return FALSE;
}

FLT_PREOP_CALLBACK_STATUS OnPreCallbackRoutine(
	__in PFLT_CALLBACK_DATA Data,
	__in PCFLT_RELATED_OBJECTS FltObjects,
	__in PVOID* CompletionContext
	)
{
	FLT_PARAMETE	fltPara		= {0x00};
	BOOLEAN			bInterrupt	= FALSE;

	fltPara.Parameters.FltCallback.Data				= Data;
	fltPara.Parameters.FltCallback.FltObjects		= FltObjects;
	fltPara.Parameters.FltCallback.CompletionContext	= CompletionContext;

	bInterrupt = BDKitFindListNodeWithHandler(g_CallBackList, &fltPara, doCallbackRoutine) != NULL;

	return bInterrupt? FLT_PREOP_COMPLETE : FLT_PREOP_SUCCESS_NO_CALLBACK;
}