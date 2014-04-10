#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "ntimage.h"
#include "BDArKit\KitHelper.h"
#include "NtosKrnl.h"
#include "Utility.h"
#include "List.h"
#include "PELoader.h"

static BOOLEAN	g_IATHasHooked				= FALSE;
static PVOID	g_KerMmFlushImageSection	= NULL;
static PVOID*	g_fnNtfFlashImage			= NULL;
static PVOID*	g_fnFastFATFlashImage		= NULL;
static WCHAR	g_MmFlushImageSectionStrW[] = L"MmFlushImageSection";
static CHAR		g_MmFlushImageSectionStrA[] = "MmFlushImageSection";

#define UPPER_ALIGN(_addr_, _align_) (((ULONG_PTR)_addr_ + (ULONG_PTR)_align_ - 1) & ~((ULONG_PTR)(_align_ - 1)))
#define LOWER_ALIGN(_addr_, _align_) ((ULONG_PTR)_addr_ & ~((ULONG_PTR)(_align_ - 1)))

//////////////////////////////////////////////////////////////////////////

HANDLE g_DelFileHandle = NULL;

PVOID GetSystemRoutineAddress(__in CONST WCHAR* wchFuncName)
{
	UNICODE_STRING usFuncName = {0x00};
	RtlInitUnicodeString (&usFuncName ,wchFuncName);

	return MmGetSystemRoutineAddress(&usFuncName);
}

PVOID KernelGetModuleBase(__in CONST PCHAR pModuleName, __out PULONG CONST ModuleSize)
{
	NTSTATUS					nsStatus			= STATUS_UNSUCCESSFUL;
	PLOADED_MOD_INFO			pSystemInfoBuffer	= NULL;
	PSYSTEM_MODULE_INFORMATION	pSysModuleEntry		= NULL;
	PVOID						pModuleBase			= NULL;
	ULONG						mIndex				= 0;
	ULONG						ulSystemInfoLen		= 0;

	do 
	{
		nsStatus = BDKitGetSystemInfo(SystemModuleInformation, &pSystemInfoBuffer, &ulSystemInfoLen);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && pSystemInfoBuffer != NULL);

		__try
		{
			pSysModuleEntry = pSystemInfoBuffer->pmi;

			for (mIndex = 0; mIndex < pSystemInfoBuffer->ulCount; mIndex++, pSysModuleEntry ++)
			{
				if ( _stricmp(pSysModuleEntry->ImageName + pSysModuleEntry->ModuleNameOffset, pModuleName) == 0 )
				{
					pModuleBase = pSysModuleEntry->Base;
					*ModuleSize = pSysModuleEntry->Size;
					break;
				}
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			// TODO
		}
	} while (FALSE);

	BDKitFreePool(pSystemInfoBuffer);

	return pModuleBase;
}

