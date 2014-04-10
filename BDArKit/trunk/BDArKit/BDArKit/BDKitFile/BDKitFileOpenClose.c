#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "DriverHelper.h"
#include "Utility.h"
#include "BDKitUtils.h"


static PVOID g_emObjFileFunction[emObjEnd]		= {0x00};
static PVOID g_emObjDeviceFunction[emObjEnd]	= {0x00};
static PVOID g_pDevObjectTypePointer			= NULL;

#define RESET_FILE_OBJECT_CALLBACK				0x00000001L
#define RESET_DEVICE_OBJECT_CALLBACK			0x10000000L
#define OPEN_PACKET_PATTERN						0xBEAA0251L

//////////////////////////////////////////////////////////////////////////
#define FSIO_A                                  0x00000010L
#define FSIO_NA                                 0x00000020L
#define IO_NO_PARAMETER_CHECKING                0x0100
#define IO_OPEN_TARGET_DIRECTORY                0x0004
#define FO_FILE_OBJECT_HAS_EXTENSION            0x00800000
#define IOP_CREATE_USE_TOP_DEVICE_OBJECT_HINT   0x1 // Define for internal flags to IopCreateFile
#define IOP_CREATE_DEVICE_OBJECT_EXTENSION      0x4
#define IO_COMPLETION_QUERY_STATE               0x0001
#define IO_COMPLETION_MODIFY_STATE              0x0002  // winnt
#define IO_COMPLETION_ALL_ACCESS                (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|0x3) // winnt
#define FileShortNameInformation                40
#define SystemHandleInformation                 16
#if (VER_PRODUCTBUILD < 7600)
#define IO_REPARSE_TAG_MOUNT_POINT              0xA0000003L
#endif
//////////////////////////////////////////////////////////////////////////

typedef struct _DUMMY_FILE_OBJECT 
{
	OBJECT_HEADER ObjectHeader;
	CHAR FileObjectBody[sizeof( FILE_OBJECT )];
} DUMMY_FILE_OBJECT, *PDUMMY_FILE_OBJECT;

typedef struct _DUMMY_FILE_OBJECT_WIN7 
{
	CHAR ObjectHeader[0x20];
	CHAR FileObjectBody[sizeof( FILE_OBJECT )];
	CHAR chEmpty[0x10];
} DUMMY_FILE_OBJECT_WIN7, *PDUMMY_FILE_OBJECT_WIN7;

// win xp  2003
typedef struct _OPEN_PACKET 
{
	CSHORT Type;
	CSHORT Size;
	PFILE_OBJECT FileObject;
	NTSTATUS FinalStatus;
	ULONG_PTR Information;
	ULONG ParseCheck;
	PFILE_OBJECT RelatedFileObject;
	LARGE_INTEGER AllocationSize;
	ULONG CreateOptions;
	USHORT FileAttributes;
	USHORT ShareAccess;
	PVOID EaBuffer;
	ULONG EaLength;
	ULONG Options;
	ULONG Disposition;

	PFILE_BASIC_INFORMATION BasicInformation;

	PFILE_NETWORK_OPEN_INFORMATION NetworkInformation;

	CREATE_FILE_TYPE CreateFileType;

	PVOID ExtraCreateParameters;

	BOOLEAN Override;

	BOOLEAN QueryOnly;

	BOOLEAN DeleteOnly;

	BOOLEAN FullAttributes;

	PDUMMY_FILE_OBJECT LocalFileObject;

	BOOLEAN TraversedMountPoint;

	ULONG           InternalFlags;
	PDEVICE_OBJECT  TopDeviceObjectHint;

} OPEN_PACKET, *POPEN_PACKET;

#if (VER_PRODUCTBUILD < 7600)
typedef struct _TXN_PARAMETER_BLOCK 
{

	USHORT Length;              // sizeof( TXN_PARAMETER_BLOCK )
	USHORT TxFsContext;         // this is mini version of the requested file
	PVOID  TransactionObject;   // referenced pointer to KTRANSACTION

} TXN_PARAMETER_BLOCK, *PTXN_PARAMETER_BLOCK;
#endif

#if (VER_PRODUCTBUILD < 7600)
typedef struct _IO_DRIVER_CREATE_CONTEXT 
{
	CSHORT Size;
	struct _ECP_LIST *ExtraCreateParameter;
	PVOID DeviceObjectHint;
	PTXN_PARAMETER_BLOCK TxnParameters;
} IO_DRIVER_CREATE_CONTEXT, *PIO_DRIVER_CREATE_CONTEXT;
#endif

typedef struct _OPEN_PACKETWIN7 
{
	CSHORT Type;
	CSHORT Size;
	PFILE_OBJECT FileObject;
	NTSTATUS FinalStatus;
	ULONG_PTR Information;
	ULONG ParseCheck;
	PFILE_OBJECT RelatedFileObject;

	POBJECT_ATTRIBUTES OriginalAttributes;

	LARGE_INTEGER AllocationSize;
	ULONG CreateOptions;
	USHORT FileAttributes;
	USHORT ShareAccess;
	PVOID EaBuffer;
	ULONG EaLength;
	ULONG Options;
	ULONG Disposition;

	PFILE_BASIC_INFORMATION BasicInformation;

	PFILE_NETWORK_OPEN_INFORMATION NetworkInformation;

	CREATE_FILE_TYPE CreateFileType;

	PVOID ExtraCreateParameters;

	BOOLEAN Override;

	BOOLEAN QueryOnly;

	BOOLEAN DeleteOnly;

	BOOLEAN FullAttributes;

	PDUMMY_FILE_OBJECT_WIN7 LocalFileObject;

	ULONG InternalFlags;

	IO_DRIVER_CREATE_CONTEXT IoDriverCreateContext;

} OPEN_PACKETWIN7, *POPEN_PACKETWIN7;

typedef struct _OPEN_PACKETWIN8 
{
	CSHORT Type;
	CSHORT Size;
	PFILE_OBJECT FileObject;
	NTSTATUS FinalStatus;
	ULONG_PTR Information;
	ULONG ParseCheck;
	PFILE_OBJECT RelatedFileObject;

	POBJECT_ATTRIBUTES OriginalAttributes;

	LARGE_INTEGER AllocationSize;
	ULONG CreateOptions;
	USHORT FileAttributes;
	USHORT ShareAccess;
	PVOID EaBuffer;
	ULONG EaLength;
	ULONG Options;
	ULONG Disposition;

	PFILE_BASIC_INFORMATION BasicInformation;

	PFILE_NETWORK_OPEN_INFORMATION NetworkInformation;

	CREATE_FILE_TYPE CreateFileType;

	PVOID ExtraCreateParameters;

	BOOLEAN Override;

	BOOLEAN QueryOnly;

	BOOLEAN DeleteOnly;

	BOOLEAN FullAttributes;

	PDUMMY_FILE_OBJECT_WIN7 LocalFileObject;

	ULONG InternalFlags;

	KPROCESSOR_MODE kRrequestorMode;

	IO_DRIVER_CREATE_CONTEXT IoDriverCreateContext;
} OPEN_PACKETWIN8, *POPEN_PACKETWIN8;
//////////////////////////////////////////////////////////////////////////

