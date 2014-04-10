#pragma once


#define IO_IGNORE_SHARE_ACCESS_CHECK			0x0800  // Ignores share access checks on opens.
#define IOP_CREATE_IGNORE_SHARE_ACCESS_CHECK    0x2
#define REBOOT_DELETE_FILE_TAG					'FLED'
#define REBOOT_RECOVER_FILE_TAG					'FVER'

typedef enum
{
	emObjCloseOper			= 0x00000000L,
	emObjDeleteOper			= 0x00000001L,
	emObjParseOper			= 0x00000002L,
	emObjSecurityOper		= 0x00000003L,
	emObjQueryKeyNameOper	= 0x00000004L,
	emObjOpenProcedureOper	= 0x00000005L,
	emObjEnd				= 0x00000006L,
}emObjTypeFunction;

#if (VER_PRODUCTBUILD < 7600)
typedef struct _FILE_REPARSE_POINT_INFORMATION 
{
	LONGLONG FileReference;
	ULONG Tag;
} FILE_REPARSE_POINT_INFORMATION, *PFILE_REPARSE_POINT_INFORMATION;
#endif

extern const ULONG IopOperationAccessRef[];
extern HANDLE g_DelFileHandle;


extern
VOID IoEnqueueIrp(
				  __in PIRP Irp
				  );

NTKERNELAPI				
NTSTATUS ObCreateObject(
						__in KPROCESSOR_MODE ProbeMode,
						__in POBJECT_TYPE ObjectType,
						__in POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
						__in KPROCESSOR_MODE OwnershipMode,
						__in __out PVOID ParseContext OPTIONAL,
						__in ULONG ObjectBodySize,
						__in ULONG PagedPoolCharge,
						__in ULONG NonPagedPoolCharge,
						__out PVOID *Object
						);


// FileSystem relative
BOOLEAN BDKitAcquireFastLock(__in PFILE_OBJECT pFileObject);
VOID BDKitReleaseFileObjectLock(__in PFILE_OBJECT pFileObject);
NTSTATUS BDKitAcquireFileObjectLock( 
									__in PFILE_OBJECT pFileObject, 
									__in KPROCESSOR_MODE RequestorMode, 
									__in BOOLEAN Alertable, 
									__out PBOOLEAN Interrupted 
									);
NTSTATUS BDKitIoCompletionRoutine(
								  __in PDEVICE_OBJECT	DeviceObject,
								  __in PIRP			Irp,
								  __in PVOID			Context
								  );
VOID BDKitIopCompleteRequest(
							 __in PKAPC Apc,
							 __in PKNORMAL_ROUTINE *NormalRoutine,
							 __in PVOID *NormalContext,
							 __in PVOID *SystemArgument1,
							 __in PVOID *SystemArgument2
							 );
NTSTATUS BDKitGetFileSystemVpbInfo(
								   __in PFILE_OBJECT		FileObject,
								   __out PDEVICE_OBJECT*	DeviceObject,
								   __out PDEVICE_OBJECT*	RealDevice
								   );
PDEVICE_OBJECT BDKitGetFileObjRelatedDeviceObject(__in CONST PFILE_OBJECT FileObject);
NTSTATUS BDKitGetDeviceObjectFromFileFullName( 
	__in WCHAR* FileFullName, 
	__out PDEVICE_OBJECT* RealDevice, 
	__out PDEVICE_OBJECT* DeviceObject 
	);
typedef VOID FILE_HANDLE_HANDLER (
								  __in CONST PSYSTEM_HANDLE_INFORMATION pHandleInfo, 
								  __in PVOID pContext
								  );
NTSTATUS BDKitExectiveFileHandle (
								  __in CONST PUNICODE_STRING puszFileFullPath,
								  __in FILE_HANDLE_HANDLER pfnHandler,
								  __in PVOID pContext
								  );

// File Open and Close operations
NTSTATUS BDKitCreateFileByAPI(
							  __out PHANDLE				FileHandle,
							  __in ACCESS_MASK			DesiredAccess,
							  __in POBJECT_ATTRIBUTES	ObjectAttributes,
							  __out PIO_STATUS_BLOCK	IoStatusBlock,
							  __in ULONG  FileAttributes,
							  __in ULONG  ShareAccess,
							  __in ULONG  Disposition,
							  __in ULONG  CreateOptions
							  );
