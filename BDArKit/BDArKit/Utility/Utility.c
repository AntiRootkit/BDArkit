#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "DriverHelper.h"
#include "List.h"
#include "BDArKit\KitHelper.h"
#include "BDKitUtils.h"
#include "BDKitHandler.h"
#include "Utility.h"


CONST WCHAR g_PathLinkPrefix[]			= L"\\??\\";
DBG_LEVEL	g_DbgOutputLevel			= emDbgNone;
HANDLE		g_AuthorizedProcessHandle	= NULL;
HANDLE		g_DeleteFileListHandle		= NULL;

#ifdef TRUE
static CONST WCHAR	g_ProductRegPath[]	= L"\\Registry\\Machine\\SOFTWARE\\Baidu\\baidusd";
static CONST WCHAR	g_DbgLevelItem[]	= L"DbgLevel";
#endif

typedef struct _tagSysModuleList		 //模块链结构
{         
	ULONG ulCount;
	SYSTEM_MODULE_INFORMATION smi[1];
} MODULES, *PMODULES;
 
WIN_VER_DETAIL	g_osVersionCheck = WINDOWS_VERSION_NONE;

typedef
NTSTATUS (*__pfnZwQuerySystemInformation)(   
	__in ULONG SystemInformationClass,   
	__in PVOID SystemInformation,   
	__in ULONG SystemInformationLength,   
	__out PULONG ReturnLength
	);
static __pfnZwQuerySystemInformation pfnZwQuerySystemInformation = NULL;

typedef
NTSTATUS (*__pfnKeAreAllApcsDisabled)(VOID);
static __pfnKeAreAllApcsDisabled pfnKeAreAllApcsDisabled = NULL;

NTSTATUS GetDbgOutputLevel(VOID)
{
	NTSTATUS		nsStatus		= STATUS_UNSUCCESSFUL;
	UNICODE_STRING	uszRegPath		= {0x00};
	UNICODE_STRING	uszValueName	= {0x00};
	OBJECT_ATTRIBUTES	oa			= {0x00};
	HANDLE			hKey			= NULL;
	PKEY_VALUE_PARTIAL_INFORMATION	pValue	= NULL;
	ULONG			ResultLength	= 0;

	do 
	{
		RtlInitUnicodeString (&uszRegPath, g_ProductRegPath);
		RtlInitUnicodeString (&uszValueName, g_DbgLevelItem);

		InitializeObjectAttributes(&oa, &uszRegPath, OBJ_CASE_INSENSITIVE, NULL, NULL);
		nsStatus = ZwOpenKey (&hKey, KEY_ALL_ACCESS, &oa);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ZwQueryValueKey (
			hKey,
			&uszValueName,
			KeyValuePartialInformation,
			NULL,
			0,
			&ResultLength
			);
		BDKit_If_Not_Break(nsStatus == STATUS_BUFFER_TOO_SMALL && ResultLength != 0);

		BDKitAllocateNonpagePool(pValue, ResultLength);
		BDKit_If_Not_Break_With_Reason(pValue != NULL, STATUS_INSUFFICIENT_RESOURCES);

		nsStatus = ZwQueryValueKey (
			hKey,
			&uszValueName,
			KeyValuePartialInformation,
			pValue,
			ResultLength,
			&ResultLength
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		g_DbgOutputLevel = (ULONG)pValue->Data;
	} while (FALSE);

	BDKitCloseHandle(hKey);
	BDKitFreePool(pValue);

	return nsStatus;
}

NTSTATUS BDKitInitializeUtility( VOID )
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	UNICODE_STRING	uszFuncName	= {0x00};

	do 
	{
		RtlInitUnicodeString (&uszFuncName, L"ZwQuerySystemInformation");
		pfnZwQuerySystemInformation = (__pfnZwQuerySystemInformation)MmGetSystemRoutineAddress (&uszFuncName);
		BDKit_If_Not_Break(pfnZwQuerySystemInformation != NULL);

		// This Function is available in Windows Server 2003+, So we don't check Return Value
		RtlInitUnicodeString (&uszFuncName, L"KeAreAllApcsDisabled");
		pfnKeAreAllApcsDisabled = (__pfnKeAreAllApcsDisabled)MmGetSystemRoutineAddress (&uszFuncName);

		nsStatus = STATUS_SUCCESS;
	} while (FALSE);

	return nsStatus;
}

NTSTATUS BDKitUnInitializeUtility( VOID )
{
	BDKitClearAuthorizedProcess();

	return STATUS_SUCCESS;
}

BOOLEAN BDKitCopyUnicodeStringW (__out PUNICODE_STRING puszDstString, __in PUNICODE_STRING puszSrcString)
{
	BOOLEAN bResult	= FALSE;

	if (	puszDstString != NULL
		&&	puszSrcString != NULL
		&&	puszSrcString->Length != 0 
		&&	puszSrcString->MaximumLength >= puszSrcString->Length )
	{
		do 
		{
			BDKitAllocateNonpagePool(puszDstString->Buffer, puszSrcString->MaximumLength);
			BDKit_If_Not_Break(puszDstString->Buffer != NULL);
			puszDstString->Length = 0;
			puszDstString->MaximumLength = puszSrcString->MaximumLength;
			RtlCopyUnicodeString (puszDstString, puszSrcString);
			BDKit_If_Not_Break(puszDstString->Length != 0);

			bResult = TRUE;
		} while (FALSE);

		if ( bResult == FALSE )
		{
			BDKitFreePool(puszDstString->Buffer);
		}
	}

	return bResult;
}

BOOLEAN BDKitCopyPUnicodeStringW (__out PUNICODE_STRING* ppuszDstString, __in PUNICODE_STRING puszSrcString)
{
	BOOLEAN bResult	= FALSE;
	PUNICODE_STRING	puszDstString = NULL;

	if (	ppuszDstString!= NULL
		&&	puszSrcString != NULL
		&&	puszSrcString->Length != 0 
		&&	puszSrcString->MaximumLength >= puszSrcString->Length )
	{
		do 
		{
			BDKitAllocateNonpagePool(puszDstString, sizeof(UNICODE_STRING) + puszSrcString->MaximumLength);
			BDKit_If_Not_Break(puszDstString != NULL);
			puszDstString->Length = 0;
			puszDstString->MaximumLength = puszSrcString->MaximumLength;
			puszDstString->Buffer = (PWCHAR)((PUCHAR)puszDstString + sizeof(UNICODE_STRING));
			RtlCopyUnicodeString (puszDstString, puszSrcString);
			BDKit_If_Not_Break(puszDstString->Length != 0);

			bResult = TRUE;
		} while (FALSE);

		if ( bResult == FALSE )
		{
			BDKitFreePool(puszDstString);
		}

		*ppuszDstString = puszDstString;
	}

	return bResult;
}

