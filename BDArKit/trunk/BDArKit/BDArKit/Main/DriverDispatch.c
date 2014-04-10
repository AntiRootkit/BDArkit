#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "BDArKit\KitHelper.h"
#include "DriverHelper.h"
#include "Utility.h"
#include "BDKitUtils.h"
#include "FileSign.h"
#include "BDKitHandler.h"


struct 
{
	int IOCTL;
	BDKIT_IOCTL_HANDLER* IoctlFunc;
} aIOCTLHandlers[] = 
{
	{ IOCTL_CREATE_FILE,			&doFocusCreateFile		},
	{ IOCTL_READ_FILE,				&doFocusReadFile		},
	{ IOCTL_WRITE_FILE,				&doFocusWriteFile		},
	{ IOCTL_CLOSE_FILE,				&doFocusCloseFile		},
	{ IOCTL_DELETE_FILE,			&doFocusDeleteFile		},
	{ IOCTL_LOCK_FILE,				&doFocusLockFile		},
	{ IOCTL_UNLOCK_FILE,			&doFocusUnlockFile		},
	{ IOCTL_COPY_FILE,				&doFocusCopyFile		},
	{ IOCTL_FIND_FIRST_FILE,		&doFindFirstFile		},
	{ IOCTL_FIND_NEXT_FILE,			&doFindNextFile			},
	{ IOCTL_QUERY_OBJECT,			&doFocusQueryObject		},
	{ IOCTL_OPEN_PROCESS,			&doFocusOpenProcess		},
	{ IOCTL_KILL_PROCESS,			&doFocusKillProcess		},
	{ IOCTL_KILL_PROCESSID,			&doFocusKillProcessId	}
};

NTSTATUS CompleteIrp (__in PIRP Irp, __in ULONG Information, __in NTSTATUS nsStatus)
{
	Irp->IoStatus.Status = nsStatus;
	Irp->IoStatus.Information = Information;
	IoSetCancelRoutine(Irp, NULL);

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return nsStatus;
}

NTSTATUS DriverPassthroughHandler (__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	return CompleteIrp (Irp, 0, STATUS_NOT_SUPPORTED);
}

NTSTATUS DriverControlUtilityHandler( __in PDEVICE_OBJECT DeviceObject, __in PIRP Irp )
{
	NTSTATUS			nsStatus	= STATUS_INVALID_DEVICE_REQUEST;
	PIO_STACK_LOCATION	IrpSp		= IoGetCurrentIrpStackLocation (Irp);
	ULONG				IOCTL		= IrpSp->Parameters.DeviceIoControl.IoControlCode;
	PVOID				pInBuffer	= NULL;
	PVOID				pOutBuffer	= NULL;
	ULONG				ulInformation		= 0;
	ULONG				ulIOCTLIndex		= 0;
	ULONG				ulInBufferLength	= IrpSp->Parameters.DeviceIoControl.InputBufferLength;
	ULONG				ulOutBufferLength	= IrpSp->Parameters.DeviceIoControl.OutputBufferLength;

	switch(METHOD_FROM_CTL_CODE(IOCTL))
	{
	case METHOD_BUFFERED:
		{
			pInBuffer = Irp->AssociatedIrp.SystemBuffer;
			pOutBuffer= Irp->AssociatedIrp.SystemBuffer;
		}
		break;

	case METHOD_NEITHER:
		{
			pInBuffer = IrpSp->Parameters.DeviceIoControl.Type3InputBuffer;
			pOutBuffer= Irp->UserBuffer;
		}
		break;

	default:
		{
			ASSERT(FALSE);
			nsStatus = STATUS_INVALID_PARAMETER;
		}
	}

	if ( ExGetPreviousMode () == UserMode && g_osVersionCheck < WINDOWS_VERSION_8 )
	{
		for (ulIOCTLIndex = 0; ulIOCTLIndex < _countof(aIOCTLHandlers); ++ulIOCTLIndex)
		{
			if ( (ULONG)aIOCTLHandlers[ulIOCTLIndex].IOCTL == IOCTL )
			{
				nsStatus = aIOCTLHandlers[ulIOCTLIndex].IoctlFunc(
					DeviceObject,
					pInBuffer,
					ulInBufferLength,
					pOutBuffer,
					ulOutBufferLength,
					&ulInformation
					);
				if ( pOutBuffer != NULL && ulInformation > 0 )
				{
					((PCOMMON_HEAD_OUT)pOutBuffer)->IoStatus.Status = nsStatus;
					((PCOMMON_HEAD_OUT)pOutBuffer)->IoStatus.Information = ulInformation;
				}

				break;
			}
		}
	}

	nsStatus = (nsStatus == STATUS_PENDING)? nsStatus : STATUS_SUCCESS;
	Irp->IoStatus.Status = nsStatus;
	Irp->IoStatus.Information = ulInformation;

	return nsStatus;
}