NTSTATUS CheckInputFileInformationClass(
										__in ULONG			FileInformationClass, 
										__in PVOID			FileInformation,
										__in ULONG			ulBufferSize,
										__in PFILE_OBJECT	FileObject
										)
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;

	switch (FileInformationClass)
	{
	case FileModeInformation:
		{
			PFILE_MODE_INFORMATION pModeInfo = (PFILE_MODE_INFORMATION)FileInformation;

			if (	
				(pModeInfo->Mode & ~FILE_VALID_SET_FLAGS) 
				||	((pModeInfo->Mode & (FSIO_A | FSIO_NA)) && (!(FileObject->Flags & FO_SYNCHRONOUS_IO))) 
				||	((!(pModeInfo->Mode & (FSIO_A | FSIO_NA))) && (FileObject->Flags & FO_SYNCHRONOUS_IO)) 
				||	(((pModeInfo->Mode & FSIO_A) && (pModeInfo->Mode & FSIO_NA) ))
				) 
			{
				nsStatus = STATUS_INVALID_PARAMETER;
			}
		}
		break;

	case FileRenameInformation:
	case FileLinkInformation:
	case FileMoveClusterInformation:
		{
			PFILE_RENAME_INFORMATION pRenameInfo = (PFILE_RENAME_INFORMATION)FileInformation;

			if (	
				pRenameInfo->FileNameLength <= 0 
				||	(pRenameInfo->FileNameLength & (sizeof(WCHAR) - 1)) 
				||	(ULONG)(ulBufferSize - FIELD_OFFSET(FILE_RENAME_INFORMATION, FileName[0])) < pRenameInfo->FileNameLength
				)
			{
				nsStatus = STATUS_INVALID_PARAMETER;
			} 
		}
		break;

	case FileShortNameInformation:
		{
			PFILE_NAME_INFORMATION pShortnameBuffer = (PFILE_NAME_INFORMATION)FileInformation;

			if (	
				pShortnameBuffer->FileNameLength <= 0 
				||	(ULONG)(ulBufferSize - FIELD_OFFSET(FILE_NAME_INFORMATION, FileName[0])) < pShortnameBuffer->FileNameLength 
				||	pShortnameBuffer->FileName[0] == (WCHAR)OBJ_NAME_PATH_SEPARATOR
				) 

			{
				nsStatus = STATUS_INVALID_PARAMETER;
			}
		}
		break;		
	}

	return nsStatus;
}

BOOLEAN KeFileSetObjectFunction(__in PVOID	pObjectType, 
								__in PVOID*	pFunctionStruct, 
								__in emObjTypeFunction emFuncIndex, 
								__out PVOID*	pRetFunctionAddr, 
								__in BOOLEAN	bSet
								)
{
	do 
	{
		BDKit_If_Not_Break(pObjectType != NULL);
		BDKit_If_Not_Break(pFunctionStruct != NULL);
		BDKit_If_Not_Break(pRetFunctionAddr != NULL);

		UNREFERENCED_PARAMETER(emFuncIndex);
		UNREFERENCED_PARAMETER(bSet);
	} while (FALSE);
	return TRUE;
}

NTSTATUS IrpCloseFile(__in PFILE_OBJECT FileObject)
{
	NTSTATUS		nsStatus		= STATUS_UNSUCCESSFUL;
	IO_STATUS_BLOCK IoStatus	= {0x00};
	PIRP			Irp				= NULL;
	KEVENT			kEvent			= {0x00};
	PIO_STACK_LOCATION	IrpSp		= NULL;
	PDEVICE_OBJECT	pBaseDeviceObject = FileObject->Vpb->DeviceObject;

	BDKit_If_DoAction(FileObject->Vpb == NULL || FileObject->Vpb->DeviceObject == NULL, return STATUS_UNSUCCESSFUL);

	do 
	{
		Irp = IoAllocateIrp(FileObject->Vpb->DeviceObject->StackSize, FALSE);
		BDKit_If_Not_Break(Irp != NULL);

		KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

		Irp->UserEvent		= &kEvent;
		Irp->UserIosb		= &IoStatus;
		Irp->RequestorMode	= KernelMode;
		Irp->Flags			= IRP_CLOSE_OPERATION | IRP_SYNCHRONOUS_API;
		Irp->Tail.Overlay.Thread = PsGetCurrentThread();
		Irp->Tail.Overlay.OriginalFileObject = FileObject;

		IrpSp = IoGetNextIrpStackLocation(Irp);
		IrpSp->MajorFunction = IRP_MJ_CLEANUP;
		IrpSp->FileObject	 = FileObject;

		IoSetCompletionRoutine(Irp, BDKitIoCompletionRoutine, 0, TRUE, TRUE, TRUE);

		nsStatus = IoCallDriver(pBaseDeviceObject, Irp);
		BDKit_If_DoAction(nsStatus == STATUS_PENDING, 
			KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL));

		Irp = NULL;

		nsStatus = IoStatus.Status;
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		// SEND IRP_MJ_CLOSE
		Irp = IoAllocateIrp(FileObject->Vpb->DeviceObject->StackSize, FALSE);
		BDKit_If_Not_Break(Irp != NULL);

		KeClearEvent(&kEvent);

		Irp->UserEvent	= &kEvent;
		Irp->UserIosb	= &IoStatus;
		Irp->Tail.Overlay.OriginalFileObject = FileObject;
		Irp->Tail.Overlay.Thread		= (PETHREAD)KeGetCurrentThread();
		Irp->AssociatedIrp.SystemBuffer = (PVOID)NULL;
		Irp->Flags = IRP_CLOSE_OPERATION | IRP_SYNCHRONOUS_API;

		IrpSp = IoGetNextIrpStackLocation(Irp);
		IrpSp->MajorFunction	= IRP_MJ_CLOSE;
		IrpSp->FileObject		= FileObject;

		if ( FileObject->Vpb && !(FileObject->Flags & FO_DIRECT_DEVICE_OPEN) )
		{
			InterlockedDecrement((LONG volatile*)&FileObject->Vpb->ReferenceCount);
			FileObject->Flags |= FO_FILE_OPEN_CANCELLED;

		}

		IoSetCompletionRoutine(Irp, BDKitIoCompletionRoutine, 0, TRUE, TRUE, TRUE);

		nsStatus = IoCallDriver(pBaseDeviceObject, Irp);
		BDKit_If_DoAction(nsStatus == STATUS_PENDING, 
			KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL));

		Irp = NULL;

		nsStatus = IoStatus.Status;
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKit_If_DoAction(Irp != NULL, IoFreeIrp (Irp));

	return nsStatus;
}

NTSTATUS BDKitCloseFile(__in HANDLE hFile)
{
	// 这里防止verifier只能关闭内核态句柄, 驱动在传递给用户态句柄时要注意转换
	if (IsKernelHandle(hFile, KernelMode)) 
	{
		return ZwClose(hFile);
	}

	return STATUS_HANDLE_NOT_CLOSABLE;
}

