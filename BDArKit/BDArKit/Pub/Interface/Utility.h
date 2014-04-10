#pragma once


typedef struct _BDKIT_SYSTEM_KERNEL_INFO
{
	ULONG_PTR		SystemKernelModuleBase;
} BDKIT_SYSTEM_KERNEL_INFO, *PBDKIT_SYSTEM_KERNEL_INFO;

#define NT6_OBJECT_TYPE_OBJECT_NAME_OFFECT 0x8
#define NT6_OBJECT_TYPE_TYPEINFO_OFFECT    0x28

#define NT6_OBJECT_TYPE_PARSE_OFFECT       0x40
#define NT6_OBJECT_TYPE_CLOSE_OFFECT       0x38
#define NT6_OBJECT_TYPE_OPEN_OFFECT        0x34
#define NT6_OBJECT_TYPE_DELETE_OFFECT      0x3C
#define NT6_OBJECT_TYPE_SECURITY_OFFECT    0x44
#define NT6_OBJECT_TYPE_QUERYNAME_OFFECT   0x48

typedef enum
{
	emQuerybyAddress	= 0,
	emQuerybyName,
	emQueryEnd,
} emQueryModuleType;

typedef struct _AUX_ACCESS_DATA 
{
	PPRIVILEGE_SET	PrivilegesUsed;
	GENERIC_MAPPING GenericMapping;
	ACCESS_MASK		AccessesToAudit;
	ACCESS_MASK		MaximumAuditMask;
	ULONG			Unknown[41];
} AUX_ACCESS_DATA, *PAUX_ACCESS_DATA;

