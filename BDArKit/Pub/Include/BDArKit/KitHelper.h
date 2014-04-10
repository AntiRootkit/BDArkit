#pragma once


#define BDArKit_DRIVER_NAMEW				L"BDArKit"
#define BDArKit_DRIVER_FILENAMEW			L"BDArKit.SYS"
#define BDArKit_DRIVER_FILENAME64W			L"BDArKit.SYS"
#define BDArKit_DEVICE_NAMEW_1(driverName)	L"\\Device\\"##driverName
#define BDArKit_SYMBOL_NAMEW_1(driverName)	L"\\DosDevices\\"##driverName
#define BDArKit_SYMBOL_LINKW_1(driverName)	L"\\\\.\\"##driverName
#define BDArKit_INSTANCE_NAMEW				L"BDArKitFilter"
#define BDArKit_Filter_ALTITUDE				L"326914"
#define BDArKit_Debug_Mode					L"DebugFlag"
#define BDArKit_DEVICE_NAMEW				BDArKit_DEVICE_NAMEW_1(BDArKit_DRIVER_NAMEW)
#define BDArKit_SYMBOL_NAMEW				BDArKit_SYMBOL_NAMEW_1(BDArKit_DRIVER_NAMEW)
#define BDArKit_R3_SYMBOL_LINK				BDArKit_SYMBOL_LINKW_1(BDArKit_DRIVER_NAMEW)

#define MAX_IMAGE_NAME						60

#define SIZEOF_MD5							16
#define USR_MAX_PATH						(MAX_PATH - 1)

typedef enum _BDKitClearLevel
{
	emNormalLevel		= 0x00000000,
	emMiddleLevel		= 0x00000001,
	emFocusLevel		= 0x00000002,
	emPowerfulLevel		= 0x00000003,
	emIgnoreLevel		= 0x00000004,
} BDKitClearLevel;

#define BDKIT_FUNCTION( Function ) (Function + 0x800)

// Process actions
#define IOCTL_KILL_PROCESS			CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(100), METHOD_BUFFERED, FILE_WRITE_DATA)
#define IOCTL_KILL_PROCESSID		CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(101), METHOD_BUFFERED, FILE_WRITE_DATA)
#define IOCTL_OPEN_PROCESS			CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(102), METHOD_BUFFERED, FILE_WRITE_DATA | FILE_READ_ACCESS)

// File actions
#define IOCTL_CREATE_FILE			CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(200), METHOD_BUFFERED, FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_CLOSE_FILE			CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(201), METHOD_BUFFERED, FILE_WRITE_DATA)
#define IOCTL_READ_FILE				CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(202), METHOD_BUFFERED, FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_WRITE_FILE			CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(203), METHOD_BUFFERED, FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_DELETE_FILE			CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(204), METHOD_BUFFERED, FILE_WRITE_DATA)
#define IOCTL_UNLOCK_FILE			CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(206), METHOD_BUFFERED, FILE_WRITE_DATA)
#define IOCTL_FILE_ACCESS_DENY		CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(207), METHOD_BUFFERED, FILE_WRITE_DATA)
#define IOCTL_FIND_FIRST_FILE		CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(208), METHOD_BUFFERED, FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_FIND_NEXT_FILE		CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(209), METHOD_BUFFERED, FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_QUERY_OBJECT			CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(210), METHOD_BUFFERED, FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_LOCK_FILE				CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(211), METHOD_BUFFERED, FILE_WRITE_DATA)
#define IOCTL_COPY_FILE				CTL_CODE(FILE_DEVICE_UNKNOWN, BDKIT_FUNCTION(212), METHOD_BUFFERED, FILE_WRITE_DATA)
#pragma pack(4)

typedef VOID* __ptr64 HANDLE64;
typedef LONG BDSTATUS;

