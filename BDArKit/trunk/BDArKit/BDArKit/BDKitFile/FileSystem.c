#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "BDArKit\KitHelper.h"
#include "DriverHelper.h"
#include "Utility.h"
#include "BDKitUtils.h"


#if (VER_PRODUCTBUILD < 7600)
#define NT_ERROR(Status)                    ((ULONG)(Status) >> 30 == 3)
#define IO_REPARSE_TAG_MOUNT_POINT          0xA0000003L     
#endif

#define IRP_RETRY_IO_COMPLETION             0x00004000L
#define FILE_DEFAULT_ACCESS					0x00000000L

#define SynchronousIo(Irp, FileObject)      (  \
	(Irp->Flags & IRP_SYNCHRONOUS_API) ||		\
	(FileObject == NULL ? 0 : FileObject->Flags & FO_SYNCHRONOUS_IO))

#define IopDequeueThreadIrp(Irp)				\
{												\
	RemoveEntryList(&Irp->ThreadListEntry);		\
	InitializeListHead(&Irp->ThreadListEntry) ; \
}
extern CONST ULONG IopOperationAccessRef[] =
{
	0,
	0,                     //  1 FileDirectoryInformation
	0,                     //  2 FileFullDirectoryInformation
	0,                     //  3 FileBothDirectoryInformation
	FILE_WRITE_ATTRIBUTES, //  4 FileBasicInformation
	0,                     //  5 FileStandardInformation
	0,                     //  6 FileInternalInformation
	0,                     //  7 FileEaInformation
	0,                     //  8 FileAccessInformation
	0,                     //  9 FileNameInformation
	DELETE,                // 10 FileRenameInformation
	0,                     // 11 FileLinkInformation
	0,                     // 12 FileNamesInformation
	DELETE,                // 13 FileDispositionInformation
	0,                     // 14 FilePositionInformation
	FILE_WRITE_EA,         // 15 FileFullEaInformation
	0,                     // 16 FileModeInformation
	0,                     // 17 FileAlignmentInformation
	0,                     // 18 FileAllInformation
	FILE_WRITE_DATA,       // 19 FileAllocationInformation
	FILE_WRITE_DATA,       // 20 FileEndOfFileInformation
	0,                     // 21 FileAlternateNameInformation
	0,                     // 22 FileStreamInformation
	FILE_WRITE_ATTRIBUTES, // 23 FilePipeInformation
	0,                     // 24 FilePipeLocalInformation
	FILE_WRITE_ATTRIBUTES, // 25 FilePipeRemoteInformation
	0,                     // 26 FileMailslotQueryInformation
	0,                     // 27 FileMailslotSetInformation
	0,                     // 28 FileCompressionInformation
	0,                     // 29 FileObjectIdInformation
	0,                     // 30 FileCompletionInformation
	FILE_WRITE_DATA,       // 31 FileMoveClusterInformation
	0,                     // 32 FileQuotaInformation
	0,                     // 33 FileReparsePointInformation
	0,                     // 34 FileNetworkOpenInformation
	0,                     // 35 FileAttributeTagInformation
	FILE_WRITE_DATA,       // 36 FileTrackingInformation
	0,                     // 37 FileIdBothDiretoryInformation
	0,                     // 38 FileIdFullDiretoryInformation
	FILE_WRITE_DATA,       // 39 FileValidDataLengthInformation
	DELETE,                // 40 FileShortNameInformation
	0xffffffff             //    FileMaximumInformation
};


