#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "BDArKit\KitHelper.h"
#include "DriverHelper.h"
#include "Utility.h"
#include "NtosKrnl.h"
#include "BDKitUtils.h"
#include "BDKitHandler.h"


NTSTATUS BDKitFocusOpenProcess(
							   __in_bcount(ulInBufferSize)		PVOID pInBuffer,
							   __in		ULONG			ulInBufferSize,
							   __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
							   __in		ULONG			ulOutBufferSize,
							   __out	PULONG			information,
							   __in		ULONG			level
							   )
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	OPEN_PROCESS_IN*	pInData		= (OPEN_PROCESS_IN*)pInBuffer;
	OPEN_PROCESS_OUT*	pOutData	= (OPEN_PROCESS_OUT*)pOutBuffer;
	OBJECT_ATTRIBUTES	oa			= {0x00};
	CLIENT_ID			ClientId	= {0x00};
	DWORD				dwDesiredAccess	= 0;
	HANDLE				hKeHandle	= NULL;

	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);

	do 
	{
		BDKit_If_Not_Break_With_Reason(pInData->fileInfo.cFilePath[0] == L'\0', STATUS_NOT_SUPPORTED);

		ClientId.UniqueProcess	= (HANDLE)pInData->CliendId.UniqueProcess;
		ClientId.UniqueThread	= (HANDLE)pInData->CliendId.UniqueThread;
		dwDesiredAccess			= pInData->dwDesiredAccess;
		InitializeObjectAttributes(&oa, NULL, (pInData->bInheritHandle? OBJ_INHERIT : 0) | OBJ_KERNEL_HANDLE, NULL, NULL);
		pOutData->hProcess		= NULL;

		switch(level)
		{
		case emNormalLevel:
			{
				nsStatus = ZwOpenProcess (
					&hKeHandle,
					dwDesiredAccess,
					&oa,
					&ClientId
					);
				BDKit_If_Not_Break(!NT_SUCCESS(nsStatus));
			}

		case emMiddleLevel:
			{
				nsStatus = BDKitOpenProcess (
					&hKeHandle,
					dwDesiredAccess,
					&oa,
					&ClientId
					);
				BDKit_If_Not_Break(!NT_SUCCESS(nsStatus));
			}

		case emFocusLevel:
			{
				nsStatus = STATUS_NOT_SUPPORTED;
			}

		case emPowerfulLevel:
			{
				nsStatus = STATUS_NOT_SUPPORTED;
			}
			break;

		default:
			{
				nsStatus = STATUS_NOT_SUPPORTED;
			}
			break;
		}
	} while (FALSE);

	if ( NT_SUCCESS(nsStatus) && hKeHandle != NULL )
	{
		if ( IsKernelHandle(hKeHandle, KernelMode) )
		{
			PEPROCESS 	EProcess	= NULL;
			nsStatus = ObReferenceObjectByHandle (
				hKeHandle,
				0,
				*PsProcessType,
				KernelMode,
				&EProcess,
				NULL
				);
			if ( NT_SUCCESS(nsStatus) )
			{
				nsStatus = ObOpenObjectByPointer(
					EProcess,
					0,
					NULL,
					PROCESS_ALL_ACCESS,
					*PsProcessType,
					UserMode,
					&pOutData->hProcess
					);

				*information = sizeof(OPEN_PROCESS_OUT);
			}

			BDKitCloseHandle(hKeHandle);
			BDKitCloseObject(EProcess);
		}
		else
		{
			pOutData->hProcess = hKeHandle;
			*information = sizeof(OPEN_PROCESS_OUT);
		}
	}

	return nsStatus;
}

NTSTATUS BDKitFocusKillProcess(
							   __in_bcount(ulInBufferSize)		PVOID pInBuffer,
							   __in		ULONG			ulInBufferSize,
							   __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
							   __in		ULONG			ulOutBufferSize,
							   __out	PULONG			information,
							   __in		ULONG			level
							   )
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	KILL_PROCESS_IN*	pInData		= (KILL_PROCESS_IN*)pInBuffer;
	PEPROCESS			EProcess	= NULL;

	UNREFERENCED_PARAMETER(ulInBufferSize);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(information);

	do
	{
		nsStatus = ObReferenceObjectByHandle ((HANDLE)pInData->hProcess, 0, *PsProcessType, UserMode, &EProcess, NULL);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		if ( EProcess == PsGetCurrentProcess() )
		{
			nsStatus = STATUS_CANT_TERMINATE_SELF;
			BDKit_If_Not_Break(FALSE);
		}

		switch(level)
		{
		case emNormalLevel:
			{
				nsStatus = BDKitTerminateProcessByAPI (EProcess, pInData->ExitCode);
				BDKit_If_Not_Break(!NT_SUCCESS(nsStatus));
			}
			break;

		case emMiddleLevel:
			{
				nsStatus = STATUS_NOT_SUPPORTED;
			}
			break;

		case emFocusLevel:
			{
				nsStatus = STATUS_NOT_SUPPORTED;
			}

		case emPowerfulLevel:
			{
				nsStatus = STATUS_NOT_SUPPORTED;
			}

		default:
			{
				nsStatus = STATUS_NOT_SUPPORTED;
				// TODO
			}
		}
	}while(FALSE);

	BDKitCloseObject(EProcess);

	return nsStatus;
}

NTSTATUS BDKitFocusKillProcessId(
								 __in_bcount(ulInBufferSize)		PVOID pInBuffer,
								 __in		ULONG			ulInBufferSize,
								 __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
								 __in		ULONG			ulOutBufferSize,
								 __out	PULONG			information,
								 __in		ULONG			level
								 )
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	KILL_PROCESSID_IN*	pInData		= (KILL_PROCESSID_IN*)pInBuffer;
	PEPROCESS			EProcess	= NULL;

	UNREFERENCED_PARAMETER(ulInBufferSize);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(information);

	do
	{
		nsStatus = PsLookupProcessByProcessId((HANDLE)pInData->dwProcessId, &EProcess);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		if ( EProcess == PsGetCurrentProcess() )
		{
			nsStatus = STATUS_CANT_TERMINATE_SELF;
			BDKit_If_Not_Break(FALSE);
		}

		switch(level)
		{
		case emNormalLevel:
			{
				nsStatus = BDKitTerminateProcessByAPI (EProcess, pInData->ExitCode);
				BDKit_If_Not_Break(!NT_SUCCESS(nsStatus));
			}
			break;

		case emMiddleLevel:
			{
				nsStatus = STATUS_NOT_SUPPORTED;
			}
			break;

		case emFocusLevel:
			{
				nsStatus = STATUS_NOT_SUPPORTED;
			}

		case emPowerfulLevel:
			{
				nsStatus = STATUS_NOT_SUPPORTED;
			}

		default:
			{
				nsStatus = STATUS_NOT_SUPPORTED;
				// TODO
			}
		}
	}while(FALSE);

	BDKitCloseObject(EProcess);

	return nsStatus;
}