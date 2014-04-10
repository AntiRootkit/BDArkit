#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "Utility.h"
#include "BDKitUtils.h"


//3790 没有这个结构体
typedef struct _QueryDirectory 
{
	ULONG					Length;
	PUNICODE_STRING			FileName;
	FILE_INFORMATION_CLASS	FileInformationClass;
	ULONG POINTER_ALIGNMENT FileIndex;
} QueryDirectory, *PQueryDirectory;

NTSTATUS IrpQueryInformationFile(
								 __in PFILE_OBJECT		pFileObject,
								 __in PDEVICE_OBJECT	DeviceObject,
								 __out PVOID			FileInformation,
								 __in ULONG				Length,
								 __in FILE_INFORMATION_CLASS FileInformationClass
								 )
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	PIRP				Irp			= NULL;
	PIO_STACK_LOCATION	IrpSp		= NULL;
	KEVENT				kEvent		= {0x00};
	IO_STATUS_BLOCK		IoStatus	= {0x00};

	do 
	{
		Irp = IoAllocateIrp(DeviceObject->StackSize, FALSE);
		BDKit_If_Not_Break(Irp != NULL);

		KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

		RtlZeroMemory(FileInformation, Length);
		Irp->AssociatedIrp.SystemBuffer = FileInformation;
		Irp->UserEvent	= &kEvent;
		Irp->UserIosb	= &IoStatus;
		Irp->RequestorMode = KernelMode;
		Irp->Tail.Overlay.Thread = PsGetCurrentThread();
		Irp->Tail.Overlay.OriginalFileObject = pFileObject;

		IrpSp = IoGetNextIrpStackLocation(Irp);
		IrpSp->MajorFunction	= IRP_MJ_QUERY_INFORMATION;
		IrpSp->DeviceObject		= DeviceObject;
		IrpSp->FileObject		= pFileObject;
		IrpSp->Parameters.QueryFile.Length = Length;
		IrpSp->Parameters.QueryFile.FileInformationClass = FileInformationClass;

		IoSetCompletionRoutine(Irp, BDKitIoCompletionRoutine, 0, TRUE, TRUE, TRUE);

		nsStatus = IoCallDriver(DeviceObject, Irp);
		BDKit_If_DoAction(nsStatus == STATUS_PENDING, 
			KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL));

		Irp = NULL;

		nsStatus = IoStatus.Status;
	} while (FALSE);

	BDKit_If_DoAction(Irp != NULL, IoFreeIrp (Irp));

	return nsStatus;
}

NTSTATUS IopSynchronousServiceTail(
								   __in PDEVICE_OBJECT DeviceObject,
								   __in PIRP Irp,
								   __in PFILE_OBJECT FileObject,
								   __in BOOLEAN Deferred,
								   __in KPROCESSOR_MODE PreviousMode,
								   __in BOOLEAN SynchIo
								   )
{
	NTSTATUS nsStatus = STATUS_UNSUCCESSFUL;

	if ( SynchIo == FALSE ) 
	{
		IoEnqueueIrp(Irp);
	}

	nsStatus = IoCallDriver(DeviceObject, Irp);

	if ( Deferred == TRUE ) 
	{
		if ( nsStatus != STATUS_PENDING ) 
		{
			PKNORMAL_ROUTINE	normalRoutine	= NULL;
			PVOID				normalContext	= NULL;
			KIRQL				Irql			= PASSIVE_LEVEL;

			ASSERT(!Irp->PendingReturned);

			BDKit_If_DoAction(SynchIo == FALSE, KeRaiseIrql (APC_LEVEL, &Irql));
			{
				BDKitIopCompleteRequest(
					&Irp->Tail.Apc,
					&normalRoutine,
					&normalContext,
					(PVOID *) &FileObject,
					&normalContext
					);
			}
			BDKit_If_DoAction(SynchIo == FALSE, KeLowerIrql (Irql));
		}
	}

	if ( SynchIo == TRUE ) 
	{
		if ( nsStatus == STATUS_PENDING ) 
		{
			nsStatus = KeWaitForSingleObject(
				&FileObject->Event,
				Executive,
				PreviousMode,
				(BOOLEAN) ((FileObject->Flags & FO_ALERTABLE_IO) != 0),
				(PLARGE_INTEGER)NULL
				);
			BDKit_If_DoAction(nsStatus == STATUS_ALERTED || nsStatus == STATUS_USER_APC,
				BDKitCancelAlertedRequest (&FileObject->Event, Irp));

			nsStatus = FileObject->FinalStatus;
		}

		BDKitReleaseFileObjectLock (FileObject);
	}

	return nsStatus;
}

