#pragma once


typedef NTSTATUS BDKIT_UTILITY_HANDLER(
									   __in_bcount(ulInBufferSize)	PVOID pInBuffer,
									   __in		ULONG			ulInBufferSize,
									   __out_bcount(ulOutBufferSize) PVOID pOutBuffer,
									   __in		ULONG			ulOutBufferSize,
									   __out	PULONG			information,
									   __in		ULONG			level
									   );

BDKIT_UTILITY_HANDLER BDKitFocusCreateFile;
BDKIT_UTILITY_HANDLER BDKitFocusReadFile;
BDKIT_UTILITY_HANDLER BDKitFocusWriteFile;
BDKIT_UTILITY_HANDLER BDKitFocusCloseFile;
BDKIT_UTILITY_HANDLER BDKitFocusDeleteFile;
BDKIT_UTILITY_HANDLER BDKitAddDeleteFileOnReboot;
BDKIT_UTILITY_HANDLER BDKitRemoveDeleteFileOnReboot;
BDKIT_UTILITY_HANDLER BDKitAddRecoverFileOnReboot;
BDKIT_UTILITY_HANDLER BDKitRemoveRecoverFileOnReboot;
BDKIT_UTILITY_HANDLER BDKitFocusLockFile;
BDKIT_UTILITY_HANDLER BDKitFocusUnlockFile;
BDKIT_UTILITY_HANDLER BDKitFocusCopyFile;
BDKIT_UTILITY_HANDLER BDKitDenyFileCreate;
BDKIT_UTILITY_HANDLER BDKitRemoveDenyFileCreate;
BDKIT_UTILITY_HANDLER BDKitFindFirstFile;
BDKIT_UTILITY_HANDLER BDKitFindNextFile;
BDKIT_UTILITY_HANDLER BDKitQueryObject;
BDKIT_UTILITY_HANDLER BDKitFocusOpenProcess;
BDKIT_UTILITY_HANDLER BDKitFocusKillProcess;
BDKIT_UTILITY_HANDLER BDKitFocusKillProcessId;

typedef struct
{
	PUNICODE_STRING  pExistingFileName;
	PUNICODE_STRING	 pNewFileName;
	PVOID			 lpProgressRoutine;
	LPVOID			 lpData;
	LPBOOL			 pbCancel;
	DWORD			 dwCopyFlags;
} COPY_FILE_EX_PROXY, *PCOPY_FILE_EX_PROXY;