NTSTATUS GetSystemKernelModuleInfo1(
									__in PDRIVER_OBJECT	DriverObject,
									__in OFFSET_VALUE		DriverEntryReturnAddr,
									__out WCHAR**			SystemKernelModulePath,
									__out _ULONG*			SystemKernelModuleBase,
									__out _ULONG*			SystemKernelModuleSize
									)
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	ULONG				ulSize		= 0;
	PMODULES			pModuleList	= NULL;
	CHAR*				pszKernelName=NULL;
	ANSI_STRING			szKernelModule	= {0x00};
	UNICODE_STRING		uszKernelModule	= {0x00};

	UNREFERENCED_PARAMETER(DriverObject);

	BDKit_If_DoAction(
		DriverObject		   == NULL
		||	SystemKernelModulePath == NULL 
		||	SystemKernelModuleBase == NULL 
		||	SystemKernelModuleSize == NULL
		||	DriverEntryReturnAddr  == 0, 
		return STATUS_UNSUCCESSFUL);

	(*SystemKernelModulePath) = NULL;
	(*SystemKernelModuleBase) = 0x00000000;
	(*SystemKernelModuleSize) = 0x00000000;

	__try
	{
		do 
		{
			nsStatus=ZwQuerySystemInformation (SystemModuleInformation, NULL, 0, &ulSize);
			BDKit_If_Not_Break(nsStatus == STATUS_INFO_LENGTH_MISMATCH);

			BDKitAllocateNonpagePool(pModuleList, ulSize);
			BDKit_If_Not_Break(pModuleList != NULL);

			nsStatus = ZwQuerySystemInformation (SystemModuleInformation, pModuleList, ulSize, &ulSize);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

			BDKitAllocateNonpagePool(*SystemKernelModulePath, 260 * 2);
			BDKit_If_Not_Break((*SystemKernelModulePath) != NULL);
			RtlZeroMemory(*SystemKernelModulePath, 260 * 2);

			pszKernelName = pModuleList->smi[0].ModuleNameOffset+pModuleList->smi[0].ImageName;
			BDKit_If_Not_Break(pszKernelName != NULL);

			RtlInitAnsiString(&szKernelModule,pszKernelName);
			RtlAnsiStringToUnicodeString(&uszKernelModule,&szKernelModule,TRUE);

			wcscat(*SystemKernelModulePath, L"\\SystemRoot\\system32\\");

			RtlCopyMemory(
				*SystemKernelModulePath + wcslen(L"\\SystemRoot\\system32\\"),
				uszKernelModule.Buffer,
				uszKernelModule.Length
				);

			*SystemKernelModuleBase=(_ULONG_PTR)pModuleList->smi[0].Base;
			*SystemKernelModuleSize=(_ULONG)pModuleList->smi[0].Size;

			BDKit_If_Not_Break(
				(DriverEntryReturnAddr >= *SystemKernelModuleBase) 
				&&	(DriverEntryReturnAddr < (*SystemKernelModuleBase + *SystemKernelModuleSize)));

			nsStatus = STATUS_SUCCESS;
		} while (FALSE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		// TODO
	}

	BDKit_If_DoAction(uszKernelModule.Buffer != NULL, RtlFreeUnicodeString (&uszKernelModule));
	BDKit_If_DoAction(pModuleList != NULL, BDKitFreePool(pModuleList));
	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), BDKitFreePool(*SystemKernelModulePath));
	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), *SystemKernelModuleBase = *SystemKernelModuleSize = 0);

	return nsStatus;
}

NTSTATUS GetSystemKernelModuleInfo2(
									__in PDRIVER_OBJECT	DriverObject,
									__in OFFSET_VALUE		DriverEntryReturnAddr,
									__out WCHAR**			SystemKernelModulePath,
									__out _ULONG*			SystemKernelModuleBase,
									__out _ULONG*			SystemKernelModuleSize
									)
{
	NTSTATUS				nsStatus		= STATUS_UNSUCCESSFUL;
	PLDR_DATA_TABLE_ENTRY	DriverSection	= NULL;
	PLDR_DATA_TABLE_ENTRY	LdrEntry		= NULL;

	BDKit_If_DoAction(
		DriverObject		   == NULL
		||	SystemKernelModulePath == NULL 
		||	SystemKernelModuleBase == NULL 
		||	SystemKernelModuleSize == NULL
		||	DriverEntryReturnAddr  == 0, 
		return STATUS_UNSUCCESSFUL);

	(*SystemKernelModulePath) = NULL;
	(*SystemKernelModuleBase) = 0x00000000;
	(*SystemKernelModuleSize) = 0x00000000;

	__try
	{
		// IoEnumerateDeviceObjectList
		DriverSection = DriverObject->DriverSection;
		LdrEntry = (PLDR_DATA_TABLE_ENTRY)DriverSection->InLoadOrderLinks.Flink;

		while(LdrEntry != NULL && DriverSection != LdrEntry)
		{
			if (	DriverEntryReturnAddr >= (ULONG)LdrEntry->DllBase
				&&	DriverEntryReturnAddr < (ULONG)LdrEntry->DllBase + LdrEntry->SizeOfImage
				&&	LdrEntry->FullDllName.Buffer != NULL
				&&	LdrEntry->FullDllName.Length != 0
				)
			{
				if (MmIsAddressValid (
					&(LdrEntry->FullDllName.Buffer[LdrEntry->FullDllName.Length / 2 - 1])))
				{
					BDbgPrint ("[BDKit]System Kernel Module %wZ Base Address 0x%p\r\n", &LdrEntry->BaseDllName, LdrEntry->DllBase);
					break;
				}
			}

			LdrEntry = (PLDR_DATA_TABLE_ENTRY)LdrEntry->InLoadOrderLinks.Flink;
		}

		do 
		{
			BDKit_If_Not_Break(LdrEntry != NULL && LdrEntry != DriverSection);

			BDKitAllocateNonpagePool(*SystemKernelModulePath, 260 * 2);
			BDKit_If_Not_Break((*SystemKernelModulePath) != NULL);
			RtlZeroMemory(*SystemKernelModulePath, 260  * 2);

			RtlCopyMemory(
				*SystemKernelModulePath, 
				LdrEntry->FullDllName.Buffer,
				LdrEntry->FullDllName.Length
				);

			*SystemKernelModuleBase = (_ULONG)LdrEntry->DllBase;
			*SystemKernelModuleSize = (_ULONG)LdrEntry->SizeOfImage;

			nsStatus = STATUS_SUCCESS;
		} while (FALSE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		// TODO
	}

	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), BDKitFreePool(*SystemKernelModulePath));
	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), *SystemKernelModuleBase = *SystemKernelModuleSize = 0);

	return nsStatus;
}

NTSTATUS BDKitGetSystemKernelModuleInfo(	
										__in PDRIVER_OBJECT	DriverObject,
										__in OFFSET_VALUE	DriverEntryReturnAddr,
										__out WCHAR**		SystemKernelModulePath,
										__out _ULONG*		SystemKernelModuleBase,
										__out _ULONG*		SystemKernelModuleSize
										)
{
	NTSTATUS nsStatus	= STATUS_SUCCESS;

	do 
	{
		nsStatus = GetSystemKernelModuleInfo1 (
			DriverObject,
			DriverEntryReturnAddr,
			SystemKernelModulePath,
			SystemKernelModuleBase,
			SystemKernelModuleSize
			);
		BDKit_If_Not_Break(!NT_SUCCESS(nsStatus));

		nsStatus = GetSystemKernelModuleInfo2 (
			DriverObject,
			DriverEntryReturnAddr,
			SystemKernelModulePath,
			SystemKernelModuleBase,
			SystemKernelModuleSize
			);
		BDKit_If_Not_Break(!NT_SUCCESS(nsStatus));
	} while (FALSE);

	return nsStatus;
}