NTSTATUS MapImageToMemory(
						  __in	ULONG_PTR		pImageBuffer,
						  __in  ULONG			ImageLen,
						  __out PVOID*	CONST	ppImageBase,
						  __out PULONG	CONST	pulImageSize
						  )
{
	NTSTATUS				nsStatus		= STATUS_UNSUCCESSFUL;
	PIMAGE_DOS_HEADER		pImgDosHeader	= NULL;
	PIMAGE_NT_HEADERS		pImgNtHeader	= NULL;
	PIMAGE_OPTIONAL_HEADER	pOptionalHeader	= NULL;
	PIMAGE_SECTION_HEADER	pSecHeader		= NULL;
	PVOID					pImageBase		= NULL;
	ULONG					ulImageSize		= 0;
	ULONG					ulSecIndex		= 0;
	ULONG					ulSecCount		= 0;
	ULONG					ulSectionAlign	= 0;
	ULONG					ulFileAlign		= 0;
	ULONG_PTR				pMap			= 0;

	do 
	{
		*pulImageSize = 0;
		*ppImageBase = NULL;

		pImgDosHeader = (PIMAGE_DOS_HEADER)pImageBuffer;
		BDKit_If_Not_Break(pImgDosHeader != NULL 
			&& ImageLen >= 0x2000 
			&& (ULONG_PTR)pImgDosHeader->e_lfanew <= 0x1000
			&& pImgDosHeader->e_magic == IMAGE_DOS_SIGNATURE);

		pImgNtHeader		= (PIMAGE_NT_HEADERS)(pImageBuffer + pImgDosHeader->e_lfanew);
		pOptionalHeader		= &(pImgNtHeader->OptionalHeader);
		ulSecCount			= pImgNtHeader->FileHeader.NumberOfSections;
		pSecHeader			= (PIMAGE_SECTION_HEADER)((ULONG_PTR)pOptionalHeader 
			+ pImgNtHeader->FileHeader.SizeOfOptionalHeader);

		ulImageSize			= pOptionalHeader->SizeOfImage;
		ulSectionAlign		= pOptionalHeader->SectionAlignment & 0xFFFF;//对付0xFF001000的极端情况
		ulFileAlign			= pOptionalHeader->FileAlignment;

		BDKit_If_Not_Break(pImgNtHeader->Signature == IMAGE_NT_SIGNATURE
			&& ulImageSize >= 0x1000 && ulImageSize <= 16 * 1024 * 1024
			&& pOptionalHeader->SizeOfHeaders < ulImageSize
			&& pImgNtHeader->FileHeader.SizeOfOptionalHeader < 0x800
			&& ulSecCount < 0x80);

		BDKitAllocateNonpagePool(pImageBase, ulImageSize);
		BDKit_If_Not_Break_With_Reason(pImageBase != NULL, STATUS_INSUFF_SERVER_RESOURCES);

		*pulImageSize = ulImageSize;

		if(ulSectionAlign < 0x1000 && ulImageSize == ImageLen)	//no map
		{
			nsStatus = STATUS_SUCCESS;
			RtlCopyMemory((PVOID)(pImageBase), (PVOID)pImageBuffer, ulImageSize);
			break;	
		}

		memcpy((PVOID)(pImageBase), (PVOID)pImageBuffer, pOptionalHeader->SizeOfHeaders);

		for (ulSecIndex = 0; ulSecIndex < ulSecCount; ulSecIndex++)
		{
			ULONG ulVilSize = pSecHeader[ulSecIndex].Misc.VirtualSize;
			ULONG ulRawSize = pSecHeader[ulSecIndex].SizeOfRawData;
			ULONG ulVilOff = pSecHeader[ulSecIndex].VirtualAddress;
			ULONG ulRawOff = pSecHeader[ulSecIndex].PointerToRawData;

			if(0 == ulVilSize) 
				ulVilSize = ulRawSize;

			ulVilSize	= UPPER_ALIGN(ulVilSize, ulSectionAlign);
			ulRawOff	= LOWER_ALIGN(ulRawOff, ulFileAlign);
			ulRawSize	= UPPER_ALIGN(ulRawSize, ulFileAlign);

			BDKit_If_Not_Break(ulRawOff <= ImageLen && ulVilOff <= ulImageSize);

			if( (ulRawOff + ulRawSize) > ImageLen)
				ulRawSize = ImageLen - ulRawOff;
			if( (ulVilOff + ulVilSize) > ulImageSize)
				ulVilSize = ulImageSize - ulVilOff;

			//////////////////////////////////////////////////////////////////////////

			pMap = (ULONG)(pImageBase) + ulVilOff;

			if(ulRawSize > ulVilSize)
				ulRawSize = ulVilSize;
			if(ulRawSize)
				memcpy((PVOID)pMap, (PVOID)(pImageBuffer + ulRawOff), ulRawSize);
			if(ulRawSize < ulVilSize)
				memset((PVOID)(pMap + ulRawSize), 0, ulVilSize - ulRawSize);
		}

		nsStatus = ulSecIndex == ulSecCount? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;

	} while (FALSE);

	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), BDKitFreePool(pImageBase));
	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), *pulImageSize = 0);

	*ppImageBase = pImageBase;

	return nsStatus;
}