NTSTATUS IrpQueryDirectoryFile(
							   __in		HANDLE				FileHandle,
							   __in_opt HANDLE				Event,
							   __in_opt PIO_APC_ROUTINE		ApcRoutine,
							   __in_opt PVOID				ApcContext,
							   __out	PIO_STATUS_BLOCK	IoStatusBlock,
							   __out	PVOID				FileInformation,
							   __in		ULONG				Length,
							   __in		FILE_INFORMATION_CLASS  FileInformationClass,
							   __in		BOOLEAN				ReturnSingleEntry,
							   __in_opt PUNICODE_STRING		FileName,
							   __in		BOOLEAN				RestartScan
							   )
{
	NTSTATUS			nsStatus			= STATUS_UNSUCCESSFUL;
	KPROCESSOR_MODE		kRequestorMode		= KernelMode;
	PIRP				Irp					= NULL;
	PIO_STACK_LOCATION	IrpSp				= NULL;
	PFILE_OBJECT		pFileObject			= NULL;
	PDEVICE_OBJECT		pDeviceObject		= NULL;
	PKEVENT				pEventObject		= NULL;
	PCHAR				pAuxiliaryBuffer	= NULL;
	PMDL				pMdl				= NULL;
	BOOLEAN				bInterrupted		= FALSE;
	BOOLEAN				bFileObjLock		= FALSE;
	BOOLEAN				bSynchronousIo		= FALSE;
	IO_STATUS_BLOCK		IoStatus			= {0x00};
	ULONG				inputLength			= 0;

	UNREFERENCED_PARAMETER(IoStatusBlock);

	do 
	{
		if ( FileInformationClass == FileDirectoryInformation ) 
		{
			inputLength = sizeof(FILE_DIRECTORY_INFORMATION);
		}
		else if ( /*MinorFunction == IRP_MN_QUERY_DIRECTORY*/ TRUE ) 
		{
			switch (FileInformationClass)
			{
			case FileFullDirectoryInformation:
				inputLength = sizeof(FILE_FULL_DIR_INFORMATION);
				break;

			case FileIdFullDirectoryInformation:
				inputLength = sizeof(FILE_ID_FULL_DIR_INFORMATION);
				break;

			case FileBothDirectoryInformation:
				inputLength = sizeof(FILE_BOTH_DIR_INFORMATION);
				break;

			case FileIdBothDirectoryInformation:
				inputLength = sizeof(FILE_ID_BOTH_DIR_INFORMATION);
				break;

			case FileNamesInformation:
				inputLength = sizeof(FILE_NAMES_INFORMATION);
				break;

			case FileObjectIdInformation:
				inputLength = sizeof(FILE_OBJECTID_INFORMATION);
				break;

			case FileQuotaInformation:
				inputLength = sizeof(FILE_QUOTA_INFORMATION);
				break;

			case FileReparsePointInformation:
				inputLength = sizeof(FILE_REPARSE_POINT_INFORMATION);
				break;                    
			}
		}

		BDKit_If_Not_Break_With_Reason(inputLength != 0, STATUS_INVALID_INFO_CLASS);
		BDKit_If_Not_Break_With_Reason(Length >= inputLength, STATUS_INFO_LENGTH_MISMATCH);

		if ( FileName != NULL ) 
		{
			UNICODE_STRING	uszFileName	= {0x00};
			PUNICODE_STRING nameBuffer	= {0x00};

			uszFileName = *FileName;

			BDKit_If_Not_Break_With_Reason(uszFileName.Buffer != NULL && uszFileName.Length != 0, STATUS_INVALID_PARAMETER);

			BDKitAllocateNonpagePool(pAuxiliaryBuffer, uszFileName.Length + sizeof(UNICODE_STRING) + sizeof(WCHAR));
			BDKit_If_Not_Break_With_Reason(pAuxiliaryBuffer != NULL, STATUS_INSUFFICIENT_RESOURCES);

			RtlCopyMemory(pAuxiliaryBuffer + sizeof(UNICODE_STRING), uszFileName.Buffer, uszFileName.Length);

			nameBuffer                = (PUNICODE_STRING)pAuxiliaryBuffer;
			nameBuffer->Length        = uszFileName.Length;
			nameBuffer->MaximumLength = uszFileName.Length + sizeof(WCHAR);
			nameBuffer->Buffer        = (PWSTR)(pAuxiliaryBuffer + sizeof(UNICODE_STRING));
		}

		nsStatus = ObReferenceObjectByHandle( 
			FileHandle,
			FILE_LIST_DIRECTORY,
			*IoFileObjectType,
			kRequestorMode,
			(PVOID *)&pFileObject,
			(POBJECT_HANDLE_INFORMATION)NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));	

		BDKit_If_Not_Break_With_Reason(pFileObject->CompletionContext == NULL && ApcRoutine == NULL, STATUS_INVALID_PARAMETER);

		if (Event != NULL) 
		{
			nsStatus = ObReferenceObjectByHandle(
				Event,
				EVENT_MODIFY_STATE,
				*ExEventObjectType,
				kRequestorMode,
				(PVOID *)&pEventObject,
				(POBJECT_HANDLE_INFORMATION)NULL
				);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

			KeClearEvent(pEventObject);
		}

		if ( pFileObject->Flags & FO_SYNCHRONOUS_IO ) 
		{

			if (!BDKitAcquireFastLock(pFileObject)) 
			{
				nsStatus = BDKitAcquireFileObjectLock(pFileObject,
					kRequestorMode,
					(BOOLEAN) ((pFileObject->Flags & FO_ALERTABLE_IO) != 0),
					&bInterrupted
					);
				BDKit_If_Not_Break(bInterrupted == FALSE);
			}

			bFileObjLock = TRUE;
			bSynchronousIo = TRUE;
		}
		else 
		{
			bSynchronousIo = FALSE;
		}

		KeClearEvent(&pFileObject->Event);

		pDeviceObject = BDKitGetFileObjRelatedDeviceObject(pFileObject);

		Irp = IoAllocateIrp(pDeviceObject->StackSize, !bSynchronousIo);
		BDKit_If_Not_Break_With_Reason(Irp != NULL, STATUS_INSUFFICIENT_RESOURCES);

		Irp->Tail.Overlay.OriginalFileObject				= pFileObject;
		Irp->Tail.Overlay.Thread							= PsGetCurrentThread ();
		Irp->RequestorMode									= kRequestorMode;
		Irp->UserEvent										= pEventObject;
		Irp->UserIosb										= &IoStatus;
		Irp->Overlay.AsynchronousParameters.UserApcRoutine	= ApcRoutine;
		Irp->Overlay.AsynchronousParameters.UserApcContext	= ApcContext;
		Irp->Tail.Overlay.AuxiliaryBuffer					= pAuxiliaryBuffer;
		Irp->AssociatedIrp.SystemBuffer						= NULL;
		Irp->MdlAddress										= NULL;

		IrpSp = IoGetNextIrpStackLocation(Irp);
		IrpSp->Flags										= 0;
		IrpSp->MajorFunction								= IRP_MJ_DIRECTORY_CONTROL;
		IrpSp->MinorFunction								= IRP_MN_QUERY_DIRECTORY;
		IrpSp->FileObject									= pFileObject;

		if ( pDeviceObject->Flags & DO_BUFFERED_IO ) 
		{
			__try
			{
				BDKitAllocateNonPageQuotaPool(Irp->AssociatedIrp.SystemBuffer, Length);
				BDKit_If_Not_Break_With_Reason(Irp != NULL, STATUS_INSUFFICIENT_RESOURCES);

				Irp->UserBuffer = FileInformation;
				Irp->Flags      = (ULONG)(IRP_BUFFERED_IO | IRP_DEALLOCATE_BUFFER | IRP_INPUT_OPERATION);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				nsStatus = GetExceptionCode();
				break;
			}

		} else if (pDeviceObject->Flags & DO_DIRECT_IO) 
		{
			__try 
			{
				pMdl = (PMDL)NULL;
				pMdl = IoAllocateMdl(FileInformation, Length, FALSE, TRUE, Irp);
				BDKit_If_Not_Break_With_Reason(pMdl != NULL, STATUS_INSUFFICIENT_RESOURCES);
				MmProbeAndLockPages(pMdl, kRequestorMode, IoWriteAccess);
			}
			__except(EXCEPTION_EXECUTE_HANDLER) 
			{
				nsStatus = GetExceptionCode();
				break;
			}
		} 
		else
		{
			Irp->UserBuffer = FileInformation;
		}

		((PQueryDirectory)&IrpSp->Parameters)->Length               = Length;
		((PQueryDirectory)&IrpSp->Parameters)->FileInformationClass = FileInformationClass;
		((PQueryDirectory)&IrpSp->Parameters)->FileIndex            = 0;
		((PQueryDirectory)&IrpSp->Parameters)->FileName             = (PUNICODE_STRING)pAuxiliaryBuffer;

		if (RestartScan)
		{
			IrpSp->Flags = SL_RESTART_SCAN;
		}

		if (ReturnSingleEntry) 
		{
			IrpSp->Flags |= SL_RETURN_SINGLE_ENTRY;
		}

		Irp->Flags |= IRP_DEFER_IO_COMPLETION;

		return IopSynchronousServiceTail (
			pDeviceObject,
			Irp,
			pFileObject,
			TRUE,
			kRequestorMode,
			bSynchronousIo
			);	

	} while (FALSE);

	BDKitIoCompletionRoutine (NULL, Irp, NULL);

	if (pFileObject != NULL)
	{
		if (pFileObject->Flags & FO_SYNCHRONOUS_IO) 
		{
			if (bFileObjLock)
			{
				BDKitReleaseFileObjectLock(pFileObject);
			}
		}

		BDKitCloseObject(pFileObject);
	}

	BDKitCloseObject(pEventObject);
	BDKitFreePool(pAuxiliaryBuffer);

	return nsStatus;
}