NTSTATUS DriverDispatchHandler (__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp)
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	PIO_STACK_LOCATION	IrpSp		= IoGetCurrentIrpStackLocation (Irp);
	PBDKIT_DEVICE_EXTENSION pDevExt = (PBDKIT_DEVICE_EXTENSION)DeviceObject->DeviceExtension;
	WCHAR				szImageName[MAX_PATH - 1] = {L"\0"};
	ULONG				ulLength	= 0;

	do 
	{
		if ( IrpSp->MajorFunction != IRP_MJ_SHUTDOWN )

		{
			if ( !NT_SUCCESS(BDKitIsAuthorizedProcess(PsGetCurrentProcess())) )
			{
				nsStatus=BDKitGetProcessImageNameByEProcess_s(PsGetCurrentProcess(), szImageName, sizeof(szImageName));
				if ( !NT_SUCCESS(nsStatus) )
				{
					Irp->IoStatus.Status = nsStatus = STATUS_ACCESS_DENIED;
					Irp->IoStatus.Information = 0;
					break;
				}

#ifndef DBG
				nsStatus = VerifyFile(szImageName);
				if ( !NT_SUCCESS(nsStatus) )
				{
					Irp->IoStatus.Status = nsStatus = STATUS_ACCESS_DENIED;
					Irp->IoStatus.Information = 0;
					break;
				}
#endif
				nsStatus = BDKitAddAuthorizedProcess (PsGetCurrentProcess());
				BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
			}
		}


		switch(IrpSp->MajorFunction)
		{
		case IRP_MJ_CREATE:
			{
				BDbgPrint ("[BDKIT]Open Deivce\r\n");
				InterlockedIncrement((volatile LONG*)&pDevExt->DeviceRefence);
				Irp->IoStatus.Information = 0;
				Irp->IoStatus.Status = nsStatus = STATUS_SUCCESS;
			}
			break;

		case IRP_MJ_DEVICE_CONTROL:
			{
				nsStatus = DriverControlUtilityHandler (DeviceObject, Irp);
			}
			break;	

		case IRP_MJ_CLOSE:
			{
				BDbgPrint ("[BDKIT]Close Device\r\n");
				InterlockedDecrement((LONG volatile*)&pDevExt->DeviceRefence);
				Irp->IoStatus.Information = 0;
				Irp->IoStatus.Status = nsStatus = STATUS_SUCCESS;

				BDKitRemoveAuthorizedProcess (PsGetCurrentProcess());
			}
			break;

		case IRP_MJ_SHUTDOWN:
			{
				//BDbgPrint ("[BDKIT]Clear Delete FileList\r\n");
				//BDKitClearDeleteFileList ();
				Irp->IoStatus.Information = 0;
				Irp->IoStatus.Status = nsStatus = STATUS_SUCCESS;
			}
			break;

		default:
			{
				return DriverPassthroughHandler (DeviceObject, Irp);
			}
		}
	} while (FALSE);

	if ( nsStatus != STATUS_PENDING )
	{
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
	}

	return nsStatus;
}