CONST UCHAR IopQueryOperationLength[] =
{
	0,
	0,                                         //  1 FileDirectoryInformation
	0,                                         //  2 FileFullDirectoryInformation
	0,                                         //  3 FileBothDirectoryInformation
	sizeof( FILE_BASIC_INFORMATION ),          //  4 FileBasicInformation
	sizeof( FILE_STANDARD_INFORMATION ),       //  5 FileStandardInformation
	sizeof( FILE_INTERNAL_INFORMATION ),       //  6 FileInternalInformation
	sizeof( FILE_EA_INFORMATION ),             //  7 FileEaInformation
	sizeof( FILE_ACCESS_INFORMATION ),         //  8 FileAccessInformation
	sizeof( FILE_NAME_INFORMATION ),           //  9 FileNameInformation
	sizeof( FILE_RENAME_INFORMATION ),         // 10 FileRenameInformation
	sizeof( FILE_LINK_INFORMATION ),           // 11 FileLinkInformation
	0,                                         // 12 FileNamesInformation
	sizeof( FILE_DISPOSITION_INFORMATION ),    // 13 FileDispositionInformation
	sizeof( FILE_POSITION_INFORMATION ),       // 14 FilePositionInformation
	0,                                         // 15 FileFullEaInformation
	sizeof( FILE_MODE_INFORMATION ),           // 16 FileModeInformation
	sizeof( FILE_ALIGNMENT_INFORMATION ),      // 17 FileAlignmentInformation
	sizeof( FILE_ALL_INFORMATION ),            // 18 FileAllInformation
	sizeof( FILE_ALLOCATION_INFORMATION ),     // 19 FileAllocationInformation
	sizeof( FILE_END_OF_FILE_INFORMATION ),    // 20 FileEndOfFileInformation
	sizeof( FILE_NAME_INFORMATION ),           // 21 FileAlternateNameInformation
	sizeof( FILE_STREAM_INFORMATION ),         // 22 FileStreamInformation
	sizeof( FILE_PIPE_INFORMATION ),           // 23 FilePipeInformation
	sizeof( FILE_PIPE_LOCAL_INFORMATION ),     // 24 FilePipeLocalInformation
	sizeof( FILE_PIPE_REMOTE_INFORMATION ),    // 25 FilePipeRemoteInformation
	sizeof( FILE_MAILSLOT_QUERY_INFORMATION ), // 26 FileMailslotQueryInformation
	0,                                         // 27 FileMailslotSetInformation
	sizeof( FILE_COMPRESSION_INFORMATION ),    // 28 FileCompressionInformation
	sizeof( FILE_OBJECTID_INFORMATION ),       // 29 FileObjectIdInformation
	0,                                         // 30 FileCompletionInformation
	0,                                         // 31 FileMoveClusterInformation
	sizeof( FILE_QUOTA_INFORMATION ),          // 32 FileQuotaInformation
	sizeof( FILE_REPARSE_POINT_INFORMATION ),  // 33 FileReparsePointInformation
	sizeof( FILE_NETWORK_OPEN_INFORMATION),    // 34 FileNetworkOpenInformation
	sizeof( FILE_ATTRIBUTE_TAG_INFORMATION),   // 35 FileAttributeTagInformation
	0,                                         // 36 FileTrackingInformation
	0,                                         // 37 FileIdBothDiretoryInformation
	0,                                         // 38 FileIdFullDiretoryInformation
	0,                                         // 39 FileValidDataLengthInformation
	0,                                         // 40 FileShortNameInformation
	0xff                                       //    FileMaximumInformation
};

