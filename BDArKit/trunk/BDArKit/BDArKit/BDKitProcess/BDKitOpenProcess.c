#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "Utility.h"
#include "NtosKrnl.h"
#include "BDKitUtils.h"
//#include "BDKitProcess.h"


#define OBJ_KERNEL_EXCLUSIVE           0x00010000L
#define OBJ_VALID_PRIVATE_ATTRIBUTES   0x00010000L
#define OBJ_ALL_VALID_ATTRIBUTES       (OBJ_VALID_PRIVATE_ATTRIBUTES | OBJ_VALID_ATTRIBUTES)


__inline ULONG ObSanitizeHandleAttributes(
	__in ULONG HandleAttributes,
	__in KPROCESSOR_MODE Mode
	)
{
	if (Mode == KernelMode) 
	{
		return HandleAttributes & OBJ_ALL_VALID_ATTRIBUTES;
	} 
	else 
	{
		return HandleAttributes & (OBJ_ALL_VALID_ATTRIBUTES & ~(OBJ_KERNEL_HANDLE | OBJ_KERNEL_EXCLUSIVE));
	}
}

NTSTATUS OpenProcessByID (
						  __out	PHANDLE				ProcessHandle,
						  __in	ACCESS_STATE		AccessState,
						  __in	POBJECT_ATTRIBUTES	pObjectAttributes,
						  __in	PCLIENT_ID			pClientId
						  )
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	PEPROCESS	EProcess	= NULL;
	PETHREAD	Thread		= NULL;
	HANDLE		hHandle		= NULL;
	ULONG		ulHandleAttributes = 0;

	do 
	{
		if (pClientId->UniqueThread) 
		{
			nsStatus = PsLookupProcessThreadByCid(
				pClientId,
				&EProcess,
				&Thread
				);
		} 
		else 
		{
			nsStatus = PsLookupProcessByProcessId(pClientId->UniqueProcess, &EProcess);
		}
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		ulHandleAttributes = ObSanitizeHandleAttributes(pObjectAttributes->Attributes, KernelMode);

		nsStatus = ObOpenObjectByPointer(
			EProcess,
			ulHandleAttributes,
			&AccessState,
			0,
			*PsProcessType,
			KernelMode,
			&hHandle
			);
		BDKit_If_Not_Break(nsStatus);
	} while (FALSE);

	*ProcessHandle = hHandle;

	BDKitCloseObject(Thread);
	BDKitCloseObject(EProcess);

	return nsStatus;
}

NTSTATUS OpenProcessByName(
						   __out PHANDLE			ProcessHandle,
						   __in	ACCESS_STATE		AccessState,
						   __in	POBJECT_ATTRIBUTES	pObjectAttributes
						   )
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	HANDLE			hHandle		= NULL;

	do 
	{
		BDKit_If_Not_Break(pObjectAttributes != NULL && pObjectAttributes->ObjectName != NULL);

		nsStatus = ObOpenObjectByName(
			pObjectAttributes,
			NULL,
			KernelMode,
			&AccessState,
			0,
			NULL,
			&hHandle
			);

		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	*ProcessHandle = hHandle;

	return nsStatus;
}

NTSTATUS BDKitOpenProcess(
						  __out	PHANDLE				ProcessHandle,
						  __in	ACCESS_MASK			DesiredAccess,
						  __in	POBJECT_ATTRIBUTES	pObjectAttributes,
						  __in	PCLIENT_ID			pClientId
						  )
{
	NTSTATUS				nsStatus	= STATUS_UNSUCCESSFUL;
	ACCESS_STATE			AccessState = {0x00};
	AUX_ACCESS_DATA	AuxData		= {0x00};
	LUID					SeDebugPriv	= {0x00};
	BOOLEAN					bSetAS		= FALSE;

	do 
	{
		nsStatus = SeCreateAccessState(
			&AccessState,
			&AuxData,
			DesiredAccess,
			IoGetFileObjectGenericMapping()
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		bSetAS = TRUE;

		SeDebugPriv = RtlConvertLongToLuid(SE_DEBUG_PRIVILEGE);

		if (SeSinglePrivilegeCheck(SeDebugPriv, KernelMode)) 
		{
			if (AccessState.RemainingDesiredAccess & MAXIMUM_ALLOWED) 
			{
				AccessState.PreviouslyGrantedAccess |= PROCESS_ALL_ACCESS;
			} 
			else 
			{
				AccessState.PreviouslyGrantedAccess |= (AccessState.RemainingDesiredAccess);
			}

			AccessState.RemainingDesiredAccess = 0;
		}

		if ( pObjectAttributes != NULL )
		{
			nsStatus = OpenProcessByName (ProcessHandle, AccessState, pObjectAttributes);
		}
		else if ( pClientId != NULL )
		{
			nsStatus = OpenProcessByID (ProcessHandle, AccessState, pObjectAttributes, pClientId);
		}
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKit_If_DoAction(bSetAS == TRUE, SeDeleteAccessState(&AccessState));

	return nsStatus;
}