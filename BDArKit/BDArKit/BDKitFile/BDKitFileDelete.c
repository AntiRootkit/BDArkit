#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "DriverHelper.h"
#include "Utility.h"
#include "NtosKrnl.h"
#include "PELoader.h"
#include "List.h"
#include "BDKitUtils.h"


typedef struct _OBJECT_HANDLE_FLAG_INFORMATION 
{
	BOOLEAN Inherit;
	BOOLEAN ProtectFromClose;
} OBJECT_HANDLE_FLAG_INFORMATION, *POBJECT_HANDLE_FLAG_INFORMATION;

#if (VER_PRODUCTBUILD < 7600)
typedef struct _FILE_MOVE_CLUSTER_INFORMATION
{
	ULONG ClusterCount;
	HANDLE RootDirectory;
	ULONG FileNameLength;
	WCHAR FileName[1];
} FILE_MOVE_CLUSTER_INFORMATION, *PFILE_MOVE_CLUSTER_INFORMATION;
#endif

NTSTATUS BDKitDeleteFileByAPI (__in CONST PUNICODE_STRING puszFileFullPath)
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	OBJECT_ATTRIBUTES	oa			= {0x00};

	switch(g_osVersionCheck)
	{
	case WINDOWS_VERSION_XP:
	case WINDOWS_VERSION_2K3:
	case WINDOWS_VERSION_2K3_SP1_SP2:
	case WINDOWS_VERSION_VISTA:
	case WINDOWS_VERSION_2008:
	case WINDOWS_VERSION_7:
		{
			InitializeObjectAttributes(
				&oa,
				puszFileFullPath,
				OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
				NULL,
				NULL
				);

			nsStatus = ZwDeleteFile (&oa); 
		}
		break;

	case WINDOWS_VERSION_2K:
		{
			// TODO
			nsStatus = STATUS_NOT_SUPPORTED;
		}
		break;

	default:
		{
			// TODO
		}
	}

	return nsStatus;
}

NTSTATUS BDKitDeleteFileByIRP (__in CONST PUNICODE_STRING puszFileFullPath)
{
	NTSTATUS						nsStatus	= STATUS_SUCCESS;
	HANDLE							hFile		= NULL;
	OBJECT_ATTRIBUTES				oa			= {0x00};
	IO_STATUS_BLOCK					IoStatus	= {0x00};
	FILE_DISPOSITION_INFORMATION    FileDispInfo= {0x00};
	FILE_END_OF_FILE_INFORMATION	EndofFileInfo	= {0x00};
	FILE_BASIC_INFORMATION			FileAttributes	= {0x00};

	do 
	{
		InitializeObjectAttributes(
			&oa,
			puszFileFullPath,
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
			NULL,
			NULL
			);

		nsStatus = BDKitCreateFileByObject (
			&hFile,
			SYNCHRONIZE | FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES,
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
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		FileDispInfo.DeleteFile = TRUE;
		EndofFileInfo.EndOfFile.LowPart = 0;
		EndofFileInfo.EndOfFile.HighPart= 0;

		nsStatus = ZwQueryInformationFile (hFile, &IoStatus, &FileAttributes, sizeof(FileAttributes), FileBasicInformation);
		FileAttributes.FileAttributes = FILE_ATTRIBUTE_NORMAL;
		nsStatus = ZwSetInformationFile (hFile, &IoStatus, &FileAttributes, sizeof(FileAttributes), FileBasicInformation);
		nsStatus = ZwSetInformationFile (hFile, &IoStatus, &EndofFileInfo, sizeof(EndofFileInfo), FileEndOfFileInformation);
		nsStatus = ZwSetInformationFile (hFile, &IoStatus, &FileDispInfo, sizeof(FileDispInfo), FileDispositionInformation);
	}while(FALSE);

	BDKitCloseHandle(hFile);

	return nsStatus;
}

VOID CloseHandleRoutine(PVOID pParam)
{
	HANDLE		hCloseHandle = pParam;
	NTSTATUS	nsStatus = STATUS_UNSUCCESSFUL;
	OBJECT_HANDLE_FLAG_INFORMATION ofi = {0x00};

	do 
	{
		nsStatus = ZwSetInformationObject(hCloseHandle, 4, &ofi, sizeof(ofi));

		if (IsKernelHandle(hCloseHandle, KernelMode))
		{
			nsStatus = ZwClose (hCloseHandle);
		}
		else
		{
			// ZwClose 如果Close一个free或者无效handle在debug下会触发蓝屏
			nsStatus = NtClose (hCloseHandle);
		}
	} while (FALSE);

}

VOID DeferFileHandle(
					 __in CONST PSYSTEM_HANDLE_INFORMATION pHandleInfo,
					 __in PVOID pContext
					 )
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	KAPC_STATE	kApcState	= {0x00};	
	PEPROCESS	EProcess	= NULL;

	UNREFERENCED_PARAMETER(pContext);

	if ( (HANDLE)pHandleInfo->ProcessId == PsGetCurrentProcessId () )
	{
		ULONG_PTR hCloseHandle = pHandleInfo->Handle;

		BDKit_If_DoAction(pHandleInfo->ProcessId == 4 || pHandleInfo->ProcessId == 8, EncodeKernelHandle(hCloseHandle));
		CloseHandleRoutine((PVOID)hCloseHandle);
	}
	else
	{
		nsStatus = PsLookupProcessByProcessId(
			(HANDLE)pHandleInfo->ProcessId,
			&EProcess
			);
		if ( NT_SUCCESS(nsStatus) )
		{
			ULONG_PTR hCloseHandle = pHandleInfo->Handle;

			nsStatus = _KeStackAttachProcess ((PKPROCESS)EProcess, &kApcState);
			if ( NT_SUCCESS(nsStatus) )
			{
				{
					BDKit_If_DoAction(pHandleInfo->ProcessId == 4 || pHandleInfo->ProcessId == 8, EncodeKernelHandle(hCloseHandle));
					CloseHandleRoutine((PVOID)hCloseHandle);
				}
				_KeUnstackDetachProcess ((PKPROCESS)EProcess, &kApcState);
			}

			BDKitCloseObject(EProcess);
		}
	}
}