CONST ULONG IopSetOperationAccess[] =
{
	FILE_DEFAULT_ACCESS,
	FILE_DEFAULT_ACCESS,						//  1 FileDirectoryInformation
	FILE_DEFAULT_ACCESS,						//  2 FileFullDirectoryInformation
	FILE_DEFAULT_ACCESS,						//  3 FileBothDirectoryInformation
	FILE_WRITE_ATTRIBUTES,						//  4 FileBasicInformation
	FILE_DEFAULT_ACCESS,						//  5 FileStandardInformation
	FILE_DEFAULT_ACCESS,						//  6 FileInternalInformation
	FILE_DEFAULT_ACCESS,						//  7 FileEaInformation
	FILE_DEFAULT_ACCESS,						//  8 FileAccessInformation
	FILE_DEFAULT_ACCESS,						//  9 FileNameInformation
	DELETE,										// 10 FileRenameInformation
	FILE_DEFAULT_ACCESS,						// 11 FileLinkInformation
	FILE_DEFAULT_ACCESS,						// 12 FileNamesInformation
	DELETE,										// 13 FileDispositionInformation
	FILE_DEFAULT_ACCESS,						// 14 FilePositionInformation
	FILE_WRITE_EA,								// 15 FileFullEaInformation
	FILE_DEFAULT_ACCESS,						// 16 FileModeInformation
	FILE_DEFAULT_ACCESS,						// 17 FileAlignmentInformation
	FILE_DEFAULT_ACCESS,						// 18 FileAllInformation
	FILE_WRITE_DATA,							// 19 FileAllocationInformation
	FILE_WRITE_DATA,							// 20 FileEndOfFileInformation
	FILE_DEFAULT_ACCESS,						// 21 FileAlternateNameInformation
	FILE_DEFAULT_ACCESS,						// 22 FileStreamInformation
	FILE_WRITE_ATTRIBUTES,						// 23 FilePipeInformation
	FILE_DEFAULT_ACCESS,						// 24 FilePipeLocalInformation
	FILE_WRITE_ATTRIBUTES,						// 25 FilePipeRemoteInformation
	FILE_DEFAULT_ACCESS,						// 26 FileMailslotQueryInformation
	FILE_DEFAULT_ACCESS,						// 27 FileMailslotSetInformation
	FILE_DEFAULT_ACCESS,						// 28 FileCompressionInformation
	FILE_DEFAULT_ACCESS,						// 29 FileObjectIdInformation
	FILE_DEFAULT_ACCESS,						// 30 FileCompletionInformation
	FILE_WRITE_DATA,							// 31 FileMoveClusterInformation
	FILE_DEFAULT_ACCESS,						// 32 FileQuotaInformation
	FILE_DEFAULT_ACCESS,						// 33 FileReparsePointInformation
	FILE_DEFAULT_ACCESS,						// 34 FileNetworkOpenInformation
	FILE_DEFAULT_ACCESS,						// 35 FileAttributeTagInformation
	FILE_WRITE_DATA,							// 36 FileTrackingInformation
	FILE_DEFAULT_ACCESS,						// 37 FileIdBothDiretoryInformation
	FILE_DEFAULT_ACCESS,						// 38 FileIdFullDiretoryInformation
	FILE_WRITE_DATA,							// 39 FileValidDataLengthInformation
	DELETE,										// 40 FileShortNameInformation
	0xffffffff									//    FileMaximumInformation
};

typedef struct _SYSTEM_HANDLE_ARRAY
{
	ULONG						uCount;
	SYSTEM_HANDLE_INFORMATION   HandleArray[1];
} SYSTEM_HANDLE_ARRAY, *PSYSTEM_HANDLE_ARRAY;
//////////////////////////////////////////////////////////////////////////

BOOLEAN BDKitAcquireFastLock(__in PFILE_OBJECT pFileObject)
{
	if ( InterlockedExchange((PLONG)&pFileObject->Busy, (ULONG)TRUE) == FALSE )
	{
		ObReferenceObject(pFileObject);
		return TRUE;
	}

	return FALSE;
}

VOID BDKitReleaseFileObjectLock(__in PFILE_OBJECT pFileObject)
{
	ULONG	result	= 0;

	result = InterlockedExchange((PLONG)&pFileObject->Busy, FALSE);
	ASSERT(result != FALSE);

	if ( pFileObject->Waiters != 0 )
	{
		KeSetEvent (&pFileObject->Lock, IO_NO_INCREMENT, FALSE);
	}

	BDKitCloseObject(pFileObject);
}

NTSTATUS BDKitAcquireFileObjectLock(
									__in PFILE_OBJECT		pFileObject,
									__in KPROCESSOR_MODE	RequestorMode,
									__in BOOLEAN			Alertable,
									__out PBOOLEAN			Interrupted
									)
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;

	do 
	{
		*Interrupted = FALSE;

		InterlockedIncrement((LONG*)&pFileObject->Waiters);

		for (;;)
		{
			if ( pFileObject->Busy == FALSE )
			{
				if ( BDKitAcquireFastLock (pFileObject) == TRUE )
				{
					InterlockedDecrement((LONG*)&pFileObject->Waiters);
					nsStatus = STATUS_SUCCESS;
					BDKit_If_Not_Break(FALSE);
				}
			}

			nsStatus = KeWaitForSingleObject (
				&pFileObject->Lock,
				Executive,
				RequestorMode,
				Alertable,
				NULL
				);
			if ( nsStatus == STATUS_USER_APC || nsStatus == STATUS_ALERTED )
			{
				InterlockedDecrement((LONG*)&pFileObject->Waiters);

				if ( pFileObject->Busy == FALSE && pFileObject->Waiters != 0 )
				{
					KeSetEvent (&pFileObject->Lock, IO_NO_INCREMENT, FALSE);
				}

				*Interrupted = TRUE;
				BDKit_If_Not_Break(FALSE);
			}
		}
	} while (FALSE);

	return nsStatus;
}