NTSTATUS BDKitQueryDirectoryFile(							   
								 __in		HANDLE				FileHandle,
								 __in_opt	HANDLE				Event,
								 __in_opt	PIO_APC_ROUTINE		ApcRoutine,
								 __in_opt	PVOID				ApcContext,
								 __out		PIO_STATUS_BLOCK	IoStatusBlock,
								 __out		PVOID				FileInformation,
								 __in		ULONG				Length,
								 __in		FILE_INFORMATION_CLASS  FileInformationClass,
								 __in		BOOLEAN				ReturnSingleEntry,
								 __in_opt	PUNICODE_STRING		FileName,
								 __in		BOOLEAN				RestartScan
								 )
{
	return IrpQueryDirectoryFile (
		FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		FileInformation,
		Length,
		FileInformationClass,
		ReturnSingleEntry,
		FileName,
		RestartScan
		);
}

NTSTATUS BDKitQueryInformationFile(
								   __in PFILE_OBJECT	pFileObject,
								   __in PDEVICE_OBJECT	DeviceObject,
								   __out PVOID			FileInformation,
								   __in ULONG			Length,
								   __in FILE_INFORMATION_CLASS FileInformationClass
								   )
{
	return IrpQueryInformationFile (
		pFileObject,
		DeviceObject,
		FileInformation,
		Length,
		FileInformationClass
		);
}