BOOLEAN BDKitMmIsAddressValid(__in PVOID pMmAddr, __in ULONG ulLen)
{
	if (pMmAddr == NULL)
	{
		return FALSE;
	}

	if (MmIsAddressValid(pMmAddr) && MmIsAddressValid((PVOID)((PBYTE)pMmAddr + ulLen)))
	{
		return TRUE;
	}

	return FALSE;
}

NTSTATUS BDKitGetWindowsRootName( __out WCHAR* WindowsRootName, __in ULONG BufferLen )
{
	UNREFERENCED_PARAMETER(BufferLen);
	wcscat (WindowsRootName, L"C:\\Windows");

	// Support in the future
	return /*STATUS_SUCCESS*/STATUS_NOT_SUPPORTED;
}

NTSTATUS BDKitObQueryNameString(__in PFILE_OBJECT pFileOBject, __out POBJECT_NAME_INFORMATION* ppNameInfo)
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	ULONG		ulReturnLen	= 0;
	POBJECT_NAME_INFORMATION	pVolNameInfo	= NULL;

	do 
	{
		BDKit_If_Not_Break(pFileOBject != NULL && ppNameInfo != NULL);

		nsStatus = ObQueryNameString (pFileOBject, NULL, 0, &ulReturnLen);
		BDKit_If_Not_Break(nsStatus == STATUS_INFO_LENGTH_MISMATCH && ulReturnLen < MAX_OBJECT_PATH_LENGTH);

		// 有些情况下回返回0,这时取最大值
		BDKit_If_DoAction(ulReturnLen == 0, ulReturnLen = MAX_OBJECT_PATH_LENGTH);

		BDKitAllocateNonpagePool(pVolNameInfo, ulReturnLen);
		BDKit_If_Not_Break(pVolNameInfo != NULL);
		pVolNameInfo->Name.Length = 0;
		pVolNameInfo->Name.MaximumLength = (USHORT)ulReturnLen;

		nsStatus = ObQueryNameString (pFileOBject, pVolNameInfo, ulReturnLen, &ulReturnLen);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	*ppNameInfo = pVolNameInfo;

	return nsStatus;
}

NTSTATUS BDKitVolumeNameToDosName(
								  __in PUNICODE_STRING	puszFileFullPath, 
								  __out PUNICODE_STRING* ppuszDosName
								  )
{
	NTSTATUS		nsStatus		= STATUS_UNSUCCESSFUL;
	HANDLE			hFileVolume		= NULL;
	PFILE_OBJECT	pVolumeObject	= NULL;
	PUNICODE_STRING	puszDosName		= NULL;
	POBJECT_NAME_INFORMATION	pVolNameInfo	= NULL;
	UNICODE_STRING	uszVolume		= {0x00};
	IO_STATUS_BLOCK	IoStatus		= {0x00};
	OBJECT_ATTRIBUTES	oa			= {0x00};
	WCHAR			wszVolumeName[]	= L"\\??\\*:\\";

	do 
	{
		BDKit_If_Not_Break(puszFileFullPath != NULL && puszFileFullPath->Length > 6 && ppuszDosName != NULL);
		BDKit_If_Not_Break((puszFileFullPath->Buffer[4] >= 'a' && puszFileFullPath->Buffer[4] <= 'z') 
			||	(puszFileFullPath->Buffer[4] >= 'A' && puszFileFullPath->Buffer[4] <= 'Z'));

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

		nsStatus = ObReferenceObjectByHandle (hFileVolume, SYNCHRONIZE, *IoFileObjectType, KernelMode, &pVolumeObject, NULL);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = BDKitObQueryNameString (pVolumeObject, &pVolNameInfo);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		BDKitAllocateNonpagePool(puszDosName, sizeof(UNICODE_STRING) + pVolNameInfo->Name.Length + puszFileFullPath->Length);
		BDKit_If_Not_Break_With_Reason(puszDosName != NULL, STATUS_INSUFFICIENT_RESOURCES);

		puszDosName->MaximumLength	= pVolNameInfo->Name.Length + puszFileFullPath->Length;
		puszDosName->Length			= 0;
		puszDosName->Buffer			= (PWCHAR)((PUCHAR)puszDosName + sizeof(UNICODE_STRING));
		RtlCopyUnicodeString (puszDosName, &pVolNameInfo->Name);
		RtlCopyMemory(
			puszDosName->Buffer + wcslen (puszDosName->Buffer), 
			puszFileFullPath->Buffer + wcslen (wszVolumeName),
			puszFileFullPath->Length - wcslen (wszVolumeName) * sizeof(WCHAR)
			);
		puszDosName->Length += (USHORT)(puszFileFullPath->Length - wcslen (wszVolumeName) * sizeof(WCHAR));

	} while (FALSE);

	*ppuszDosName = puszDosName;

	BDKitFreePool(pVolNameInfo);
	BDKitCloseObject(pVolumeObject);
	BDKitCloseHandle(hFileVolume);

	return nsStatus;
}

typedef NTSTATUS (NTAPI * PFN_RtlGetVersion)(__out PRTL_OSVERSIONINFOW lpVersionInformation);

NTSTATUS BDKitGetOSVersion(VOID)
{
	UNICODE_STRING			ustrFuncName	= {0x00}; 
	RTL_OSVERSIONINFOEXW	osverinfo		= {sizeof(osverinfo)}; 
	PFN_RtlGetVersion		pfnRtlGetVersion= NULL;

	do 
	{
		g_osVersionCheck = WINDOWS_VERSION_NONE;

		RtlInitUnicodeString(&ustrFuncName, L"RtlGetVersion"); 
		pfnRtlGetVersion = (PFN_RtlGetVersion)MmGetSystemRoutineAddress(&ustrFuncName); 

		if ( pfnRtlGetVersion != NULL )
		{ 
			pfnRtlGetVersion((PRTL_OSVERSIONINFOW)&osverinfo); 
		} 
		else 
		{

			PsGetVersion(&osverinfo.dwMajorVersion, &osverinfo.dwMinorVersion, &osverinfo.dwBuildNumber, NULL);
		}

		if (osverinfo.dwMajorVersion == 5 && osverinfo.dwMinorVersion == 0) 
		{
			g_osVersionCheck = WINDOWS_VERSION_2K;
		} 
		else if (osverinfo.dwMajorVersion == 5 && osverinfo.dwMinorVersion == 1) 
		{
			g_osVersionCheck = WINDOWS_VERSION_XP;
		} 
		else if (osverinfo.dwMajorVersion == 5 && osverinfo.dwMinorVersion == 2) 
		{
			if (osverinfo.wServicePackMajor==0) 
			{ 
				g_osVersionCheck = WINDOWS_VERSION_2K3;
			} 
			else 
			{
				g_osVersionCheck = WINDOWS_VERSION_2K3_SP1_SP2;
			}
		} 
		else if (osverinfo.dwMajorVersion == 6 && osverinfo.dwMinorVersion == 0) 
		{
			g_osVersionCheck = WINDOWS_VERSION_2K3_SP1_SP2;
		}
		else if (osverinfo.dwMajorVersion == 6 && osverinfo.dwMinorVersion == 1)
		{
			g_osVersionCheck = WINDOWS_VERSION_7;
		}
		else if (osverinfo.dwMajorVersion == 6 && osverinfo.dwMinorVersion == 2)
		{
			g_osVersionCheck = WINDOWS_VERSION_8;
		} 
		else if (osverinfo.dwMajorVersion == 6 && osverinfo.dwMinorVersion == 3)
		{
			g_osVersionCheck = WINDOWS_VERSION_8_1;
		} 

	} while (FALSE);

	return g_osVersionCheck == WINDOWS_VERSION_NONE? STATUS_UNSUCCESSFUL : STATUS_SUCCESS;
}