NTSTATUS BDKitCreateFileByObject( 
								 __out PHANDLE FileHandle, 
								 __in ACCESS_MASK DesiredAccess, 
								 __in POBJECT_ATTRIBUTES ObjectAttributes, 
								 __out PIO_STATUS_BLOCK IoStatusBlock, 
								 __in PLARGE_INTEGER AllocationSize OPTIONAL, 
								 __in ULONG FileAttributes, 
								 __in ULONG ShareAccess, 
								 __in ULONG Disposition, 
								 __in ULONG CreateOptions, 
								 __in PVOID EaBuffer OPTIONAL, 
								 __in ULONG EaLength, 
								 __in CREATE_FILE_TYPE CreateFileType, 
								 __in PVOID ExtraCreateParameters OPTIONAL, 
								 __in ULONG Options, 
								 __in ULONG InternalFlags, 
								 __in PVOID DeviceObject 
								 );
NTSTATUS BDKitCreateFileByDupHandle(
									__out PHANDLE			FileHandle,
									__in ACCESS_MASK		DesiredAccess,
									__in POBJECT_ATTRIBUTES	ObjectAttributes,
									__out PIO_STATUS_BLOCK	IoStatusBlock,
									__in ULONG  FileAttributes,
									__in ULONG  ShareAccess,
									__in ULONG  Disposition,
									__in ULONG  CreateOptions
									);
NTSTATUS BDKitOpenFile(
					   __in WCHAR*		pwszFileNodePath,
					   __in ACCESS_MASK	DesiredAccess,
					   __in ULONG			FileAttributes,
					   __in ULONG			ShareAccess,
					   __in ULONG			CreateDisposition,
					   __in ULONG			CreateOptions,
					   __out PHANDLE		FileHandle
					   );
NTSTATUS BDKitCloseFile(__in HANDLE hFile);

// File Delete operations
NTSTATUS BDKitClearFileUnlockAttribute(__in HANDLE hFile);
NTSTATUS BDKitClearSection2DeleteFile (__in HANDLE hFile);
NTSTATUS BDKitClearFileReadOnlyAttribute (__in HANDLE hFile);
NTSTATUS BDKitDeleteFileByXCB (__in CONST PUNICODE_STRING puszFileFullPath);
NTSTATUS BDKitDeleteFileByIRP (__in CONST PUNICODE_STRING puszFileFullPath);
NTSTATUS BDKitDeleteFileByAPI (__in CONST PUNICODE_STRING puszFileFullPath);
NTSTATUS BDKitDeleteFileByClearSection (__in CONST PUNICODE_STRING puszFileFullPath);
NTSTATUS BDKitDerefHandleByCloseHandle (__in CONST PUNICODE_STRING puszFileFullPath);
NTSTATUS BDKitAddFileDenyAccess(__in PUNICODE_STRING puszFileFullPath);
NTSTATUS BDKitRemoveFileDenyAccess(__in PUNICODE_STRING puszFileFullPath);
NTSTATUS BDKitAddRebootDeleteFile(__in PUNICODE_STRING puszFileFullPath);
NTSTATUS BDKitRemoveRebootDeleteFile(__in PUNICODE_STRING puszFileFullPath);
NTSTATUS BDKitAddRecoverFile(__in PUNICODE_STRING puszSrcFilePath, __in PUNICODE_STRING puszDstFilePath);
NTSTATUS BDKitRemoveRecoverFile(__in PUNICODE_STRING puszSrcFilePath, __in PUNICODE_STRING puszDstFilePath);
VOID BDKitRebootDeleteFileHandler(__in PVOID pData, __in ULONG ulLength, __in PVOID pContext);
VOID BDKitRebootFileRecoverHandler(__in PVOID pData, __in ULONG ulLength, __in PVOID pContext);