NTSTATUS BDKitSetFileAttributes (
								 __in		HANDLE				FileHandle,
								 __in_opt	HANDLE				Event,
								 __in_opt	PIO_APC_ROUTINE		ApcRoutine,
								 __in_opt	PVOID				ApcContext,
								 __out		PIO_STATUS_BLOCK	IoStatusBlock,
								 __out		PVOID				FileInformation,
								 __in		ULONG				Length,
								 __in		FILE_INFORMATION_CLASS  FileInformationClass,
								 __in		BOOLEAN				ReturnSingleEntry,
								 __in_opt	PUNICODE_STRING		FileName,
								 __in		BOOLEAN				RestartScan
								 )
{
	NTSTATUS                nsStatus        = STATUS_UNSUCCESSFUL;  
	PFILE_BASIC_INFORMATION  pFileInformation	= (PFILE_BASIC_INFORMATION)FileInformation; 
	IO_STATUS_BLOCK         IoStatus        = {0x00};
	KEVENT                  kEvent			= {0x00};
	PIO_STACK_LOCATION      IrpSp			= NULL;
	PFILE_OBJECT            pFileObject		= NULL;  
	PIRP                    Irp				= NULL; 
	PDEVICE_OBJECT          pDeviceObject   = NULL;
	KPROCESSOR_MODE         requestorMode	= KernelMode;

	UNREFERENCED_PARAMETER(Event);
	UNREFERENCED_PARAMETER(ApcRoutine);
	UNREFERENCED_PARAMETER(ApcContext);
	UNREFERENCED_PARAMETER(IoStatusBlock);
	UNREFERENCED_PARAMETER(Length);
	UNREFERENCED_PARAMETER(ReturnSingleEntry);
	UNREFERENCED_PARAMETER(FileName);
	UNREFERENCED_PARAMETER(RestartScan);

	do 
	{
		nsStatus = ObReferenceObjectByHandle(
			FileHandle,
			IopOperationAccessRef[FileInformationClass],
			*IoFileObjectType,
			requestorMode,
			(PVOID*)&pFileObject,
			NULL
			); 
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		pDeviceObject = BDKitGetFileObjRelatedDeviceObject(pFileObject);
		BDKit_If_Not_Break(pDeviceObject != NULL);

		Irp = IoAllocateIrp(pDeviceObject->StackSize, TRUE);
		BDKit_If_Not_Break_With_Reason(Irp != NULL, STATUS_INSUFFICIENT_RESOURCES);

		KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

		pFileInformation->FileAttributes = FILE_ATTRIBUTE_NORMAL;  

		// 初始化IRP   
		Irp->AssociatedIrp.SystemBuffer                  = pFileInformation;   
		Irp->UserEvent                                   = &kEvent;   
		Irp->UserIosb                                    = &IoStatus;   
		Irp->Tail.Overlay.OriginalFileObject             = pFileObject;   
		Irp->Tail.Overlay.Thread                         = PsGetCurrentThread();
		Irp->RequestorMode                               = requestorMode;   

		//设置IRP堆栈信息   
		IrpSp = IoGetNextIrpStackLocation(Irp);   
		IrpSp->MajorFunction                             = IRP_MJ_SET_INFORMATION;   
		IrpSp->DeviceObject                              = pDeviceObject;   
		IrpSp->FileObject                                = pFileObject;

		IrpSp->Parameters.SetFile.Length                 = sizeof(FILE_BASIC_INFORMATION);   
		IrpSp->Parameters.SetFile.FileInformationClass   = FileBasicInformation;   
		IrpSp->Parameters.SetFile.FileObject             = pFileObject;

		IoSetCompletionRoutine (Irp, BDKitIoCompletionRoutine, NULL, TRUE, TRUE, TRUE);

		nsStatus = IoCallDriver(pDeviceObject, Irp);
		BDKit_If_DoAction(nsStatus == STATUS_PENDING, 
			KeWaitForSingleObject(&kEvent, Executive, KernelMode, TRUE, (PLARGE_INTEGER)NULL));

		nsStatus = IoStatus.Status;

	} while (FALSE);

	BDKitCloseObject(pFileObject);

	return nsStatus;
}