#ifndef _NTDDK_
typedef struct _UNICODE_STRING {
	USHORT				Length;
	USHORT				MaximumLength;
	WCHAR* __ptr64		Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef enum _OBJECT_INFO_CLASS {
	ObjectBasicInfo,
	ObjectNameInfo,
	ObjectTypeInfo,
	ObjectAllTypesInfo,
	ObjectProtectionInfo
} OBJECT_INFO_CLASS;
#endif

typedef struct _CLIENT_ID64 {
	HANDLE64			UniqueProcess;
	HANDLE64			UniqueThread;
} CLIENT_ID64;
typedef CLIENT_ID64 *PCLIENT_ID64;

typedef struct _KIT_STATUS_BLOCK32 {
	BDSTATUS			Status;
	ULONG				Information;
} KIT_STATUS_BLOCK32, *PKIT_STATUS_BLOCK32;

typedef enum
{
	DO_OnceAction		= 0x00000001,
	DO_RebootAction		= 0x00000002,
	DO_RemoveAction		= 0x00000004,
	DO_DenyAction		= 0x00000008,
	DO_PassAction		= 0x00000010,
} Action;

// We Should put this at the top member
typedef struct _COMMON_HEAD_OUT
{
	KIT_STATUS_BLOCK32	IoStatus;
} COMMON_HEAD_OUT, *PCOMMON_HEAD_OUT;

typedef struct _FILE_COMMON_HEAD
{
	WCHAR				cFilePath[ USR_MAX_PATH ];
	UCHAR				FileMD5[SIZEOF_MD5];
} FILE_COMMON_HEAD;

typedef struct _FILE_CREATE_IN
{
	ULONG				AccessMask;
	ULONG				FileAttributes;
	ULONG				ShareAccess;
	ULONG				CreateDisposition;
	ULONG				CreateOptions;
	FILE_COMMON_HEAD	fileInfo;
} FILE_CREATE_IN;

typedef struct _FILE_CREATE_OUT
{
	COMMON_HEAD_OUT		commHead;
	HANDLE64			hFile;
} FILE_CREATE_OUT;

typedef struct _FILE_READ_IN
{
	HANDLE64			Handle;
	HANDLE64			Event;
	VOID*	__ptr64		ApcContext;
	ULONG				nNumberBytesOfToRead;
	LARGE_INTEGER		Offset;
	ULONG*	__ptr64		nNumberBytesOfRead;
	VOID*	__ptr64		Buffer;
} FILE_READ_IN;

typedef struct _FILE_READ_OUT
{
	COMMON_HEAD_OUT		commHead;
	ULONG*	__ptr64		nNumberBytesOfRead;
	VOID*	__ptr64		Buffer;
} FILE_READ_OUT;

typedef struct _FILE_WRITE_IN
{
	HANDLE64			Handle;
	HANDLE64			Event;
	VOID*	__ptr64		ApcContext;
	ULONG				nNumberBytesOfToWrite;
	LARGE_INTEGER		Offset;
	ULONG*	__ptr64		nNumberBytesOfWrite;
	VOID*	__ptr64		Buffer;
} FILE_WRITE_IN;

typedef struct _FILE_WRITE_OUT
{
	COMMON_HEAD_OUT		commHead;
	ULONG*	__ptr64		nNumberBytesOfWrite;
} FILE_WRITE_OUT;

typedef struct _FILE_CLOSE_IN
{
	HANDLE64			hFile;
} FILE_CLOSE_IN;

typedef struct _DELETE_FILE_IN
{
	ULONG				DoFileAction;
	FILE_COMMON_HEAD	fileInfo;
} DELETE_FILE_IN;

typedef struct _LOCK_FILE_IN
{
	FILE_COMMON_HEAD	fileInfo;
} LOCK_FILE_IN;

typedef struct _UNLOCK_FILE_IN
{
	FILE_COMMON_HEAD	fileInfo;
} UNLOCK_FILE_IN;

typedef struct _FILE_ACCESS_DENY_IN
{
	FILE_COMMON_HEAD	fileInfo;
} FILE_ACCESS_DENY_IN;

typedef struct _FIND_FIRST_FILE_IN
{
	WCHAR				szDirectory[ USR_MAX_PATH ];	
	WCHAR				szFilePattern[ 14 ];
} FIND_FIRST_FILE_IN;

typedef struct _FIND_FIRST_FILE_OUT
{
	COMMON_HEAD_OUT	commHead;
	HANDLE64		hDirectory;
	DWORD			dwFileAttributes;
	LARGE_INTEGER	ftCreationTime;
	LARGE_INTEGER	ftLastAccessTime;
	LARGE_INTEGER	ftLastWriteTime;
	DWORD			nFileSizeHigh;
	DWORD			nFileSizeLow;
	DWORD			dwReserved0;
	DWORD			dwReserved1;
	WCHAR			cFileName[ USR_MAX_PATH ];
	WCHAR			cAlternateFileName[ 14 ];
} FIND_FIRST_FILE_OUT;

typedef struct _FIND_NEXT_FILE_IN
{
	HANDLE64		hDirectory;
	WCHAR			szFilePattern[ 14 ];
} FIND_NEXT_FILE_IN;

typedef struct _FIND_NEXT_FILE_OUT
{
	COMMON_HEAD_OUT	commHead;
	HANDLE64		hDirectory;
	DWORD			dwFileAttributes;
	LARGE_INTEGER	ftCreationTime;
	LARGE_INTEGER	ftLastAccessTime;
	LARGE_INTEGER	ftLastWriteTime;
	DWORD			nFileSizeHigh;
	DWORD			nFileSizeLow;
	DWORD			dwReserved0;
	DWORD			dwReserved1;
	WCHAR			cFileName[ 260 ];
	WCHAR			cAlternateFileName[ 14 ];
} FIND_NEXT_FILE_OUT;

typedef struct _COPY_FILE_IN
{
	Action				DoFileAction;
	FILE_COMMON_HEAD	fileSrcInfo;
	FILE_COMMON_HEAD	fileDstInfo;
	VOID*	__ptr64		pProgressRoutine;
	VOID*	__ptr64		lpData;
	BOOL*	__ptr64		pbCancel;
	DWORD				dwCopyFlags;
} COPY_FILE_IN;

typedef struct _QUERY_OBJECT_IN
{
	HANDLE64			hObjectHandle;
	OBJECT_INFO_CLASS	ObjectInformationClass;
	ULONG				Length;
	VOID*	__ptr64		ObjectInformation;
	ULONG*	__ptr64		ResultLength;
} QUERY_OBJECT_IN;

typedef struct _OPEN_PROCESS_IN
{
	FILE_COMMON_HEAD	fileInfo;
	DWORD				dwDesiredAccess;
	BOOL				bInheritHandle;
	CLIENT_ID64			CliendId;
} OPEN_PROCESS_IN;

typedef struct _OPEN_PROCESS_OUT
{
	COMMON_HEAD_OUT		commHead;
	HANDLE64			hProcess;
} OPEN_PROCESS_OUT;

typedef struct _KILL_PROCESS_IN
{
	FILE_COMMON_HEAD	fileInfo;
	HANDLE64			hProcess;
	ULONG				ExitCode;
} KILL_PROCESS_IN;

typedef struct _KILL_PROCESSID_IN
{
	FILE_COMMON_HEAD	fileInfo;
	DWORD				dwProcessId;
	ULONG				ExitCode;
} KILL_PROCESSID_IN;

#pragma pack()