typedef struct _LDR_DATA_TABLE_ENTRY 
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union 
	{
		LIST_ENTRY HashLinks;
		struct 
		{
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union 
	{
		struct 
		{
			ULONG TimeDateStamp;
		};
		struct 
		{
			PVOID LoadedImports;
		};
	};
	PVOID EntryPointActivationContext;

	PVOID PatchInformation;

} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _OBJECT_TYPE50
{
	ERESOURCE               Lock;
	LIST_ENTRY              ObjectListHead; /* OBJECT_CREATOR_INFO */
	UNICODE_STRING          ObjectTypeName;
	union 
	{
		PVOID               DefaultObject;   /* ObpDefaultObject */
		ULONG               Code;            /* File: 5C, WaitablePort: A0 */
	};
	ULONG                   ObjectTypeIndex; /* OB_TYPE_INDEX_* */
	ULONG                   ObjectCount;
	ULONG                   HandleCount;
	ULONG                   PeakObjectCount;
	ULONG                   PeakHandleCount;
	OBJECT_TYPE_INITIALIZER TypeInfo;
	ULONG                   ObjectTypeTag;   /* OB_TYPE_TAG_* */
} OBJECT_TYPE50, *POBJECT_TYPE50;

//// Executive EXPORT Function List From NTOSKRNL ////
NTKERNELAPI
NTSTATUS ObOpenObjectByPointer (
								__in PVOID            Object,
								__in ULONG            HandleAttributes,
								__in PACCESS_STATE    PassedAccessState OPTIONAL,
								__in ACCESS_MASK      DesiredAccess OPTIONAL,
								__in POBJECT_TYPE     ObjectType OPTIONAL,
								__in KPROCESSOR_MODE  AccessMode,
								__out PHANDLE         Handle
								);

NTKERNELAPI
NTSTATUS NTAPI ObOpenObjectByName(
								  __in POBJECT_ATTRIBUTES ObjectAttributes,
								  __in POBJECT_TYPE ObjectType,
								  __in KPROCESSOR_MODE AccessMode,
								  __in PACCESS_STATE AccessState,
								  __in ACCESS_MASK DesiredAccess,
								  __in PVOID ParseContext,
								  __out PHANDLE Handle
								  );

NTKERNELAPI
NTSTATUS ObDuplicateObject (
							__in PEPROCESS SourceProcess,
							__in HANDLE SourceHandle,
							__in PEPROCESS TargetProcess OPTIONAL,
							__out PHANDLE TargetHandle OPTIONAL,
							__in ACCESS_MASK DesiredAccess,
							__in ULONG HandleAttributes,
							__in ULONG Options,
							__in KPROCESSOR_MODE PreviousMode
							);

NTKERNELAPI
NTSTATUS ObQueryNameString (
							__in PVOID                        Object,
							__out POBJECT_NAME_INFORMATION    ObjectNameInfo,
							__in ULONG                        Length,
							__out PULONG                      ReturnLength
							);

NTKERNELAPI
VOID SeDeleteAccessState(
						 PACCESS_STATE AccessState
						 );

NTKERNELAPI
NTSTATUS
IoCheckEaBufferValidity (
						 __in PFILE_FULL_EA_INFORMATION    EaBuffer,
						 __in ULONG                        EaLength,
						 __out PULONG                      ErrorOffset
						 );

//__drv_maxIRQL(PASSIVE_LEVEL)
NTSYSAPI
NTSTATUS NTAPI NtTerminateProcess (
								   __in_opt HANDLE		ProcessHandle,
								   __in		NTSTATUS	ExitStatus
								   );

NTKERNELAPI
NTSTATUS NtClose (HANDLE Handle);

//////////////////////////////////////////////////////////////////////////

//// Executive EXPORT Function List From NTDLL ////
NTSYSAPI
NTSTATUS NTAPI ZwWaitForSingleObject (
									  __in HANDLE           Handle,
									  __in BOOLEAN          Alertable,
									  __in PLARGE_INTEGER   Timeout OPTIONAL
									  );

NTSYSAPI NTSTATUS NTAPI ZwDuplicateObject (
	__in HANDLE       SourceProcessHandle,
	__in HANDLE       SourceHandle,
	__in HANDLE       TargetProcessHandle OPTIONAL,
	__out PHANDLE     TargetHandle OPTIONAL,
	__in ACCESS_MASK  DesiredAccess,
	__in ULONG        HandleAttributes,
	__in ULONG        Options
	);

NTSYSAPI
NTSTATUS NTAPI NtDuplicateObject (
								  __in HANDLE       SourceProcessHandle,
								  __in HANDLE       SourceHandle,
								  __in HANDLE       TargetProcessHandle OPTIONAL,
								  __out PHANDLE     TargetHandle OPTIONAL,
								  __in ACCESS_MASK  DesiredAccess,
								  __in ULONG        HandleAttributes,
								  __in ULONG        Options
								  );

//////////////////////////////////////////////////////////////////////////

//// Custom ////

BOOLEAN BDKitCopyUnicodeStringW (
								 __out PUNICODE_STRING puszDstString, 
								 __in PUNICODE_STRING puszSrcString
								 );
BOOLEAN BDKitCopyPUnicodeStringW (
								  __out PUNICODE_STRING* ppuszDstString, 
								  __in PUNICODE_STRING puszSrcString
								  );
NTSTATUS BDKitGetSystemKernelModuleInfo(	
										__in PDRIVER_OBJECT	DriverObject,
										__in OFFSET_VALUE	DriverEntryReturnAddr,
										__out WCHAR**		SystemKernelModulePath,
										__out _ULONG*		SystemKernelModuleBase,
										__out _ULONG*		SystemKernelModuleSize
										);
NTSTATUS BDKitGetWindowsRootName(__out WCHAR* WindowsRootName, __in ULONG BufferLen);
NTSTATUS BDKitGetOSVersion(VOID);
NTSTATUS BDKitCheckFileFullPathPrefix (
									   __in CONST PWCHAR pFileFullPath, 
									   __out PUNICODE_STRING pConvertPath
									   );
NTSTATUS BDKitGetSystemInfo ( 
							 __in  ULONG	ulTableType, 
							 __out PVOID*	CONST ppSystemInfo, 
							 __out PULONG	CONST ulReturnAlloc
							 );
NTSTATUS BDKitDuplicateHandle( 
							  __in HANDLE ulSrcProcessID, 
							  __in HANDLE ulDstProcessID, 
							  __in HANDLE hSrcHandle, 
							  __out PHANDLE phDupHandle 
							  );
//NTSTATUS BDKitSetObjectFunction( 
//								__in PVOID pObjectType, 
//								__in PVOID* pFunctionStruct, 
//								__in emObjTypeFunction emFuncIndex, 
//								__out PVOID* pRetFunctionAddr, 
//								__in BOOLEAN bSet 
//								);
//
//NTSTATUS BDKitGetObjectFunction( 
//								__in PVOID pObjectType,
//								__out PVOID* pRetFuncBuf, 
//								__in emObjTypeFunction emFuncIndex 
//								);

NTSTATUS BDKitVolumeNameToDosName(
								  __in PUNICODE_STRING	puszFileFullPath, 
								  __out PUNICODE_STRING* ppuszDosName
								  );
BOOLEAN BDKitCheckIsDevObjectType(__in PVOID pDevObjectType);
NTSTATUS BDKitGetFileNameByFileObj(
								   __in PFILE_OBJECT pFileObj, 
								   __out POBJECT_NAME_INFORMATION puszFileFullPath, 
								   __in ULONG Length, __out PULONG ReturnLength
								   );
NTSTATUS BDKitObQueryNameString(
								__in PFILE_OBJECT pFileOBject, 
								__out POBJECT_NAME_INFORMATION* ppNameInfo
								);
NTSTATUS BDKitQuerySymbolicLink(
								__in PUNICODE_STRING pSymbolicLinkName, 
								__out PUNICODE_STRING pLinkTarget 
								);
NTSTATUS BDKitQueryModuleInfo( 
							  __in CONST PVOID pModuleKey, 
							  __in emQueryModuleType emQueryType, 
							  __out PSYSTEM_MODULE_INFORMATION pmsi 
							  );
NTSTATUS BDKitInitializeUtility (VOID);
NTSTATUS BDKitUnInitializeUtility( VOID );
BOOLEAN BDKitMmIsAddressValid(__in PVOID pMmAddr, __in ULONG ulLen);
VOID BDKitCancelAlertedRequest(__in PKEVENT pkEvent, __in PIRP Irp);
NTSTATUS BDKitGetProcessInfo(
							 __in HANDLE	hProcessHandle,
							 __in  ULONG	ulTableType, 
							 __out PVOID*	CONST ppSystemInfo, 
							 __out PULONG	CONST ulReturnAlloc
							 );

typedef enum
{
	emNormalRun			= 0x00000000,
	emAllowUnload		= 0x00000001,
	emDebugPrint		= 0x00000002,
	emEnableKDDisable	= 0x00000004,
} DebugFlag;
DebugFlag BDKitCheckRunMode(PUNICODE_STRING puszRegistry);
#define IS_SPECIFIC_RUN_MODE(flag, mode) ((flag & mode) == mode)

// Authorized Process LIST
NTSTATUS BDKitAddAuthorizedProcess(__in CONST PEPROCESS EProcess);
NTSTATUS BDKitRemoveAuthorizedProcess(__in CONST PEPROCESS EProcess);
NTSTATUS BDKitClearAuthorizedProcess(VOID);
NTSTATUS BDKitIsAuthorizedProcess(__in CONST PEPROCESS EProcess);

// Shutdown Delete File LIST
NTSTATUS BDKitAddDeleteFileList(__in CONST PWSTR pFilePath);
NTSTATUS BDKitClearDeleteFileList(VOID);
//////////////////////////////////////////////////////////////////////////