NTSTATUS BDKitIoCompletionRoutine(
								  __in PDEVICE_OBJECT	DeviceObject,
								  __in PIRP				Irp,
								  __in PVOID			Context
								  )
{
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Context);

	if ( Irp != NULL )
	{
		*Irp->UserIosb = Irp->IoStatus;

		if ( Irp->UserEvent != NULL )
		{
			KeSetEvent(Irp->UserEvent, IO_NO_INCREMENT, FALSE);
		}

		if ( Irp->MdlAddress != NULL )
		{
			IoFreeMdl(Irp->MdlAddress);
			Irp->MdlAddress = NULL;
		}

		IoFreeIrp(Irp);
	}

	return STATUS_MORE_PROCESSING_REQUIRED;
}

VOID BDKitIopCompleteRequest(
							 __in PKAPC Apc,
							 __in PKNORMAL_ROUTINE *NormalRoutine,
							 __in PVOID *NormalContext,
							 __in PVOID *SystemArgument1,
							 __in PVOID *SystemArgument2
							 )
{

	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	PIRP			Irp			= NULL;
	PMDL			pMdl		= NULL;
	PMDL			pNextMdl	= NULL;
	PETHREAD		Thread		= NULL;
	PFILE_OBJECT	pFileObject = NULL;

	UNREFERENCED_PARAMETER( NormalRoutine );
	UNREFERENCED_PARAMETER( NormalContext );

	Irp = CONTAINING_RECORD(Apc, IRP, Tail.Apc);
	Thread = PsGetCurrentThread();
	pFileObject = (PFILE_OBJECT) *SystemArgument1;

	ASSERT(Irp->IoStatus.Status != 0xffffffff);

	if ( *SystemArgument2 != NULL )
	{
		if (
			(Irp->IoStatus.Status == STATUS_REPARSE)
			&& (Irp->IoStatus.Information == IO_REPARSE_TAG_MOUNT_POINT)
			)
		{
			return ;
		}
	}

	if (Irp->Flags & IRP_BUFFERED_IO) 
	{
		if (
				Irp->Flags & IRP_INPUT_OPERATION  
			&&	Irp->IoStatus.Status != STATUS_VERIFY_REQUIRED 
			&&	!NT_ERROR( Irp->IoStatus.Status)
			) 
		{
			nsStatus = STATUS_SUCCESS;

			__try 
			{
				ProbeForWrite(Irp->UserBuffer, (ULONG)Irp->IoStatus.Information, 4L);
				RtlCopyMemory( Irp->UserBuffer, Irp->AssociatedIrp.SystemBuffer, Irp->IoStatus.Information);
			} 
			__except(EXCEPTION_EXECUTE_HANDLER) 
			{
				if (nsStatus == STATUS_MULTIPLE_FAULT_VIOLATION) 
				{
					Irp->Tail.Overlay.OriginalFileObject = pFileObject;
					Irp->Flags |= IRP_RETRY_IO_COMPLETION;
					return;
				}

				Irp->IoStatus.Status = GetExceptionCode();
			}
		}

		BDKit_If_DoAction(Irp->Flags & IRP_DEALLOCATE_BUFFER, BDKitFreePool(Irp->AssociatedIrp.SystemBuffer));
	}

	Irp->Flags &= ~(IRP_DEALLOCATE_BUFFER | IRP_BUFFERED_IO);

	if ( Irp->MdlAddress != NULL ) 
	{
		for (pMdl = Irp->MdlAddress; pMdl != NULL; pMdl = pNextMdl) 
		{
			pNextMdl = pMdl->Next;
			IoFreeMdl(pMdl);
		}
	}

	Irp->MdlAddress = NULL;

	if (
			!NT_ERROR(Irp->IoStatus.Status) 
		||	(NT_ERROR(Irp->IoStatus.Status)
		&&	Irp->PendingReturned 
		&&	!SynchronousIo(Irp, pFileObject))
		) 
	{

		PVOID	Port				= NULL;
		PVOID	Key					= NULL;
		BOOLEAN bCreateOperation	= FALSE;

		if (pFileObject && pFileObject->CompletionContext) 
		{
			Port = pFileObject->CompletionContext->Port;
			Key  = pFileObject->CompletionContext->Key;
		}

		nsStatus = STATUS_SUCCESS;

		__try 
		{

			Irp->UserIosb->Information = Irp->IoStatus.Information;
			KeMemoryBarrierWithoutFence();
			*((volatile NTSTATUS *) &Irp->UserIosb->Status) = Irp->IoStatus.Status;

		} 
		__except(EXCEPTION_EXECUTE_HANDLER) 
		{
			if (nsStatus == STATUS_MULTIPLE_FAULT_VIOLATION) 
			{
				Irp->Tail.Overlay.OriginalFileObject = pFileObject;
				Irp->Flags |= IRP_RETRY_IO_COMPLETION;
				return;
			}
		}

		if (Irp->UserEvent) 
		{
			KeSetEvent(Irp->UserEvent, 0, FALSE);

			if (pFileObject) 
			{
				if (!(Irp->Flags & IRP_SYNCHRONOUS_API)) 
				{
					ObDereferenceObject(Irp->UserEvent);
				}

				if (pFileObject->Flags & FO_SYNCHRONOUS_IO && !(Irp->Flags & IRP_OB_QUERY_NAME)) 
				{
					KeSetEvent(&pFileObject->Event, 0, FALSE);
					pFileObject->FinalStatus = Irp->IoStatus.Status;
				}

				if (Irp->Flags & IRP_CREATE_OPERATION)
				{
					bCreateOperation = TRUE;
					Irp->Overlay.AsynchronousParameters.UserApcRoutine = (PIO_APC_ROUTINE)NULL;
				}
			}

		} 
		else if (pFileObject) 
		{
			KeSetEvent(&pFileObject->Event, 0, FALSE);

			pFileObject->FinalStatus = Irp->IoStatus.Status;

			if (Irp->Flags & IRP_CREATE_OPERATION) 
			{
				bCreateOperation = TRUE;
				Irp->Overlay.AsynchronousParameters.UserApcRoutine = (PIO_APC_ROUTINE)NULL;
			}
		}

		IopDequeueThreadIrp(Irp);
		IoFreeIrp(Irp);

		//解除引用
		if (pFileObject && !bCreateOperation) 
		{
			BDKitCloseObject(pFileObject);	
		}
	} 
	else 
	{
		if (Irp->PendingReturned && pFileObject) 
		{

			if (Irp->Flags & IRP_SYNCHRONOUS_API) 
			{
				*Irp->UserIosb = Irp->IoStatus;
				if (Irp->UserEvent)
				{
					KeSetEvent(Irp->UserEvent, 0, FALSE);
				} 
				else 
				{
					KeSetEvent(&pFileObject->Event, 0, FALSE);
				}
			} 
			else 
			{
				pFileObject->FinalStatus = Irp->IoStatus.Status;
				KeSetEvent(&pFileObject->Event, 0, FALSE);
			}
		}

		if (pFileObject != NULL && !(Irp->Flags & IRP_CREATE_OPERATION)) 
		{
			ObDereferenceObject(pFileObject);	
		}

		if (Irp->UserEvent && pFileObject && !(Irp->Flags & IRP_SYNCHRONOUS_API))
		{
			BDKitCloseObject(Irp->UserEvent);
		}

		IopDequeueThreadIrp(Irp);
		IoFreeIrp(Irp);
	}
}