NTSTATUS IopCreateFileXp2003(
							 __out PHANDLE			FileHandle,
							 __in ACCESS_MASK			DesiredAccess,
							 __in POBJECT_ATTRIBUTES	ObjectAttributes,
							 __out PIO_STATUS_BLOCK	IoStatusBlock,
							 __in PLARGE_INTEGER		AllocationSize OPTIONAL,
							 __in ULONG				FileAttributes,
							 __in ULONG				ShareAccess,
							 __in ULONG				Disposition,
							 __in ULONG				CreateOptions,
							 __in PVOID				EaBuffer OPTIONAL,
							 __in ULONG				EaLength,
							 __in CREATE_FILE_TYPE	CreateFileType,
							 __in PVOID				ExtraCreateParameters OPTIONAL,
							 __in ULONG				Options,
							 __in ULONG				InternalFlags,
							 __in PVOID				DeviceObject
							 )
{
	KPROCESSOR_MODE		kRrequestorMode		= KernelMode;
	NTSTATUS			nsStatus			= STATUS_UNSUCCESSFUL;
	HANDLE				hHandle				= NULL;
	POPEN_PACKET		pOpenPacket			= NULL;
	PVOID				pFileObjRetFunc		= NULL;
	PVOID				pDeviceObjRetFunc	= NULL;
	BOOLEAN				bSuccessfulIoParse	= FALSE;
	BOOLEAN				bSetObjFunc			= FALSE;
	ULONG				ulFileOrDevice		= 0;
	LARGE_INTEGER	liInitialAllocationSize = {0x00};

	do 
	{
		kRrequestorMode = KernelMode;

		if (Options & IO_NO_PARAMETER_CHECKING)
		{
			kRrequestorMode = KernelMode;
		}

		if (AllocationSize != NULL) 
		{
			liInitialAllocationSize.QuadPart = AllocationSize->QuadPart;
		} 
		else 
		{
			liInitialAllocationSize.QuadPart = 0;
		}

		BDKitAllocateNonpagePool(pOpenPacket, sizeof(*pOpenPacket));
		BDKit_If_Not_Break(pOpenPacket != NULL);

		if (EaBuffer != NULL && EaLength != 0) 
		{
			ULONG errorOffset = 0;

			BDKitAllocatePagePool(pOpenPacket->EaBuffer, EaLength);
			BDKit_If_Not_Break(pOpenPacket->EaBuffer != NULL);

			pOpenPacket->EaLength = EaLength;

			__try
			{
				RtlCopyMemory(pOpenPacket->EaBuffer, EaBuffer, EaLength);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				nsStatus= STATUS_INVALID_PARAMETER;
				BDKit_If_Not_Break(FALSE);	
			}

			nsStatus = IoCheckEaBufferValidity((PFILE_FULL_EA_INFORMATION)pOpenPacket->EaBuffer, EaLength, &errorOffset);
			if (!NT_SUCCESS(nsStatus)) 
			{
				IoStatusBlock->Status      = nsStatus;
				IoStatusBlock->Information = errorOffset;
				BDKit_If_Not_Break(FALSE);
			}
		} 
		else 
		{
			pOpenPacket->EaBuffer = (PVOID)NULL;
			pOpenPacket->EaLength = 0L;
		}

		pOpenPacket->Type					= IO_TYPE_OPEN_PACKET;
		pOpenPacket->Size					= sizeof(*pOpenPacket);
		pOpenPacket->ParseCheck				= 0L;
		pOpenPacket->AllocationSize			= liInitialAllocationSize;
		pOpenPacket->CreateOptions			= CreateOptions;
		pOpenPacket->FileAttributes			= (USHORT) FileAttributes;
		pOpenPacket->ShareAccess			= (USHORT) ShareAccess;
		pOpenPacket->Disposition			= Disposition;
		pOpenPacket->Override				= FALSE;
		pOpenPacket->QueryOnly				= FALSE;
		pOpenPacket->DeleteOnly				= FALSE;
		pOpenPacket->Options				= Options;
		pOpenPacket->RelatedFileObject		= (PFILE_OBJECT)NULL;
		pOpenPacket->CreateFileType			= CreateFileType;
		pOpenPacket->ExtraCreateParameters	= ExtraCreateParameters;
		pOpenPacket->InternalFlags          = InternalFlags;
		pOpenPacket->TraversedMountPoint    = FALSE;
		pOpenPacket->TopDeviceObjectHint    = (PDEVICE_OBJECT)DeviceObject;

		pOpenPacket->FinalStatus			= STATUS_SUCCESS;
		pOpenPacket->FileObject				= (PFILE_OBJECT)NULL;

		{
			//恢复Object CallBack
			bSetObjFunc = KeFileSetObjectFunction(*IoFileObjectType, g_emObjFileFunction, emObjParseOper, &pFileObjRetFunc, TRUE);
			if (bSetObjFunc && pFileObjRetFunc != NULL)
			{
				ulFileOrDevice |= RESET_FILE_OBJECT_CALLBACK;
			}

			bSetObjFunc = KeFileSetObjectFunction(g_pDevObjectTypePointer, g_emObjDeviceFunction, emObjParseOper, &pDeviceObjRetFunc, TRUE);
			if (bSetObjFunc && pDeviceObjRetFunc != NULL)
			{
				ulFileOrDevice |= RESET_DEVICE_OBJECT_CALLBACK;
			}

			nsStatus = ObOpenObjectByName(
				ObjectAttributes,
				NULL,
				kRrequestorMode,
				NULL,
				DesiredAccess,
				pOpenPacket,
				&hHandle
				);

			//恢复设置
			if (ulFileOrDevice & RESET_FILE_OBJECT_CALLBACK)
			{
				KeFileSetObjectFunction(*IoFileObjectType, g_emObjFileFunction, emObjParseOper, &pFileObjRetFunc, FALSE);
				ulFileOrDevice &= ~RESET_FILE_OBJECT_CALLBACK;
			}

			if (ulFileOrDevice & RESET_DEVICE_OBJECT_CALLBACK)
			{
				KeFileSetObjectFunction(g_pDevObjectTypePointer, g_emObjDeviceFunction, emObjParseOper, &pDeviceObjRetFunc, FALSE);
				ulFileOrDevice &= ~RESET_DEVICE_OBJECT_CALLBACK;
			}
		}

		BDKitFreePool(pOpenPacket->EaBuffer);
		pOpenPacket->EaLength = 0;

		bSuccessfulIoParse = (BOOLEAN)(pOpenPacket->ParseCheck == OPEN_PACKET_PATTERN);

		if ( !NT_SUCCESS(nsStatus) || bSuccessfulIoParse == FALSE ) 
		{

			if (NT_SUCCESS(nsStatus)) 
			{
				BDKitCloseHandle(hHandle);	
				nsStatus = STATUS_OBJECT_TYPE_MISMATCH;
			}

			if ( !NT_SUCCESS(pOpenPacket->FinalStatus) ) 
			{
				nsStatus = pOpenPacket->FinalStatus;
				if ( NT_WARNING(nsStatus) ) 
				{
					__try 
					{
						IoStatusBlock->Status = pOpenPacket->FinalStatus;
						IoStatusBlock->Information = pOpenPacket->Information;

					} 
					__except(EXCEPTION_EXECUTE_HANDLER) 
					{
						nsStatus = GetExceptionCode();
					}
				}
			} 
			else if ( pOpenPacket->FileObject != NULL && !bSuccessfulIoParse ) 
			{
				if ( pOpenPacket->FileObject->FileName.Length != 0 ) 
				{
					BDKitFreePool(pOpenPacket->FileObject->FileName.Buffer);
					pOpenPacket->FileObject->FileName.Length = 0;
				}

				pOpenPacket->FileObject->DeviceObject = (PDEVICE_OBJECT)NULL;
				BDKitCloseObject(pOpenPacket->FileObject);
			}

			if ((nsStatus == STATUS_OBJECT_NAME_NOT_FOUND) && (pOpenPacket->Information == IO_REPARSE_TAG_MOUNT_POINT)) 
			{
				nsStatus = STATUS_REPARSE_POINT_NOT_RESOLVED;
			}
		}
		else 
		{
			pOpenPacket->FileObject->Flags |= FO_HANDLE_CREATED;

			ASSERT( pOpenPacket->FileObject->Type == IO_TYPE_FILE );

			*FileHandle = hHandle;

			__try 
			{
				IoStatusBlock->Information = pOpenPacket->Information;
				IoStatusBlock->Status = pOpenPacket->FinalStatus;
				nsStatus = pOpenPacket->FinalStatus;
			} 
			__except(EXCEPTION_EXECUTE_HANDLER) 
			{
				nsStatus = GetExceptionCode();
			}
		}

		if ( bSuccessfulIoParse == TRUE ) 
		{
			BDKitCloseObject(pOpenPacket->FileObject);
		}
	} while (FALSE);

	BDKit_If_DoAction(pOpenPacket != NULL, BDKitFreePool(pOpenPacket->EaBuffer));
	BDKitFreePool(pOpenPacket);

	return nsStatus;
}

