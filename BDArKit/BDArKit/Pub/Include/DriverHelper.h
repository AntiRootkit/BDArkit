#pragma once


#define IO_ATTACH_DEVICE_API            0x80000000

typedef enum 
{
	WINDOWS_VERSION_NONE			= 0x00000000,
	WINDOWS_VERSION_2K				= 0x00000001,
	WINDOWS_VERSION_XP				= 0x00000002,
	WINDOWS_VERSION_2K3				= 0x00000004,
	WINDOWS_VERSION_2K3_SP1_SP2		= 0x00000008,
	WINDOWS_VERSION_VISTA			= 0x00000010,
	WINDOWS_VERSION_7				= 0x00000020,
	WINDOWS_VERSION_2008			= 0x00000040,
	WINDOWS_VERSION_8				= 0x00000080,
	WINDOWS_VERSION_8_1				= 0x00000100,
	WINDOWS_VERSION_END				= 0x00000200,
} WIN_VER_DETAIL;

typedef enum
{
	emDbgNone						= 0x00000000,
	emDbgNormal						= 0x00000001,
	emDbgWarning					= 0x00000002,
	emDbgError						= 0x00000004,
} DBG_LEVEL;

typedef struct _BDKIT_DEVICE_EXTENSION
{
	volatile ULONG		DeviceRefence;
	UNICODE_STRING		uszDeviceName;
	UNICODE_STRING		uszSymbolLink;
	PDEVICE_OBJECT		pFunDeviceObject;
	//BDKIT_PROCESS_ENV	ProcessEnv;
} BDKIT_DEVICE_EXTENSION, *PBDKIT_DEVICE_EXTENSION;


extern WIN_VER_DETAIL	g_osVersionCheck;
extern CONST ULONG32	g_bRunningX86;
extern DBG_LEVEL		g_DbgOutputLevel;
extern PDRIVER_OBJECT	g_DriverObject;
extern PUNICODE_STRING	g_DriverRegistry;

typedef NTSTATUS BDKIT_IOCTL_HANDLER(
									 __in		PDEVICE_OBJECT	DeviceObject,
									 __in_bcount(ulInBufferSize) PVOID pInBuffer,
									 __in		ULONG			ulInBufferSize,
									 __out_bcount(ulOutBufferSize) PVOID pOutBuffer,
									 __in		ULONG			ulOutBufferSize,
									 __out		PULONG			information
									 );

typedef NTSTATUS BDKIT_DRIVER_DISKPATCHER(__in PDEVICE_OBJECT pDeviceObject, __in PIRP Irp);
BDKIT_DRIVER_DISKPATCHER DriverDispatchHandler;
BDKIT_DRIVER_DISKPATCHER DriverControlUtilityHandler;

BDKIT_IOCTL_HANDLER doFocusCreateFile;
BDKIT_IOCTL_HANDLER doFocusReadFile;
BDKIT_IOCTL_HANDLER doFocusWriteFile;
BDKIT_IOCTL_HANDLER doFocusCloseFile;
BDKIT_IOCTL_HANDLER doFocusDeleteFile;
BDKIT_IOCTL_HANDLER doFocusLockFile;
BDKIT_IOCTL_HANDLER doFocusUnlockFile;
BDKIT_IOCTL_HANDLER doFocusCopyFile;
BDKIT_IOCTL_HANDLER doDenyFileCreate;
BDKIT_IOCTL_HANDLER doFindFirstFile;
BDKIT_IOCTL_HANDLER doFindNextFile;
BDKIT_IOCTL_HANDLER doFocusQueryObject;
BDKIT_IOCTL_HANDLER doFocusOpenProcess;
BDKIT_IOCTL_HANDLER doFocusKillProcess;
BDKIT_IOCTL_HANDLER doFocusKillProcessId;