PVOID BDKitGetKernelModuleBase(__in PDRIVER_OBJECT DriverObject, __out PCHAR KernelModuleName)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(KernelModuleName);
	//PLDR_DATA_TABLE_ENTRY	DriverSection	= NULL;
	//PLDR_DATA_TABLE_ENTRY	LdrEntry		= NULL;
	//ANSI_STRING				aszKernelModuleName	= {0x00};
	//UNICODE_STRING			uszKernelModuleName	= {0x00};
	//UNICODE_STRING			uszModuleName		= {0x00};
	//PWCHAR					pBuffer			= NULL;
	//ULONG					Lentgh			= 0;
	//ULONG					Index			= 0;

	//do 
	//{
	//	RtlInitAnsiString(&aszKernelModuleName, KernelModuleName);
	//	RtlAnsiStringToUnicodeString(&uszKernelModuleName,&aszKernelModuleName,TRUE);

	//	BDKitAllocateNonpagePool(pBuffer, 260 * 2);
	//	BDKit_If_Not_Break(pBuffer != NULL);

	//	RtlZeroMemory(Buffer,206*2);
	//	DriverSection = DriverObject->DriverSection;
	//	LdrEntry=(PLDR_DATA_TABLE_ENTRY)DriverSection->InLoadOrderLinks.Flink;
	//	while (LdrEntry != NULL && DriverSection != LdrEntry)
	//	{

	//		if ((DWORD)LdrEntry->DllBase>=*(DWORD*)MmSystemRangeStart&&
	//			LdrEntry->FullDllName.Length>0&&
	//			LdrEntry->FullDllName.Buffer!=NULL)
	//		{

	//			if (MmIsAddressValid(&LdrEntry->FullDllName.Buffer[LdrEntry->FullDllName.Length/2-1]))
	//			{
	//				Lentgh=LdrEntry->FullDllName.Length/2;
	//				for (Index=Lentgh-1;Index>0;Index--)
	//				{
	//					if (LdrEntry->FullDllName.Buffer[Index]==0x005C)
	//					{
	//						break;
	//					}
	//				}
	//				if (LdrEntry->FullDllName.Buffer[Index]==0x005C)
	//				{
	//					RtlCopyMemory(Buffer,&(LdrEntry->FullDllName.Buffer[Index+1]),(Lentgh-Index-1)*2);
	//					ModuleName.Buffer=Buffer;
	//					ModuleName.Length=(Lentgh-Index-1)*2;
	//					ModuleName.MaximumLength=260*2;
	//				}
	//				else
	//				{
	//					RtlCopyMemory(Buffer,LdrEntry->FullDllName.Buffer,Lentgh*2);
	//					ModuleName.Buffer=Buffer;
	//					ModuleName.Length=Lentgh*2;
	//					ModuleName.MaximumLength=260*2;

	//				}

	//				if (RtlEqualUnicodeString(&ModuleName,&UniKernelModuleName,TRUE))
	//				{
	//					ExFreePool(Buffer);
	//					return LdrEntry->DllBase;
	//				}

	//			}

	//		}	
	//		LdrEntry=(PLDR_DATA_TABLE_ENTRY)LdrEntry->InLoadOrderLinks.Flink;
	//	}
	//	ExFreePool(Buffer);
	//} while (FALSE);

	return NULL;
}

//将所有的路径类型转换为\\??\\*:\\形式
NTSTATUS BDKitCheckFileFullPathPrefix (__in CONST PWCHAR pFileFullPath, __out PUNICODE_STRING pConvertPath)
{
	NTSTATUS	nsStatus			= STATUS_UNSUCCESSFUL;
	WCHAR		SystemRootName[]	= L"\\Systemroot";
	WCHAR		DosName[]			= L"\\DosDevices\\";
	WCHAR		SymName[]			= L"\\??\\";
	WCHAR		StandName[]			= L":\\";
	WCHAR*		pwszFileNodePath	= NULL;

	do 
	{
		BDKit_If_Not_Break(pFileFullPath != NULL);

		BDKitAllocateNonpagePool(pwszFileNodePath, (160 + wcslen (pFileFullPath)) * sizeof(WCHAR));
		BDKit_If_Not_Break(pwszFileNodePath != NULL);


		wcscat(pwszFileNodePath, SymName);

		if ( _wcsnicmp (pFileFullPath, SystemRootName, wcslen (SystemRootName)) == 0 )
		{
			do
			{
				nsStatus = BDKitGetWindowsRootName (pwszFileNodePath, 160 * sizeof(WCHAR));
				BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

				wcscat(pwszFileNodePath, &pFileFullPath[wcslen (SystemRootName)]);
			} while (FALSE);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		} else if ( _wcsnicmp (pFileFullPath, DosName, wcslen (DosName)) == 0 )
		{
			do 
			{
				wcscat(pwszFileNodePath, &pFileFullPath[wcslen (DosName)]);
			} while (FALSE);

		} else if ( _wcsnicmp (pFileFullPath, SymName, wcslen (SymName)) == 0 )
		{
			do 
			{
				RtlCopyMemory(pwszFileNodePath, pFileFullPath, wcslen (pFileFullPath) * sizeof(WCHAR));
			} while (FALSE);
		} else if ( _wcsnicmp (&pFileFullPath[1], StandName, wcslen (StandName)) == 0 )
		{
			do 
			{
				wcscat (pwszFileNodePath, pFileFullPath);
			} while (FALSE);
		}
		else
		{
			nsStatus = STATUS_NOT_SUPPORTED;
			break;
		}

		pConvertPath->Buffer = pwszFileNodePath;
		pConvertPath->Length = (USHORT)(wcslen (pwszFileNodePath) * sizeof(WCHAR));
		pConvertPath->MaximumLength = (USHORT)((160 + wcslen (pFileFullPath)) * sizeof(WCHAR));

		pwszFileNodePath = NULL;
		nsStatus = STATUS_SUCCESS;

	} while (FALSE);

	BDKitFreePool(pwszFileNodePath);

	return nsStatus;
}