NTSTATUS ReadImageFile(
					   __in CONST PWCHAR	FileName, 
					   __out PVOID* CONST	ppImageBuffer, 
					   __out PULONG CONST	pImageLen
					   )
{
	NTSTATUS			nsStatus				= {0x00};
	UNICODE_STRING		uszNtModuleName			= {0x00};
	WCHAR				wszPathBuffer[MAX_PATH]	= {0x00};
	OBJECT_ATTRIBUTES	oa						= {0x00};
	IO_STATUS_BLOCK		IoStatus				= {0x00};
	FILE_STANDARD_INFORMATION fileStandardInfo	= {0x00};
	HANDLE				hNtModule				= NULL;
	PVOID				pImageBuffer			= NULL;
	ULONG				ulImageLen				= 0;

	do 
	{
		BDKit_If_Not_Break((wcslen (FileName) + wcslen (L"\\SystemRoot\\System32\\")) < MAX_PATH);

		wcscat(wszPathBuffer, L"\\SystemRoot\\System32\\");
		wcscat(wszPathBuffer, FileName);

		RtlInitUnicodeString(&uszNtModuleName, wszPathBuffer);

		InitializeObjectAttributes(&oa, &uszNtModuleName, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);
		nsStatus = ZwOpenFile(
			&hNtModule,
			GENERIC_READ,
			&oa,
			&IoStatus,
			FILE_SHARE_VALID_FLAGS,
			FILE_NON_DIRECTORY_FILE|FILE_SYNCHRONOUS_IO_ALERT
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = ZwQueryInformationFile(
			hNtModule,
			&IoStatus,
			&fileStandardInfo,
			sizeof(FILE_STANDARD_INFORMATION),
			FileStandardInformation
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		ulImageLen = (ULONG)fileStandardInfo.EndOfFile.QuadPart;
		BDKitAllocateNonpagePool(pImageBuffer, ulImageLen);
		BDKit_If_Not_Break_With_Reason(pImageBuffer != NULL, STATUS_INSUFF_SERVER_RESOURCES);

		nsStatus = ZwReadFile(
			hNtModule,
			NULL,
			NULL,
			NULL,
			&IoStatus,
			pImageBuffer,
			ulImageLen,
			NULL,
			NULL
			);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	BDKitCloseHandle(hNtModule);
	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), BDKitFreePool(pImageBuffer));
	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), ulImageLen = 0);

	*ppImageBuffer	= pImageBuffer;
	*pImageLen		= ulImageLen;

	return nsStatus;
}

PVOID* GetFuncIATFromImage(__in PUCHAR pModuleBase, __in ULONG ModuleSize, __in PCHAR FuncName)
{
	PIMAGE_DOS_HEADER			pImgDosHeader	= NULL;
	PIMAGE_NT_HEADERS			pImgNtHeader	= NULL;
	PIMAGE_DATA_DIRECTORY		ImportsDir		= NULL;
	PIMAGE_IMPORT_DESCRIPTOR	Import			= NULL;
	PULONG_PTR					pThunk			= NULL;
	PULONG_PTR					pFirstThunk		= NULL;
	PCHAR						TmpStr			= NULL;
	PVOID*						pFuncAddress	= NULL;
	BOOLEAN						bImportNtoskrnl = FALSE;

	do 
	{
		pImgDosHeader = (PIMAGE_DOS_HEADER)pModuleBase;
		BDKit_If_Not_Break(pModuleBase != NULL
			&& ModuleSize >= 0x2000 && (ULONG_PTR)pImgDosHeader->e_lfanew <= 0x1000
			&& pImgDosHeader->e_magic == IMAGE_DOS_SIGNATURE);

		pImgNtHeader=(PIMAGE_NT_HEADERS)(pModuleBase + pImgDosHeader->e_lfanew);

		ImportsDir = pImgNtHeader->OptionalHeader.DataDirectory + IMAGE_DIRECTORY_ENTRY_IMPORT;

		BDKit_If_Not_Break(ImportsDir->VirtualAddress <= ModuleSize 
			|| (ImportsDir->VirtualAddress + sizeof(IMAGE_IMPORT_DESCRIPTOR)) <= ModuleSize);

		Import = (PIMAGE_IMPORT_DESCRIPTOR)(ImportsDir->VirtualAddress + pModuleBase);

		_try 
		{
			while(0 != Import->Name && Import->Name < ModuleSize)
			{
				TmpStr = (PCHAR)(Import->Name + pModuleBase);
				if(0 == _strnicmp(TmpStr, "ntoskrnl.exe", 12))
				{
					bImportNtoskrnl = TRUE;
					break;
				}

				Import ++;
			}

		}
		_except(EXCEPTION_EXECUTE_HANDLER)
		{
			// TODO
		}

		BDKit_If_Not_Break(bImportNtoskrnl == TRUE);

		_try
		{
			pThunk = (ULONG_PTR *)(Import->FirstThunk + pModuleBase);
			pFirstThunk = (ULONG_PTR *)(Import->OriginalFirstThunk + pModuleBase);

			for(; *pFirstThunk && *pThunk; pFirstThunk ++, pThunk ++)
			{
				if(*(int *)pFirstThunk < 0)
				{
				}
				else
				{
					TmpStr = (PCHAR)(*pFirstThunk + pModuleBase) + 2;
					if(0 == strcmp(TmpStr, FuncName))
					{
						pFuncAddress = (PVOID *)((ULONG_PTR)pThunk - (ULONG_PTR)pModuleBase);
						break;
					}
				}

			}
		}
		_except(EXCEPTION_EXECUTE_HANDLER)
		{
			// TODO
		}

	} while (FALSE);

	return pFuncAddress;
}