NTSTATUS BDKitDerefHandleByCloseHandle (__in CONST PUNICODE_STRING puszFileFullPath)
{
	return BDKitExectiveFileHandle(puszFileFullPath, DeferFileHandle, NULL);
}

NTSTATUS BDKitDeleteFileByClearSection (__in CONST PUNICODE_STRING puszFileFullPath)
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	HANDLE			hFile		= NULL;
	IO_STATUS_BLOCK	IoStatus	= {0x00};
	OBJECT_ATTRIBUTES	oa		= {0x00};

	do 
	{
		InitializeObjectAttributes(
			&oa, 
			puszFileFullPath, 
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 
			NULL, 
			NULL
			);
		nsStatus = BDKitCreateFileByObject (
			&hFile,
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
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && hFile != NULL);

		BDKitClearFileReadOnlyAttribute (hFile);
		BDKitClearFileUnlockAttribute (hFile);

		nsStatus = BDKitClearSection2DeleteFile (hFile);

	} while (FALSE);

	BDKitCloseHandle(hFile);

	return nsStatus;
}

NTSTATUS BDKitClearSection2DeleteFile (__in HANDLE hFile)
{
	NTSTATUS						nsStatus	= STATUS_SUCCESS;
	PFILE_OBJECT					pFileObject	= NULL;
	PDEVICE_OBJECT					pDeviceObj	= NULL;
	PIRP							Irp			= NULL;
	PIO_STACK_LOCATION				IrpSp		= NULL;
	KEVENT							kEvent		= {0x00};
	IO_STATUS_BLOCK					IoStatus	= {0x00};
	FILE_DISPOSITION_INFORMATION	fileDispos	= {0x00};
	SECTION_OBJECT_POINTERS			SectionObjectPointer= {0x00};
	BOOLEAN							bIsNeedReWriteCacheData	= FALSE;

	do 
	{
		nsStatus = ObReferenceObjectByHandle(
			hFile,
			DELETE,
			*IoFileObjectType,
			KernelMode,
			&pFileObject,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		pDeviceObj = BDKitGetFileObjRelatedDeviceObject(pFileObject);
		BDKit_If_Not_Break_With_Reason(pDeviceObj != NULL, STATUS_INVALID_DEVICE_OBJECT_PARAMETER);

		Irp = IoAllocateIrp(pDeviceObj->StackSize, TRUE);
		BDKit_If_Not_Break_With_Reason(Irp != NULL, STATUS_INSUFF_SERVER_RESOURCES);

		KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

		fileDispos.DeleteFile = TRUE;

		Irp->AssociatedIrp.SystemBuffer = &fileDispos;
		Irp->MdlAddress					= NULL;
		Irp->UserEvent					= &kEvent;
		Irp->UserIosb					= &IoStatus;
		Irp->Tail.Overlay.OriginalFileObject = pFileObject;
		Irp->Tail.Overlay.Thread		= PsGetCurrentThread();
		Irp->RequestorMode				= KernelMode;
		Irp->Overlay.AsynchronousParameters.UserApcRoutine = (PIO_APC_ROUTINE)NULL;

		IrpSp = IoGetNextIrpStackLocation(Irp);
		IrpSp->MajorFunction	= IRP_MJ_SET_INFORMATION;
		IrpSp->DeviceObject		= pDeviceObj;
		IrpSp->FileObject		= pFileObject;
#ifdef _X86_
		IrpSp->Parameters.SetFile.DeleteHandle			= hFile;
#else
		IrpSp->Parameters.SetFile.DeleteHandle			= ULongToHandle(HandleToULong(hFile) & 0xFFFFFFFF);
#endif // _X86_
		IrpSp->Parameters.SetFile.Length				= sizeof(FILE_DISPOSITION_INFORMATION);
		IrpSp->Parameters.SetFile.FileInformationClass	= FileDispositionInformation;
		IrpSp->Parameters.SetFile.FileObject			= pFileObject;
		IrpSp->Parameters.SetFile.ClusterCount			= 0;

		IoSetCompletionRoutine (Irp, BDKitIoCompletionRoutine, NULL, TRUE, TRUE, TRUE);

		SectionObjectPointer = *(pFileObject->SectionObjectPointer);

		if(SectionObjectPointer.ImageSectionObject || SectionObjectPointer.DataSectionObject)
		{
			if ( BDKitEnableIATHook () == TRUE )
			{
				BDKitInsertListNode (g_DelFileHandle, &SectionObjectPointer);
			}
			else
			{
				bIsNeedReWriteCacheData = TRUE;
				pFileObject->SectionObjectPointer->DataSectionObject	= NULL;
				pFileObject->SectionObjectPointer->ImageSectionObject	= NULL;
				pFileObject->SectionObjectPointer->SharedCacheMap		= NULL;
			}
		}

		nsStatus = IoCallDriver(pDeviceObj, Irp);
		BDKit_If_DoAction(nsStatus == STATUS_PENDING, 
			KeWaitForSingleObject (&kEvent, Executive, KernelMode, TRUE, 0));

		nsStatus =  IoStatus.Status;

		Irp = NULL;

		if(SectionObjectPointer.ImageSectionObject || SectionObjectPointer.DataSectionObject)
		{
			// 驱动退出的时候再清除IAT HOOK
			if ( bIsNeedReWriteCacheData == FALSE )
			{
				BDKitRemoveListNode(g_DelFileHandle, &SectionObjectPointer);
			}
			else
			{
				if (pFileObject->SectionObjectPointer)
				{
					*(pFileObject->SectionObjectPointer) = SectionObjectPointer;
				}
			}
		}

	} while (FALSE);

	BDKit_If_DoAction(Irp != NULL, BDKitIoCompletionRoutine (NULL, Irp, NULL));	

	BDKitCloseObject(pFileObject);

	return nsStatus;
}

NTSTATUS BDKitDeleteFileByXCB (__in CONST PUNICODE_STRING puszFileFullPath)
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	HANDLE				hFile		= NULL;
	PFILE_OBJECT		pFileObject	= NULL;
	PDEVICE_OBJECT		pLowVolumeDevice = NULL;
	PDRIVER_OBJECT		pLowVolumeDriver = NULL;
	OBJECT_ATTRIBUTES	oa			= {0x00};
	IO_STATUS_BLOCK		IoStatus	= {0x00};
	UNICODE_STRING		uszFastFat	= {0x00};
	UNICODE_STRING		uszNtfs		= {0x00};
	ULONG				fat_fcbCleanupOffset = 0x74u;  
	ULONG				ntfs_ccbLcbOffset = 0x20u;  
	ULONG				ntfs_lcbCleanupOffset = 0x7Cu;  
	ULONG				ntfs_scbCleanupOffset = 0x58u;  
	ULONG				ntfs_scbFcbOffset = 0x48u;  
	ULONG				ntfs_fcbCleanupOffset = 0x10u;  
	PULONG				pFCB		= NULL;
	PULONG				pSCB		= NULL;
	PULONG				pCCB		= NULL;
	PULONG				pLCB		= NULL;
	ULONG				v7			= 0;

	do 
	{
		RtlInitUnicodeString(&uszFastFat, L"\\FileSystem\\FastFat");  
		RtlInitUnicodeString(&uszNtfs, L"\\FileSystem\\Ntfs");  

		InitializeObjectAttributes(
			&oa, 
			puszFileFullPath, 
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 
			NULL, 
			NULL
			);

		nsStatus = BDKitCreateFileByObject (
			&hFile,
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
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ObReferenceObjectByHandle (
			hFile,
			GENERIC_READ,
			*IoFileObjectType,
			KernelMode,
			&pFileObject,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = BDKitDeleteFileByIRP (puszFileFullPath);

		pLowVolumeDevice = IoGetBaseFileSystemDeviceObject (pFileObject);
		BDKit_If_Not_Break(pLowVolumeDevice != NULL && pLowVolumeDevice->DriverObject);

		BDKitCloseHandle(hFile);

		pLowVolumeDriver = pLowVolumeDevice->DriverObject;
		BDKit_If_Not_Break(pLowVolumeDriver != NULL && pLowVolumeDriver->DriverName.Buffer);

		if ( g_osVersionCheck >= WINDOWS_VERSION_XP && g_osVersionCheck <= WINDOWS_VERSION_2K3)
		{
			// FileObject
			//			 --> +0Ch SCB
			//			 			 --> +48h FCB						; by NtfsOpenExistingPrefixFcb
			//			 						 --> +10h CleanupCount	; by NtfsCheckFileForDelete
			//			 			 --> +4Ch VCB						; by NtfsAddScbToFspClose
			//						 --> +58h CleanupCount				; by NtfsOpenAttributeCheck
			//			 --> +10h CCB
			//						 --> +20h LCB						; by NtfsSetDispositionInfo
			//									 --> +7Ch CleanupCount	; by NtfsCombineLcbs
			fat_fcbCleanupOffset	= 0x74u;
			ntfs_ccbLcbOffset		= 0x20u;
			ntfs_scbFcbOffset		= 0x48u;
			ntfs_lcbCleanupOffset	= 0x7Cu;
			ntfs_scbCleanupOffset	= 0x58u;
			ntfs_fcbCleanupOffset	= 0x10u;

		} else if ( g_osVersionCheck >= WINDOWS_VERSION_2K3_SP1_SP2 && g_osVersionCheck <= WINDOWS_VERSION_7 )
		{
			// FileObject
			//			 --> +0Ch SCB
			//						 --> +50h FCB						; by NtfsOpenSubdirectory
			//									 --> +C4h CleanupCount	; by NtfsCheckFileForDelete
			//			 			 --> +54h VCB						; by NtfsAddScbToFspClose
			//						 --> +60h CleanupCount				; by NtfsOpenAttributeCheck
			//			 --> +10h CCB
			//						 --> +2Ch LCB						; by NtfsSetDispositionInfo
			//									 --> +84h CleanupCount	; by NtfsCombineLcbs
			fat_fcbCleanupOffset	= 0x7Cu;  
			ntfs_ccbLcbOffset		= 0x2Cu;
			ntfs_scbFcbOffset		= 0x08u;
			ntfs_lcbCleanupOffset	= 0x84u;
			ntfs_scbCleanupOffset	= 0x64u;
			ntfs_fcbCleanupOffset	= 0x9Cu;
		}

		if ( RtlCompareUnicodeString (&pLowVolumeDevice->DriverObject->DriverName, &uszFastFat, TRUE) == 0 )
		{
			//fat  
			pFCB = pFileObject->FsContext;  
			if ( pFCB != NULL )  
			{  
				v7 = *((PWORD)pFCB);  
				if ( *(PWORD)pFCB == 0x502 || v7 == 0x503 || v7 == 0x504 )  
					*(PULONG)((PCHAR)pFCB + fat_fcbCleanupOffset) = 1;  
			}

		} else if ( RtlCompareUnicodeString (&pLowVolumeDevice->DriverObject->DriverName, &uszNtfs, TRUE) == 0 )
		{
			pSCB = pFileObject->FsContext;
			if ( pSCB != NULL )
			{
				if ( pFileObject->FsContext2 )
				{
					*(PULONG)((PCHAR)pSCB + ntfs_scbCleanupOffset) = 1;    // SCB->CleanUpCount=1  
					pFCB = (PULONG)(*(PULONG)((PCHAR)pSCB + ntfs_scbFcbOffset));
					if ( pFCB != NULL )  
					{  
						if ( MmIsAddressValid((PVOID)pFCB) )
						{
							*(PULONG)((PCHAR)pFCB + ntfs_fcbCleanupOffset) = 1;       // fcb->CleanUpCount=1
						}
					}

					pCCB = pFileObject->FsContext2;  
					if ( pCCB != NULL )  
					{  
						if ( MmIsAddressValid((PVOID)pCCB) )  
						{  
							pLCB = (PULONG)(*(PVOID*)((PCHAR)pCCB + ntfs_ccbLcbOffset));  
							if ( pLCB != NULL )  
							{  
								if ( MmIsAddressValid(pLCB) )  
								{
									*(PULONG)((PCHAR)pLCB + ntfs_lcbCleanupOffset) = 1;// ccb->CleanUpCount=1 
								}
							}  
						}  
					}  
				}
			}
		}
	} while (FALSE);

	BDKitCloseHandle(hFile);
	BDKitCloseObject(pFileObject);

	return nsStatus;
}