PDEVICE_OBJECT BDKitGetFileObjRelatedDeviceObject(__in CONST PFILE_OBJECT FileObject)
{

	PDEVICE_OBJECT DeviceObject = NULL;

	if ( FileObject->Vpb != NULL && FileObject->Vpb->DeviceObject != NULL ) 
	{
		DeviceObject = FileObject->Vpb->DeviceObject;

	} 
	else if (
		!(FileObject->Flags & FO_DIRECT_DEVICE_OPEN) 
		&&	FileObject->DeviceObject->Vpb != NULL 
		&&	FileObject->DeviceObject->Vpb->DeviceObject != NULL
		) 
	{
		DeviceObject = FileObject->DeviceObject->Vpb->DeviceObject;
	} 
	else 
	{
		DeviceObject = FileObject->DeviceObject;
	}

	return DeviceObject;
}

NTSTATUS BDKitGetFileSystemVpbInfo(
								   __in PFILE_OBJECT		FileObject,
								   __out PDEVICE_OBJECT*	DeviceObject,
								   __out PDEVICE_OBJECT*	RealDevice
								   )
{
	if ( FileObject == NULL || DeviceObject == NULL || RealDevice == NULL )
	{
		return STATUS_UNSUCCESSFUL;
	}

	*DeviceObject	= NULL;
	*RealDevice		= NULL;

	if(FileObject->Vpb != NULL && FileObject->Vpb->DeviceObject != NULL)
	{
		*DeviceObject	= FileObject->Vpb->DeviceObject;
		*RealDevice		= FileObject->Vpb->RealDevice;
	}
	else if (
		!(FileObject->Flags & FO_DIRECT_DEVICE_OPEN)
		&&	FileObject->DeviceObject->Vpb != NULL
		&&	FileObject->DeviceObject->Vpb->DeviceObject != NULL
		)
	{
		*DeviceObject	= FileObject->DeviceObject->Vpb->DeviceObject;
		*RealDevice		= FileObject->DeviceObject->Vpb->RealDevice;
	}
	else
	{
		*DeviceObject	= FileObject->DeviceObject;
		*RealDevice		=NULL;
	}

	if ( *RealDevice == NULL || *DeviceObject == NULL )
	{
		return STATUS_UNSUCCESSFUL;
	}

	return (*RealDevice == NULL || *DeviceObject == NULL)? 
STATUS_UNSUCCESSFUL : STATUS_SUCCESS;
}