NTSTATUS BDKitGetSystemInfo (
							 __in  ULONG	ulTableType, 
							 __out PVOID*	CONST ppSystemInfo, 
							 __out PULONG	CONST ulReturnAlloc
							 )
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	ULONG		ulReturnLen	= 0;
	PVOID		pSystemInfo	= NULL;

	ulReturnLen = 0;
	pSystemInfo	= NULL;

	do 
	{
		nsStatus = pfnZwQuerySystemInformation (
			ulTableType,
			pSystemInfo,
			ulReturnLen,
			&ulReturnLen
			);
		BDKit_If_Not_Break(nsStatus == STATUS_INFO_LENGTH_MISMATCH && ulReturnLen < MAX_HANDLE_TABLE_SIZE);

		BDKitFreePool(pSystemInfo);
		ulReturnLen += MEM_SIZE_STEP;
		BDKitAllocateNonpagePool(pSystemInfo, ulReturnLen);
		BDKit_If_Not_Break(pSystemInfo != NULL);
	} while (TRUE);

	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), BDKitFreePool(pSystemInfo));
	*ppSystemInfo = pSystemInfo;
	*ulReturnAlloc = pSystemInfo == NULL? 0 : ulReturnLen;

	return nsStatus;
}

NTSTATUS BDKitGetProcessInfo(
							 __in HANDLE	hProcessHandle,
							 __in  ULONG	ulTableType, 
							 __out PVOID*	CONST ppSystemInfo, 
							 __out PULONG	CONST ulReturnAlloc
							 )
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	ULONG		ulReturnLen	= 0;
	PVOID		pSystemInfo	= NULL;

	ulReturnLen = 0;
	pSystemInfo	= NULL;

	do 
	{
		nsStatus = ZwQueryInformationProcess (
			hProcessHandle,
			ulTableType,
			pSystemInfo,
			ulReturnLen,
			&ulReturnLen
			);
		BDKit_If_Not_Break(nsStatus == STATUS_INFO_LENGTH_MISMATCH && ulReturnLen < MAX_HANDLE_TABLE_SIZE);

		BDKitFreePool(pSystemInfo);
		ulReturnLen += MEM_SIZE_STEP;
		BDKitAllocateNonpagePool(pSystemInfo, ulReturnLen);
		BDKit_If_Not_Break(pSystemInfo != NULL);
	} while (TRUE);

	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), BDKitFreePool(pSystemInfo));
	*ppSystemInfo = pSystemInfo;
	*ulReturnAlloc = pSystemInfo == NULL? 0 : ulReturnLen;

	return nsStatus;
}