NTSTATUS IopCreateFileWin7(
						   __out PHANDLE				FileHandle,
						   __in ACCESS_MASK			DesiredAccess,
						   __in POBJECT_ATTRIBUTES	ObjectAttributes,
						   __out PIO_STATUS_BLOCK		IoStatusBlock,
						   __in PLARGE_INTEGER		AllocationSize OPTIONAL,
						   __in ULONG					FileAttributes,
						   __in ULONG					ShareAccess,
						   __in ULONG					Disposition,
						   __in ULONG					CreateOptions,
						   __in PVOID					EaBuffer OPTIONAL,
						   __in ULONG					EaLength,
						   __in CREATE_FILE_TYPE		CreateFileType,
						   __in PVOID					ExtraCreateParameters OPTIONAL,
						   __in ULONG					Options,
						   __in ULONG					InternalFlags,
						   __in PVOID					DeviceObject
						   )
{
	KPROCESSOR_MODE		kRrequestorMode		= KernelMode;
	NTSTATUS			nsStatus			= STATUS_UNSUCCESSFUL;
	HANDLE				hHandle				= NULL;
	POPEN_PACKETWIN7	pOpenPacket			= NULL;
	PVOID				pFileObjRetFunc		= NULL;
	PVOID				pDeviceObjRetFunc	= NULL;
	BOOLEAN				bSuccessfulIoParse	= FALSE;
	BOOL				bSetObjFunc			= FALSE;
	ULONG				ulFileOrDevice		= 0;
	LARGE_INTEGER	liInitialAllocationSize = {0x00};

	do 
	{
		kRrequestorMode = KernelMode;

		if (Options & IO_NO_PARAMETER_CHECKING)
		{
			kRrequestorMode = KernelMode;
		}

		if (AllocationSize != NULL) 
		{
			liInitialAllocationSize.QuadPart = AllocationSize->QuadPart;
		} 
		else 
		{
			liInitialAllocationSize.QuadPart = 0;
		}

		BDKitAllocateNonpagePool(pOpenPacket, sizeof(*pOpenPacket));
		BDKit_If_Not_Break(pOpenPacket != NULL);

		if (EaBuffer != NULL && EaLength != 0) 
		{
			ULONG errorOffset = 0;

			BDKitAllocatePagePool(pOpenPacket->EaBuffer, EaLength);
			BDKit_If_Not_Break(pOpenPacket->EaBuffer != NULL);

			pOpenPacket->EaLength = EaLength;

			__try
			{
				RtlCopyMemory(pOpenPacket->EaBuffer, EaBuffer, EaLength);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				nsStatus= STATUS_INVALID_PARAMETER;
				BDKit_If_Not_Break(FALSE);	
			}

			nsStatus = IoCheckEaBufferValidity((PFILE_FULL_EA_INFORMATION)pOpenPacket->EaBuffer, EaLength, &errorOffset);
			if (!NT_SUCCESS(nsStatus)) 
			{
				IoStatusBlock->Status      = nsStatus;
				IoStatusBlock->Information = errorOffset;
				BDKit_If_Not_Break(FALSE);
			}
		} 
		else 
		{
			pOpenPacket->EaBuffer = (PVOID)NULL;
			pOpenPacket->EaLength = 0L;
		}

		pOpenPacket->Type					= IO_TYPE_OPEN_PACKET;
		pOpenPacket->Size					= sizeof(*pOpenPacket);
		pOpenPacket->ParseCheck				= 0L;
		pOpenPacket->AllocationSize			= liInitialAllocationSize;
		pOpenPacket->CreateOptions			= CreateOptions;
		pOpenPacket->FileAttributes			= (USHORT) FileAttributes;
		pOpenPacket->ShareAccess			= (USHORT) ShareAccess;
		pOpenPacket->Disposition			= Disposition;
		pOpenPacket->Override				= FALSE;
		pOpenPacket->QueryOnly				= FALSE;
		pOpenPacket->DeleteOnly				= FALSE;
		pOpenPacket->Options				= Options;
		pOpenPacket->RelatedFileObject		= (PFILE_OBJECT)NULL;
		pOpenPacket->CreateFileType			= CreateFileType;
		pOpenPacket->ExtraCreateParameters	= ExtraCreateParameters;

		pOpenPacket->OriginalAttributes		= ObjectAttributes;

		InternalFlags = 0;

		if (Options & IO_IGNORE_SHARE_ACCESS_CHECK)
		{
			InternalFlags |= IOP_CREATE_IGNORE_SHARE_ACCESS_CHECK;
		}

		//win7新增的结构
		pOpenPacket->IoDriverCreateContext.Size                 = sizeof(IO_DRIVER_CREATE_CONTEXT);
		pOpenPacket->IoDriverCreateContext.ExtraCreateParameter = NULL;
		pOpenPacket->IoDriverCreateContext.DeviceObjectHint     = NULL;
		pOpenPacket->IoDriverCreateContext.TxnParameters        = NULL;

		if (DeviceObject)
		{
			pOpenPacket->IoDriverCreateContext.DeviceObjectHint = DeviceObject;
			InternalFlags |= IOP_CREATE_USE_TOP_DEVICE_OBJECT_HINT;
		}

		pOpenPacket->InternalFlags          = InternalFlags;

		pOpenPacket->FinalStatus			= STATUS_SUCCESS;
		pOpenPacket->FileObject				= (PFILE_OBJECT)NULL;

		{
			//恢复Object CallBack
			bSetObjFunc = KeFileSetObjectFunction(*IoFileObjectType, g_emObjFileFunction, emObjParseOper, &pFileObjRetFunc, TRUE);
			if (bSetObjFunc && pFileObjRetFunc != NULL)
			{
				ulFileOrDevice |= RESET_FILE_OBJECT_CALLBACK;
			}

			bSetObjFunc = KeFileSetObjectFunction(g_pDevObjectTypePointer, g_emObjDeviceFunction, emObjParseOper, &pDeviceObjRetFunc, TRUE);
			if (bSetObjFunc && pDeviceObjRetFunc != NULL)
			{
				ulFileOrDevice |= RESET_DEVICE_OBJECT_CALLBACK;
			}

			if ( g_osVersionCheck == WINDOWS_VERSION_VISTA || g_osVersionCheck == WINDOWS_VERSION_2008 )
			{
				nsStatus = ObOpenObjectByName(
					ObjectAttributes,
					NULL,
					kRrequestorMode,
					NULL,
					DesiredAccess,
					pOpenPacket,
					&hHandle
					);
			}
			else
			{
				nsStatus = ObOpenObjectByName(
					ObjectAttributes,
					*IoFileObjectType,
					kRrequestorMode,
					NULL,
					DesiredAccess,
					pOpenPacket,
					&hHandle
					);
			}

			//恢复设置
			if (ulFileOrDevice & RESET_FILE_OBJECT_CALLBACK)
			{
				KeFileSetObjectFunction(*IoFileObjectType, g_emObjFileFunction, emObjParseOper, &pFileObjRetFunc, FALSE);
				ulFileOrDevice &= ~RESET_FILE_OBJECT_CALLBACK;
			}

			if (ulFileOrDevice & RESET_DEVICE_OBJECT_CALLBACK)
			{
				KeFileSetObjectFunction(g_pDevObjectTypePointer, g_emObjDeviceFunction, emObjParseOper, &pDeviceObjRetFunc, FALSE);
				ulFileOrDevice &= ~RESET_DEVICE_OBJECT_CALLBACK;
			}
		}

		BDKitFreePool(pOpenPacket->EaBuffer);
		pOpenPacket->EaLength = 0;

		bSuccessfulIoParse = (BOOLEAN)(pOpenPacket->ParseCheck == OPEN_PACKET_PATTERN);

		if ( !NT_SUCCESS(nsStatus) || bSuccessfulIoParse == FALSE ) 
		{

			if (NT_SUCCESS(nsStatus)) 
			{
				BDKitCloseHandle(hHandle);	
				nsStatus = STATUS_OBJECT_TYPE_MISMATCH;
			}

			if ( !NT_SUCCESS(pOpenPacket->FinalStatus) ) 
			{
				nsStatus = pOpenPacket->FinalStatus;
				if ( NT_WARNING(nsStatus) ) 
				{
					__try 
					{
						IoStatusBlock->Status = pOpenPacket->FinalStatus;
						IoStatusBlock->Information = pOpenPacket->Information;

					} 
					__except(EXCEPTION_EXECUTE_HANDLER) 
					{
						nsStatus = GetExceptionCode();
					}
				}
			} 
			else if ( pOpenPacket->FileObject != NULL && !bSuccessfulIoParse ) 
			{
				if ( pOpenPacket->FileObject->FileName.Length != 0 ) 
				{
					BDKitFreePool(pOpenPacket->FileObject->FileName.Buffer);
					pOpenPacket->FileObject->FileName.Length = 0;
				}

				pOpenPacket->FileObject->DeviceObject = (PDEVICE_OBJECT)NULL;
				BDKitCloseObject(pOpenPacket->FileObject);
			}

			if ((nsStatus == STATUS_OBJECT_NAME_NOT_FOUND) && (pOpenPacket->Information == IO_REPARSE_TAG_MOUNT_POINT)) 
			{
				nsStatus = STATUS_REPARSE_POINT_NOT_RESOLVED;
			}
		}
		else 
		{
			pOpenPacket->FileObject->Flags |= FO_HANDLE_CREATED;

			ASSERT( pOpenPacket->FileObject->Type == IO_TYPE_FILE );

			*FileHandle = hHandle;

			__try 
			{
				IoStatusBlock->Information = pOpenPacket->Information;
				IoStatusBlock->Status = pOpenPacket->FinalStatus;
				nsStatus = pOpenPacket->FinalStatus;
			} 
			__except(EXCEPTION_EXECUTE_HANDLER) 
			{
				nsStatus = GetExceptionCode();
			}
		}

		BDKit_If_DoAction(bSuccessfulIoParse == TRUE,
			BDKitCloseObject(pOpenPacket->FileObject));

	} while (FALSE);

	BDKit_If_DoAction(pOpenPacket != NULL, BDKitFreePool(pOpenPacket->EaBuffer));
	BDKitFreePool(pOpenPacket);

	return nsStatus;
}

