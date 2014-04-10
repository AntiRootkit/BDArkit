#pragma once


typedef VOID (*REBOOT_ACTION_CALLBACK) (PVOID pData, ULONG ulLength, PVOID pContext);
typedef struct
{
	DWORD		Signature;
	ULONG64		ulNextOffset;
	ULONG		ulLength;
	UCHAR		Data[0];
} SectionHeader, *PSectionHeader;

NTSTATUS BDKitAppendDataToFile(__in ULONG Tag, __in PVOID pUsrData, __in ULONG ulUsrLength);
NTSTATUS BDKitRemoveDataFromFile(__in ULONG Tag, __in PVOID pUsrData, __in ULONG ulDataLength);
// This Function should be called ONCE!!
NTSTATUS BDKitExecuteRebootHandler(__in PDEVICE_OBJECT DeviceObject);
NTSTATUS BDKitUnloadRebootHandler(VOID);