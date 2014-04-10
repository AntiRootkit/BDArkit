#pragma once




NTSYSAPI
PVOID NTAPI RtlImageDirectoryEntryToData (
	__in PVOID	Base,
	__in BOOLEAN	MappedAsImage,
	__in USHORT	DirectoryEntry,
	__out PULONG	Size
	);


NTSTATUS BDkitPELoader(
					   __in WCHAR*		wszPEFullFileName,
					   __in ULONG_PTR		SystemKernelModuleBase,
					   __out PULONG_PTR	pImageModuleBase
					   );

NTSTATUS BDKitLoadExportFunction(__in ULONG_PTR KernelModuleBase, __in ULONG_PTR ImageModuleBase);

PVOID BDKitGetFunctionAddressFromImage( __in PVOID ImageBase, __in PCHAR pFuncName );

NTSTATUS BDKitMapImageSection2Memory( __in PUNICODE_STRING puszImageFullPath, __out PULONG_PTR pImageBase );

BOOLEAN BDKitEnableIATHook(VOID);

VOID BDKitDisableIATHook(VOID);