NTSTATUS IopCreateFileWin8(
						   __out PHANDLE				FileHandle,
						   __in ACCESS_MASK			DesiredAccess,
						   __in POBJECT_ATTRIBUTES	ObjectAttributes,
						   __out PIO_STATUS_BLOCK		IoStatusBlock,
						   __in PLARGE_INTEGER		AllocationSize OPTIONAL,
						   __in ULONG					FileAttributes,
						   __in ULONG					ShareAccess,
						   __in ULONG					Disposition,
						   __in ULONG					CreateOptions,
						   __in PVOID					EaBuffer OPTIONAL,
						   __in ULONG					EaLength,
						   __in CREATE_FILE_TYPE		CreateFileType,
						   __in PVOID					ExtraCreateParameters OPTIONAL,
						   __in ULONG					Options,
						   __in ULONG					InternalFlags,
						   __in PVOID					DeviceObject
						   )
{
	KPROCESSOR_MODE		kRrequestorMode		= KernelMode;
	NTSTATUS			nsStatus			= STATUS_UNSUCCESSFUL;
	HANDLE				hHandle				= NULL;
	POPEN_PACKETWIN8	pOpenPacket			= NULL;
	PVOID				pFileObjRetFunc		= NULL;
	PVOID				pDeviceObjRetFunc	= NULL;
	BOOLEAN				bSuccessfulIoParse	= FALSE;
	BOOL				bSetObjFunc			= FALSE;
	ULONG				ulFileOrDevice		= 0;
	LARGE_INTEGER	liInitialAllocationSize = {0x00};

	do 
	{
		kRrequestorMode = KernelMode;

		if (Options & IO_NO_PARAMETER_CHECKING)
		{
			kRrequestorMode = KernelMode;
		}

		if (AllocationSize != NULL) 
		{
			liInitialAllocationSize.QuadPart = AllocationSize->QuadPart;
		} 
		else 
		{
			liInitialAllocationSize.QuadPart = 0;
		}

		BDKitAllocateNonpagePool(pOpenPacket, sizeof(*pOpenPacket));
		BDKit_If_Not_Break(pOpenPacket != NULL);

		if (EaBuffer != NULL && EaLength != 0) 
		{
			ULONG errorOffset = 0;

			BDKitAllocatePagePool(pOpenPacket->EaBuffer, EaLength);
			BDKit_If_Not_Break(pOpenPacket->EaBuffer != NULL);

			pOpenPacket->EaLength = EaLength;

			__try
			{
				RtlCopyMemory(pOpenPacket->EaBuffer, EaBuffer, EaLength);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				nsStatus= STATUS_INVALID_PARAMETER;
				BDKit_If_Not_Break(FALSE);	
			}

			nsStatus = IoCheckEaBufferValidity((PFILE_FULL_EA_INFORMATION)pOpenPacket->EaBuffer, EaLength, &errorOffset);
			if (!NT_SUCCESS(nsStatus)) 
			{
				IoStatusBlock->Status      = nsStatus;
				IoStatusBlock->Information = errorOffset;
				BDKit_If_Not_Break(FALSE);
			}
		} 
		else 
		{
			pOpenPacket->EaBuffer = (PVOID)NULL;
			pOpenPacket->EaLength = 0L;
		}

		pOpenPacket->Type					= IO_TYPE_OPEN_PACKET;
		pOpenPacket->Size					= sizeof(*pOpenPacket);
		pOpenPacket->ParseCheck				= 0L;
		pOpenPacket->AllocationSize			= liInitialAllocationSize;
		pOpenPacket->CreateOptions			= CreateOptions;
		pOpenPacket->FileAttributes			= (USHORT) FileAttributes;
		pOpenPacket->ShareAccess			= (USHORT) ShareAccess;
		pOpenPacket->Disposition			= Disposition;
		pOpenPacket->Override				= FALSE;
		pOpenPacket->QueryOnly				= FALSE;
		pOpenPacket->DeleteOnly				= FALSE;
		pOpenPacket->Options				= Options;
		pOpenPacket->RelatedFileObject		= (PFILE_OBJECT)NULL;
		pOpenPacket->CreateFileType			= CreateFileType;
		pOpenPacket->ExtraCreateParameters	= ExtraCreateParameters;

		pOpenPacket->OriginalAttributes		= ObjectAttributes;

		InternalFlags = 0;

		if (Options & IO_IGNORE_SHARE_ACCESS_CHECK)
		{
			InternalFlags |= IOP_CREATE_IGNORE_SHARE_ACCESS_CHECK;
		}

		//win7新增的结构
		pOpenPacket->IoDriverCreateContext.Size                 = sizeof(IO_DRIVER_CREATE_CONTEXT);
		pOpenPacket->IoDriverCreateContext.ExtraCreateParameter = NULL;
		pOpenPacket->IoDriverCreateContext.DeviceObjectHint     = NULL;
		pOpenPacket->IoDriverCreateContext.TxnParameters        = NULL;
		pOpenPacket->kRrequestorMode                            = kRrequestorMode;

		if (DeviceObject)
		{
			pOpenPacket->IoDriverCreateContext.DeviceObjectHint = DeviceObject;
			InternalFlags |= IOP_CREATE_USE_TOP_DEVICE_OBJECT_HINT;
		}

		pOpenPacket->InternalFlags          = InternalFlags;

		pOpenPacket->FinalStatus			= STATUS_SUCCESS;
		pOpenPacket->FileObject				= (PFILE_OBJECT)NULL;

		if (DeviceObject)
		{
			pOpenPacket->IoDriverCreateContext.DeviceObjectHint = DeviceObject;
			InternalFlags |= IOP_CREATE_USE_TOP_DEVICE_OBJECT_HINT;
		}

		pOpenPacket->InternalFlags  = InternalFlags;
		pOpenPacket->FinalStatus = STATUS_SUCCESS;
		pOpenPacket->FileObject  = (PFILE_OBJECT)NULL;

		{
			//恢复Object CallBack
			bSetObjFunc = KeFileSetObjectFunction(*IoFileObjectType, g_emObjFileFunction, emObjParseOper, &pFileObjRetFunc, TRUE);
			if (bSetObjFunc && pFileObjRetFunc != NULL)
			{
				ulFileOrDevice |= RESET_FILE_OBJECT_CALLBACK;
			}

			bSetObjFunc = KeFileSetObjectFunction(g_pDevObjectTypePointer, g_emObjDeviceFunction, emObjParseOper, &pDeviceObjRetFunc, TRUE);
			if (bSetObjFunc && pDeviceObjRetFunc != NULL)
			{
				ulFileOrDevice |= RESET_DEVICE_OBJECT_CALLBACK;
			}

			nsStatus = ObOpenObjectByName(
				ObjectAttributes,
				*IoFileObjectType,
				kRrequestorMode,
				NULL,
				DesiredAccess,
				pOpenPacket,
				&hHandle
				);

			//恢复设置
			if (ulFileOrDevice & RESET_FILE_OBJECT_CALLBACK)
			{
				KeFileSetObjectFunction(*IoFileObjectType, g_emObjFileFunction, emObjParseOper, &pFileObjRetFunc, FALSE);
				ulFileOrDevice &= ~RESET_FILE_OBJECT_CALLBACK;
			}

			if (ulFileOrDevice & RESET_DEVICE_OBJECT_CALLBACK)
			{
				KeFileSetObjectFunction(g_pDevObjectTypePointer, g_emObjDeviceFunction, emObjParseOper, &pDeviceObjRetFunc, FALSE);
				ulFileOrDevice &= ~RESET_DEVICE_OBJECT_CALLBACK;
			}
		}

		BDKitFreePool(pOpenPacket->EaBuffer);
		pOpenPacket->EaLength = 0;

		bSuccessfulIoParse = (BOOLEAN)(pOpenPacket->ParseCheck == OPEN_PACKET_PATTERN);

		if ( !NT_SUCCESS(nsStatus) || bSuccessfulIoParse == FALSE ) 
		{

			if (NT_SUCCESS(nsStatus)) 
			{
				BDKitCloseHandle(hHandle);	
				nsStatus = STATUS_OBJECT_TYPE_MISMATCH;
			}

			if ( !NT_SUCCESS(pOpenPacket->FinalStatus) ) 
			{
				nsStatus = pOpenPacket->FinalStatus;
				if ( NT_WARNING(nsStatus) ) 
				{
					__try 
					{
						IoStatusBlock->Status = pOpenPacket->FinalStatus;
						IoStatusBlock->Information = pOpenPacket->Information;

					} 
					__except(EXCEPTION_EXECUTE_HANDLER) 
					{
						nsStatus = GetExceptionCode();
					}
				}
			} 
			else if ( pOpenPacket->FileObject != NULL && !bSuccessfulIoParse ) 
			{
				if ( pOpenPacket->FileObject->FileName.Length != 0 ) 
				{
					BDKitFreePool(pOpenPacket->FileObject->FileName.Buffer);
					pOpenPacket->FileObject->FileName.Length = 0;
				}

				pOpenPacket->FileObject->DeviceObject = (PDEVICE_OBJECT)NULL;
				BDKitCloseObject(pOpenPacket->FileObject);
			}

			if ((nsStatus == STATUS_OBJECT_NAME_NOT_FOUND) && (pOpenPacket->Information == IO_REPARSE_TAG_MOUNT_POINT)) 
			{
				nsStatus = STATUS_REPARSE_POINT_NOT_RESOLVED;
			}
		}
		else 
		{
			pOpenPacket->FileObject->Flags |= FO_HANDLE_CREATED;

			ASSERT( pOpenPacket->FileObject->Type == IO_TYPE_FILE );

			*FileHandle = hHandle;

			__try 
			{
				IoStatusBlock->Information = pOpenPacket->Information;
				IoStatusBlock->Status = pOpenPacket->FinalStatus;
				nsStatus = pOpenPacket->FinalStatus;
			} 
			__except(EXCEPTION_EXECUTE_HANDLER) 
			{
				nsStatus = GetExceptionCode();
			}
		}

		BDKit_If_DoAction(bSuccessfulIoParse == TRUE,
			BDKitCloseObject(pOpenPacket->FileObject));
	} while(FALSE);

	BDKit_If_DoAction(pOpenPacket != NULL, BDKitFreePool(pOpenPacket->EaBuffer));
	BDKitFreePool(pOpenPacket);

	return nsStatus;
}