BOOLEAN MatchFileMemoryVersion(__in PVOID FileModuleBase, __in PVOID KerModuleBase, __in ULONG ModuleSize)
{
	ULONG_PTR			fileRAVOffset	= 0;
	ULONG_PTR			memRAVOffset	= 0;
	IMAGE_NT_HEADERS*	fileMemory		= NULL;
	IMAGE_NT_HEADERS*	memMemory		= NULL;
	BOOLEAN				bVersionOk		= FALSE;

	do 
	{
		fileRAVOffset = ((IMAGE_DOS_HEADER*)FileModuleBase)->e_lfanew;
		BDKit_If_Not_Break(fileRAVOffset >= sizeof(IMAGE_DOS_HEADER) && fileRAVOffset < ModuleSize);

		fileMemory = (IMAGE_NT_HEADERS*)((ULONG_PTR)(FileModuleBase) + fileRAVOffset);

		memRAVOffset=((IMAGE_DOS_HEADER*)KerModuleBase)->e_lfanew;
		BDKit_If_Not_Break(memRAVOffset >= sizeof(IMAGE_DOS_HEADER) && memRAVOffset < ModuleSize);

		memMemory = (IMAGE_NT_HEADERS*)((ULONG_PTR)(KerModuleBase) + memRAVOffset);
		
		BDKit_If_DoAction(memMemory->FileHeader.TimeDateStamp == fileMemory->FileHeader.TimeDateStamp,
			bVersionOk = TRUE);
		BDKit_If_DoAction(memMemory->OptionalHeader.CheckSum == fileMemory->OptionalHeader.CheckSum,
			bVersionOk = TRUE);
	} while(FALSE);

	return bVersionOk;
}

PVOID* GetFuncIATFromFile(__in CONST PWCHAR SysFileName, __in CONST PCHAR FuncName, __in_opt PVOID KerModuleBase)
{
	NTSTATUS	nsStatus		= STATUS_UNSUCCESSFUL;
	PVOID*		pFuncAddress	= NULL;
	ULONG		ImageLen		= 0;
	PVOID		ImageBase		= NULL;
	PVOID		ImageBuffer		= NULL;
	ULONG		ImageSize		= 0;

	do 
	{
		nsStatus = ReadImageFile(SysFileName, (PVOID* CONST)&ImageBuffer, &ImageLen);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && ImageBuffer != 0);

		nsStatus = MapImageToMemory((ULONG_PTR)ImageBuffer, ImageLen, (PVOID* CONST)&ImageBase, &ImageSize);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus) && ImageBase != 0);

		if ( KerModuleBase != NULL )
		{
			BDKit_If_Not_Break_With_Reason(MatchFileMemoryVersion (ImageBase, KerModuleBase, ImageSize) == TRUE,
				STATUS_DS_VERSION_CHECK_FAILURE);
		}

		pFuncAddress = GetFuncIATFromImage((PUCHAR)ImageBase, ImageSize, FuncName);
		BDKit_If_Not_Break(pFuncAddress != NULL);

	} while (FALSE);

	BDKitFreePool(ImageBuffer);
	BDKitFreePool(ImageBase);

	return pFuncAddress;
}

void PageProtectOn()
{
#ifdef _X86_
	_asm
	{
		push eax
			mov eax,cr0
			or  eax,0x10000
			mov cr0,eax
			pop eax
			sti
	}
#endif // _X86_
}

void PageProtectOff()
{
#ifdef _X86_
	_asm
	{
		cli
			push eax
			mov eax,cr0
			and eax,not 0x10000
			mov cr0,eax
			pop eax
	}
#endif // _X86_
}

