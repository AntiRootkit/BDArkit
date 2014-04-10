#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "Utility.h"
#include "BDKitUtils.h"


typedef
NTSTATUS (*__pfnMmCreateSection)(   
								 __out PVOID               *SectionObject,
								 __in ACCESS_MASK          DesiredAccess,
								 __in POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
								 __in PLARGE_INTEGER       MaximumSize,
								 __in ULONG                SectionPageProtection,
								 __in ULONG                AllocationAttributes,
								 __in HANDLE               FileHandle OPTIONAL,
								 __in PFILE_OBJECT         FileObject OPTIONAL
								 );
static __pfnMmCreateSection pfnMmCreateSection = NULL;

typedef
NTSTATUS (*__pfnMmMapViewOfSection)(
									__in PVOID                SectionObject,
									__in PEPROCESS            Process,
									__in __out PVOID          *BaseAddress,
									__in ULONG                ZeroBits,
									__in ULONG                CommitSize,
									__in __out PLARGE_INTEGER SectionOffset OPTIONAL,
									__in __out PULONG         ViewSize,
									__in SECTION_INHERIT      InheritDisposition,
									__in ULONG                AllocationType,
									__in ULONG                Protect
									);
static __pfnMmMapViewOfSection pfnMmMapViewOfSection = NULL;