NTSTATUS BDKitCreateFileByObject( 
								 __out PHANDLE FileHandle, 
								 __in ACCESS_MASK DesiredAccess, 
								 __in POBJECT_ATTRIBUTES ObjectAttributes, 
								 __out PIO_STATUS_BLOCK IoStatusBlock, 
								 __in PLARGE_INTEGER AllocationSize OPTIONAL, 
								 __in ULONG FileAttributes, __in ULONG ShareAccess,
								 __in ULONG Disposition, __in ULONG CreateOptions, 
								 __in PVOID EaBuffer OPTIONAL, __in ULONG EaLength, 
								 __in CREATE_FILE_TYPE CreateFileType, 
								 __in PVOID ExtraCreateParameters OPTIONAL, 
								 __in ULONG Options, __in ULONG InternalFlags, 
								 __in PVOID DeviceObject
								 )
{
	NTSTATUS nsStatus	= STATUS_UNSUCCESSFUL;

	do 
	{
		nsStatus = STATUS_INVALID_PARAMETER;

		//参数检查
		if (Options & IO_CHECK_CREATE_PARAMETERS) 
		{
			if (	(FileAttributes & ~FILE_ATTRIBUTE_VALID_FLAGS)
				||	(ShareAccess	& ~FILE_SHARE_VALID_FLAGS)
				||	(Disposition	> FILE_MAXIMUM_DISPOSITION)
				||	(CreateOptions	& ~FILE_VALID_OPTION_FLAGS)
				||	(CreateOptions	& (FILE_SYNCHRONOUS_IO_ALERT | FILE_SYNCHRONOUS_IO_NONALERT) &&	(!(DesiredAccess& SYNCHRONIZE)))
				||	(CreateOptions	& FILE_DELETE_ON_CLOSE && (!(DesiredAccess & DELETE)))
				||	((CreateOptions & (FILE_SYNCHRONOUS_IO_ALERT | FILE_SYNCHRONOUS_IO_NONALERT)) == (FILE_SYNCHRONOUS_IO_ALERT | FILE_SYNCHRONOUS_IO_NONALERT))
				||	((CreateOptions & FILE_DIRECTORY_FILE) && !(CreateOptions & FILE_NON_DIRECTORY_FILE) 
				&&	(
				(CreateOptions & ~(FILE_DIRECTORY_FILE |
				FILE_SYNCHRONOUS_IO_ALERT |
				FILE_SYNCHRONOUS_IO_NONALERT |
				FILE_WRITE_THROUGH |
				FILE_COMPLETE_IF_OPLOCKED |
				FILE_OPEN_FOR_BACKUP_INTENT |
				FILE_DELETE_ON_CLOSE |
				FILE_OPEN_FOR_FREE_SPACE_QUERY |
				FILE_OPEN_BY_FILE_ID |
				FILE_NO_COMPRESSION|
				FILE_OPEN_REPARSE_POINT))
				||	((Disposition != FILE_CREATE) && (Disposition != FILE_OPEN) && (Disposition != FILE_OPEN_IF))
				)
				)
				||	((CreateOptions & FILE_COMPLETE_IF_OPLOCKED) && (CreateOptions & FILE_RESERVE_OPFILTER))
				||	(!DesiredAccess)
				||	(CreateOptions & FILE_NO_INTERMEDIATE_BUFFERING && (DesiredAccess & FILE_APPEND_DATA))
				||	CreateFileTypeNone != CreateFileTypeNone) 

			{
				BDKit_If_Not_Break(FALSE);	
			}
		}

		if (CreateOptions & IO_ATTACH_DEVICE_API) 
		{
			Options |= IO_ATTACH_DEVICE;
			CreateOptions &= ~IO_ATTACH_DEVICE_API;
		}

		if ( g_osVersionCheck >= WINDOWS_VERSION_8 && g_osVersionCheck <= WINDOWS_VERSION_8_1 )
		{
			nsStatus = IopCreateFileWin8 (
				FileHandle,
				DesiredAccess,
				ObjectAttributes,
				IoStatusBlock,
				AllocationSize,
				FileAttributes,
				ShareAccess,
				Disposition,
				CreateOptions,
				EaBuffer,
				EaLength,
				CreateFileType,
				ExtraCreateParameters,
				Options,
				InternalFlags,
				DeviceObject
				);

		} else if ( g_osVersionCheck >= WINDOWS_VERSION_VISTA && g_osVersionCheck <= WINDOWS_VERSION_2008 )
		{
			nsStatus = IopCreateFileWin7 (
				FileHandle,
				DesiredAccess,
				ObjectAttributes,
				IoStatusBlock,
				AllocationSize,
				FileAttributes,
				ShareAccess,
				Disposition,
				CreateOptions,
				EaBuffer,
				EaLength,
				CreateFileType,
				ExtraCreateParameters,
				Options,
				InternalFlags,
				DeviceObject
				);

		} else if ( g_osVersionCheck >= WINDOWS_VERSION_2K && g_osVersionCheck < WINDOWS_VERSION_VISTA )
		{
			nsStatus = IopCreateFileXp2003(
				FileHandle, 
				DesiredAccess,
				ObjectAttributes,
				IoStatusBlock,
				AllocationSize,
				FileAttributes,
				ShareAccess,
				Disposition,
				CreateOptions,
				EaBuffer,
				EaLength,
				CreateFileType,
				ExtraCreateParameters,
				Options,
				InternalFlags,
				DeviceObject
				);

		} else
		{
			nsStatus = STATUS_NOT_SUPPORTED;
		}

	} while (FALSE);

	return nsStatus;
}