NTSTATUS BDKitDuplicateHandle(
							  __in	HANDLE	ulSrcProcessID,
							  __in	HANDLE	ulDstProcessID,
							  __in	HANDLE	hSrcHandle,
							  __out	PHANDLE phDupHandle
							  )
{
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;
	CLIENT_ID			clientID	= {0x00};
	OBJECT_ATTRIBUTES	oaProcessAtt= {0x00};
	HANDLE				hSourceProcess	= NULL;
	HANDLE				hTargetProcess	= NULL;

	do 
	{
		clientID.UniqueProcess	= ulSrcProcessID;
		clientID.UniqueThread	= 0;
		InitializeObjectAttributes(
			&oaProcessAtt,
			NULL,
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
			NULL,
			NULL
			);
		nsStatus = ZwOpenProcess (&hSourceProcess, 0x1F0FFFu, &oaProcessAtt, &clientID);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		clientID.UniqueProcess	= ulDstProcessID;
		clientID.UniqueThread	= 0;
		InitializeObjectAttributes(
			&oaProcessAtt,
			NULL,
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
			NULL,
			NULL
			);
		nsStatus = ZwOpenProcess (&hTargetProcess, 0x1F0FFFu, &oaProcessAtt, &clientID);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ZwDuplicateObject (
			hSourceProcess,
			hSrcHandle,
			hTargetProcess,
			phDupHandle,
			0,
			0,
			DUPLICATE_SAME_ACCESS
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
	} while (FALSE);

	BDKitCloseHandle(hSourceProcess);
	BDKitCloseHandle(hTargetProcess);

	return nsStatus;
}

NTSTATUS BDKitGetObjectFunction(
								__in	PVOID				pObjectType, 
								__out	PVOID*				pRetFuncBuf, 
								__in	emObjTypeFunction	emFuncIndex
								)
{
	NTSTATUS	nsStatus		= STATUS_UNSUCCESSFUL;
	ULONG		ulNeededSize	= 0;
	PVOID		pModuleBuffer	= NULL;
	PVOID pKernelImageAddrBegin = NULL;
	PVOID pKernelImageAddrEnd   = NULL;
	PSYSTEM_MODULE_INFORMATION pModuleList = NULL;

	do 
	{
		BDKit_If_Not_Break(pObjectType == NULL || emFuncIndex >= emObjEnd);

		pRetFuncBuf[emFuncIndex] = NULL;

		nsStatus = BDKitGetSystemInfo(SystemModuleInformation, &pModuleBuffer, &ulNeededSize);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		pModuleList = (PSYSTEM_MODULE_INFORMATION)((PBYTE)pModuleBuffer + sizeof(ULONG) * 2);

		pKernelImageAddrBegin = (PVOID)pModuleList->Base;
		pKernelImageAddrEnd   = (PVOID)((PBYTE)pKernelImageAddrBegin + pModuleList->Size);

		BDKitFreePool(pModuleBuffer);	
		pModuleList   = NULL;

		if ( g_osVersionCheck >= WINDOWS_VERSION_XP && g_osVersionCheck <=  WINDOWS_VERSION_2K3_SP1_SP2 )
		{
			//5.0内核的
			POBJECT_TYPE50 pType = (POBJECT_TYPE50)pObjectType;
			if (emFuncIndex == emObjCloseOper)
			{
				pRetFuncBuf[emFuncIndex] = pType->TypeInfo.CloseProcedure;
			}
			else if (emFuncIndex == emObjDeleteOper)
			{
				pRetFuncBuf[emFuncIndex] = pType->TypeInfo.DeleteProcedure;
			}
			else if (emFuncIndex == emObjParseOper)
			{
				pRetFuncBuf[emFuncIndex] = pType->TypeInfo.ParseProcedure;
			}
			else if (emFuncIndex == emObjSecurityOper)
			{
				pRetFuncBuf[emFuncIndex] = pType->TypeInfo.SecurityProcedure;
			}
			else if (emFuncIndex == emObjQueryKeyNameOper)
			{
				pRetFuncBuf[emFuncIndex] = pType->TypeInfo.QueryNameProcedure;
			}
			else if (emFuncIndex == emObjOpenProcedureOper)
			{
				pRetFuncBuf[emFuncIndex] = pType->TypeInfo.OpenProcedure;
			}
			else
			{
				pRetFuncBuf[emFuncIndex] = NULL;
			}

			//检查是否内核地址范围
			if ((ULONG)pKernelImageAddrBegin <= (ULONG)pRetFuncBuf[emFuncIndex] &&
				(ULONG)pKernelImageAddrEnd   >= (ULONG)pRetFuncBuf[emFuncIndex])
			{
				return TRUE;
			}

			//已经被HOOK了
			pRetFuncBuf[emFuncIndex] = NULL;
			return FALSE;
		} else if ( g_osVersionCheck >= WINDOWS_VERSION_VISTA && g_osVersionCheck <= WINDOWS_VERSION_8 )
		{
			//6.0的内核
			PVOID pParseProcedure = (PVOID)((PBYTE)pObjectType + NT6_OBJECT_TYPE_TYPEINFO_OFFECT);

			if (emFuncIndex == emObjCloseOper)
			{
				pRetFuncBuf[emFuncIndex] = *(PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_CLOSE_OFFECT);
			}
			else if (emFuncIndex == emObjDeleteOper)
			{
				pRetFuncBuf[emFuncIndex] = *(PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_DELETE_OFFECT);
			}
			else if (emFuncIndex == emObjParseOper)
			{
				pRetFuncBuf[emFuncIndex] = *(PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_PARSE_OFFECT);
			}
			else if (emFuncIndex == emObjSecurityOper)
			{
				pRetFuncBuf[emFuncIndex] = *(PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_SECURITY_OFFECT);
			}
			else if (emFuncIndex == emObjQueryKeyNameOper)
			{
				pRetFuncBuf[emFuncIndex] = *(PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_QUERYNAME_OFFECT);
			}
			else if (emFuncIndex == emObjOpenProcedureOper)
			{
				pRetFuncBuf[emFuncIndex] = *(PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_OPEN_OFFECT);
			}
			else
			{
				pRetFuncBuf[emFuncIndex] = NULL;
			}

			//检查是否内核地址范围
			BDKit_If_Not_Break(
				(ULONG)pKernelImageAddrEnd   >= (ULONG)pRetFuncBuf[emFuncIndex]
			&&	(ULONG)pKernelImageAddrBegin <= (ULONG)pRetFuncBuf[emFuncIndex] 
			);

			nsStatus = STATUS_SUCCESS;
		}
		else
		{
			nsStatus = STATUS_NOT_SUPPORTED;
		}
	} while (FALSE);

	BDKitFreePool(pModuleBuffer);

	return nsStatus;
}

NTSTATUS BDKitSetObjectFunction(
								__in	PVOID				pObjectType, 
								__in	PVOID*				pFunctionStruct, 
								__in	emObjTypeFunction	emFuncIndex, 
								__out	PVOID*				pRetFunctionAddr, 
								__in	BOOLEAN				bSet
								)
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;

	do 
	{
		BDKit_If_Not_Break(emFuncIndex >= emObjCloseOper && emFuncIndex < emObjEnd);
		BDKit_If_Not_Break(pFunctionStruct != NULL && pFunctionStruct[emFuncIndex] != NULL);

		if ( g_osVersionCheck >= WINDOWS_VERSION_XP && g_osVersionCheck <= WINDOWS_VERSION_2K3_SP1_SP2 )
		{
			//5.0内核的
			POBJECT_TYPE50 pType = (POBJECT_TYPE50)pObjectType;

			if (emFuncIndex == emObjCloseOper)
			{
				//设置
				if (bSet)
				{
					BDKit_If_Not_Break(pType->TypeInfo.CloseProcedure != pFunctionStruct[emFuncIndex]);

					*pRetFunctionAddr = pType->TypeInfo.CloseProcedure;

					pType->TypeInfo.CloseProcedure = pFunctionStruct[emFuncIndex];
				}
				else
				{
					BDKit_If_Not_Break(*pRetFunctionAddr != pType->TypeInfo.CloseProcedure);

					pType->TypeInfo.CloseProcedure = *pRetFunctionAddr;
				}
			} else if (emFuncIndex == emObjDeleteOper)
			{
				//设置
				if (bSet)
				{
					BDKit_If_Not_Break(pType->TypeInfo.DeleteProcedure != pFunctionStruct[emFuncIndex]);

					*pRetFunctionAddr = pType->TypeInfo.DeleteProcedure;

					pType->TypeInfo.DeleteProcedure = pFunctionStruct[emFuncIndex];
				}
				else
				{
					//还原
					BDKit_If_Not_Break(*pRetFunctionAddr != pType->TypeInfo.DeleteProcedure);

					pType->TypeInfo.DeleteProcedure = *pRetFunctionAddr;
				}
			} else if (emFuncIndex == emObjParseOper)
			{
				//设置
				if (bSet)
				{
					BDKit_If_Not_Break(pType->TypeInfo.ParseProcedure != pFunctionStruct[emFuncIndex]);

					*pRetFunctionAddr = pType->TypeInfo.ParseProcedure;

					pType->TypeInfo.ParseProcedure = pFunctionStruct[emFuncIndex];
				}
				else
				{
					//还原
					BDKit_If_Not_Break(*pRetFunctionAddr != pType->TypeInfo.ParseProcedure);

					pType->TypeInfo.ParseProcedure = *pRetFunctionAddr;
				}
			} else if (emFuncIndex == emObjSecurityOper)
			{
				//设置
				if (bSet)
				{
					BDKit_If_Not_Break(pType->TypeInfo.SecurityProcedure != pFunctionStruct[emFuncIndex]);

					*pRetFunctionAddr = pType->TypeInfo.SecurityProcedure;

					pType->TypeInfo.SecurityProcedure = pFunctionStruct[emFuncIndex];
				}
				else
				{
					//还原
					BDKit_If_Not_Break(*pRetFunctionAddr != pType->TypeInfo.SecurityProcedure);

					pType->TypeInfo.SecurityProcedure = *pRetFunctionAddr;
				}
			} else if (emFuncIndex == emObjQueryKeyNameOper)
			{
				//设置
				if (bSet)
				{
					BDKit_If_Not_Break(pType->TypeInfo.QueryNameProcedure != pFunctionStruct[emFuncIndex]);

					*pRetFunctionAddr = pType->TypeInfo.QueryNameProcedure;

					pType->TypeInfo.QueryNameProcedure = pFunctionStruct[emFuncIndex];
				}
				else
				{
					//还原
					BDKit_If_Not_Break(*pRetFunctionAddr != pType->TypeInfo.QueryNameProcedure);

					pType->TypeInfo.QueryNameProcedure = *pRetFunctionAddr;
				}
			}
			else
			{
				//设置
				if (bSet)
				{
					BDKit_If_Not_Break(pType->TypeInfo.OpenProcedure != pFunctionStruct[emFuncIndex]);

					*pRetFunctionAddr = pType->TypeInfo.OpenProcedure;

					pType->TypeInfo.OpenProcedure = pFunctionStruct[emFuncIndex];
				}
				else
				{
					//还原
					BDKit_If_Not_Break(*pRetFunctionAddr != pType->TypeInfo.OpenProcedure);

					pType->TypeInfo.OpenProcedure = *pRetFunctionAddr;
				}
			}
		} else if ( g_osVersionCheck >= WINDOWS_VERSION_VISTA && g_osVersionCheck <= WINDOWS_VERSION_8 )
		{
			//6.0的内核
			PVOID pParseProcedure = (PVOID)((PBYTE)pObjectType + NT6_OBJECT_TYPE_TYPEINFO_OFFECT);
			PVOID *pObjectFunctionOffect = NULL;

			switch(emFuncIndex)
			{
			case emObjCloseOper:	{pObjectFunctionOffect = (PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_CLOSE_OFFECT);}break;
			case emObjDeleteOper:	{pObjectFunctionOffect = (PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_DELETE_OFFECT);}break;
			case emObjParseOper:	{pObjectFunctionOffect = (PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_PARSE_OFFECT);}break;
			case emObjSecurityOper:	{pObjectFunctionOffect = (PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_SECURITY_OFFECT);}break;
			case emObjQueryKeyNameOper: {pObjectFunctionOffect = (PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_QUERYNAME_OFFECT);}break;
			case emObjOpenProcedureOper:{pObjectFunctionOffect = (PVOID *)((PBYTE)pParseProcedure + NT6_OBJECT_TYPE_OPEN_OFFECT);}break;
			}

			//设置
			if (bSet)
			{
				BDKit_If_Not_Break(*pObjectFunctionOffect != pFunctionStruct[emFuncIndex]);

				*pRetFunctionAddr = *pObjectFunctionOffect;

				*pObjectFunctionOffect = pFunctionStruct[emFuncIndex];
			}
			else
			{
				//还原
				BDKit_If_Not_Break(*pRetFunctionAddr != *pObjectFunctionOffect);

				*pObjectFunctionOffect = *pRetFunctionAddr;
			}
		}

		nsStatus = STATUS_SUCCESS;				

	} while (FALSE);

	return nsStatus;
}