BOOLEAN SectionPointCompare(PVOID pNode1, PVOID pNode2)
{
	PSECTION_OBJECT_POINTERS	pSection1 = (PSECTION_OBJECT_POINTERS)pNode1;
	PSECTION_OBJECT_POINTERS	pSection2 = (PSECTION_OBJECT_POINTERS)pNode2;

	return 0 == memcmp(pSection1, pSection2, sizeof(SECTION_OBJECT_POINTERS));
}

BOOLEAN WINAPI _MmFlushImageSection(
									__in PSECTION_OBJECT_POINTERS SectionPointer,
									__in MMFLUSH_TYPE FlushType
									)
{
	if(MmFlushForDelete == FlushType && g_DelFileHandle != NULL)
	{
		if (BDKitFindListNodeWithHandler (g_DelFileHandle, SectionPointer, SectionPointCompare) != NULL )
		{
			return TRUE;
		}
	}

	return MmFlushImageSection(SectionPointer, FlushType);
}

BOOLEAN BDKitEnableIATHook( VOID )
{
#ifdef _X86_
	ULONG	KernelModuleSize	= 0;
	PVOID	pKerModuleBase		= NULL;
	PVOID*	IatOffset			= NULL;

	do 
	{
		BDKit_If_Not_Break(g_IATHasHooked == FALSE);

		g_KerMmFlushImageSection = GetSystemRoutineAddress(g_MmFlushImageSectionStrW);
		BDKit_If_Not_Break(g_KerMmFlushImageSection != NULL);

		pKerModuleBase = KernelGetModuleBase("ntfs.sys", &KernelModuleSize);
		if ( pKerModuleBase != NULL )
		{
			IatOffset = GetFuncIATFromFile(L"drivers\\ntfs.sys", g_MmFlushImageSectionStrA, pKerModuleBase);
			if( IatOffset != NULL )
			{
				IatOffset = (PVOID*)((ULONG_PTR)IatOffset + (ULONG_PTR)pKerModuleBase);
				if(*IatOffset == g_KerMmFlushImageSection)
				{
					PageProtectOff();
					InterlockedExchange((LONG volatile*)IatOffset, (LONG)_MmFlushImageSection);
					PageProtectOn();
					g_fnNtfFlashImage = IatOffset;
				}
			}
		}

		pKerModuleBase = KernelGetModuleBase("FastFat.sys", &KernelModuleSize);
		BDKit_If_Not_Break(pKerModuleBase != NULL);

		IatOffset = GetFuncIATFromFile(L"drivers\\FastFat.sys", g_MmFlushImageSectionStrA, pKerModuleBase);
		if(IatOffset)
		{
			IatOffset = (PVOID*)((ULONG_PTR)IatOffset + (ULONG_PTR)pKerModuleBase);
			if(*IatOffset == g_KerMmFlushImageSection)
			{
				PageProtectOff();
				InterlockedExchange((LONG volatile*)IatOffset, (LONG)_MmFlushImageSection);
				PageProtectOn();
				g_fnFastFATFlashImage = IatOffset;
			}
		}

	} while (FALSE);

	BDKit_If_DoAction(g_fnFastFATFlashImage != NULL || g_fnNtfFlashImage != NULL, 
		InterlockedExchange((LONG volatile*)&g_IATHasHooked, TRUE));
	BDKit_If_DoAction(g_IATHasHooked == TRUE && g_DelFileHandle == NULL, g_DelFileHandle = BDKitInitializeList('EDAI'));
#endif // _X86_

	return g_IATHasHooked;
}

VOID BDKitDisableIATHook(VOID)
{
	if( g_fnNtfFlashImage != NULL )
	{
		PageProtectOff();
		InterlockedExchange((LONG volatile*)g_fnNtfFlashImage, (LONG)g_KerMmFlushImageSection);
		PageProtectOn();
	}

	if( g_fnFastFATFlashImage != NULL )
	{
		PageProtectOff();
		InterlockedExchange((LONG volatile*)g_fnFastFATFlashImage, (LONG)g_KerMmFlushImageSection);
		PageProtectOn();
	}

	InterlockedExchange((LONG volatile*)&g_IATHasHooked, FALSE);

	BDKitCloseListHandle(g_DelFileHandle);
	g_DelFileHandle = NULL;
}