NTSTATUS doFocusCreateFile(
						   __in		PDEVICE_OBJECT	DeviceObject,
						   __in_bcount(ulInBufferSize)		PVOID pInBuffer,
						   __in		ULONG			ulInBufferSize,
						   __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
						   __in		ULONG			ulOutBufferSize,
						   __out	PULONG			information
						   )
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	FILE_CREATE_IN*		pInData		= (FILE_CREATE_IN*)pInBuffer;
	FILE_CREATE_OUT*	pOutData	= (FILE_CREATE_OUT*)pOutBuffer;
	FILE_CREATE_IN		ConvData	= {0x00};
	UNICODE_STRING		uszConvStr	= {0x00};

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(ulOutBufferSize);
	UNREFERENCED_PARAMETER(ulInBufferSize);

	do 
	{
		ANSI_STRING		ansFilePath = {0x00};

		__try
		{
			*information = sizeof(COMMON_HEAD_OUT);

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(FILE_CREATE_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(ulOutBufferSize == sizeof(FILE_CREATE_OUT), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pOutData), STATUS_INVALID_PARAMETER);

			nsStatus = BDKitCheckFileFullPathPrefix (pInData->fileInfo.cFilePath, &uszConvStr);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && uszConvStr.Length < sizeof(pInData->fileInfo.cFilePath));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		ConvData.AccessMask			= pInData->AccessMask;
		ConvData.CreateDisposition	= pInData->CreateDisposition;
		ConvData.CreateOptions		= pInData->CreateOptions;
		ConvData.FileAttributes		= pInData->FileAttributes;
		ConvData.ShareAccess		= pInData->ShareAccess;
		RtlCopyMemory(ConvData.fileInfo.cFilePath, uszConvStr.Buffer, uszConvStr.Length);

		RtlUnicodeStringToAnsiString (&ansFilePath, &uszConvStr, TRUE);
		BDbgPrint ("[BDKIT]OpenFile->%Z\r\n", &ansFilePath);
		RtlFreeAnsiString (&ansFilePath);

		nsStatus = BDKitFocusCreateFile (
			&ConvData,
			sizeof(ConvData),
			pOutData,
			ulOutBufferSize,
			information,
			emNormalLevel	
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKitFreePool(uszConvStr.Buffer);

	return nsStatus;
}

NTSTATUS doFocusReadFile(
						 __in		PDEVICE_OBJECT	DeviceObject,
						 __in_bcount(ulInBufferSize)		PVOID pInBuffer,
						 __in		ULONG			ulInBufferSize,
						 __out_bcount(ulOutBufferSize)		PVOID pOutBuffer,
						 __in		ULONG			ulOutBufferSize,
						 __out	PULONG				information
						 )
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	FILE_READ_IN*	pInData		= (FILE_READ_IN*)pInBuffer;
	FILE_READ_OUT*	pOutData	= (FILE_READ_OUT*)pOutBuffer;

	UNREFERENCED_PARAMETER(DeviceObject);

	do 
	{
		__try
		{
			*information = sizeof(COMMON_HEAD_OUT);

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(FILE_READ_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(ulOutBufferSize == sizeof(FILE_READ_OUT), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pOutData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData->Buffer), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(FILE_READ_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(pInData->Handle != NULL && pInData->Handle != (HANDLE)-1, STATUS_INVALID_HANDLE);
			BDKit_If_DoAction(pInData->nNumberBytesOfRead != NULL, ProbeForWrite (pInData->nNumberBytesOfRead, sizeof(PULONG), 4L));
			BDKit_If_DoAction(pInData->Buffer != NULL, ProbeForRead (pInData->Buffer, pInData->nNumberBytesOfToRead, 1L));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = STATUS_INVALID_PARAMETER;
			break;
		}

		nsStatus = BDKitFocusReadFile(
			pInData,
			ulInBufferSize,
			pOutData,
			ulOutBufferSize,
			information,
			emIgnoreLevel
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while(FALSE);

	return nsStatus;
}

NTSTATUS doFocusWriteFile(
						  __in		PDEVICE_OBJECT	DeviceObject,
						  __in_bcount(ulInBufferSize)		PVOID pInBuffer,
						  __in		ULONG			ulInBufferSize,
						  __out_bcount(ulOutBufferSize)		PVOID pOutBuffer,
						  __in		ULONG			ulOutBufferSize,
						  __out	PULONG				information
						  )
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	FILE_WRITE_IN*	pInData		= (FILE_WRITE_IN*)pInBuffer;
	FILE_WRITE_OUT*	pOutData	= (FILE_WRITE_OUT*)pOutBuffer;

	UNREFERENCED_PARAMETER(DeviceObject);

	do 
	{
		__try
		{
			*information = sizeof(COMMON_HEAD_OUT);

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(FILE_WRITE_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(ulOutBufferSize == sizeof(FILE_WRITE_OUT), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pOutData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData->Buffer), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(FILE_READ_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(pInData->Handle != NULL && pInData->Handle != (HANDLE)-1, STATUS_INVALID_HANDLE);
			BDKit_If_DoAction(pInData->nNumberBytesOfWrite != NULL, ProbeForWrite (pInData->nNumberBytesOfWrite, sizeof(PULONG), 4L));
			BDKit_If_DoAction(pInData->Buffer != NULL, ProbeForWrite (pInData->Buffer, pInData->nNumberBytesOfToWrite, 1L));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = STATUS_INVALID_PARAMETER;
			break;
		}

		nsStatus = BDKitFocusWriteFile(
			pInData,
			ulInBufferSize,
			pOutData,
			ulOutBufferSize,
			information,
			emIgnoreLevel
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while(FALSE);

	return nsStatus;
}

NTSTATUS doFocusCloseFile(
						  __in		PDEVICE_OBJECT	DeviceObject,
						  __in_bcount(ulInBufferSize)		PVOID pInBuffer,
						  __in		ULONG			ulInBufferSize,
						  __out_bcount(ulOutBufferSize)		PVOID pOutBuffer,
						  __in		ULONG			ulOutBufferSize,
						  __out		PULONG			information
						  )
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	FILE_CLOSE_IN*	pInData		= (FILE_CLOSE_IN*)pInBuffer;

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);

	do 
	{
		__try
		{
			*information = 0;

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(FILE_CLOSE_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = STATUS_INVALID_PARAMETER;
			break;
		}

		nsStatus = BDKitFocusCloseFile (
			pInData->hFile, 
			sizeof(pInData->hFile), 
			NULL, 
			0, 
			information, 
			emNormalLevel
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	return nsStatus;
}

NTSTATUS doFocusDeleteFile(
						   __in		PDEVICE_OBJECT	DeviceObject,
						   __in_bcount(ulInBufferSize)		PVOID pInBuffer,
						   __in		ULONG			ulInBufferSize,
						   __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
						   __in		ULONG			ulOutBufferSize,
						   __out	PULONG			information
						   )
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	DELETE_FILE_IN*	pInData		= (DELETE_FILE_IN*)pInBuffer;
	UNICODE_STRING	uszConvStr	= {0x00};

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);

	do 
	{
		ANSI_STRING		ansFilePath = {0x00};

		__try
		{
			*information = 0;

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(DELETE_FILE_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData->fileInfo.cFilePath), STATUS_INVALID_PARAMETER);
			//BDKit_If_DoAction(pInData->fileInfo.cFilePath != NULL, 
			//	ProbeForRead (pInData->fileInfo.cFilePath, sizeof(pInData->fileInfo.cFilePath), 1L));

			nsStatus = BDKitCheckFileFullPathPrefix (pInData->fileInfo.cFilePath, &uszConvStr);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && uszConvStr.Length < sizeof(pInData->fileInfo.cFilePath));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		RtlUnicodeStringToAnsiString (&ansFilePath, &uszConvStr, TRUE);
		BDbgPrint ("[BDKIT]DelFile->%Z %u\r\n", &ansFilePath, pInData->DoFileAction);
		RtlFreeAnsiString (&ansFilePath);

		if ( (pInData->DoFileAction & DO_RebootAction) == DO_RebootAction )
		{
			nsStatus = BDKitAddDeleteFileOnReboot(
				&uszConvStr,
				sizeof(UNICODE_STRING) + uszConvStr.MaximumLength,
				pOutBuffer,
				ulOutBufferSize,
				information,
				emNormalLevel
				);

		} else if ( (pInData->DoFileAction & DO_RemoveAction) == DO_RemoveAction )
		{
			nsStatus = BDKitRemoveDeleteFileOnReboot(
				&uszConvStr,
				sizeof(UNICODE_STRING) + uszConvStr.MaximumLength,
				pOutBuffer,
				ulOutBufferSize,
				information,
				emNormalLevel
				);

		} else if ( (pInData->DoFileAction & DO_OnceAction) == DO_OnceAction )
		{
			nsStatus = BDKitFocusDeleteFile (
				&uszConvStr, 
				sizeof(UNICODE_STRING) + uszConvStr.MaximumLength, 
				pOutBuffer, 
				ulOutBufferSize, 
				information, 
				emNormalLevel
				);
		}

		// 这两个先不需要返回结果
		if ( (pInData->DoFileAction & DO_DenyAction) == DO_DenyAction )
		{
			BDKitDenyFileCreate(
				&uszConvStr,
				sizeof(UNICODE_STRING) + uszConvStr.MaximumLength,
				pOutBuffer,
				ulOutBufferSize,
				information,
				emNormalLevel
				);

		} else if ( (pInData->DoFileAction & DO_PassAction) == DO_PassAction )
		{
			BDKitRemoveDenyFileCreate(
				&uszConvStr,
				sizeof(UNICODE_STRING) + uszConvStr.MaximumLength,
				pOutBuffer,
				ulOutBufferSize,
				information,
				emNormalLevel
				);
		}
	
	} while (FALSE);

	BDKitFreePool(uszConvStr.Buffer);

	return nsStatus;
}

NTSTATUS doFocusLockFile(
						 __in		PDEVICE_OBJECT	DeviceObject,
						 __in_bcount(ulInBufferSize)		PVOID pInBuffer,
						 __in		ULONG			ulInBufferSize,
						 __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
						 __in		ULONG			ulOutBufferSize,
						 __out	PULONG			information
						 )
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	LOCK_FILE_IN*	pInData		= (LOCK_FILE_IN*)pInBuffer;
	UNICODE_STRING	uszConvStr	= {0x00};

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);

	do 
	{
		ANSI_STRING		ansFilePath = {0x00};

		__try
		{
			*information = 0;

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(UNLOCK_FILE_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData->fileInfo.cFilePath), STATUS_INVALID_PARAMETER);
			//BDKit_If_DoAction(pInData->fileInfo.cFilePath != NULL, 
			//	ProbeForRead (pInData->fileInfo.cFilePath, sizeof(pInData->fileInfo.cFilePath), 1L));

			nsStatus = BDKitCheckFileFullPathPrefix (pInData->fileInfo.cFilePath, &uszConvStr);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && uszConvStr.Length < sizeof(pInData->fileInfo.cFilePath));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		RtlUnicodeStringToAnsiString (&ansFilePath, &uszConvStr, TRUE);
		BDbgPrint ("[BDKIT]LockFile->%Z\r\n", &ansFilePath);
		RtlFreeAnsiString (&ansFilePath);

		nsStatus = BDKitFocusLockFile (
			&uszConvStr, 
			sizeof(UNICODE_STRING) + uszConvStr.MaximumLength, 
			pOutBuffer, 
			ulOutBufferSize, 
			information, 
			emNormalLevel
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKitFreePool(uszConvStr.Buffer);

	return nsStatus;
}

NTSTATUS doFocusUnlockFile(
						   __in		PDEVICE_OBJECT	DeviceObject,
						   __in_bcount(ulInBufferSize)		PVOID pInBuffer,
						   __in		ULONG			ulInBufferSize,
						   __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
						   __in		ULONG			ulOutBufferSize,
						   __out	PULONG			information
						   )
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	UNLOCK_FILE_IN*	pInData		= (UNLOCK_FILE_IN*)pInBuffer;
	UNICODE_STRING	uszConvStr	= {0x00};

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);

	do 
	{
		ANSI_STRING		ansFilePath = {0x00};

		__try
		{
			*information = 0;

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(UNLOCK_FILE_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData->fileInfo.cFilePath), STATUS_INVALID_PARAMETER);
			//BDKit_If_DoAction(pInData->fileInfo.cFilePath != NULL, 
			//	ProbeForRead (pInData->fileInfo.cFilePath, sizeof(pInData->fileInfo.cFilePath), 1L));

			nsStatus = BDKitCheckFileFullPathPrefix (pInData->fileInfo.cFilePath, &uszConvStr);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && uszConvStr.Length < sizeof(pInData->fileInfo.cFilePath));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		RtlUnicodeStringToAnsiString (&ansFilePath, &uszConvStr, TRUE);
		BDbgPrint ("[BDKIT]UnlockFile->%Z\r\n", &ansFilePath);
		RtlFreeAnsiString (&ansFilePath);

		nsStatus = BDKitFocusUnlockFile (
			&uszConvStr, 
			sizeof(UNICODE_STRING) + uszConvStr.MaximumLength, 
			pOutBuffer, 
			ulOutBufferSize, 
			information, 
			emNormalLevel
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKitFreePool(uszConvStr.Buffer);

	return nsStatus;
}

NTSTATUS doFocusCopyFile(
						  __in		PDEVICE_OBJECT	DeviceObject,
						  __in_bcount(ulInBufferSize)		PVOID pInBuffer,
						  __in		ULONG			ulInBufferSize,
						  __out_bcount(ulOutBufferSize)		PVOID pOutBuffer,
						  __in		ULONG			ulOutBufferSize,
						  __out		PULONG			information
						  )
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	COPY_FILE_IN*	pInData		= (COPY_FILE_IN*)pInBuffer;
	UNICODE_STRING	uszConvSrcStr	= {0x00};
	UNICODE_STRING	uszConvDstStr	= {0x00};
	COPY_FILE_EX_PROXY	cpyFile		= {0x00};

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);

	do 
	{
		ANSI_STRING		ansSrcFilePath = {0x00};
		ANSI_STRING		ansDstFilePath = {0x00};

		__try
		{
			*information = 0;

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(COPY_FILE_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData->fileSrcInfo.cFilePath), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData->fileDstInfo.cFilePath), STATUS_INVALID_PARAMETER);
			BDKit_If_DoAction(pInData->pbCancel != NULL, ProbeForRead (pInData->pbCancel, sizeof(*pInData->pbCancel), 1L));

			nsStatus = BDKitCheckFileFullPathPrefix (pInData->fileSrcInfo.cFilePath, &uszConvSrcStr);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && uszConvSrcStr.Length < sizeof(pInData->fileSrcInfo.cFilePath));

			nsStatus = BDKitCheckFileFullPathPrefix (pInData->fileDstInfo.cFilePath, &uszConvDstStr);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && uszConvDstStr.Length < sizeof(pInData->fileDstInfo.cFilePath));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		RtlUnicodeStringToAnsiString (&ansSrcFilePath, &uszConvSrcStr, TRUE);
		RtlUnicodeStringToAnsiString (&ansDstFilePath, &uszConvDstStr, TRUE);
		BDbgPrint ("[BDKIT]CopyFile->%Z -> %Z %u\r\n", &ansSrcFilePath, &ansDstFilePath, pInData->DoFileAction);
		RtlFreeAnsiString (&ansSrcFilePath);
		RtlFreeAnsiString (&ansDstFilePath);

		cpyFile.pExistingFileName	= &uszConvSrcStr;
		cpyFile.pNewFileName		= &uszConvDstStr;
		cpyFile.lpData		= pInData->lpData;
		cpyFile.dwCopyFlags = pInData->dwCopyFlags;
		cpyFile.pbCancel	= pInData->pbCancel;
		cpyFile.lpProgressRoutine	= pInData->pProgressRoutine;

		if ( pInData->DoFileAction == DO_RebootAction )
		{
			// 这里暂时先不考虑其他参数
			nsStatus = BDKitAddRecoverFileOnReboot(
				&cpyFile, 
				sizeof(cpyFile),
				pOutBuffer, 
				ulOutBufferSize, 
				information, 
				emNormalLevel
				);

		} else if ( pInData->DoFileAction == DO_RemoveAction )
		{
			nsStatus = BDKitRemoveRecoverFileOnReboot(
				&cpyFile, 
				sizeof(cpyFile),
				pOutBuffer, 
				ulOutBufferSize, 
				information, 
				emNormalLevel
				);
		
		} else if ( pInData->DoFileAction == DO_OnceAction )
		{
			nsStatus = BDKitFocusCopyFile (
				&cpyFile, 
				sizeof(cpyFile),
				pOutBuffer, 
				ulOutBufferSize, 
				information, 
				emNormalLevel
				);

		} else
		{
			nsStatus = STATUS_INVALID_PARAMETER;
		}
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKitFreePool(uszConvSrcStr.Buffer);
	BDKitFreePool(uszConvDstStr.Buffer);

	return nsStatus;
}