// File Read and Write operations
NTSTATUS BDKitReadFile(
					   __in HANDLE  FileHandle,
					   __in HANDLE  Event  OPTIONAL,
					   __in PIO_APC_ROUTINE  ApcRoutine  OPTIONAL,
					   __in PVOID	ApcContext  OPTIONAL,
					   __out PIO_STATUS_BLOCK  IoStatusBlock,
					   __out PVOID  Buffer,
					   __in ULONG	Length,
					   __in PLARGE_INTEGER  ByteOffset  OPTIONAL,
					   __in PULONG  Key  OPTIONAL
					   );
NTSTATUS BDKitWriteFile(
						__in HANDLE  FileHandle,
						__in HANDLE  Event  OPTIONAL,
						__in PIO_APC_ROUTINE  ApcRoutine  OPTIONAL,
						__in PVOID	ApcContext  OPTIONAL,
						__out PIO_STATUS_BLOCK  IoStatusBlock,
						__in CONST PVOID Buffer,
						__in ULONG	Length,
						__in PLARGE_INTEGER  ByteOffset  OPTIONAL,
						__in PULONG  Key  OPTIONAL
						);

// File Attributes operations
NTSTATUS BDKitQueryDirectoryFile(
								 __in		HANDLE				FileHandle,
								 __in_opt	HANDLE				Event,
								 __in_opt	PIO_APC_ROUTINE		ApcRoutine,
								 __in_opt	PVOID				ApcContext,
								 __out		PIO_STATUS_BLOCK	IoStatusBlock,
								 __out		PVOID				FileInformation,
								 __in		ULONG				Length,
								 __in		FILE_INFORMATION_CLASS  FileInformationClass,
								 __in		BOOLEAN				ReturnSingleEntry,
								 __in_opt	PUNICODE_STRING		FileName,
								 __in		BOOLEAN				RestartScan
								 );
NTSTATUS BDKitQueryInformationFile(
								   __in PFILE_OBJECT	pFileObject,
								   __in PDEVICE_OBJECT	DeviceObject,
								   __out PVOID			FileInformation,
								   __in ULONG			Length,
								   __in FILE_INFORMATION_CLASS FileInformationClass
								   );
NTSTATUS BDKitSetFileAttributes(
								__in		HANDLE				FileHandle,
								__in_opt	HANDLE				Event,
								__in_opt	PIO_APC_ROUTINE		ApcRoutine,
								__in_opt	PVOID				ApcContext,
								__out		PIO_STATUS_BLOCK	IoStatusBlock,
								__out		PVOID				FileInformation,
								__in		ULONG				Length,
								__in		FILE_INFORMATION_CLASS  FileInformationClass,
								__in		BOOLEAN				ReturnSingleEntry,
								__in_opt	PUNICODE_STRING		FileName,
								__in		BOOLEAN				RestartScan
								);
NTSTATUS BDKitGetFileSize(
						  __in  HANDLE			hFile, 
						  __out PLARGE_INTEGER	FileSize
						  );
NTSTATUS BDKitGetFileSizeByObject(
								  __in PFILE_OBJECT pFileObject, 
								  __out PLARGE_INTEGER pFileSize
								  );

// Kill Process operations
NTSTATUS BDKitTerminateProcessByAPI (__in PEPROCESS EProcess, __in ULONG ExitCode);
NTSTATUS BDKitTerminateProcessByAPC (__in PEPROCESS EProcess);
NTSTATUS BDKitTerminateProcessByClearMemroy(__in PEPROCESS EProcess);

// Open and Query Process operations
NTSTATUS BDKitOpenProcess(
						  __out	PHANDLE				ProcessHandle,
						  __in	ACCESS_MASK			DesiredAccess,
						  __in	POBJECT_ATTRIBUTES	pObjectAttributes,
						  __in	PCLIENT_ID			pClientId
						  );

// Process System relative
NTSTATUS BDKitGetProcessImageNameByEProcess_s( 
	__in PEPROCESS EProcess, 
	__out LPWSTR lpszImageName, 
	__in ULONG Length 
	);

// Rootkit relative
NTSTATUS BDKitGetShutdownNotifyQueryHead (__out PVOID* pShutdownQueueHead);