BOOLEAN BDKitCheckIsDevObjectType(__in PVOID pDevObjectType)
{
	UNICODE_STRING	uniKeyObject	= RTL_CONSTANT_STRING(L"Device");
	BOOLEAN			bResult			= FALSE;

	do 
	{
		BDKit_If_Not_Break(pDevObjectType != NULL);

		if (g_osVersionCheck >= WINDOWS_VERSION_XP && g_osVersionCheck <= WINDOWS_VERSION_2K3_SP1_SP2)
		{
			//5.0内核的
			POBJECT_TYPE50	pType = NULL;
			pType = (POBJECT_TYPE50)pDevObjectType;

			BDKit_If_Not_Break(BDKitMmIsAddressValid(&pType->ObjectTypeName, sizeof(UNICODE_STRING)));
			BDKit_If_Not_Break(BDKitMmIsAddressValid(pType->ObjectTypeName.Buffer, pType->ObjectTypeName.Length));
			BDKit_If_Not_Break(RtlCompareUnicodeString(&uniKeyObject, &pType->ObjectTypeName, TRUE) == 0);
		} else if (g_osVersionCheck >= WINDOWS_VERSION_VISTA && g_osVersionCheck <= WINDOWS_VERSION_8)
		{
			//6.0的内核
			PUNICODE_STRING pObjectName = (PUNICODE_STRING)((PBYTE)pDevObjectType + NT6_OBJECT_TYPE_OBJECT_NAME_OFFECT);

			BDKit_If_Not_Break(BDKitMmIsAddressValid(pObjectName, sizeof(UNICODE_STRING)));
			BDKit_If_Not_Break(BDKitMmIsAddressValid(pObjectName->Buffer, pObjectName->Length));
			BDKit_If_Not_Break(RtlCompareUnicodeString(&uniKeyObject, pObjectName, TRUE) == 0);
		}

		bResult = TRUE;
	} while (FALSE);

	return bResult;
}

NTSTATUS BDKitGetFileNameByFileObj( 
								   __in	PFILE_OBJECT				pFileObj, 
								   __out	POBJECT_NAME_INFORMATION	puszFileFullPath, 
								   __in	ULONG						Length, 
								   __out	PULONG						ReturnLength 
								   )
{
	NTSTATUS		nsStatus	= STATUS_UNSUCCESSFUL;
	UNREFERENCED_PARAMETER(pFileObj);
	UNREFERENCED_PARAMETER(puszFileFullPath);
	UNREFERENCED_PARAMETER(Length);
	UNREFERENCED_PARAMETER(ReturnLength);
	//UNICODE_STRING	unKnowUsed	= RTL_CONSTANT_STRING(L"\\clfs");

	//PAGED_CODE();

	//do 
	//{
	//	nsStatus = ObQueryNameString (pFileObj, puszFileFullPath, Length, ReturnLength);
	//	BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	//	// 此处需要查下
	//	if ( RtlCompareUnicodeString (&puszFileFullPath->Name, &unKnowUsed, FALSE) == 0 )
	//	{
	//		nsStatus = STATUS_SUCCESS;
	//	}
	//	else
	//	{
	//		// TODO use FILEOBJECT TO CONSTRACTE PATH
	//		nsStatus = STATUS_NOT_FOUND;
	//	}
	//}
	//while(FALSE);

	return nsStatus;
}

NTSTATUS BDKitQuerySymbolicLink(
								__in	PUNICODE_STRING pSymbolicLinkName,
								__out PUNICODE_STRING	pLinkTarget
								)
{
	OBJECT_ATTRIBUTES	oa		= {0x00};
	NTSTATUS			nsStatus= STATUS_UNSUCCESSFUL;
	HANDLE				hHandle	= NULL;
	ULONG				ulReturn= 0;

	do 
	{
		BDKit_If_Not_Break(pSymbolicLinkName != NULL && pLinkTarget != NULL && pSymbolicLinkName->Buffer != NULL);

		InitializeObjectAttributes(&oa, pSymbolicLinkName, OBJ_CASE_INSENSITIVE, NULL, NULL);

		nsStatus = ZwOpenSymbolicLinkObject(&hHandle, GENERIC_READ, &oa);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		pLinkTarget->Buffer = NULL;
		pLinkTarget->Length = 0;
		pLinkTarget->MaximumLength = 0;
		nsStatus = ZwQuerySymbolicLinkObject (hHandle, pLinkTarget, &ulReturn);
		BDKit_If_Not_Break(nsStatus == STATUS_BUFFER_TOO_SMALL);

		BDKitAllocateNonpagePool(pLinkTarget->Buffer, ulReturn);
		BDKit_If_Not_Break(pLinkTarget != NULL);
		pLinkTarget->Length			= 0;
		pLinkTarget->MaximumLength	= (USHORT)ulReturn;

		nsStatus = ZwQuerySymbolicLinkObject(hHandle, pLinkTarget, NULL);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKitCloseHandle(hHandle);
	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), pLinkTarget->Buffer);

	return nsStatus;
}