NTSTATUS BDKitClearFileReadOnlyAttribute (__in HANDLE hFile)
{
	NTSTATUS				nsStatus			= STATUS_UNSUCCESSFUL;
	IO_STATUS_BLOCK			IoStatus			= {0x00};
	FILE_BASIC_INFORMATION	basicInformation	= {0x00};

	do 
	{
		// This will instead of orig fun
		nsStatus = ZwQueryInformationFile (
			hFile,
			&IoStatus,
			&basicInformation,
			sizeof(basicInformation),
			FileBasicInformation
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		basicInformation.FileAttributes &= ~FILE_ATTRIBUTE_READONLY;
		nsStatus = ZwSetInformationFile (
			hFile,
			&IoStatus,
			&basicInformation,
			sizeof(basicInformation),
			FileBasicInformation
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	return nsStatus;
}

NTSTATUS BDKitClearFileUnlockAttribute(__in HANDLE hFile)
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	BOOLEAN				bInterrupted= FALSE;
	BOOLEAN				bFileObjLock= FALSE;
	PDEVICE_OBJECT		DeviceObject= NULL;
	PEPROCESS			EProcess	= NULL;
	PFAST_IO_DISPATCH	pFastIoDisp	= NULL;
	PFILE_OBJECT		FileObject	= NULL;
	IO_STATUS_BLOCK		IoStatus	= {0x00};
	KEVENT				kEvent		= {0x00};

	do 
	{
		nsStatus = ObReferenceObjectByHandle (
			hFile,
			GENERIC_READ,
			*IoFileObjectType,
			0,
			&FileObject,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		if ( FileObject->Flags & FO_DIRECT_DEVICE_OPEN )
		{
			DeviceObject = FileObject->DeviceObject;
		}
		else
		{
			DeviceObject = IoGetBaseFileSystemDeviceObject (FileObject);
		}

		if ( FileObject->Flags & FO_SYNCHRONOUS_IO )
		{
			bInterrupted = FALSE;
			if ( BDKitAcquireFastLock (FileObject) == FALSE )
			{
				nsStatus = BDKitAcquireFileObjectLock (
					FileObject,
					KernelMode,
					FALSE,
					&bInterrupted
					);
			}
			BDKit_If_Not_Break(bInterrupted == FALSE);

			bFileObjLock = TRUE;
		}

		pFastIoDisp = DeviceObject->DriverObject->FastIoDispatch;
		EProcess = IoGetCurrentProcess ();

		if (	pFastIoDisp != NULL
			&&	pFastIoDisp->FastIoUnlockAll != NULL)
		{
			if ( pFastIoDisp->FastIoUnlockAll(
				FileObject,
				EProcess,
				&IoStatus,
				DeviceObject
				) == FALSE 
				)
			{
				// FASTIO不成功自己发送IRP处理
				PIRP				Irp			= NULL;
				PIO_STACK_LOCATION	IrpSp		= NULL;

				KeInitializeEvent (&kEvent, SynchronizationEvent, FALSE);
				KeClearEvent (&FileObject->Event);

				Irp = IoAllocateIrp (DeviceObject->StackSize, FALSE);
				BDKit_If_Not_Break_With_Reason(Irp != NULL, STATUS_INSUFFICIENT_RESOURCES);

				Irp->Tail.Overlay.OriginalFileObject	= FileObject;
				Irp->Tail.Overlay.Thread				= PsGetCurrentThread();
				Irp->RequestorMode						= KernelMode;
				Irp->UserEvent							= &kEvent;
				Irp->UserIosb							= &IoStatus;
				Irp->Flags								= IRP_SYNCHRONOUS_API;
				Irp->Overlay.AsynchronousParameters.UserApcRoutine = NULL;

				IrpSp									= IoGetNextIrpStackLocation (Irp);
				IrpSp->MajorFunction					= IRP_MJ_LOCK_CONTROL;
				IrpSp->MinorFunction					= IRP_MN_UNLOCK_ALL;
				IrpSp->FileObject						= FileObject;

				ObReferenceObject(FileObject);
				IoEnqueueIrp (Irp);

				nsStatus = IoCallDriver(DeviceObject, Irp);
				BDKit_If_DoAction(nsStatus == STATUS_PENDING,
					KeWaitForSingleObject (&kEvent, Executive, KernelMode, FALSE, NULL));
			}

			nsStatus = IoStatus.Status;

		}
	} while (FALSE);

	BDKit_If_DoAction((FileObject->Flags & FO_SYNCHRONOUS_IO) && bFileObjLock == TRUE,
		BDKitReleaseFileObjectLock (FileObject));

	BDKitCloseObject(FileObject);

	return nsStatus;
}

NTSTATUS BDKitGetFileSizeByObject(
								  __in PFILE_OBJECT pFileObject, 
								  __out PLARGE_INTEGER pFileSize
								  )
{
	NTSTATUS					nsStatus					= STATUS_UNSUCCESSFUL;
	FILE_STANDARD_INFORMATION	stFileStandardInformation	= {0x00};
	PDEVICE_OBJECT				DeviceObject				= NULL;
	PDEVICE_OBJECT				RealDevice					= NULL;

	do 
	{
		nsStatus = BDKitGetFileSystemVpbInfo(
			pFileObject,
			&DeviceObject,
			&RealDevice
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = BDKitQueryInformationFile(
			pFileObject,
			DeviceObject,
			&stFileStandardInformation,
			sizeof(FILE_STANDARD_INFORMATION),
			FileStandardInformation
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		pFileSize->HighPart	= stFileStandardInformation.EndOfFile.HighPart;
		pFileSize->LowPart	= stFileStandardInformation.EndOfFile.LowPart;

	} while (FALSE);

	return nsStatus;
}

NTSTATUS BDKitGetFileSize(
						  __in  HANDLE			hFile, 
						  __out PLARGE_INTEGER	FileSize
						  )
{
	NTSTATUS					nsStatus	= STATUS_UNSUCCESSFUL;
	PFILE_OBJECT				pFileObject	= NULL;

	do 
	{
		nsStatus=ObReferenceObjectByHandle(
			hFile, 
			0,			// 需要查下
			*IoFileObjectType,
			KernelMode, 
			&pFileObject, 
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = BDKitGetFileSizeByObject(pFileObject, FileSize);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKitCloseObject(pFileObject);

	return nsStatus;
}