NTSTATUS BDKitGetDeviceObjectFromFileFullName(
	__in WCHAR*			FileFullName,
	__out PDEVICE_OBJECT*	RealDevice, 
	__out PDEVICE_OBJECT*	DeviceObject
	)
{
	NTSTATUS			nsStatus		= STATUS_UNSUCCESSFUL;
	WCHAR				wRootName[]		= L"\\DosDevices\\*:\\";
	UNICODE_STRING		RootName		= {0x00};
	OBJECT_ATTRIBUTES	ObjectAttributes= {0x00};
	IO_STATUS_BLOCK		IoStatus		= {0x00};
	HANDLE				hFile			= NULL;
	PFILE_OBJECT		pFileObject		= NULL;

	do 
	{
		wRootName[12] = FileFullName[0];
		RtlInitUnicodeString(&RootName,wRootName);

		InitializeObjectAttributes(
			&ObjectAttributes, 
			&RootName,
			OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 
			NULL, 
			NULL);

		nsStatus = IoCreateFile(
			&hFile,
			SYNCHRONIZE,
			&ObjectAttributes,
			&IoStatus,
			0,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			FILE_OPEN,
			FILE_DIRECTORY_FILE|FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			0,
			CreateFileTypeNone,
			NULL,
			IO_NO_PARAMETER_CHECKING);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus=ObReferenceObjectByHandle(
			hFile,
			1,	//不明白代表什么,先暂时写1
			*IoFileObjectType,
			KernelMode,
			&pFileObject,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = BDKitGetFileSystemVpbInfo(
			pFileObject, 
			DeviceObject, 
			RealDevice
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
	} while (FALSE);

	BDKitCloseObject(pFileObject);
	BDKitCloseHandle(hFile);

	return nsStatus;

}

/* 输入的路径格式为 \\??\\*:\\*/
NTSTATUS BDKitExectiveFileHandle (
								  __in CONST PUNICODE_STRING puszFileFullPath,
								  __in FILE_HANDLE_HANDLER pfnHandler,
								   __in PVOID pContext
								   )
{
	NTSTATUS					nsStatus			= STATUS_UNSUCCESSFUL;
	IO_STATUS_BLOCK				IoStatus			= {0x00};
	OBJECT_ATTRIBUTES			oa					= {0x00};
	UNICODE_STRING				uszVolume			= {0x00};
	ULONG						ulReturnLen			= 0;
	ULONG						ulIndex				= 0;
	UCHAR						ucTypeNumber		= 0;
	PSYSTEM_HANDLE_ARRAY		pSysHandleArray		= NULL;
	PUNICODE_STRING				puszFilePath		= NULL;
	HANDLE						hFileVolume			= NULL;
	HANDLE						hDupHandle			= NULL;
	PFILE_OBJECT				pVolumeHandleObj	= NULL;
	PFILE_OBJECT				pQueryFileObj		= NULL;
	PFILE_OBJECT				pTargetFileObj		= NULL;
	POBJECT_NAME_INFORMATION	pVolNameInfo		= NULL;
	POBJECT_NAME_INFORMATION	pObjectPath			= NULL;
	PSYSTEM_HANDLE_INFORMATION	pSysHandleEntry		= NULL;
	WCHAR						wszVolumeName[]		= L"\\??\\*:\\";
	BOOLEAN						bUseDupObject		= TRUE;

	do 
	{
		BDKit_If_Not_Break(puszFileFullPath != NULL && puszFileFullPath->Buffer != NULL);

		wszVolumeName[4] = puszFileFullPath->Buffer[4];
		RtlInitUnicodeString(&uszVolume, wszVolumeName);

		InitializeObjectAttributes(&oa, &uszVolume, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
		nsStatus = IoCreateFile(
			&hFileVolume,
			SYNCHRONIZE,
			&oa,
			&IoStatus,
			NULL,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_OPEN,
			FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
			NULL,
			0,
			CreateFileTypeNone,
			NULL,
			IO_NO_PARAMETER_CHECKING
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ObReferenceObjectByHandle (hFileVolume, DELETE, *IoFileObjectType, KernelMode, &pVolumeHandleObj, NULL);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		if ( TRUE )
		{
			HANDLE hTargetFile = NULL;

			InitializeObjectAttributes(&oa, puszFileFullPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
			nsStatus = BDKitCreateFileByObject (
				&hTargetFile,
				SYNCHRONIZE,
				&oa,
				&IoStatus,
				NULL,
				FILE_ATTRIBUTE_NORMAL,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				FILE_OPEN,
				FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
				NULL,
				0,
				CreateFileTypeNone,
				NULL,
				IO_NO_PARAMETER_CHECKING | IO_CHECK_CREATE_PARAMETERS,
				IOP_CREATE_IGNORE_SHARE_ACCESS_CHECK,
				NULL
				);
			if ( NT_SUCCESS(nsStatus) && hTargetFile != NULL)
			{
				nsStatus = ObReferenceObjectByHandle (hTargetFile, SYNCHRONIZE, *IoFileObjectType, KernelMode, &pTargetFileObj, NULL);
				BDKit_If_DoAction(NT_SUCCESS(nsStatus) && pTargetFileObj->FsContext != NULL, bUseDupObject = FALSE);
			}

			BDKitCloseHandle(hTargetFile);
		}

		if ( bUseDupObject == TRUE )
		{
			nsStatus = BDKitObQueryNameString (pVolumeHandleObj, &pVolNameInfo);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

			BDKitAllocateNonpagePool(puszFilePath, puszFileFullPath->Length + pVolNameInfo->Name.Length + sizeof(UNICODE_STRING));
			BDKit_If_Not_Break(puszFilePath != NULL);

			puszFilePath->Buffer = (PWCHAR)((PUCHAR)puszFilePath + sizeof(UNICODE_STRING));
			puszFilePath->Length = 0;
			puszFilePath->MaximumLength = puszFileFullPath->Length + pVolNameInfo->Name.Length;

			RtlCopyUnicodeString (puszFilePath, &pVolNameInfo->Name);
			RtlCopyMemory(
				puszFilePath->Buffer + wcslen (puszFilePath->Buffer), 
				puszFileFullPath->Buffer + wcslen (wszVolumeName),
				puszFileFullPath->Length - wcslen (wszVolumeName) * sizeof(WCHAR)
				);
			puszFilePath->Length += (USHORT)(puszFileFullPath->Length - wcslen (wszVolumeName) * sizeof(WCHAR));

			BDKitAllocateNonpagePool(pObjectPath, MAX_OBJECT_PATH_LENGTH);
			BDKit_If_Not_Break_With_Reason(pObjectPath != NULL, STATUS_INSUFFICIENT_RESOURCES);
			pObjectPath->Name.MaximumLength = MAX_OBJECT_PATH_LENGTH;
		}

		nsStatus = BDKitGetSystemInfo (SystemHandleInformation, &pSysHandleArray, &ulReturnLen);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		for (ulIndex = 0; ulIndex < pSysHandleArray->uCount; ++ulIndex)
		{
			pSysHandleEntry = &pSysHandleArray->HandleArray[ulIndex];
			if ( pSysHandleEntry->Object == pVolumeHandleObj )
			{
				ucTypeNumber = pSysHandleEntry->ObjectTypeNumber;
				break;
			}
		}
		BDKit_If_Not_Break(ucTypeNumber != 0);

		for (ulIndex = 0; ulIndex < pSysHandleArray->uCount; ++ulIndex)
		{
			pSysHandleEntry = &pSysHandleArray->HandleArray[ulIndex];
			BDKit_If_DoAction(
				pSysHandleEntry->ObjectTypeNumber != ucTypeNumber 
				|| pSysHandleEntry->Handle == 0,
				continue);

			do 
			{
				__try
				{
					if ( bUseDupObject == TRUE )
					{
						do 
						{
							nsStatus = BDKitDuplicateHandle (
								(HANDLE)pSysHandleEntry->ProcessId,
								PsGetCurrentProcessId(),
								(HANDLE)pSysHandleEntry->Handle,
								&hDupHandle
								);
							BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

							nsStatus = ObReferenceObjectByHandle (
								hDupHandle,
								GENERIC_READ,
								*IoFileObjectType,
								KernelMode,
								&pQueryFileObj,
								NULL
								);
							BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

							BDKit_If_Not_Break(MmIsAddressValid (pQueryFileObj));
							BDKit_If_Not_Break(pQueryFileObj->DeviceObject->DeviceType == IO_TYPE_MASTER_ADAPTER);

							RtlZeroMemory(pObjectPath, MAX_OBJECT_PATH_LENGTH);
							nsStatus = ObQueryNameString (
								pQueryFileObj, 
								pObjectPath, 
								MAX_OBJECT_PATH_LENGTH,
								&ulReturnLen
								);
							BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
						} while (FALSE);

						if ( !NT_SUCCESS(nsStatus) )
						{
							// 复制句柄的方式不成功,则改用ObjQuery的方式,暂未实现
							nsStatus = BDKitGetFileNameByFileObj (
								(PFILE_OBJECT)pSysHandleEntry->Object, 
								pObjectPath,
								MAX_OBJECT_PATH_LENGTH,
								&ulReturnLen
								);
							BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
						}
					}

					if ( 
						bUseDupObject == TRUE? 
						RtlEqualUnicodeString (puszFilePath, &pObjectPath->Name, TRUE) == TRUE :
					((PFILE_OBJECT)pSysHandleEntry->Object)->FsContext == pTargetFileObj->FsContext
						)
					{
						pfnHandler(pSysHandleEntry, pContext);
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					// TODO
					nsStatus = GetExceptionCode();
				}

			} while (FALSE);

			//BDKitCloseObject(pTargetFileObj;
			BDKitCloseObject(pQueryFileObj);
			BDKitCloseHandle(hDupHandle);
		}

	} while (FALSE);

	BDKitFreePool(pVolNameInfo);
	BDKitFreePool(pObjectPath);
	BDKitFreePool(puszFilePath);
	BDKitFreePool(pSysHandleArray);
	BDKitCloseObject(pTargetFileObj);
	BDKitCloseObject(pVolumeHandleObj);
	BDKitCloseHandle(hFileVolume);
	BDKitCloseHandle(hDupHandle);

	return STATUS_SUCCESS;
}