NTSTATUS doDenyFileCreate(
						  __in		PDEVICE_OBJECT	DeviceObject,
						  __in_bcount(ulInBufferSize)		PVOID pInBuffer,
						  __in		ULONG			ulInBufferSize,
						  __out_bcount(ulOutBufferSize)		PVOID pOutBuffer,
						  __in		ULONG			ulOutBufferSize,
						  __out		PULONG			information
						  )

{
	NTSTATUS				nsStatus	= STATUS_UNSUCCESSFUL;
	FILE_ACCESS_DENY_IN*	pInData		= (FILE_ACCESS_DENY_IN*)pInBuffer;
	UNICODE_STRING			uszConvStr	= {0x00};

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);

	do 
	{
		ANSI_STRING		ansFilePath = {0x00};

		__try
		{
			*information = 0;

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(FILE_ACCESS_DENY_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData->fileInfo.cFilePath), STATUS_INVALID_PARAMETER);

			nsStatus = BDKitCheckFileFullPathPrefix (pInData->fileInfo.cFilePath, &uszConvStr);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && uszConvStr.Length < sizeof(pInData->fileInfo.cFilePath));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		RtlUnicodeStringToAnsiString (&ansFilePath, &uszConvStr, TRUE);
		BDbgPrint ("[BDKIT]DenyFile->%Z\r\n", &ansFilePath);
		RtlFreeAnsiString (&ansFilePath);

		nsStatus = BDKitDenyFileCreate (
			&uszConvStr, 
			sizeof(UNICODE_STRING) + uszConvStr.MaximumLength, 
			NULL, 
			0, 
			information, 
			emNormalLevel
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKitFreePool(uszConvStr.Buffer);

	return nsStatus;
}