NTSTATUS BDKitReadFileWithMap(
							  __in PFILE_OBJECT		pFileObject,
							  __in ULONG			ulLength,
							  __in PLARGE_INTEGER	pllFileOffset,
							  __out PVOID			pBuffer,
							  __out LARGE_INTEGER	llFileSize
							  )
{
	NTSTATUS		nsStatus		= STATUS_UNSUCCESSFUL;
	PVOID			pCharme			= NULL;
	PVOID			pSection		= NULL;
	HANDLE			hSection		= NULL;
	ULONG			ViewSize		= 0;
	ULONG			ulOffset		= 0;
	LARGE_INTEGER	llSectionOffset = {0x00};
	LARGE_INTEGER	llTemp			= {0x00};

	do 
	{
		BDKit_If_Not_Break(pFileObject != NULL && ulLength != 0 && pllFileOffset->QuadPart >= 0 && pBuffer != NULL);

		if ( pfnMmCreateSection == NULL )
		{
			UNICODE_STRING uszFunctionName;
			RtlInitUnicodeString (&uszFunctionName, L"MmCreateSection");
			pfnMmCreateSection = (__pfnMmCreateSection)MmGetSystemRoutineAddress(&uszFunctionName);
		}

		if ( pfnMmMapViewOfSection == NULL )
		{
			UNICODE_STRING uszFunctionName;
			RtlInitUnicodeString (&uszFunctionName, L"MmMapViewOfSection");
			pfnMmMapViewOfSection = (__pfnMmMapViewOfSection)MmGetSystemRoutineAddress(&uszFunctionName);
		}
			
		BDKit_If_Not_Break_With_Reason(pfnMmCreateSection != NULL && pfnMmMapViewOfSection != NULL,
			STATUS_NOT_SUPPORTED);

		llTemp.HighPart = 0;
		llTemp.LowPart = (llFileSize.LowPart + 0xFFFF) & 0xFFFF0000;
		ViewSize = (ulLength + 0xFFF) & 0xFFFFF000;
		llSectionOffset.LowPart = pllFileOffset->LowPart;
		llSectionOffset.HighPart = 0;

		nsStatus = MmCreateSection(
			&pSection,
			SECTION_MAP_READ,NULL,
			&llTemp,
			PAGE_READONLY,
			SEC_COMMIT,
			NULL,
			pFileObject
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ObInsertObject(pSection, NULL, SECTION_MAP_READ, 0, NULL, &hSection);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = MmMapViewOfSection(
			pSection,
			IoGetCurrentProcess(),
			&pCharme,
			0,
			0,
			&llSectionOffset,
			&ViewSize,
			ViewShare,
			0,
			PAGE_READONLY
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		ulOffset = pllFileOffset->LowPart - llSectionOffset.LowPart;
		BDKit_If_Not_Break(ViewSize >= ((ulLength + 0xFFF) & 0xFFFFF000) && ulOffset <= ViewSize);

		RtlCopyMemory(pBuffer, (PUCHAR)pCharme + ulOffset, ulLength);

		nsStatus = STATUS_SUCCESS;

	} while (FALSE);

	BDKit_If_DoAction(pCharme != NULL, ZwUnmapViewOfSection ((HANDLE)-1, pCharme));
	BDKitCloseHandle(hSection);

	return nsStatus;
}

NTSTATUS IrpReadFile(
					 __in	HANDLE  FileHandle,
					 __in	HANDLE  Event	OPTIONAL,
					 __in	PIO_APC_ROUTINE		ApcRoutine  OPTIONAL,
					 __in	PVOID	ApcContext  OPTIONAL,
					 __out	PIO_STATUS_BLOCK	IoStatusBlock,
					 __out	PVOID	Buffer,
					 __in	ULONG	Length,
					 __in	PLARGE_INTEGER		ByteOffset  OPTIONAL,
					 __in	PULONG  Key		OPTIONAL
					 )
{
	NTSTATUS		nsStatus		= STATUS_UNSUCCESSFUL;
	KPROCESSOR_MODE kCurrMode		= KernelMode;//ExGetPreviousMode();
	PFILE_OBJECT	pFileObject		= NULL;
	PDEVICE_OBJECT	pRelaDevObj		= NULL;
	PVOID			pEventObject	= NULL;
	PDEVICE_OBJECT	pFileRelaDevObj	= NULL;
	PIRP			Irp				= NULL;
	PMDL			pMdl			= NULL;
	PIO_STACK_LOCATION IrpSp		= NULL;
	BOOLEAN			bSynchronousIo	= FALSE;
	BOOLEAN			bInterrupted	= FALSE;
	BOOLEAN			bFileObjLock	= FALSE;
	LARGE_INTEGER	llFileSize		= {0x00};
	LARGE_INTEGER	fileOffset		= {0x00};
	ULONG			ulKeuValue		= 0;

	do 
	{
		nsStatus = ObReferenceObjectByHandle(
			FileHandle,
			FILE_READ_DATA,
			*IoFileObjectType,
			kCurrMode,
			(PVOID*)&pFileObject,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		BDKit_If_Not_Break_With_Reason(pFileObject->CompletionContext == NULL && ApcRoutine == NULL,
			STATUS_INVALID_PARAMETER);

		pRelaDevObj = BDKitGetFileObjRelatedDeviceObject (pFileObject);
		BDKit_If_Not_Break_With_Reason(pRelaDevObj != NULL, STATUS_INVALID_PARAMETER);

		if (pFileObject->Flags & FO_NO_INTERMEDIATE_BUFFERING)
		{
			if (	(pRelaDevObj->SectorSize && (Length & (pRelaDevObj->SectorSize - 1)))
				||	(ULONG_PTR) Buffer & pRelaDevObj->AlignmentRequirement) 
			{

				if (	(pRelaDevObj->SectorSize && Length % pRelaDevObj->SectorSize)
					||	(ULONG_PTR)Buffer & pRelaDevObj->AlignmentRequirement)
				{
					nsStatus = STATUS_INVALID_PARAMETER;
					break;
				}
			}

			if (ByteOffset != NULL) 
			{
				if (pRelaDevObj->SectorSize && (ByteOffset->LowPart & (pRelaDevObj->SectorSize - 1)))
				{
					nsStatus = STATUS_INVALID_PARAMETER;
					break;
				}
			}
		}

		if (Key) 
		{
			ulKeuValue = *Key;
		}

		if (ByteOffset != NULL)
		{
			fileOffset = *ByteOffset;
		}

		pFileRelaDevObj = BDKitGetFileObjRelatedDeviceObject(pFileObject);


		if (Event != NULL) 
		{
			nsStatus = ObReferenceObjectByHandle(
				Event,
				EVENT_MODIFY_STATE,
				*ExEventObjectType,
				kCurrMode,
				(PVOID *)&pEventObject,
				NULL
				);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
			KeClearEvent((PKEVENT)pEventObject);
		}

		if (pFileObject->Flags & FO_SYNCHRONOUS_IO) 
		{
			bInterrupted = FALSE;
			if (!BDKitAcquireFastLock(pFileObject))
			{

				nsStatus = BDKitAcquireFileObjectLock(
					pFileObject,
					kCurrMode,
					(BOOLEAN)((pFileObject->Flags & FO_ALERTABLE_IO) != 0),
					&bInterrupted
					);
				BDKit_If_Not_Break(bInterrupted == FALSE);
			}

			bFileObjLock = TRUE;

			if (	(ByteOffset != NULL && !fileOffset.LowPart)
				||	(fileOffset.LowPart == FILE_USE_FILE_POINTER_POSITION && fileOffset.HighPart == -1)) 
			{
				fileOffset = pFileObject->CurrentByteOffset;
			}


			bSynchronousIo = TRUE;

			if (pFileObject->PrivateCacheMap) 
			{
				IO_STATUS_BLOCK		IoStatus		= {0x00};
				PFAST_IO_DISPATCH	fastIoDispatch	= NULL;

				fastIoDispatch = pFileRelaDevObj->DriverObject->FastIoDispatch;

				if (fileOffset.HighPart < 0) 
				{
					nsStatus = STATUS_INVALID_PARAMETER;
					break;
				}

				if (fastIoDispatch->FastIoRead(
					pFileObject,
					&fileOffset,
					Length,
					TRUE,
					ulKeuValue,
					Buffer,
					&IoStatus,
					pFileRelaDevObj
					) == TRUE
					) 
				{
					if (	((IoStatus.Status == STATUS_SUCCESS)
						||	(IoStatus.Status == STATUS_BUFFER_OVERFLOW) 
						||	(IoStatus.Status == STATUS_END_OF_FILE))
						)
					{
						__try 
						{

							*IoStatusBlock = IoStatus;

						} 
						__except( EXCEPTION_EXECUTE_HANDLER ) 
						{
							IoStatus.Status = GetExceptionCode();
							IoStatus.Information = 0;
						}

						if (Event != NULL && pEventObject != NULL) 
						{
							KeSetEvent(pEventObject, 0, FALSE);
						}

						nsStatus = IoStatus.Status;
						break;
					}

				}
			}
		} 
		else if (ByteOffset == NULL && !(pFileObject->Flags & (FO_NAMED_PIPE | FO_MAILSLOT))) 
		{
			nsStatus = STATUS_INVALID_PARAMETER;
			break;
		} 
		else 
		{
			bSynchronousIo = FALSE;
		}


		BDKit_If_Not_Break_With_Reason(fileOffset.HighPart >= 0, STATUS_INVALID_PARAMETER);

		KeClearEvent(&pFileObject->Event);

		Irp = IoAllocateIrp(pFileRelaDevObj->StackSize, !bSynchronousIo);
		BDKit_If_Not_Break_With_Reason(Irp != NULL, STATUS_INSUFFICIENT_RESOURCES);

		Irp->Tail.Overlay.OriginalFileObject	= pFileObject;
		Irp->Tail.Overlay.Thread				= PsGetCurrentThread();
		Irp->Tail.Overlay.AuxiliaryBuffer		= (PCHAR) NULL;
		Irp->RequestorMode						= kCurrMode;
		Irp->PendingReturned					= FALSE;
		Irp->Cancel								= FALSE;
		Irp->CancelRoutine						= (PDRIVER_CANCEL)NULL;
		Irp->UserEvent							= (PKEVENT)pEventObject;
		Irp->UserIosb							= IoStatusBlock;
		Irp->AssociatedIrp.SystemBuffer			= (PVOID)NULL;
		Irp->MdlAddress							= (PMDL)NULL;
		Irp->Overlay.AsynchronousParameters.UserApcRoutine = (PIO_APC_ROUTINE)ApcRoutine;
		Irp->Overlay.AsynchronousParameters.UserApcContext = ApcContext;


		IrpSp = IoGetNextIrpStackLocation(Irp);
		BDKit_If_DoAction(pFileObject->Flags & FO_WRITE_THROUGH, IrpSp->Flags = SL_WRITE_THROUGH);
		IrpSp->MajorFunction = IRP_MJ_READ;
		IrpSp->FileObject = pFileObject;

		if (pFileRelaDevObj->Flags & DO_BUFFERED_IO) 
		{
			if (Length != 0) 
			{
				__try 
				{
					BDKitAllocateNonPageQuotaPool(Irp->AssociatedIrp.SystemBuffer, Length);
					BDKit_If_Not_Break_With_Reason(Irp->AssociatedIrp.SystemBuffer, STATUS_INSUFFICIENT_RESOURCES);
				} 
				__except(EXCEPTION_EXECUTE_HANDLER) 
				{
					nsStatus = STATUS_INSUFFICIENT_RESOURCES;
					break;
				}

				Irp->UserBuffer = Buffer;
				Irp->Flags = IRP_BUFFERED_IO | IRP_DEALLOCATE_BUFFER | IRP_INPUT_OPERATION;
			} 
			else
			{
				Irp->Flags = IRP_BUFFERED_IO | IRP_INPUT_OPERATION;
			}

		} else if (pFileRelaDevObj->Flags & DO_DIRECT_IO)
		{
			pMdl = (PMDL)NULL;
			Irp->Flags = 0;

			if (Length)
			{
				__try 
				{
					pMdl = IoAllocateMdl(Buffer, Length, FALSE, TRUE, Irp);
					BDKit_If_Not_Break_With_Reason(pMdl != NULL, STATUS_INSUFFICIENT_RESOURCES);
				}
				__except(EXCEPTION_EXECUTE_HANDLER) 
				{
					nsStatus = STATUS_INSUFFICIENT_RESOURCES;
					break;
				}
			}
		} 
		else
		{
			Irp->Flags = 0;
			Irp->UserBuffer = Buffer;
		}

		if (pFileObject->Flags & FO_NO_INTERMEDIATE_BUFFERING) 
		{
			Irp->Flags |= IRP_NOCACHE | IRP_READ_OPERATION | IRP_DEFER_IO_COMPLETION;
		} 
		else
		{
			Irp->Flags |= IRP_READ_OPERATION | IRP_DEFER_IO_COMPLETION;
		}

		IrpSp->Parameters.Read.Length     = Length;
		IrpSp->Parameters.Read.Key        = ulKeuValue;
		IrpSp->Parameters.Read.ByteOffset = fileOffset;

		if (!bSynchronousIo)
		{
			IoEnqueueIrp(Irp);
		}

		nsStatus = IoCallDriver(pFileRelaDevObj, Irp);

		if (nsStatus != STATUS_PENDING) 
		{
			PKNORMAL_ROUTINE normalRoutine = NULL;
			PVOID normalContext            = NULL;
			KIRQL Irql                     = PASSIVE_LEVEL; 

			ASSERT(!Irp->PendingReturned);

			BDKit_If_DoAction(bSynchronousIo == FALSE, KeRaiseIrql (APC_LEVEL, &Irql));
			{
				BDKitIopCompleteRequest(
					&Irp->Tail.Apc,
					&normalRoutine,
					&normalContext,
					(PVOID *)&pFileObject,
					&normalContext
					);
			}
			BDKit_If_DoAction(bSynchronousIo == FALSE, KeLowerIrql (Irql));
		}

		if (bSynchronousIo)
		{
			if (nsStatus == STATUS_PENDING)
			{
				nsStatus = KeWaitForSingleObject(
					&pFileObject->Event,
					Executive,
					KernelMode,
					(BOOLEAN)((pFileObject->Flags & FO_ALERTABLE_IO) != 0),
					(PLARGE_INTEGER) NULL
					);
				BDKit_If_DoAction(nsStatus == STATUS_PENDING || nsStatus == STATUS_USER_APC,
					BDKitCancelAlertedRequest(&pFileObject->Event, Irp));

				nsStatus = pFileObject->FinalStatus;
			}

			if(nsStatus == STATUS_FILE_LOCK_CONFLICT)
			{
				llFileSize.QuadPart = 0;
				if(BDKitGetFileSizeByObject(pFileObject, &llFileSize))
				{
					nsStatus = BDKitReadFileWithMap(pFileObject,Length, &fileOffset, Buffer, llFileSize);
					if(NT_SUCCESS(nsStatus))
					{
						IoStatusBlock->Status = STATUS_SUCCESS;

						if (pFileObject->CurrentByteOffset.QuadPart + Length <= llFileSize.QuadPart)
						{
							pFileObject->CurrentByteOffset.QuadPart += Length;
							IoStatusBlock->Information = Length;
						}
						else
						{
							LARGE_INTEGER llTemp = {0};
							llTemp.QuadPart = llFileSize.QuadPart - pFileObject->CurrentByteOffset.QuadPart;
							IoStatusBlock->Information = llTemp.LowPart;
							pFileObject->CurrentByteOffset.QuadPart = llFileSize.QuadPart;
						}
					}
				}
			}
			BDKitReleaseFileObjectLock(pFileObject);
		}

		return nsStatus;

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

	return nsStatus;
}

NTSTATUS IrpWriteFile(
					 __in	HANDLE  FileHandle,
					 __in	HANDLE  Event	OPTIONAL,
					 __in	PIO_APC_ROUTINE		ApcRoutine  OPTIONAL,
					 __in	PVOID	ApcContext  OPTIONAL,
					 __out	PIO_STATUS_BLOCK	IoStatusBlock,
					 __out	PVOID	Buffer,
					 __in	ULONG	Length,
					 __in	PLARGE_INTEGER		ByteOffset  OPTIONAL,
					 __in	PULONG  Key		OPTIONAL
					 )
{
	NTSTATUS nsStatus                     = STATUS_UNSUCCESSFUL;
	PFILE_OBJECT pFileObject              = NULL;
	PDEVICE_OBJECT pRelaDevObj            = NULL;
	ULONG ulKeyValue                      = 0;
	OBJECT_HANDLE_INFORMATION HandleInfo  = {0x00};
	LARGE_INTEGER fileOffset              = {0x00};
	PDEVICE_OBJECT pFileRelaDevObj        = NULL;
	PVOID pEventObject                    = NULL;
	BOOLEAN bSynchronousIo                = FALSE;
	KPROCESSOR_MODE kCurrMode;
	BOOLEAN bInterrupted                  = FALSE;
	PIRP Irp                             = NULL;
	PMDL pMdl                             = NULL;
	PIO_STACK_LOCATION pIrpSp             = NULL;
	BOOLEAN bFileObjLock                  = FALSE;

	do 
	{
		kCurrMode = KernelMode;//ExGetPreviousMode();

		nsStatus = ObReferenceObjectByHandle(
			FileHandle, 
			FILE_GENERIC_WRITE, 
			*IoFileObjectType, 
			kCurrMode, 
			(PVOID *)&pFileObject, 
			&HandleInfo
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		BDKit_If_Not_Break_With_Reason(pFileObject->CompletionContext == NULL && ApcRoutine == NULL,
			STATUS_INVALID_PARAMETER);

		pRelaDevObj = BDKitGetFileObjRelatedDeviceObject (pFileObject);
		BDKit_If_Not_Break_With_Reason(pRelaDevObj != NULL, STATUS_INVALID_PARAMETER);

		if (pFileObject->Flags & FO_NO_INTERMEDIATE_BUFFERING)
		{
			if ((pRelaDevObj->SectorSize && (Length & (pRelaDevObj->SectorSize - 1))) ||
				(ULONG_PTR) Buffer & pRelaDevObj->AlignmentRequirement) 
			{

				if ((pRelaDevObj->SectorSize && Length % pRelaDevObj->SectorSize) ||
					(ULONG_PTR)Buffer & pRelaDevObj->AlignmentRequirement)
				{
					nsStatus = STATUS_INVALID_PARAMETER;
					break;
				}
			}

			if (ByteOffset != NULL) 
			{
				if (ByteOffset->LowPart == FILE_WRITE_TO_END_OF_FILE &&
					ByteOffset->HighPart == -1) 
				{
					//NOTHING;
				} 
				else if (ByteOffset->LowPart == FILE_USE_FILE_POINTER_POSITION &&
					ByteOffset->HighPart == -1 &&
					(pFileObject->Flags & FO_SYNCHRONOUS_IO))
				{
					//NOTHING;
				} 
				else if (pRelaDevObj->SectorSize &&
					(ByteOffset->LowPart & (pRelaDevObj->SectorSize - 1))) 
				{
					nsStatus = STATUS_INVALID_PARAMETER;
					break;
				}

				fileOffset = *ByteOffset;
			}
		}


		if (Key) 
		{
			ulKeyValue = *Key;
		}

		if ((HandleInfo.GrantedAccess & (FILE_APPEND_DATA | FILE_WRITE_DATA)) == FILE_APPEND_DATA)
		{
			fileOffset.LowPart  = FILE_WRITE_TO_END_OF_FILE;
			fileOffset.HighPart = -1;
		}

		pFileRelaDevObj = BDKitGetFileObjRelatedDeviceObject (pFileObject);

		if (Event != NULL) 
		{
			nsStatus = ObReferenceObjectByHandle(
				Event,
				EVENT_MODIFY_STATE,
				*ExEventObjectType,
				kCurrMode,
				(PVOID *)&pEventObject,
				NULL
				);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
			KeClearEvent((PRKEVENT)pEventObject);
		}

		if (pFileObject->Flags & FO_SYNCHRONOUS_IO) 
		{
			bInterrupted = FALSE;
			if (!BDKitAcquireFastLock(pFileObject))
			{
				nsStatus = BDKitAcquireFileObjectLock(
					pFileObject,
					kCurrMode,
					(BOOLEAN)((pFileObject->Flags & FO_ALERTABLE_IO) != 0),
					&bInterrupted
					);
				BDKit_If_Not_Break(bInterrupted == FALSE);
			}

			bFileObjLock = TRUE;

			bSynchronousIo = TRUE;

			if ((ByteOffset != NULL && !fileOffset.LowPart ) ||
				(fileOffset.LowPart == FILE_USE_FILE_POINTER_POSITION && fileOffset.HighPart == -1 )) 
			{
				fileOffset = pFileObject->CurrentByteOffset;
			}


			if (pFileObject->PrivateCacheMap) 
			{

				IO_STATUS_BLOCK localIoStatus = {0x00};
				PFAST_IO_DISPATCH fastIoDispatch = NULL;

				fastIoDispatch = pFileRelaDevObj->DriverObject->FastIoDispatch;

				if (fileOffset.HighPart < 0 &&
					(fileOffset.HighPart != -1 ||
					fileOffset.LowPart != FILE_WRITE_TO_END_OF_FILE)) 
				{
					nsStatus = STATUS_INVALID_PARAMETER;
					break;
				}

				if (fastIoDispatch->FastIoWrite(
					pFileObject,
					&fileOffset,
					Length,
					TRUE,
					ulKeyValue,
					Buffer,
					&localIoStatus,
					pFileRelaDevObj) 
					&&	(localIoStatus.Status == STATUS_SUCCESS)
					)
				{
					__try 
					{
						*IoStatusBlock = localIoStatus;
					} 
					__except(EXCEPTION_EXECUTE_HANDLER) 
					{
						localIoStatus.Status = GetExceptionCode();
						localIoStatus.Information = 0;
					}

					if (Event != NULL && pEventObject != NULL) 
					{
						KeSetEvent(pEventObject, 0, FALSE);
					}

					nsStatus = localIoStatus.Status;
					break;
				}
			}
		} 
		else if (ByteOffset == NULL && !(pFileObject->Flags & (FO_NAMED_PIPE | FO_MAILSLOT))) 
		{
			nsStatus = STATUS_INVALID_PARAMETER;
			break;
		} 
		else 
		{
			bSynchronousIo = FALSE;
		}

		if (fileOffset.HighPart < 0 &&
			(fileOffset.HighPart != -1 || fileOffset.LowPart != FILE_WRITE_TO_END_OF_FILE))
		{
			nsStatus = STATUS_INVALID_PARAMETER;
			break;
		}

		KeClearEvent(&pFileObject->Event);

		Irp = IoAllocateIrp(pFileRelaDevObj->StackSize, !bSynchronousIo);
		BDKit_If_Not_Break_With_Reason(Irp != NULL, STATUS_INSUFFICIENT_RESOURCES);

		Irp->Tail.Overlay.OriginalFileObject = pFileObject;
		Irp->Tail.Overlay.Thread             = PsGetCurrentThread();
		Irp->Tail.Overlay.AuxiliaryBuffer    = (PCHAR) NULL;
		Irp->RequestorMode                   = kCurrMode;
		Irp->PendingReturned                 = FALSE;
		Irp->Cancel                          = FALSE;
		Irp->CancelRoutine                   = (PDRIVER_CANCEL)NULL;

		Irp->UserEvent                       = (PKEVENT)pEventObject;
		Irp->UserIosb                        = IoStatusBlock;
		Irp->Overlay.AsynchronousParameters.UserApcRoutine = (PIO_APC_ROUTINE)ApcRoutine;
		Irp->Overlay.AsynchronousParameters.UserApcContext = ApcContext;

		pIrpSp = IoGetNextIrpStackLocation(Irp);

		*(PULONG)pIrpSp    = IRP_MJ_WRITE;
		pIrpSp->FileObject = pFileObject;

		if (pFileObject->Flags & FO_WRITE_THROUGH) 
		{
			pIrpSp->Flags = SL_WRITE_THROUGH;
		}

		Irp->AssociatedIrp.SystemBuffer = (PVOID)NULL;
		Irp->MdlAddress                 = (PMDL)NULL;

		if (pFileRelaDevObj->Flags & DO_BUFFERED_IO) 
		{
			if (Length != 0) 
			{
				__try 
				{
					BDKitAllocateNonPageQuotaPool(Irp->AssociatedIrp.SystemBuffer, Length);
					BDKit_If_Not_Break_With_Reason(Irp->AssociatedIrp.SystemBuffer, STATUS_INSUFFICIENT_RESOURCES);
					RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, Buffer, Length);
				} 
				__except(EXCEPTION_EXECUTE_HANDLER) 
				{
					nsStatus = STATUS_INSUFFICIENT_RESOURCES;
					break;
				}

				Irp->Flags = IRP_BUFFERED_IO | IRP_DEALLOCATE_BUFFER;

			} 
			else
			{
				Irp->Flags = IRP_BUFFERED_IO;
			}

		} 
		else if (pFileRelaDevObj->Flags & DO_DIRECT_IO)
		{
			pMdl = (PMDL)NULL;
			Irp->Flags = 0;

			if (Length != 0)
			{
				__try 
				{
					pMdl = IoAllocateMdl(Buffer, Length, FALSE, TRUE, Irp);
					if (pMdl == NULL) 
					{
						nsStatus = STATUS_INSUFFICIENT_RESOURCES;
						break;
					}

					MmProbeAndLockPages(pMdl, kCurrMode, IoReadAccess);
				}
				__except(EXCEPTION_EXECUTE_HANDLER) 
				{
					nsStatus = STATUS_INSUFFICIENT_RESOURCES;
					break;
				}
			}
		} 
		else
		{
			Irp->Flags = 0;
			Irp->UserBuffer = Buffer;
		}

		if (pFileObject->Flags & FO_NO_INTERMEDIATE_BUFFERING) 
		{
			Irp->Flags |= IRP_NOCACHE | IRP_WRITE_OPERATION | IRP_DEFER_IO_COMPLETION;
		} 
		else
		{
			Irp->Flags |= IRP_WRITE_OPERATION | IRP_DEFER_IO_COMPLETION;
		}

		pIrpSp->Parameters.Write.Length     = Length;
		pIrpSp->Parameters.Write.Key        = ulKeyValue;
		pIrpSp->Parameters.Write.ByteOffset = fileOffset;


		if (!bSynchronousIo)
		{
			IoEnqueueIrp(Irp);
		}

		nsStatus = IoCallDriver(pFileRelaDevObj, Irp);
		if (nsStatus != STATUS_PENDING) 
		{
			PKNORMAL_ROUTINE normalRoutine = NULL;
			PVOID normalContext            = NULL;
			KIRQL Irql                     = PASSIVE_LEVEL; 

			ASSERT(!Irp->PendingReturned);

			BDKit_If_DoAction(bSynchronousIo == FALSE, KeRaiseIrql (APC_LEVEL, &Irql));
			{
				BDKitIopCompleteRequest(
					&Irp->Tail.Apc,
					&normalRoutine,
					&normalContext,
					(PVOID *)&pFileObject,
					&normalContext
					);
			}
			BDKit_If_DoAction(bSynchronousIo == FALSE, KeLowerIrql (Irql));
		}

		if (bSynchronousIo)
		{
			if (nsStatus == STATUS_PENDING)
			{
				nsStatus = KeWaitForSingleObject(
					&pFileObject->Event,
					Executive,
					KernelMode,
					(BOOLEAN)((pFileObject->Flags & FO_ALERTABLE_IO) != 0),
					(PLARGE_INTEGER) NULL
					);
				BDKit_If_DoAction(nsStatus == STATUS_ALERTED || nsStatus == STATUS_USER_APC,
					BDKitCancelAlertedRequest(&pFileObject->Event, Irp));

				nsStatus = pFileObject->FinalStatus;
			}

			BDKitReleaseFileObjectLock (pFileObject);
		}

		return nsStatus;

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

	return nsStatus;
}

NTSTATUS BDKitReadFile(
					   __in HANDLE  FileHandle,
					   __in HANDLE  Event  OPTIONAL,
					   __in PIO_APC_ROUTINE  ApcRoutine  OPTIONAL,
					   __in PVOID	ApcContext  OPTIONAL,
					   __out PIO_STATUS_BLOCK  IoStatusBlock,
					   __out PVOID  Buffer,
					   __in ULONG	Length,
					   __in PLARGE_INTEGER  ByteOffset  OPTIONAL,
					   __in PULONG  Key  OPTIONAL
					   )
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;

	do 
	{
		nsStatus=IrpReadFile(
			FileHandle,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			Buffer,
			Length,
			ByteOffset,
			Key
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	return nsStatus;
}

NTSTATUS BDKitWriteFile(
						__in HANDLE  FileHandle,
						__in HANDLE  Event  OPTIONAL,
						__in PIO_APC_ROUTINE  ApcRoutine  OPTIONAL,
						__in PVOID	ApcContext  OPTIONAL,
						__out PIO_STATUS_BLOCK  IoStatusBlock,
						__in CONST PVOID  Buffer,
						__in ULONG	Length,
						__in PLARGE_INTEGER  ByteOffset  OPTIONAL,
						__in PULONG  Key  OPTIONAL
						)
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;

	do 
	{
		nsStatus=IrpWriteFile(
			FileHandle,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			Buffer,
			Length,
			ByteOffset,
			Key
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	return nsStatus;
}