NTSTATUS BDKitQueryModuleInfo(
							  __in CONST PVOID					pModuleKey, 
							  __in emQueryModuleType				emQueryType,
							  __out PSYSTEM_MODULE_INFORMATION	pmsi
							  )
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	PMODULES	pModuleInfo	= NULL;
	ULONG		ulAllocSize	= 0;
	ULONG		ulIndex		= 0;

	do 
	{
		BDKit_If_Not_Break(
				MmIsAddressValid (pModuleKey) 
			&&	emQueryType >=0 
			&&	emQueryType < emQueryEnd 
			&&	pmsi != NULL
			);

		nsStatus = BDKitGetSystemInfo (SystemModuleInformation, &pModuleInfo, &ulAllocSize);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		for (ulIndex = 0; ulIndex < pModuleInfo->ulCount; ++ulIndex)
		{
			switch(emQueryType)
			{
			case emQuerybyName:
				{
					PCHAR	pImageName	= NULL;
					pImageName = pModuleInfo->smi[ulIndex].ImageName;
					BDKit_If_Not_Break(pImageName != NULL);

					if ( _strnicmp ((CONST PCHAR)pModuleKey, pImageName, strlen (pImageName)) == 0 )
					{
						nsStatus = STATUS_SUCCESS;	
						*pmsi = pModuleInfo->smi[ulIndex];
						BDKit_If_Not_Break(FALSE);
					}
				}
				break;

			case emQuerybyAddress:
				{
					ULONG_PTR	ModuleBaseStart	= (ULONG_PTR)pModuleInfo->smi[ulIndex].Base;
					ULONG_PTR	ModuleBaseEnd	= (ULONG_PTR)pModuleInfo->smi[ulIndex].Base 
						+ pModuleInfo->smi[ulIndex].Size;

					if ( (ULONG_PTR)pModuleKey > ModuleBaseStart && (ULONG_PTR)pModuleKey < ModuleBaseEnd )
					{
						nsStatus = STATUS_SUCCESS;
						*pmsi = pModuleInfo->smi[ulIndex];
						BDKit_If_Not_Break(FALSE);
					}
				}
				break;

			default:
				{
					nsStatus = STATUS_NOT_SUPPORTED;
				}
			}

			BDKit_If_Not_Break(nsStatus == STATUS_UNSUCCESSFUL);
		}
	} while (FALSE);

	return nsStatus;
}

VOID BDKitCancelAlertedRequest(__in PKEVENT pkEvent, __in PIRP Irp)
{
	KIRQL			kIrql			= PASSIVE_LEVEL;
	LARGE_INTEGER	liDeltaTime		= {0x00};
	BOOLEAN			bCanceled		= FALSE;

	if ( pkEvent != NULL && Irp != NULL )
	{
		KeRaiseIrql(APC_LEVEL, &kIrql);

		if (KeReadStateEvent(pkEvent) == 0) 
		{
			bCanceled = IoCancelIrp(Irp);

			KeLowerIrql(kIrql);

			if (bCanceled) 
			{
				liDeltaTime.QuadPart = - 10 * 1000 * 10;

				while (KeReadStateEvent(pkEvent) == 0) 
				{
					KeDelayExecutionThread(KernelMode, FALSE, &liDeltaTime);
				}
			} 
			else 
			{
				(VOID)KeWaitForSingleObject(
					pkEvent,
					Executive,
					KernelMode,
					FALSE,
					(PLARGE_INTEGER)NULL
					);
			}
		} 
		else 
		{
			KeLowerIrql(kIrql);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Authorized Process //

NTSTATUS BDKitAddAuthorizedProcess(__in CONST PEPROCESS EProcess)
{
	if ( g_AuthorizedProcessHandle == NULL )
	{
		g_AuthorizedProcessHandle = BDKitInitializeList ('CORP');
	}

	ObReferenceObject(EProcess);

	BDKitInsertListNode (g_AuthorizedProcessHandle, EProcess);

	return STATUS_SUCCESS;
}

NTSTATUS BDKitIsAuthorizedProcess(__in CONST PEPROCESS EProcess)
{
	if ( g_AuthorizedProcessHandle == NULL )
	{
		return STATUS_NOT_FOUND;
	}

	return BDKitFindListNode (g_AuthorizedProcessHandle, EProcess) != NULL? STATUS_SUCCESS : STATUS_NOT_FOUND;
}

VOID ClearEProcessObject(PVOID pNode)
{
	if ( pNode != NULL )
	{
		BDKitCloseObject(pNode);
	}
}

NTSTATUS BDKitRemoveAuthorizedProcess( __in CONST PEPROCESS EProcess )
{
	if ( g_AuthorizedProcessHandle == NULL )
	{	
		return STATUS_NOT_FOUND;
	}

	if ( BDKitRemoveListNode (g_AuthorizedProcessHandle, EProcess) == TRUE )
	{
		ClearEProcessObject ((PVOID)EProcess);

		return STATUS_SUCCESS;
	}

	return STATUS_NOT_FOUND;
}

NTSTATUS BDKitClearAuthorizedProcess(VOID)
{
	BDKitCloseListHandleWithHandler (g_AuthorizedProcessHandle, ClearEProcessObject);

	return STATUS_SUCCESS;
}

// Authorized Process //
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Shutdown Delete File List //

VOID DeleteFilePathFiles(PVOID pNode)
{
	if ( KeGetCurrentIrql() == PASSIVE_LEVEL && MmIsAddressValid (pNode) )
	{
		UNICODE_STRING	uszFilePath = {0x00};
		ULONG information	= 0;

		RtlInitUnicodeString (&uszFilePath, (PWSTR)pNode);

		BDKitFocusDeleteFile(&uszFilePath, wcslen (pNode) * sizeof(WCHAR), NULL, 0, &information, emNormalLevel);

		BDKitFreePool(pNode);
	}
}

NTSTATUS BDKitAddDeleteFileList(__in CONST PWSTR pFilePath)
{
	if ( g_DeleteFileListHandle == NULL )
	{
		g_DeleteFileListHandle = BDKitInitializeList ('ELIF');
	}

	BDKitInsertListNode (g_DeleteFileListHandle, pFilePath);

	return STATUS_SUCCESS;
}

NTSTATUS BDKitClearDeleteFileList( VOID )
{
	BDKitCloseListHandleWithHandler (g_DeleteFileListHandle, DeleteFilePathFiles);

	return STATUS_SUCCESS;
}

// Shutdown Delete File List //
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// BDArKit Relative //
DebugFlag BDKitCheckRunMode(PUNICODE_STRING puszRegistry)
{
	ULONG	 ulDebug  = emNormalRun;

	UNREFERENCED_PARAMETER(puszRegistry);

	if (puszRegistry != NULL) {
		NTSTATUS nsStatus = STATUS_UNSUCCESSFUL;

		RTL_QUERY_REGISTRY_TABLE QueryTable[] = 
		{
			{NULL, RTL_QUERY_REGISTRY_REQUIRED | RTL_QUERY_REGISTRY_DIRECT | RTL_QUERY_REGISTRY_DELETE, 
			BDArKit_Debug_Mode, &ulDebug, REG_DWORD, NULL, sizeof(ulDebug)},
			{NULL, 0, NULL, NULL, 0, NULL, 0}
		};

		do 
		{
			__try
			{
				nsStatus = RtlQueryRegistryValues(
					RTL_REGISTRY_SERVICES,
					BDArKit_DRIVER_NAMEW,
					QueryTable, 
					NULL, 
					NULL
					);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
			}
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
		} while (FALSE);
		if (!NT_SUCCESS(nsStatus)) {
			ulDebug = emNormalRun;
		}
	}

	return (DebugFlag)ulDebug;
}
// BDArKit Relative //
//////////////////////////////////////////////////////////////////////////