NTSTATUS doFindFirstFile(
						 __in		PDEVICE_OBJECT	DeviceObject,
						 __in_bcount(ulInBufferSize)	PVOID pInBuffer,
						 __in		ULONG			ulInBufferSize,
						 __out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
						 __in		ULONG			ulOutBufferSize,
						 __out		PULONG			information
						 )
{
	NTSTATUS				nsStatus	= STATUS_UNSUCCESSFUL;
	FIND_FIRST_FILE_IN*		pInData		= (FIND_FIRST_FILE_IN*)pInBuffer;
	FIND_FIRST_FILE_OUT*	pOutData	= (FIND_FIRST_FILE_OUT*)pOutBuffer;
	FIND_FIRST_FILE_IN		ConvData	= {0x00};
	UNICODE_STRING			uszConvStr	= {0x00};

	UNREFERENCED_PARAMETER(DeviceObject);

	do 
	{
		ANSI_STRING		ansFilePath = {0x00};

		__try
		{
			*information = sizeof(COMMON_HEAD_OUT);

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(FIND_FIRST_FILE_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(ulOutBufferSize == sizeof(FIND_FIRST_FILE_OUT), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pOutData), STATUS_INVALID_PARAMETER);

			nsStatus = BDKitCheckFileFullPathPrefix (pInData->szDirectory, &uszConvStr);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && uszConvStr.Length < sizeof(pInData->szDirectory));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		RtlCopyMemory(ConvData.szDirectory, uszConvStr.Buffer, uszConvStr.Length);
		RtlCopyMemory(ConvData.szFilePattern, pInData->szFilePattern, wcslen (pInData->szFilePattern) * sizeof(WCHAR));

		RtlUnicodeStringToAnsiString (&ansFilePath, &uszConvStr, TRUE);
		BDbgPrint ("[BDKIT]QueryFile->%Z\r\n", &ansFilePath);
		RtlFreeAnsiString (&ansFilePath);

		nsStatus = BDKitFindFirstFile (
			&ConvData,
			sizeof(ConvData),
			pOutData,
			ulOutBufferSize,
			information,
			emIgnoreLevel
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKitFreePool(uszConvStr.Buffer);

	return nsStatus;
}

NTSTATUS doFindNextFile(
						__in		PDEVICE_OBJECT	DeviceObject,
						__in_bcount(ulInBufferSize)		PVOID pInBuffer,
						__in		ULONG			ulInBufferSize,
						__out_bcount(ulOutBufferSize)	PVOID pOutBuffer,
						__in		ULONG			ulOutBufferSize,
						__out		PULONG			information
						)
{
	NTSTATUS				nsStatus	= STATUS_UNSUCCESSFUL;
	FIND_NEXT_FILE_IN*		pInData		= (FIND_NEXT_FILE_IN*)pInBuffer;
	FIND_NEXT_FILE_OUT*		pOutData	= (FIND_NEXT_FILE_OUT*)pOutBuffer;

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);

	do 
	{
		__try
		{
			*information = sizeof(COMMON_HEAD_OUT);

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(FIND_NEXT_FILE_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(ulOutBufferSize == sizeof(FIND_NEXT_FILE_OUT), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pOutData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(pInData->hDirectory != NULL, STATUS_INVALID_PARAMETER);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		nsStatus = BDKitFindNextFile (
			pInData,
			ulInBufferSize,
			pOutData,
			ulOutBufferSize,
			information,
			emIgnoreLevel	
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	return nsStatus;
}

NTSTATUS doFocusQueryObject(
							__in		PDEVICE_OBJECT	DeviceObject,
							__in_bcount(ulInBufferSize) PVOID pInBuffer,
							__in		ULONG			ulInBufferSize,
							__out_bcount(ulOutBufferSize) PVOID pOutBuffer,
							__in		ULONG			ulOutBufferSize,
							__out		PULONG			information
							)
{
	NTSTATUS				nsStatus	= STATUS_UNSUCCESSFUL;
	QUERY_OBJECT_IN*		pInData		= (QUERY_OBJECT_IN*)pInBuffer;

	UNREFERENCED_PARAMETER(DeviceObject);

	do 
	{
		__try
		{
			*information = sizeof(COMMON_HEAD_OUT);

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(QUERY_OBJECT_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(pInData->ObjectInformation != NULL && pInData->ResultLength != NULL, STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(pInData->hObjectHandle != NULL, STATUS_INVALID_PARAMETER);

			ProbeForWrite (pInData->ObjectInformation, pInData->Length, sizeof(ULONG));
			ProbeForWrite (pInData->ResultLength, sizeof(ULONG), sizeof(ULONG));
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		nsStatus = BDKitQueryObject (
			pInData,
			ulInBufferSize,
			NULL,
			0,
			information,
			emIgnoreLevel	
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	return nsStatus;
}

NTSTATUS doFocusOpenProcess(
							__in		PDEVICE_OBJECT	DeviceObject,
							__in_bcount(ulInBufferSize) PVOID pInBuffer,
							__in		ULONG			ulInBufferSize,
							__out_bcount(ulOutBufferSize) PVOID pOutBuffer,
							__in		ULONG			ulOutBufferSize,
							__out		PULONG			information
							)
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	OPEN_PROCESS_IN*	pInData		= (OPEN_PROCESS_IN*)pInBuffer;
	OPEN_PROCESS_OUT*	pOutData	= (OPEN_PROCESS_OUT*)pOutBuffer;

	UNREFERENCED_PARAMETER(DeviceObject);

	do 
	{
		__try
		{
			*information = sizeof(COMMON_HEAD_OUT);

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(OPEN_PROCESS_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(ulOutBufferSize == sizeof(OPEN_PROCESS_OUT), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pOutData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(pInData->CliendId.UniqueProcess != 0, STATUS_INVALID_PARAMETER);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		BDbgPrint ("[BDKIT]OpenProcess->%u\r\n", pInData->CliendId.UniqueProcess);

		nsStatus = BDKitFocusOpenProcess(
			pInData,
			ulInBufferSize,
			pOutData,
			ulOutBufferSize,
			information,
			emNormalLevel
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	return nsStatus;
}

NTSTATUS doFocusKillProcess(
							__in		PDEVICE_OBJECT	DeviceObject,
							__in_bcount(ulInBufferSize) PVOID pInBuffer,
							__in		ULONG			ulInBufferSize,
							__out_bcount(ulOutBufferSize) PVOID pOutBuffer,
							__in		ULONG			ulOutBufferSize,
							__out		PULONG			information
							)
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	KILL_PROCESS_IN*	pInData		= (KILL_PROCESS_IN*)pInBuffer;

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);

	do 
	{
		__try
		{
			*information = 0;

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(KILL_PROCESS_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData->fileInfo.cFilePath) 
				&& pInData->hProcess != NULL
				&& pInData->hProcess != (HANDLE)0xFFFFFFFF, STATUS_INVALID_PARAMETER);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		BDbgPrint ("[BDKIT]KillProcess->%p\r\n", pInData->hProcess);

		nsStatus = BDKitFocusKillProcess (
			pInData, 
			ulInBufferSize,
			pOutBuffer,
			ulOutBufferSize,
			information,
			emNormalLevel
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	return nsStatus;
}

NTSTATUS doFocusKillProcessId(
							  __in		PDEVICE_OBJECT	DeviceObject,
							  __in_bcount(ulInBufferSize) PVOID pInBuffer,
							  __in		ULONG			ulInBufferSize,
							  __out_bcount(ulOutBufferSize) PVOID pOutBuffer,
							  __in		ULONG			ulOutBufferSize,
							  __out		PULONG			information
							  )
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	KILL_PROCESSID_IN*	pInData		= (KILL_PROCESSID_IN*)pInBuffer;

	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(pOutBuffer);
	UNREFERENCED_PARAMETER(ulOutBufferSize);

	do 
	{
		__try
		{
			*information = 0;

			BDKit_If_Not_Break_With_Reason(ulInBufferSize == sizeof(KILL_PROCESSID_IN), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (information), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData), STATUS_INVALID_PARAMETER);
			BDKit_If_Not_Break_With_Reason(MmIsAddressValid (pInData->fileInfo.cFilePath) 
				&& pInData->dwProcessId != 0 && pInData->dwProcessId != 4
				&& pInData->dwProcessId != (DWORD)0xFFFFFFFF, STATUS_INVALID_PARAMETER);
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			nsStatus = GetExceptionCode();
			break;
		}

		BDbgPrint ("[BDKIT]KillProcessId->%u\r\n", pInData->dwProcessId);

		nsStatus = BDKitFocusKillProcessId (
			pInData, 
			ulInBufferSize,
			pOutBuffer,
			ulOutBufferSize,
			information,
			emNormalLevel
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	return nsStatus;
}