NTSTATUS IrpCreateFile(
					   __in PUNICODE_STRING	puszFilePath,
					   __in ACCESS_MASK		DesireAccess,
					   __in ULONG				FileAttributes,
					   __in ULONG				ShareAccess,
					   __in ULONG				CreateDisposition,
					   __in ULONG				CreateOptions,
					   __in PDEVICE_OBJECT	DeviceObject,
					   __in PDEVICE_OBJECT	RealDeviceObj,
					   __out PFILE_OBJECT*	pFileObject
					   )
{
	NTSTATUS				nsStatus			= STATUS_UNSUCCESSFUL;
	WCHAR*					pFileNameBuffer		= NULL;
	PIRP					Irp					= NULL;
	PIO_STACK_LOCATION		IrpSp				= NULL;
	PFILE_OBJECT			_FileObject			= NULL;
	OBJECT_ATTRIBUTES		ObjectAttributes	= {0x00};
	IO_STATUS_BLOCK			IoStatus		= {0x00};
	KEVENT					kEvent				= {0x00};
	ACCESS_STATE			AccessState			= {0x00};
	AUX_ACCESS_DATA	AuxData				= {0x00};
	IO_SECURITY_CONTEXT		SecurityContext		= {0x00};
	WORD					FileObjectSize		= 0;

	switch(g_osVersionCheck)
	{
	case WINDOWS_VERSION_2K:
	case WINDOWS_VERSION_XP:
	case WINDOWS_VERSION_2K3:
	case WINDOWS_VERSION_2K3_SP1_SP2:
		{
			FileObjectSize = 0x70;
		}
		break;

	case WINDOWS_VERSION_VISTA:
	case WINDOWS_VERSION_2008:
	case WINDOWS_VERSION_7:
		{
			FileObjectSize = 0x80;
		}
		break;

	default:
		{
			// TODO
		}
	}

	do 
	{
		InitializeObjectAttributes(&ObjectAttributes, NULL, OBJ_CASE_INSENSITIVE, NULL, NULL);
		nsStatus = ObCreateObject (
			KernelMode,
			*IoFileObjectType,
			&ObjectAttributes,
			KernelMode,
			NULL,
			FileObjectSize,
			0,
			0,
			&_FileObject);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		Irp = IoAllocateIrp (DeviceObject->StackSize, FALSE);
		BDKit_If_Not_Break(Irp != NULL);

		KeInitializeEvent (&kEvent, SynchronizationEvent, FALSE);

		RtlZeroMemory(_FileObject, FileObjectSize);
		_FileObject->Type = IO_TYPE_FILE;
		_FileObject->Size = FileObjectSize;
		_FileObject->DeviceObject = RealDeviceObj;
		_FileObject->RelatedFileObject = NULL;
		_FileObject->Flags= FO_SYNCHRONOUS_IO;

		BDKitAllocateNonpagePool(pFileNameBuffer, puszFilePath->MaximumLength);
		BDKit_If_Not_Break(pFileNameBuffer != NULL);
		RtlCopyMemory(pFileNameBuffer, puszFilePath->Buffer, puszFilePath->Length);

		_FileObject->FileName.Buffer = pFileNameBuffer;
		_FileObject->FileName.Length = puszFilePath->Length;
		_FileObject->FileName.MaximumLength = puszFilePath->MaximumLength;

		if (	g_osVersionCheck == WINDOWS_VERSION_7 
			||	g_osVersionCheck == WINDOWS_VERSION_VISTA 
			||	g_osVersionCheck == WINDOWS_VERSION_2008 
			)
		{
			PLIST_ENTRY IrpList;
			IrpList = (PLIST_ENTRY)((ULONG)_FileObject + 0x74);
			IrpList->Flink = IrpList;
			IrpList->Blink = IrpList;
		}

		KeInitializeEvent (&_FileObject->Lock,  SynchronizationEvent, FALSE);
		KeInitializeEvent (&_FileObject->Event, NotificationEvent, FALSE);

		RtlZeroMemory(&AuxData, sizeof(AuxData));
		nsStatus = SeCreateAccessState (
			&AccessState,
			&AuxData,
			DesireAccess,
			IoGetFileObjectGenericMapping ()
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		SecurityContext.SecurityQos = NULL;
		SecurityContext.AccessState = &AccessState;
		SecurityContext.DesiredAccess = DesireAccess;
		SecurityContext.FullCreateOptions = 0;

		Irp->MdlAddress			= NULL;
		Irp->AssociatedIrp.SystemBuffer = NULL;
		Irp->Flags = IRP_CREATE_OPERATION | IRP_SYNCHRONOUS_API;
		Irp->RequestorMode		= KernelMode;
		Irp->UserIosb			= &IoStatus;
		Irp->UserEvent			= &kEvent;
		Irp->PendingReturned	= FALSE;
		Irp->Cancel				= FALSE;
		Irp->CancelRoutine		= NULL;
		Irp->Tail.Overlay.Thread= (PETHREAD)KeGetCurrentThread();
		Irp->Tail.Overlay.AuxiliaryBuffer = NULL;
		Irp->Tail.Overlay.OriginalFileObject = _FileObject;

		IrpSp = IoGetNextIrpStackLocation (Irp);
		IrpSp->MajorFunction	= IRP_MJ_CREATE;
		IrpSp->DeviceObject		= DeviceObject;
		IrpSp->FileObject		= _FileObject;
		IrpSp->Parameters.Create.SecurityContext	= &SecurityContext;
		IrpSp->Parameters.Create.Options			= (CreateDisposition << 24) | CreateOptions;
		IrpSp->Parameters.Create.FileAttributes		= (USHORT)FileAttributes;
		IrpSp->Parameters.Create.ShareAccess		= (USHORT)ShareAccess;
		IrpSp->Parameters.Create.EaLength			= 0;

		IoSetCompletionRoutine (Irp, BDKitIoCompletionRoutine, NULL, TRUE, TRUE, TRUE);

		nsStatus = IoCallDriver(DeviceObject, Irp);
		BDKit_If_DoAction(STATUS_PENDING, 
			KeWaitForSingleObject (&kEvent, Executive, KernelMode, TRUE, 0));

		// 完成函数已经释放
		Irp = NULL;

		nsStatus = IoStatus.Status;
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		InterlockedIncrement(&_FileObject->DeviceObject->ReferenceCount);
		if ( _FileObject->Vpb != NULL )
		{
			InterlockedIncrement((LONG volatile*)&_FileObject->Vpb->ReferenceCount);
		}
		*pFileObject = _FileObject;

		nsStatus = STATUS_SUCCESS;
	} while (FALSE);

	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), BDKitFreePool(pFileNameBuffer));
	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), BDKitCloseObject(_FileObject));
	BDKitIoCompletionRoutine(NULL, Irp, NULL);

	return nsStatus;
}

// 输入路径不能带有盘符
NTSTATUS BDKitOpenFile(
					   __in WCHAR*		pwszFileNodePath,
					   __in ACCESS_MASK	DesiredAccess,
					   __in ULONG			FileAttributes,
					   __in ULONG			ShareAccess,
					   __in ULONG			CreateDisposition,
					   __in ULONG			CreateOptions,
					   __out PHANDLE		FileHandle
					   )
{
	NTSTATUS		nsStatus		= STATUS_UNSUCCESSFUL;
	UNICODE_STRING	uszFilePath		= {0x00};
	PDEVICE_OBJECT	RealDeviceObj	= NULL;
	PDEVICE_OBJECT	DeviceObject	= NULL;
	PFILE_OBJECT	pFileObject		= NULL;

	do 
	{
		nsStatus = BDKitGetDeviceObjectFromFileFullName(
			pwszFileNodePath,
			&RealDeviceObj,
			&DeviceObject
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		// 不能带有盘符
		RtlInitUnicodeString(&uszFilePath,&pwszFileNodePath[2]);
		nsStatus = IrpCreateFile(
			&uszFilePath,
			DesiredAccess,
			FileAttributes,
			ShareAccess,
			CreateDisposition,
			CreateOptions,
			DeviceObject,
			RealDeviceObj,
			&pFileObject
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ObOpenObjectByPointer(
			pFileObject,
			OBJ_KERNEL_HANDLE,    //verifier下测试要指定OBJ_KERNEL_HANDLE
			NULL,
			DesiredAccess | SYNCHRONIZE,
			*IoFileObjectType,
			KernelMode,
			FileHandle);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
	} while (FALSE);

	BDKitCloseObject(pFileObject);

	return nsStatus;
}

NTSTATUS BDKitCreateFileByAPI(
							  __out PHANDLE				FileHandle,
							  __in ACCESS_MASK			DesiredAccess,
							  __in POBJECT_ATTRIBUTES	ObjectAttributes,
							  __out PIO_STATUS_BLOCK	IoStatusBlock,
							  __in ULONG  FileAttributes,
							  __in ULONG  ShareAccess,
							  __in ULONG  Disposition,
							  __in ULONG  CreateOptions
							  )
{
	return IoCreateFile (
		FileHandle,
		DesiredAccess,
		ObjectAttributes,
		IoStatusBlock,
		NULL,
		FileAttributes,
		ShareAccess,
		Disposition,
		CreateOptions,
		NULL,
		0,
		CreateFileTypeNone,
		NULL,
		IO_NO_PARAMETER_CHECKING
		);
}

typedef struct _FILE_HANDLE_INFO
{
	PHANDLE		pFileHandle;
	ACCESS_MASK	DesiredAccess;
	ULONG		FileAttributes;
	ULONG		ShareAccess;
	ULONG		Disposition;
} FILE_HANDLE_INFO, *PFILE_HANDLE_INFO;

VOID DuplicateFileHandle(
						 __in CONST PSYSTEM_HANDLE_INFORMATION pHandleInfo,
						 __in PVOID pContext
						 )
{
	NTSTATUS			nsStatus	= STATUS_SUCCESS;
	PFILE_HANDLE_INFO	pUserInfo	= (PFILE_HANDLE_INFO)pContext;
	HANDLE				hDupHandle	= NULL;
	do 
	{
		BDKit_If_Not_Break(pHandleInfo != NULL && pUserInfo != NULL);
		BDKit_If_Not_Break(*pUserInfo->pFileHandle == NULL);

		nsStatus = BDKitDuplicateHandle (
			(HANDLE)pHandleInfo->ProcessId,
			PsGetCurrentProcessId(),
			(HANDLE)pHandleInfo->Handle,
			&hDupHandle
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKit_If_DoAction(hDupHandle != NULL, *pUserInfo->pFileHandle = hDupHandle);
}

NTSTATUS BDKitCreateFileByDupHandle(
									__out PHANDLE			FileHandle,
									__in ACCESS_MASK		DesiredAccess,
									__in POBJECT_ATTRIBUTES	ObjectAttributes,
									__out PIO_STATUS_BLOCK	IoStatusBlock,
									__in ULONG  FileAttributes,
									__in ULONG  ShareAccess,
									__in ULONG  Disposition,
									__in ULONG  CreateOptions
									)
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	FILE_HANDLE_INFO	HandleInfo	= {0x00};

	UNREFERENCED_PARAMETER(IoStatusBlock);
	UNREFERENCED_PARAMETER(Disposition);
	UNREFERENCED_PARAMETER(CreateOptions);

	*FileHandle					= NULL;
	HandleInfo.pFileHandle		= FileHandle;
	HandleInfo.DesiredAccess	= DesiredAccess;
	HandleInfo.FileAttributes	= FileAttributes;
	HandleInfo.ShareAccess		= ShareAccess;

	nsStatus = BDKitExectiveFileHandle (ObjectAttributes->ObjectName, DuplicateFileHandle, &HandleInfo);

	return nsStatus;
}