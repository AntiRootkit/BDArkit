#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include <ntddk.h>
#include <ntverp.h>

	typedef struct _SERVICE_DESCRIPTOR_TABLE    *PSERVICE_DESCRIPTOR_TABLE;
	typedef struct _SE_EXPORTS                  *PSE_EXPORTS;
#if (VER_PRODUCTBUILD < 7600)
	extern PUCHAR                       *FsRtlLegalAnsiCharacterArray;
#endif
	extern POBJECT_TYPE                 *IoDriverObjectType;
	extern PSERVICE_DESCRIPTOR_TABLE    KeServiceDescriptorTable;
	extern PSHORT                       NtBuildNumber;
#if (VER_PRODUCTBUILD < 7600)
	extern PSE_EXPORTS                  SeExports;
#endif
	extern PACL                         SePublicDefaultDacl;
	extern PACL                         SeSystemDefaultDacl;

#define ACCESS_ALLOWED_ACE_TYPE         (0x0)
#define ACCESS_DENIED_ACE_TYPE          (0x1)
#define SYSTEM_AUDIT_ACE_TYPE           (0x2)
#define SYSTEM_ALARM_ACE_TYPE           (0x3)

#define ANSI_DOS_STAR                   ('<')
#define ANSI_DOS_QM                     ('>')
#define ANSI_DOS_DOT                    ('"')

#define DOS_STAR                        (L'<')
#define DOS_QM                          (L'>')
#define DOS_DOT                         (L'"')

#define COMPRESSION_FORMAT_NONE         (0x0000)
#define COMPRESSION_FORMAT_DEFAULT      (0x0001)
#define COMPRESSION_FORMAT_LZNT1        (0x0002)
#define COMPRESSION_ENGINE_STANDARD     (0x0000)
#define COMPRESSION_ENGINE_MAXIMUM      (0x0100)
#define COMPRESSION_ENGINE_HIBER        (0x0200)

#define FILE_ACTION_ADDED                   0x00000001
#define FILE_ACTION_REMOVED                 0x00000002
#define FILE_ACTION_MODIFIED                0x00000003
#define FILE_ACTION_RENAMED_OLD_NAME        0x00000004
#define FILE_ACTION_RENAMED_NEW_NAME        0x00000005
#define FILE_ACTION_ADDED_STREAM            0x00000006
#define FILE_ACTION_REMOVED_STREAM          0x00000007
#define FILE_ACTION_MODIFIED_STREAM         0x00000008
#define FILE_ACTION_REMOVED_BY_DELETE       0x00000009
#define FILE_ACTION_ID_NOT_TUNNELLED        0x0000000A
#define FILE_ACTION_TUNNELLED_ID_COLLISION  0x0000000B

#define FILE_EA_TYPE_BINARY             0xfffe
#define FILE_EA_TYPE_ASCII              0xfffd
#define FILE_EA_TYPE_BITMAP             0xfffb
#define FILE_EA_TYPE_METAFILE           0xfffa
#define FILE_EA_TYPE_ICON               0xfff9
#define FILE_EA_TYPE_EA                 0xffee
#define FILE_EA_TYPE_MVMT               0xffdf
#define FILE_EA_TYPE_MVST               0xffde
#define FILE_EA_TYPE_ASN1               0xffdd
#define FILE_EA_TYPE_FAMILY_IDS         0xff01

#define FILE_NEED_EA                    0x00000080

#define FILE_NOTIFY_CHANGE_FILE_NAME    0x00000001
#define FILE_NOTIFY_CHANGE_DIR_NAME     0x00000002
#define FILE_NOTIFY_CHANGE_NAME         0x00000003
#define FILE_NOTIFY_CHANGE_ATTRIBUTES   0x00000004
#define FILE_NOTIFY_CHANGE_SIZE         0x00000008
#define FILE_NOTIFY_CHANGE_LAST_WRITE   0x00000010
#define FILE_NOTIFY_CHANGE_LAST_ACCESS  0x00000020
#define FILE_NOTIFY_CHANGE_CREATION     0x00000040
#define FILE_NOTIFY_CHANGE_EA           0x00000080
#define FILE_NOTIFY_CHANGE_SECURITY     0x00000100
#define FILE_NOTIFY_CHANGE_STREAM_NAME  0x00000200
#define FILE_NOTIFY_CHANGE_STREAM_SIZE  0x00000400
#define FILE_NOTIFY_CHANGE_STREAM_WRITE 0x00000800
#define FILE_NOTIFY_VALID_MASK          0x00000fff

#define FILE_OPLOCK_BROKEN_TO_LEVEL_2   0x00000007
#define FILE_OPLOCK_BROKEN_TO_NONE      0x00000008

#define FILE_OPBATCH_BREAK_UNDERWAY     0x00000009

#define FILE_CASE_SENSITIVE_SEARCH      0x00000001
#define FILE_CASE_PRESERVED_NAMES       0x00000002
#define FILE_UNICODE_ON_DISK            0x00000004
#define FILE_PERSISTENT_ACLS            0x00000008
#define FILE_FILE_COMPRESSION           0x00000010
#define FILE_VOLUME_QUOTAS              0x00000020
#define FILE_SUPPORTS_SPARSE_FILES      0x00000040
#define FILE_SUPPORTS_REPARSE_POINTS    0x00000080
#define FILE_SUPPORTS_REMOTE_STORAGE    0x00000100
#define FS_LFN_APIS                     0x00004000
#define FILE_VOLUME_IS_COMPRESSED       0x00008000
#define FILE_SUPPORTS_OBJECT_IDS        0x00010000
#define FILE_SUPPORTS_ENCRYPTION        0x00020000
#define FILE_NAMED_STREAMS              0x00040000
#define FILE_READ_ONLY_VOLUME           0x00080000

#define FILE_PIPE_BYTE_STREAM_TYPE      0x00000000
#define FILE_PIPE_MESSAGE_TYPE          0x00000001

#define FILE_PIPE_BYTE_STREAM_MODE      0x00000000
#define FILE_PIPE_MESSAGE_MODE          0x00000001

#define FILE_PIPE_QUEUE_OPERATION       0x00000000
#define FILE_PIPE_COMPLETE_OPERATION    0x00000001

#define FILE_PIPE_INBOUND               0x00000000
#define FILE_PIPE_OUTBOUND              0x00000001
#define FILE_PIPE_FULL_DUPLEX           0x00000002

#define FILE_PIPE_DISCONNECTED_STATE    0x00000001
#define FILE_PIPE_LISTENING_STATE       0x00000002
#define FILE_PIPE_CONNECTED_STATE       0x00000003
#define FILE_PIPE_CLOSING_STATE         0x00000004

#define FILE_PIPE_CLIENT_END            0x00000000
#define FILE_PIPE_SERVER_END            0x00000001

#define FILE_PIPE_READ_DATA             0x00000000
#define FILE_PIPE_WRITE_SPACE           0x00000001

#define FILE_STORAGE_TYPE_SPECIFIED             0x00000041  // FILE_DIRECTORY_FILE | FILE_NON_DIRECTORY_FILE
#define FILE_STORAGE_TYPE_DEFAULT               (StorageTypeDefault << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_DIRECTORY             (StorageTypeDirectory << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_FILE                  (StorageTypeFile << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_DOCFILE               (StorageTypeDocfile << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_JUNCTION_POINT        (StorageTypeJunctionPoint << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_CATALOG               (StorageTypeCatalog << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_STRUCTURED_STORAGE    (StorageTypeStructuredStorage << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_EMBEDDING             (StorageTypeEmbedding << FILE_STORAGE_TYPE_SHIFT)
#define FILE_STORAGE_TYPE_STREAM                (StorageTypeStream << FILE_STORAGE_TYPE_SHIFT)
#define FILE_MINIMUM_STORAGE_TYPE               FILE_STORAGE_TYPE_DEFAULT
#define FILE_MAXIMUM_STORAGE_TYPE               FILE_STORAGE_TYPE_STREAM
#define FILE_STORAGE_TYPE_MASK                  0x000f0000
#define FILE_STORAGE_TYPE_SHIFT                 16

#define FILE_VC_QUOTA_NONE              0x00000000
#define FILE_VC_QUOTA_TRACK             0x00000001
#define FILE_VC_QUOTA_ENFORCE           0x00000002
#define FILE_VC_QUOTA_MASK              0x00000003

#define FILE_VC_QUOTAS_LOG_VIOLATIONS   0x00000004
#define FILE_VC_CONTENT_INDEX_DISABLED  0x00000008

#define FILE_VC_LOG_QUOTA_THRESHOLD     0x00000010
#define FILE_VC_LOG_QUOTA_LIMIT         0x00000020
#define FILE_VC_LOG_VOLUME_THRESHOLD    0x00000040
#define FILE_VC_LOG_VOLUME_LIMIT        0x00000080

#define FILE_VC_QUOTAS_INCOMPLETE       0x00000100
#define FILE_VC_QUOTAS_REBUILDING       0x00000200

#define FILE_VC_VALID_MASK              0x000003ff

#define FSRTL_FCB_HEADER_V0             (0x00)
#define FSRTL_FCB_HEADER_V1             (0x01)

#define FSRTL_FLAG_FILE_MODIFIED        (0x01)
#define FSRTL_FLAG_FILE_LENGTH_CHANGED  (0x02)
#define FSRTL_FLAG_LIMIT_MODIFIED_PAGES (0x04)
#define FSRTL_FLAG_ACQUIRE_MAIN_RSRC_EX (0x08)
#define FSRTL_FLAG_ACQUIRE_MAIN_RSRC_SH (0x10)
#define FSRTL_FLAG_USER_MAPPED_FILE     (0x20)
#define FSRTL_FLAG_ADVANCED_HEADER      (0x40)
#define FSRTL_FLAG_EOF_ADVANCE_ACTIVE   (0x80)

#define FSRTL_FLAG2_DO_MODIFIED_WRITE           (0x01)
#define FSRTL_FLAG2_SUPPORTS_FILTER_CONTEXTS    (0x02)
#define FSRTL_FLAG2_PURGE_WHEN_MAPPED           (0x04)
#define FSRTL_FLAG2_IS_PAGING_FILE              (0x08)

#if (VER_PRODUCTBUILD<7600)
#define FSRTL_FSP_TOP_LEVEL_IRP         (0x01)
#define FSRTL_CACHE_TOP_LEVEL_IRP       (0x02)
#define FSRTL_MOD_WRITE_TOP_LEVEL_IRP   (0x03)
#define FSRTL_FAST_IO_TOP_LEVEL_IRP     (0x04)
#define FSRTL_MAX_TOP_LEVEL_IRP_FLAG    (0x04)
#endif

#define FSRTL_VOLUME_DISMOUNT           1
#define FSRTL_VOLUME_DISMOUNT_FAILED    2
#define FSRTL_VOLUME_LOCK               3
#define FSRTL_VOLUME_LOCK_FAILED        4
#define FSRTL_VOLUME_UNLOCK             5
#define FSRTL_VOLUME_MOUNT              6

#define FSRTL_WILD_CHARACTER            0x08

#ifdef _X86_
#define HARDWARE_PTE    HARDWARE_PTE_X86
#define PHARDWARE_PTE   PHARDWARE_PTE_X86
#else
#define HARDWARE_PTE    ULONG
#define PHARDWARE_PTE   PULONG
#endif

#define IO_CHECK_CREATE_PARAMETERS      0x0200
#define IO_ATTACH_DEVICE                0x0400

#define IO_ATTACH_DEVICE_API            0x80000000

#define IO_COMPLETION_QUERY_STATE       0x0001
#define IO_COMPLETION_MODIFY_STATE      0x0002
#define IO_COMPLETION_ALL_ACCESS        (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|0x3)

#define IO_FILE_OBJECT_NON_PAGED_POOL_CHARGE    64
#define IO_FILE_OBJECT_PAGED_POOL_CHARGE        1024

#define IO_REPARSE_TAG_RESERVED_ZERO    (0)
#define IO_REPARSE_TAG_RESERVED_ONE     (1)

#define IO_TYPE_APC                     18
#define IO_TYPE_DPC                     19
#define IO_TYPE_DEVICE_QUEUE            20
#define IO_TYPE_EVENT_PAIR              21
#define IO_TYPE_INTERRUPT               22
#define IO_TYPE_PROFILE                 23

#define IRP_BEING_VERIFIED              0x10

#define MAILSLOT_CLASS_FIRSTCLASS       1
#define MAILSLOT_CLASS_SECONDCLASS      2

#define MAILSLOT_SIZE_AUTO              0

#define MAP_PROCESS                     1L
#define MAP_SYSTEM                      2L

#define MEM_DOS_LIM                     0x40000000
#define MEM_IMAGE                       SEC_IMAGE

#define OB_FLAG_CREATE_INFO     0x01 /* Object header has OBJECT_CREATE_INFO */
#define OB_FLAG_KERNEL_MODE     0x02 /* Created by kernel */
#define OB_FLAG_CREATOR_INFO    0x04 /* Object header has OBJECT_CREATOR_INFO */
#define OB_FLAG_EXCLUSIVE       0x08 /* OBJ_EXCLUSIVE */
#define OB_FLAG_PERMAMENT       0x10 /* OBJ_PERMAMENT */
#define OB_FLAG_SECURITY        0x20 /* Object header has SecurityDescriptor != NULL */
#define OB_FLAG_SINGLE_PROCESS  0x40 /* absent HandleDBList */

#define OB_SECURITY_CHARGE      0x00000800

#define OB_TYPE_TYPE                    1
#define OB_TYPE_DIRECTORY               2
#define OB_TYPE_SYMBOLIC_LINK           3
#define OB_TYPE_TOKEN                   4
#define OB_TYPE_PROCESS                 5
#define OB_TYPE_THREAD                  6
#define OB_TYPE_EVENT                   7
#define OB_TYPE_EVENT_PAIR              8
#define OB_TYPE_MUTANT                  9
#define OB_TYPE_SEMAPHORE               10
#define OB_TYPE_TIMER                   11
#define OB_TYPE_PROFILE                 12
#define OB_TYPE_WINDOW_STATION          13
#define OB_TYPE_DESKTOP                 14
#define OB_TYPE_SECTION                 15
#define OB_TYPE_KEY                     16
#define OB_TYPE_PORT                    17
#define OB_TYPE_ADAPTER                 18
#define OB_TYPE_CONTROLLER              19
#define OB_TYPE_DEVICE                  20
#define OB_TYPE_DRIVER                  21
#define OB_TYPE_IO_COMPLETION           22
#define OB_TYPE_FILE                    23

#define PIN_WAIT                        (1)
#define PIN_EXCLUSIVE                   (2)
#define PIN_NO_READ                     (4)
#define PIN_IF_BCB                      (8)

#define MAP_WAIT                        (1)
#define MAP_NO_READ                     (16)

#define PORT_CONNECT                    0x0001
#define PORT_ALL_ACCESS                 (STANDARD_RIGHTS_ALL |\
	PORT_CONNECT)

#define SEC_BASED                       0x00200000
#define SEC_NO_CHANGE                   0x00400000
#define SEC_FILE                        0x00800000
#define SEC_IMAGE                       0x01000000
#if (VER_PRODUCTBUILD < 7600)
#define SEC_COMMIT                      0x08000000
#endif
#define SEC_NOCACHE                     0x10000000

#define SECURITY_WORLD_SID_AUTHORITY    {0,0,0,0,0,1}
#define SECURITY_WORLD_RID              (0x00000000L)

#if (VER_PRODUCTBUILD<7600)
#define SID_REVISION                    1
#endif

#define THREAD_STATE_INITIALIZED        0
#define THREAD_STATE_READY              1
#define THREAD_STATE_RUNNING            2
#define THREAD_STATE_STANDBY            3
#define THREAD_STATE_TERMINATED         4
#define THREAD_STATE_WAIT               5
#define THREAD_STATE_TRANSITION         6
#define THREAD_STATE_UNKNOWN            7

#define TOKEN_ASSIGN_PRIMARY            (0x0001)
#define TOKEN_DUPLICATE                 (0x0002)
#define TOKEN_IMPERSONATE               (0x0004)
#define TOKEN_QUERY                     (0x0008)
#define TOKEN_QUERY_SOURCE              (0x0010)
#define TOKEN_ADJUST_PRIVILEGES         (0x0020)
#define TOKEN_ADJUST_GROUPS             (0x0040)
#define TOKEN_ADJUST_DEFAULT            (0x0080)

#if (VER_PRODUCTBUILD<7600)
#define TOKEN_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED |\
	TOKEN_ASSIGN_PRIMARY     |\
	TOKEN_DUPLICATE          |\
	TOKEN_IMPERSONATE        |\
	TOKEN_QUERY              |\
	TOKEN_QUERY_SOURCE       |\
	TOKEN_ADJUST_PRIVILEGES  |\
	TOKEN_ADJUST_GROUPS      |\
	TOKEN_ADJUST_DEFAULT)
#endif

#define TOKEN_READ       (STANDARD_RIGHTS_READ     |\
	TOKEN_QUERY)

#define TOKEN_WRITE      (STANDARD_RIGHTS_WRITE    |\
	TOKEN_ADJUST_PRIVILEGES  |\
	TOKEN_ADJUST_GROUPS      |\
	TOKEN_ADJUST_DEFAULT)

#define TOKEN_EXECUTE    (STANDARD_RIGHTS_EXECUTE)

#if (VER_PRODUCTBUILD<7600)
#define TOKEN_SOURCE_LENGTH 8

#define TOKEN_HAS_TRAVERSE_PRIVILEGE    0x01
#define TOKEN_HAS_BACKUP_PRIVILEGE      0x02
#define TOKEN_HAS_RESTORE_PRIVILEGE     0x04
#define TOKEN_HAS_ADMIN_GROUP           0x08
#define TOKEN_IS_RESTRICTED             0x10
#define TOKEN_SESSION_NOT_REFERENCED    0x20
#define TOKEN_SANDBOX_INERT             0x40
#define TOKEN_HAS_IMPERSONATE_PRIVILEGE 0x80
#endif

#define VACB_MAPPING_GRANULARITY        (0x40000)
#define VACB_OFFSET_SHIFT               (18)

#define FSCTL_REQUEST_OPLOCK_LEVEL_1    CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  0, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_REQUEST_OPLOCK_LEVEL_2    CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_REQUEST_BATCH_OPLOCK      CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_OPLOCK_BREAK_ACKNOWLEDGE  CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  3, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_OPBATCH_ACK_CLOSE_PENDING CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  4, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_OPLOCK_BREAK_NOTIFY       CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  5, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_LOCK_VOLUME               CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_UNLOCK_VOLUME             CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  7, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_DISMOUNT_VOLUME           CTL_CODE(FILE_DEVICE_FILE_SYSTEM,  8, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define FSCTL_IS_VOLUME_MOUNTED         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_IS_PATHNAME_VALID         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 11, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_MARK_VOLUME_DIRTY         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 12, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define FSCTL_QUERY_RETRIEVAL_POINTERS  CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 14,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_GET_COMPRESSION           CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 15, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_SET_COMPRESSION           CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 16, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)

#if (VER_PRODUCTBUILD<7600)
#define FSCTL_MARK_AS_SYSTEM_HIVE       CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 19,  METHOD_NEITHER, FILE_ANY_ACCESS)
#endif
#define FSCTL_OPLOCK_BREAK_ACK_NO_2     CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 20, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_INVALIDATE_VOLUMES        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_QUERY_FAT_BPB             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 22, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_REQUEST_FILTER_OPLOCK     CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 23, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_FILESYSTEM_GET_STATISTICS CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 24, METHOD_BUFFERED, FILE_ANY_ACCESS)

#if (VER_PRODUCTBUILD >= 1381 && VER_PRODUCTBUILD < 7600)

#define FSCTL_GET_NTFS_VOLUME_DATA      CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 25, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_GET_NTFS_FILE_RECORD      CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 26, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_GET_VOLUME_BITMAP         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 27,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_GET_RETRIEVAL_POINTERS    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 28,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_MOVE_FILE                 CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 29, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_IS_VOLUME_DIRTY           CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 30, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_GET_HFS_INFORMATION       CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 31, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_ALLOW_EXTENDED_DASD_IO    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 32, METHOD_NEITHER,  FILE_ANY_ACCESS)

#endif // (VER_PRODUCTBUILD >= 1381)

#if (VER_PRODUCTBUILD >= 2195 && VER_PRODUCTBUILD < 7600)

#define FSCTL_READ_PROPERTY_DATA        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 33, METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_WRITE_PROPERTY_DATA       CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 34, METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_FIND_FILES_BY_SID         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 35, METHOD_NEITHER, FILE_ANY_ACCESS)

#define FSCTL_DUMP_PROPERTY_DATA        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 37,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_SET_OBJECT_ID             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 38, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_GET_OBJECT_ID             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 39, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_DELETE_OBJECT_ID          CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 40, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_SET_REPARSE_POINT         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 41, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_GET_REPARSE_POINT         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 42, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_DELETE_REPARSE_POINT      CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 43, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_ENUM_USN_DATA             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 44,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_SECURITY_ID_CHECK         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 45,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_READ_USN_JOURNAL          CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 46,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_SET_OBJECT_ID_EXTENDED    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 47, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_CREATE_OR_GET_OBJECT_ID   CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 48, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_SET_SPARSE                CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 49, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define FSCTL_SET_ZERO_DATA             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 50, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_QUERY_ALLOCATED_RANGES    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 51,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_ENABLE_UPGRADE            CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 52, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_SET_ENCRYPTION            CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 53, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_ENCRYPTION_FSCTL_IO       CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 54,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_WRITE_RAW_ENCRYPTED       CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 55,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_READ_RAW_ENCRYPTED        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 56,  METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_CREATE_USN_JOURNAL        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 57,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_READ_FILE_USN_DATA        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 58,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_WRITE_USN_CLOSE_RECORD    CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 59,  METHOD_NEITHER, FILE_READ_DATA)
#define FSCTL_EXTEND_VOLUME             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 60, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_QUERY_USN_JOURNAL         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 61, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_DELETE_USN_JOURNAL        CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 62, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_MARK_HANDLE               CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 63, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_SIS_COPYFILE              CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 64, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_SIS_LINK_FILES            CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 65, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_HSM_MSG                   CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 66, METHOD_BUFFERED, FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_NSS_CONTROL               CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 67, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_HSM_DATA                  CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 68, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_RECALL_FILE               CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 69, METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_NSS_RCONTROL              CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 70, METHOD_BUFFERED, FILE_READ_DATA)
#define FSCTL_READ_FROM_PLEX            CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 71, METHOD_OUT_DIRECT, FILE_READ_DATA)
#define FSCTL_FILE_PREFETCH             CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 72, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

#endif // (VER_PRODUCTBUILD >= 2195)

#define FSCTL_MAILSLOT_PEEK             CTL_CODE(FILE_DEVICE_MAILSLOT, 0, METHOD_NEITHER, FILE_READ_DATA)

#define FSCTL_NETWORK_SET_CONFIGURATION_INFO    CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 102, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_GET_CONFIGURATION_INFO    CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 103, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_GET_CONNECTION_INFO       CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 104, METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_ENUMERATE_CONNECTIONS     CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 105, METHOD_NEITHER, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_DELETE_CONNECTION         CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 107, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_GET_STATISTICS            CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 116, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_SET_DOMAIN_NAME           CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 120, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_NETWORK_REMOTE_BOOT_INIT_SCRT     CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 250, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define FSCTL_PIPE_ASSIGN_EVENT         CTL_CODE(FILE_DEVICE_NAMED_PIPE, 0, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_DISCONNECT           CTL_CODE(FILE_DEVICE_NAMED_PIPE, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_LISTEN               CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_PEEK                 CTL_CODE(FILE_DEVICE_NAMED_PIPE, 3, METHOD_BUFFERED, FILE_READ_DATA)
#define FSCTL_PIPE_QUERY_EVENT          CTL_CODE(FILE_DEVICE_NAMED_PIPE, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_TRANSCEIVE           CTL_CODE(FILE_DEVICE_NAMED_PIPE, 5, METHOD_NEITHER,  FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_PIPE_WAIT                 CTL_CODE(FILE_DEVICE_NAMED_PIPE, 6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_IMPERSONATE          CTL_CODE(FILE_DEVICE_NAMED_PIPE, 7, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_SET_CLIENT_PROCESS   CTL_CODE(FILE_DEVICE_NAMED_PIPE, 8, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_QUERY_CLIENT_PROCESS CTL_CODE(FILE_DEVICE_NAMED_PIPE, 9, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_PIPE_INTERNAL_READ        CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2045, METHOD_BUFFERED, FILE_READ_DATA)
#define FSCTL_PIPE_INTERNAL_WRITE       CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2046, METHOD_BUFFERED, FILE_WRITE_DATA)
#define FSCTL_PIPE_INTERNAL_TRANSCEIVE  CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2047, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define FSCTL_PIPE_INTERNAL_READ_OVFLOW CTL_CODE(FILE_DEVICE_NAMED_PIPE, 2048, METHOD_BUFFERED, FILE_READ_DATA)

#define IOCTL_REDIR_QUERY_PATH          CTL_CODE(FILE_DEVICE_NETWORK_FILE_SYSTEM, 99, METHOD_NEITHER, FILE_ANY_ACCESS)

	typedef PVOID PEJOB;
	typedef PVOID PNOTIFY_SYNC;
	typedef PVOID OPLOCK, *POPLOCK;
	typedef PVOID PWOW64_PROCESS;

	typedef ULONG LBN;
	typedef LBN *PLBN;

	typedef ULONG VBN;
	typedef VBN *PVBN;

	typedef struct _CACHE_MANAGER_CALLBACKS         *PCACHE_MANAGER_CALLBACKS;
	typedef struct _EPROCESS_QUOTA_BLOCK            *PEPROCESS_QUOTA_BLOCK;
	typedef struct _FILE_GET_QUOTA_INFORMATION      *PFILE_GET_QUOTA_INFORMATION;
	typedef struct _HANDLE_TABLE                    *PHANDLE_TABLE;
	typedef struct _KEVENT_PAIR                     *PKEVENT_PAIR;
	typedef struct _KPROCESS                        *PKPROCESS;
	typedef struct _KQUEUE                          *PKQUEUE;
	typedef struct _KTRAP_FRAME                     *PKTRAP_FRAME;
	typedef struct _LPC_MESSAGE                     *PLPC_MESSAGE;
	typedef struct _MAILSLOT_CREATE_PARAMETERS      *PMAILSLOT_CREATE_PARAMETERS;
	typedef struct _MMWSL                           *PMMWSL;
	typedef struct _NAMED_PIPE_CREATE_PARAMETERS    *PNAMED_PIPE_CREATE_PARAMETERS;
	typedef struct _OBJECT_DIRECTORY                *POBJECT_DIRECTORY;
	typedef struct _PAGEFAULT_HISTORY               *PPAGEFAULT_HISTORY;
	typedef struct _PEB                             *PPEB;
	typedef struct _PS_IMPERSONATION_INFORMATION    *PPS_IMPERSONATION_INFORMATION;
	typedef struct _SECTION_OBJECT                  *PSECTION_OBJECT;
	typedef struct _SERVICE_DESCRIPTOR_TABLE        *PSERVICE_DESCRIPTOR_TABLE;
	typedef struct _SHARED_CACHE_MAP                *PSHARED_CACHE_MAP;
	typedef struct _TERMINATION_PORT                *PTERMINATION_PORT;
	typedef struct _VACB                            *PVACB;
	typedef struct _VAD_HEADER                      *PVAD_HEADER;

#if (VER_PRODUCTBUILD < 2195)
	typedef ULONG SIZE_T, *PSIZE_T;
#endif

#if (VER_PRODUCTBUILD < 7600)
	typedef enum _FAST_IO_POSSIBLE {
		FastIoIsNotPossible,
		FastIoIsPossible,
		FastIoIsQuestionable
	} FAST_IO_POSSIBLE;
#endif
	typedef enum _FILE_STORAGE_TYPE {
		StorageTypeDefault = 1,
		StorageTypeDirectory,
		StorageTypeFile,
		StorageTypeJunctionPoint,
		StorageTypeCatalog,
		StorageTypeStructuredStorage,
		StorageTypeEmbedding,
		StorageTypeStream
	} FILE_STORAGE_TYPE;

	typedef enum _IO_COMPLETION_INFORMATION_CLASS {
		IoCompletionBasicInformation
	} IO_COMPLETION_INFORMATION_CLASS;

#if (VER_PRODUCTBUILD == 2195)

	typedef enum _KSPIN_LOCK_QUEUE_NUMBER {
		LockQueueDispatcherLock,
		LockQueueContextSwapLock,
		LockQueuePfnLock,
		LockQueueSystemSpaceLock,
		LockQueueVacbLock,
		LockQueueMasterLock,
		LockQueueNonPagedPoolLock,
		LockQueueIoCancelLock,
		LockQueueWorkQueueLock,
		LockQueueIoVpbLock,
		LockQueueIoDatabaseLock,
		LockQueueIoCompletionLock,
		LockQueueNtfsStructLock,
		LockQueueAfdWorkQueueLock,
		LockQueueBcbLock,
		LockQueueMaximumLock
	} KSPIN_LOCK_QUEUE_NUMBER;

#endif // (VER_PRODUCTBUILD == 2195)

	typedef enum _LPC_TYPE {
		LPC_NEW_MESSAGE,
		LPC_REQUEST,
		LPC_REPLY,
		LPC_DATAGRAM,
		LPC_LOST_REPLY,
		LPC_PORT_CLOSED,
		LPC_CLIENT_DIED,
		LPC_EXCEPTION,
		LPC_DEBUG_EVENT,
		LPC_ERROR_EVENT,
		LPC_CONNECTION_REQUEST
	} LPC_TYPE;

#if (VER_PRODUCTBUILD < 7600)
	typedef enum _MMFLUSH_TYPE {
		MmFlushForDelete,
		MmFlushForWrite
	} MMFLUSH_TYPE;
#endif
	typedef enum _OBJECT_INFO_CLASS {
		ObjectBasicInfo,
		ObjectNameInfo,
		ObjectTypeInfo,
		ObjectAllTypesInfo,
		ObjectProtectionInfo
	} OBJECT_INFO_CLASS;

	typedef enum _PORT_INFORMATION_CLASS {
		PortNoInformation
	} PORT_INFORMATION_CLASS;

	typedef enum _SECTION_INFORMATION_CLASS {
		SectionBasicInformation,
		SectionImageInformation
	} SECTION_INFORMATION_CLASS;
#if (VER_PRODUCTBUILD < 7600)
	typedef enum _SID_NAME_USE {
		SidTypeUser = 1,
		SidTypeGroup,
		SidTypeDomain,
		SidTypeAlias,
		SidTypeWellKnownGroup,
		SidTypeDeletedAccount,
		SidTypeInvalid,
		SidTypeUnknown
	} SID_NAME_USE;
#endif
	typedef enum _SYSTEM_INFORMATION_CLASS {
		SystemBasicInformation,
		SystemProcessorInformation,
		SystemPerformanceInformation,
		SystemTimeOfDayInformation,
		SystemNotImplemented1,
		SystemProcessesAndThreadsInformation,
		SystemCallCounts,
		SystemConfigurationInformation,
		SystemProcessorTimes,
		SystemGlobalFlag,
		SystemNotImplemented2,
		SystemModuleInformation,
		SystemLockInformation,
		SystemNotImplemented3,
		SystemNotImplemented4,
		SystemNotImplemented5,
		SystemHandleInformation,
		SystemObjectInformation,
		SystemPagefileInformation,
		SystemInstructionEmulationCounts,
		SystemInvalidInfoClass1,
		SystemCacheInformation,
		SystemPoolTagInformation,
		SystemProcessorStatistics,
		SystemDpcInformation,
		SystemNotImplemented6,
		SystemLoadImage,
		SystemUnloadImage,
		SystemTimeAdjustment,
		SystemNotImplemented7,
		SystemNotImplemented8,
		SystemNotImplemented9,
		SystemCrashDumpInformation,
		SystemExceptionInformation,
		SystemCrashDumpStateInformation,
		SystemKernelDebuggerInformation,
		SystemContextSwitchInformation,
		SystemRegistryQuotaInformation,
		SystemLoadAndCallImage,
		SystemPrioritySeparation,
		SystemNotImplemented10,
		SystemNotImplemented11,
		SystemInvalidInfoClass2,
		SystemInvalidInfoClass3,
		SystemTimeZoneInformation,
		SystemLookasideInformation,
		SystemSetTimeSlipEvent,
		SystemCreateSession,
		SystemDeleteSession,
		SystemInvalidInfoClass4,
		SystemRangeStartInformation,
		SystemVerifierInformation,
		SystemAddVerifier,
		SystemSessionProcessesInformation
	} SYSTEM_INFORMATION_CLASS;

	typedef enum _THREAD_STATE {
		StateInitialized,
		StateReady,
		StateRunning,
		StateStandby,
		StateTerminated,
		StateWait,
		StateTransition,
		StateUnknown
	} THREAD_STATE;
#if (VER_PRODUCTBUILD < 7600)
	typedef enum _TOKEN_INFORMATION_CLASS {
		TokenUser = 1,
		TokenGroups,
		TokenPrivileges,
		TokenOwner,
		TokenPrimaryGroup,
		TokenDefaultDacl,
		TokenSource,
		TokenType,
		TokenImpersonationLevel,
		TokenStatistics,
		TokenRestrictedSids
	} TOKEN_INFORMATION_CLASS;

	typedef enum _TOKEN_TYPE {
		TokenPrimary = 1,
		TokenImpersonation
	} TOKEN_TYPE;
#endif
	typedef struct _HARDWARE_PTE_X86 {
		ULONG Valid             : 1;
		ULONG Write             : 1;
		ULONG Owner             : 1;
		ULONG WriteThrough      : 1;
		ULONG CacheDisable      : 1;
		ULONG Accessed          : 1;
		ULONG Dirty             : 1;
		ULONG LargePage         : 1;
		ULONG Global            : 1;
		ULONG CopyOnWrite       : 1;
		ULONG Prototype         : 1;
		ULONG reserved          : 1;
		ULONG PageFrameNumber   : 20;
	} HARDWARE_PTE_X86, *PHARDWARE_PTE_X86;
#if (VER_PRODUCTBUILD < 7600)
	typedef struct _KAPC_STATE 
	{
		LIST_ENTRY	ApcListHead[MaximumMode];
		PKPROCESS	Process;
		BOOLEAN		KernelApcInProgress;
		BOOLEAN		KernelApcPending;
		BOOLEAN		UserApcPending;
	} KAPC_STATE, *PKAPC_STATE, *PRKAPC_STATE;
#endif
	typedef struct _KGDTENTRY {
		USHORT LimitLow;
		USHORT BaseLow;
		union {
			struct {
				UCHAR BaseMid;
				UCHAR Flags1;
				UCHAR Flags2;
				UCHAR BaseHi;
			} Bytes;
			struct {
				ULONG BaseMid       : 8;
				ULONG Type          : 5;
				ULONG Dpl           : 2;
				ULONG Pres          : 1;
				ULONG LimitHi       : 4;
				ULONG Sys           : 1;
				ULONG Reserved_0    : 1;
				ULONG Default_Big   : 1;
				ULONG Granularity   : 1;
				ULONG BaseHi        : 8;
			} Bits;
		} HighWord;
	} KGDTENTRY, *PKGDTENTRY;

	typedef struct _KIDTENTRY {
		USHORT Offset;
		USHORT Selector;
		USHORT Access;
		USHORT ExtendedOffset;
	} KIDTENTRY, *PKIDTENTRY;

#if (VER_PRODUCTBUILD >= 2600)

	typedef struct _KPROCESS {
		DISPATCHER_HEADER   Header;
		LIST_ENTRY          ProfileListHead;
		ULONG               DirectoryTableBase[2];
		KGDTENTRY           LdtDescriptor;
		KIDTENTRY           Int21Descriptor;
		USHORT              IopmOffset;
		UCHAR               Iopl;
		UCHAR               Unused;
		ULONG               ActiveProcessors;
		ULONG               KernelTime;
		ULONG               UserTime;
		LIST_ENTRY          ReadyListHead;  
		SINGLE_LIST_ENTRY   SwapListEntry;
		PVOID               VdmTrapcHandler;
		LIST_ENTRY          ThreadListHead;
		KSPIN_LOCK          ProcessLock;
		KAFFINITY           Affinity;
		USHORT              StackCount;
		CHAR                BasePriority;
		CHAR                ThreadQuantum;
		BOOLEAN             AutoAlignment;
		UCHAR               State;
		UCHAR               ThreadSeed;
		BOOLEAN             DisableBoost;
		UCHAR               PowerState;
		BOOLEAN             DisableQuantum;
		UCHAR               IdealNode;
		UCHAR               Spare;
	} KPROCESS, *PKPROCESS;

#else

	typedef struct _KPROCESS {
		DISPATCHER_HEADER   Header;
		LIST_ENTRY          ProfileListHead;
		ULONG               DirectoryTableBase[2];
		KGDTENTRY           LdtDescriptor;
		KIDTENTRY           Int21Descriptor;
		USHORT              IopmOffset;
		UCHAR               Iopl;
		UCHAR               VdmFlag;
		ULONG               ActiveProcessors;
		ULONG               KernelTime;
		ULONG               UserTime;
		LIST_ENTRY          ReadyListHead;  
		SINGLE_LIST_ENTRY   SwapListEntry;
		PVOID               Reserved1;
		LIST_ENTRY          ThreadListHead;
		KSPIN_LOCK          ProcessLock;
		KAFFINITY           Affinity;
		USHORT              StackCount;
		UCHAR               BasePriority;
		UCHAR               ThreadQuantum;
		BOOLEAN             AutoAlignment;
		UCHAR               State;
		UCHAR               ThreadSeed;
		BOOLEAN             DisableBoost;
#if (VER_PRODUCTBUILD >= 2195)
		UCHAR               PowerState;
		BOOLEAN             DisableQuantum;
		UCHAR               IdealNode;
		UCHAR               Spare;
#endif // (VER_PRODUCTBUILD >= 2195)
	} KPROCESS, *PKPROCESS;

#endif

#if (VER_PRODUCTBUILD >= 3790)

	typedef struct _KTHREAD {
		DISPATCHER_HEADER           Header;
		LIST_ENTRY                  MutantListHead; // 0x10
		PVOID                       InitialStack; // 0x18
		PVOID                       StackLimit; // 0x1c
		PVOID                       KernelStack; // 0x20
		ULONG                       ThreadLock; // 0x24
		ULONG                       ContextSwitches; // 0x28
		UCHAR                       State; // 0x2c
		UCHAR                       NpxState; // 0x2d
		UCHAR                       WaitIrql; // 0x2e
		CHAR                        WaitMode; // 0x2f
		struct _TEB                 *Teb; // 0x30
		KAPC_STATE                  ApcState; // 0x34
		KSPIN_LOCK                  ApcQueueLock; // 0x4c
		NTSTATUS                    WaitStatus; // 0x50
		PKWAIT_BLOCK                WaitBlockList; // 0x54
		BOOLEAN                     Alertable; // 0x58
		UCHAR                       WaitNext; // 0x59
		UCHAR                       WaitReason; // 0x5a
		CHAR                        Priority; // 0x5b
		BOOLEAN                     EnableStackSwap; // 0x5c
		BOOLEAN                     SwapBusy; // 0x5d
		UCHAR                       Alerted[2]; // 0x5e
		union {
			LIST_ENTRY              WaitListEntry; // 0x60
			SINGLE_LIST_ENTRY       SwapListEntry; // 0x60
		};
		PKQUEUE                     Queue; // 0x68
		ULONG                       WaitTime; // 0x6c
		union {
			struct {
				USHORT              KernelApcDisable; // 0x70
				USHORT              SpecialApcDisable; // 0x72
			};
			USHORT                  CombinedApcDisable; // 0x70
		};
		KTIMER                      Timer; // 0x78
		KWAIT_BLOCK                 WaitBlock[4]; // 0xa0
		LIST_ENTRY                  QueueListEntry; // 0x100
		UCHAR                       ApcStateIndex; // 0x108
		BOOLEAN                     ApcQueueable; // 0x109
		BOOLEAN                     Preempted; // 0x10a
		BOOLEAN                     ProcessReadyQueue; // 0x10b
		BOOLEAN                     KernelStackResident; // 0x10c
		CHAR                        Saturation; // 0x10d
		UCHAR                       IdealProcessor; // 0x10e
		UCHAR                       NextProcessor; // 0x10f
		CHAR                        BasePriority; // 0x110
		UCHAR                       Spare4; // 0x111
		CHAR                        PriorityDecrement; // 0x112
		CHAR                        Quantum; // 0x113
		BOOLEAN                     SystemAffinityActive; // 0x114
		CHAR                        PreviousMode; // 0x115
		UCHAR                       ResourceIndex; // 0x116
		BOOLEAN                     DisableBoost; // 0x117
		ULONG                       UserAffinity; // 0x118
		PKPROCESS                   Process; // 0x11c
		ULONG                       Affinity; // 0x120
		PSERVICE_DESCRIPTOR_TABLE   ServiceTable; // 0x124
		PKAPC_STATE                 ApcStatePointer[2]; // 0x128
		KAPC_STATE                  SavedApcState; // 0x130
		PVOID                       CallbackStack; // 0x148
		PVOID                       Win32Thread; // 0x14c
		PKTRAP_FRAME                TrapFrame; // 0x150
		ULONG                       KernelTime; // 0x154
		ULONG                       UserTime; // 0x158
		PVOID                       StackBase; // 0x15c
		KAPC                        SuspendApc; // 0x160
		KSEMAPHORE                  SuspendSemaphore; // 0x190
		PVOID                       TlsArray; // 0x1a4
		PVOID                       LegoData; // 0x1a8
		LIST_ENTRY                  ThreadListEntry; // 0x1ac
		BOOLEAN                     LargeStack; // 0x1b4
		UCHAR                       PowerState; // 0x1b5
		UCHAR                       NpxIrql; // 0x1b6
		UCHAR                       Spare5; // 0x1b7
		BOOLEAN                     AutoAlignment; // 0x1b8
		UCHAR                       Iopl; // 0x1b9
		CHAR                        FreezeCount; // 0x1ba
		CHAR                        SuspendCount; // 0x1bb
		UCHAR                       Spare0[1]; // 0x1bc
		UCHAR                       UserIdealProcessor; // 0x1bd
		UCHAR                       DeferredProcessor; // 0x1be
		UCHAR                       AdjustReason; // 0x1bf
		CHAR                        AdjustIncrement; // 0x1c0
		UCHAR                       Spare2[3]; // 0x1c1
	} KTHREAD, *PKTHREAD;

#elif (VER_PRODUCTBUILD >= 2600)

	typedef struct _KTHREAD {
		DISPATCHER_HEADER           Header;
		LIST_ENTRY                  MutantListHead;
		PVOID                       InitialStack;
		PVOID                       StackLimit;
		struct _TEB                 *Teb;
		PVOID                       TlsArray;
		PVOID                       KernelStack;
		BOOLEAN                     DebugActive;
		UCHAR                       State;
		UCHAR                       Alerted[2];
		UCHAR                       Iopl;
		UCHAR                       NpxState;
		CHAR                        Saturation;
		CHAR                        Priority;
		KAPC_STATE                  ApcState;
		ULONG                       ContextSwitches;
		UCHAR                       IdleSwapBlock;
		UCHAR                       Spare0[3];
		NTSTATUS                    WaitStatus;
		UCHAR                       WaitIrql;
		CHAR                        WaitMode;
		UCHAR                       WaitNext;
		UCHAR                       WaitReason;
		PKWAIT_BLOCK                WaitBlockList;
		union {
			LIST_ENTRY              WaitListEntry;
			SINGLE_LIST_ENTRY       SwapListEntry;
		};
		ULONG                       WaitTime;
		CHAR                        BasePriority;
		UCHAR                       DecrementCount;
		CHAR                        PriorityDecrement;
		CHAR                        Quantum;
		KWAIT_BLOCK                 WaitBlock[4];
		PVOID                       LegoData;
		ULONG                       KernelApcDisable;
		ULONG                       UserAffinity;
		BOOLEAN                     SystemAffinityActive;
		UCHAR                       PowerState;
		UCHAR                       NpxIrql;
		UCHAR                       InitialNode;
		PSERVICE_DESCRIPTOR_TABLE   ServiceTable;
		PKQUEUE                     Queue;
		KSPIN_LOCK                  ApcQueueLock;
		KTIMER                      Timer;
		LIST_ENTRY                  QueueListEntry;
		ULONG                       SoftAffinity;
		ULONG                       Affinity;
		BOOLEAN                     Preempted;
		BOOLEAN                     ProcessReadyQueue;
		BOOLEAN                     KernelStackResident;
		UCHAR                       NextProcessor;
		PVOID                       CallbackStack;
		PVOID                       Win32Thread;
		PKTRAP_FRAME                TrapFrame;
		PKAPC_STATE                 ApcStatePointer[2];
		CHAR                        PreviousMode;
		BOOLEAN                     EnableStackSwap;
		BOOLEAN                     LargeStack;
		UCHAR                       ResourceIndex;
		ULONG                       KernelTime;
		ULONG                       UserTime;
		KAPC_STATE                  SavedApcState;
		BOOLEAN                     Alertable;
		UCHAR                       ApcStateIndex;
		BOOLEAN                     ApcQueueable;
		BOOLEAN                     AutoAlignment;
		PVOID                       StackBase;
		KAPC                        SuspendApc;
		KSEMAPHORE                  SuspendSemaphore;
		LIST_ENTRY                  ThreadListEntry;
		CHAR                        FreezeCount;
		CHAR                        SuspendCount;
		UCHAR                       IdealProcessor;
		BOOLEAN                     DisableBoost;
	} KTHREAD, *PKTHREAD;

#else

	typedef struct _KTHREAD {
		DISPATCHER_HEADER           Header;
		LIST_ENTRY                  MutantListHead;
		PVOID                       InitialStack;
		PVOID                       StackLimit;
		struct _TEB                 *Teb;
		PVOID                       TlsArray;
		PVOID                       KernelStack;
		BOOLEAN                     DebugActive;
		UCHAR                       State;
		USHORT                      Alerted;
		UCHAR                       Iopl;
		UCHAR                       NpxState;
		UCHAR                       Saturation;
		UCHAR                       Priority;
		KAPC_STATE                  ApcState;
		ULONG                       ContextSwitches;
		NTSTATUS                    WaitStatus;
		UCHAR                       WaitIrql;
		UCHAR                       WaitMode;
		UCHAR                       WaitNext;
		UCHAR                       WaitReason;
		PKWAIT_BLOCK                WaitBlockList;
		LIST_ENTRY                  WaitListEntry;
		ULONG                       WaitTime;
		UCHAR                       BasePriority;
		UCHAR                       DecrementCount;
		UCHAR                       PriorityDecrement;
		UCHAR                       Quantum;
		KWAIT_BLOCK                 WaitBlock[4];
		ULONG                       LegoData;
		ULONG                       KernelApcDisable;
		ULONG                       UserAffinity;
		BOOLEAN                     SystemAffinityActive;
#if (VER_PRODUCTBUILD < 2195)
		UCHAR                       Pad[3];
#else // (VER_PRODUCTBUILD >= 2195)
		UCHAR                       PowerState;
		UCHAR                       NpxIrql;
		UCHAR                       Pad[1];
#endif // (VER_PRODUCTBUILD >= 2195)
		PSERVICE_DESCRIPTOR_TABLE   ServiceDescriptorTable;
		PKQUEUE                     Queue;
		KSPIN_LOCK                  ApcQueueLock;
		KTIMER                      Timer;
		LIST_ENTRY                  QueueListEntry;
		ULONG                       Affinity;
		BOOLEAN                     Preempted;
		BOOLEAN                     ProcessReadyQueue;
		BOOLEAN                     KernelStackResident;
		UCHAR                       NextProcessor;
		PVOID                       CallbackStack;
		PVOID                       Win32Thread;
		PKTRAP_FRAME                TrapFrame;
		PKAPC_STATE                 ApcStatePointer[2];
#if (VER_PRODUCTBUILD >= 2195)
		UCHAR                       PreviousMode;
#endif // (VER_PRODUCTBUILD >= 2195)
		BOOLEAN                     EnableStackSwap;
		BOOLEAN                     LargeStack;
		UCHAR                       ResourceIndex;
#if (VER_PRODUCTBUILD < 2195)
		UCHAR                       PreviousMode;
#endif // (VER_PRODUCTBUILD < 2195)
		ULONG                       KernelTime;
		ULONG                       UserTime;
		KAPC_STATE                  SavedApcState;
		BOOLEAN                     Alertable;
		UCHAR                       ApcStateIndex;
		BOOLEAN                     ApcQueueable;
		BOOLEAN                     AutoAlignment;
		PVOID                       StackBase;
		KAPC                        SuspendApc;
		KSEMAPHORE                  SuspendSemaphore;
		LIST_ENTRY                  ThreadListEntry;
		UCHAR                       FreezeCount;
		UCHAR                       SuspendCount;
		UCHAR                       IdealProcessor;
		BOOLEAN                     DisableBoost;
	} KTHREAD, *PKTHREAD;

#endif

#if (VER_PRODUCTBUILD >= 3790)

	typedef struct _MMSUPPORT_FLAGS {
		ULONG SessionSpace              : 1;
		ULONG BeingTrimmed              : 1;
		ULONG SessionLeader             : 1;
		ULONG TrimHard                  : 1;
		ULONG MaximumWorkingSetHard     : 1;
		ULONG ForceTrim                 : 1;
		ULONG MinimumWorkingSetHard     : 1;
		ULONG Available0                : 1;
		ULONG MemoryPriority            : 8;
		ULONG GrowWsleHash              : 1;
		ULONG AcquiredUnsafe            : 1;
		ULONG Available                 : 14;
	} MMSUPPORT_FLAGS, *PMMSUPPORT_FLAGS;

#elif (VER_PRODUCTBUILD >= 2600)

	typedef struct _MMSUPPORT_FLAGS {
		ULONG SessionSpace              : 1;
		ULONG BeingTrimmed              : 1;
		ULONG SessionLeader             : 1;
		ULONG TrimHard                  : 1;
		ULONG WorkingSetHard            : 1;
		ULONG AddressSpaceBeingDeleted  : 1;
		ULONG Available                 : 10;
		ULONG AllowWorkingSetAdjustment : 8;
		ULONG MemoryPriority            : 8;
	} MMSUPPORT_FLAGS, *PMMSUPPORT_FLAGS;

#else

	typedef struct _MMSUPPORT_FLAGS {
		ULONG SessionSpace      : 1;
		ULONG BeingTrimmed      : 1;
		ULONG ProcessInSession  : 1;
		ULONG SessionLeader     : 1;
		ULONG TrimHard          : 1;
		ULONG WorkingSetHard    : 1;
		ULONG WriteWatch        : 1;
		ULONG Filler            : 25;
	} MMSUPPORT_FLAGS, *PMMSUPPORT_FLAGS;

#endif

#if (VER_PRODUCTBUILD >= 3790)
	/*
	typedef struct _KGUARDED_MUTEX {
	LONG            Count;
	PKTHREAD        Owner; // 0x4
	ULONG           Contention; // 0x8
	KEVENT          Event; // 0xc
	union {
	struct {
	USHORT  KernelApcDisable; // 0x1c
	USHORT  SpecialApcDisable; // 0x1e
	};
	USHORT      CombinedApcDisable; // 0x1c
	};
	} KGUARDED_MUTEX, *PKGUARDED_MUTEX;
	*/
	typedef struct _MMSUPPORT {
		LIST_ENTRY      WorkingSetExpansionLinks;
		LARGE_INTEGER   LastTrimTime; // 0x8
		MMSUPPORT_FLAGS Flags; // 0x10
		ULONG           PageFaultCount; // 0x14
		ULONG           PeakWorkingSetSize; // 0x18
		ULONG           GrowthSinceLastEstimate; // 0x1c
		ULONG           MinimumWorkingSetSize; // 0x20
		ULONG           MaximumWorkingSetSize; // 0x24
		PMMWSL          VmWorkingSetList; // 0x28 
		ULONG           Claim; // 0x2c
		ULONG           NextEstimationSlot; // 0x30
		ULONG           NextAgingSlot; // 0x34
		ULONG           EstimatedAvailable; // 0x38
		ULONG           WorkingSetSize;  //0x3c
		KGUARDED_MUTEX  Mutex; // 0x40
	} MMSUPPORT, *PMMSUPPORT;

#elif (VER_PRODUCTBUILD >= 2600)

	typedef struct _MMSUPPORT {
		LARGE_INTEGER   LastTrimTime;
		MMSUPPORT_FLAGS Flags;
		ULONG           PageFaultCount;
		ULONG           PeakWorkingSetSize;
		ULONG           WorkingSetSize;
		ULONG           MinimumWorkingSetSize;
		ULONG           MaximumWorkingSetSize;
		PMMWSL          VmWorkingSetList;
		LIST_ENTRY      WorkingSetExpansionLinks;
		ULONG           Claim;
		ULONG           NextEstimationSlot;
		ULONG           NextAgingSlot;
		ULONG           EstimatedAvailable;
		ULONG           GrowthSinceLastEstimate;
	} MMSUPPORT, *PMMSUPPORT;

#else

	typedef struct _MMSUPPORT {
		LARGE_INTEGER   LastTrimTime;
		ULONG           LastTrimFaultCount;
		ULONG           PageFaultCount;
		ULONG           PeakWorkingSetSize;
		ULONG           WorkingSetSize;
		ULONG           MinimumWorkingSetSize;
		ULONG           MaximumWorkingSetSize;
		PMMWSL          VmWorkingSetList;
		LIST_ENTRY      WorkingSetExpansionLinks;
		BOOLEAN         AllowWorkingSetAdjustment;
		BOOLEAN         AddressSpaceBeingDeleted;
		UCHAR           ForegroundSwitchCount;
		UCHAR           MemoryPriority;
#if (VER_PRODUCTBUILD >= 2195)
		union {
			ULONG           LongFlags;
			MMSUPPORT_FLAGS Flags;
		} u;
		ULONG           Claim;
		ULONG           NextEstimationSlot;
		ULONG           NextAgingSlot;
		ULONG           EstimatedAvailable;
		ULONG           GrowthSinceLastEstimate;
#endif // (VER_PRODUCTBUILD >= 2195)
	} MMSUPPORT, *PMMSUPPORT;

#endif

	typedef struct _SE_AUDIT_PROCESS_CREATION_INFO {
		POBJECT_NAME_INFORMATION ImageFileName;
	} SE_AUDIT_PROCESS_CREATION_INFO, *PSE_AUDIT_PROCESS_CREATION_INFO;
#if (VER_PRODUCTBUILD < 7600)
	typedef struct _SID_IDENTIFIER_AUTHORITY {
		UCHAR Value[6];
	} SID_IDENTIFIER_AUTHORITY, *PSID_IDENTIFIER_AUTHORITY;

	typedef struct _SID {
		UCHAR                       Revision;
		UCHAR                       SubAuthorityCount;
		SID_IDENTIFIER_AUTHORITY    IdentifierAuthority;
		ULONG                       SubAuthority[1];
	} SID, *PREAL_SID;
#endif
	typedef struct _BITMAP_DESCRIPTOR {
		ULONGLONG   StartLcn;
		ULONGLONG   ClustersToEndOfVol;
		UCHAR       Map[1];
	} BITMAP_DESCRIPTOR, *PBITMAP_DESCRIPTOR; 

	typedef struct _BITMAP_RANGE {
		LIST_ENTRY      Links;
		LARGE_INTEGER   BasePage;
		ULONG           FirstDirtyPage;
		ULONG           LastDirtyPage;
		ULONG           DirtyPages;
		PULONG          Bitmap;
	} BITMAP_RANGE, *PBITMAP_RANGE;
#if (VER_PRODUCTBUILD < 7600)
	typedef struct _CACHE_UNINITIALIZE_EVENT {
		struct _CACHE_UNINITIALIZE_EVENT    *Next;
		KEVENT                              Event;
	} CACHE_UNINITIALIZE_EVENT, *PCACHE_UNINITIALIZE_EVENT;

	typedef struct _CC_FILE_SIZES {
		LARGE_INTEGER AllocationSize;
		LARGE_INTEGER FileSize;
		LARGE_INTEGER ValidDataLength;
	} CC_FILE_SIZES, *PCC_FILE_SIZES;

	typedef struct _COMPRESSED_DATA_INFO {
		USHORT  CompressionFormatAndEngine;
		UCHAR   CompressionUnitShift;
		UCHAR   ChunkShift;
		UCHAR   ClusterShift;
		UCHAR   Reserved;
		USHORT  NumberOfChunks;
		ULONG   CompressedChunkSizes[ANYSIZE_ARRAY];
	} COMPRESSED_DATA_INFO, *PCOMPRESSED_DATA_INFO;
#endif
	typedef struct _DEVICE_MAP {
		POBJECT_DIRECTORY   DosDevicesDirectory;
		POBJECT_DIRECTORY   GlobalDosDevicesDirectory;
		ULONG               ReferenceCount;
		ULONG               DriveMap;
		UCHAR               DriveType[32];
	} DEVICE_MAP, *PDEVICE_MAP; 

	typedef struct _DIRECTORY_BASIC_INFORMATION {
		UNICODE_STRING ObjectName;
		UNICODE_STRING ObjectTypeName;
	} DIRECTORY_BASIC_INFORMATION, *PDIRECTORY_BASIC_INFORMATION;

#if (VER_PRODUCTBUILD >= 2600)

	typedef struct _EX_FAST_REF {
		union {
			PVOID Object;
			ULONG RefCnt : 3;
			ULONG Value;
		};
	} EX_FAST_REF, *PEX_FAST_REF;

#if (VER_PRODUCTBUILD < 7600)
	typedef struct _EX_PUSH_LOCK {
		union {
			struct {
				ULONG   Waiting     : 1;
				ULONG   Exclusive   : 1;
				ULONG   Shared      : 30;
			};
			ULONG   Value;
			PVOID   Ptr;
		};
	} EX_PUSH_LOCK, *PEX_PUSH_LOCK;
#endif

#endif // (VER_PRODUCTBUILD >= 2600)

#if (VER_PRODUCTBUILD == 2600)

	typedef struct _EX_RUNDOWN_REF {
		union {
			ULONG Count;
			PVOID Ptr;
		};
	} EX_RUNDOWN_REF, *PEX_RUNDOWN_REF;

#endif // (VER_PRODUCTBUILD == 2600)

#if (VER_PRODUCTBUILD >= 3790 && VER_PRODUCTBUILD < 7600)

	typedef struct _MM_ADDRESS_NODE {
		union {
			ULONG                   Balance : 2;
			struct _MM_ADDRESS_NODE *Parent; // lower 2 bits of Parent are Balance and must be zeroed to obtain Parent
		};
		struct _MM_ADDRESS_NODE     *LeftChild;
		struct _MM_ADDRESS_NODE     *RightChild;
		ULONG_PTR                   StartingVpn;
		ULONG_PTR                   EndingVpn;
	} MMADDRESS_NODE, *PMMADDRESS_NODE;

	typedef struct _MM_AVL_TABLE {
		MMADDRESS_NODE  BalancedRoot; // Vadroot; incorrectly represents the NULL pages (EndingVpn should be 0xf, etc.)
		ULONG           DepthOfTree : 5; // 0x14
		ULONG           Unused : 3;
		ULONG           NumberGenericTableElements : 24; // total number of nodes
		PVOID           NodeHint; // 0x18 (0x270 __in _EPROCESS)
		PVOID           NodeFreeHint; // 0x1c
	} MM_AVL_TABLE, *PMM_AVL_TABLE;

	typedef struct _EPROCESS {
		KPROCESS                        Pcb; // +0x000
		EX_PUSH_LOCK                    ProcessLock; // +0x06c
		LARGE_INTEGER                   CreateTime; // +0x070
		LARGE_INTEGER                   ExitTime; // +0x078
		EX_RUNDOWN_REF                  RundownProtect; // +0x080
		ULONG                           UniqueProcessId; // +0x084
		LIST_ENTRY                      ActiveProcessLinks; // +0x088
		ULONG                           QuotaUsage[3]; // +0x090
		ULONG                           QuotaPeak[3]; // +0x09c
		ULONG                           CommitCharge; // +0x0a8
		ULONG                           PeakVirtualSize; // +0x0ac
		ULONG                           VirtualSize; // +0x0b0
		LIST_ENTRY                      SessionProcessLinks; // +0x0b4
		PVOID                           DebugPort; // +0x0bc
		PVOID                           ExceptionPort; // +0x0c0
		PHANDLE_TABLE                   ObjectTable; // +0x0c4
		EX_FAST_REF                     Token; // +0x0c8
		ULONG                           WorkingSetPage; // +0x0cc
		KGUARDED_MUTEX                  AddressCreationLock; // +0x0d0
		ULONG                           HyperSpaceLock; // +0x0f0
		PETHREAD                        ForkInProgress; // +0x0f4
		ULONG                           HardwareTrigger; // +0x0f8
		PMM_AVL_TABLE                   PhysicalVadRoot; // +0x0fc
		PVOID                           CloneRoot; // +0x100
		ULONG                           NumberOfPrivatePages; // +0x104
		ULONG                           NumberOfLockedPages; // +0x108
		PVOID                           Win32Process; // +0x10c
		PEJOB                           Job; // +0x110
		PVOID                           SectionObject; // +0x114
		PVOID                           SectionBaseAddress; // +0x118
		PEPROCESS_QUOTA_BLOCK           QuotaBlock; // +0x11c
		PPAGEFAULT_HISTORY              WorkingSetWatch; // +0x120
		PVOID                           Win32WindowStation; // +0x124
		ULONG                           InheritedFromUniqueProcessId; // +0x128
		PVOID                           LdtInformation; // +0x12c
		PVOID                           VadFreeHint; // +0x130
		PVOID                           VdmObjects; // +0x134
		PVOID                           DeviceMap; // +0x138
		PVOID                           Spare0[3]; // +0x13c
		union {
			HARDWARE_PTE                PageDirectoryPte; // +0x148
			UINT64                      Filler; // +0x148
		};
		PVOID                           Session; // +0x150
		UCHAR                           ImageFileName[16]; // +0x154
		LIST_ENTRY                      JobLinks; // +0x164
		PVOID                           LockedPagesList; // +0x16c
		LIST_ENTRY                      ThreadListHead; // +0x170
		PVOID                           SecurityPort; // +0x178
		PVOID                           PaeTop; // +0x17c
		ULONG                           ActiveThreads; // +0x180
		ULONG                           GrantedAccess; // +0x184
		ULONG                           DefaultHardErrorProcessing; // +0x188
		SHORT                           LastThreadExitStatus; // +0x18c
		PPEB                            Peb; // +0x190
		EX_FAST_REF                     PrefetchTrace; // +0x194
		LARGE_INTEGER                   ReadOperationCount; // +0x198
		LARGE_INTEGER                   WriteOperationCount; // +0x1a0
		LARGE_INTEGER                   OtherOperationCount; // +0x1a8
		LARGE_INTEGER                   ReadTransferCount; // +0x1b0
		LARGE_INTEGER                   WriteTransferCount; // +0x1b8
		LARGE_INTEGER                   OtherTransferCount; // +0x1c0
		ULONG                           CommitChargeLimit; // +0x1c8
		ULONG                           CommitChargePeak; // +0x1cc
		PVOID                           AweInfo; // +0x1d0
		SE_AUDIT_PROCESS_CREATION_INFO  SeAuditProcessCreationInfo; // +0x1d4
		MMSUPPORT                       Vm; // +0x1d8
		LIST_ENTRY                      MmProcessLinks; // +0x238
		ULONG                           ModifiedPageCount; // +0x240
		ULONG                           JobStatus; // +0x244
		union {
			ULONG                       Flags; // 0x248
			struct {
				ULONG                   CreateReported              : 1;
				ULONG                   NoDebugInherit              : 1;
				ULONG                   ProcessExiting              : 1;
				ULONG                   ProcessDelete               : 1;
				ULONG                   Wow64SplitPages             : 1;
				ULONG                   VmDeleted                   : 1;
				ULONG                   OutswapEnabled              : 1;
				ULONG                   Outswapped                  : 1;
				ULONG                   ForkFailed                  : 1;
				ULONG                   Wow64VaSpace4Gb             : 1;
				ULONG                   AddressSpaceInitialized     : 2;
				ULONG                   SetTimerResolution          : 1;
				ULONG                   BreakOnTermination          : 1;
				ULONG                   SessionCreationUnderway     : 1;
				ULONG                   WriteWatch                  : 1;
				ULONG                   ProcessInSession            : 1;
				ULONG                   OverrideAddressSpace        : 1;
				ULONG                   HasAddressSpace             : 1;
				ULONG                   LaunchPrefetched            : 1;
				ULONG                   InjectInpageErrors          : 1;
				ULONG                   VmTopDown                   : 1;
				ULONG                   ImageNotifyDone             : 1;
				ULONG                   PdeUpdateNeeded             : 1;
				ULONG                   VdmAllowed                  : 1;
				ULONG                   Unused                      : 7;
			};
		};
		NTSTATUS                        ExitStatus; // +0x24c
		USHORT                          NextPageColor; // +0x250
		union {
			struct {
				UCHAR                   SubSystemMinorVersion; // +0x252
				UCHAR                   SubSystemMajorVersion; // +0x253
			};
			USHORT                      SubSystemVersion; // +0x252
		};
		UCHAR                           PriorityClass; // +0x254
		MM_AVL_TABLE                    VadRoot; // +0x258
	} EPROCESS, *PEPROCESS; // 0x278 __in total

#elif (VER_PRODUCTBUILD >= 2600 && VER_PRODUCTBUILD < 7600)

	typedef struct _EPROCESS {
		KPROCESS                        Pcb;
		EX_PUSH_LOCK                    ProcessLock;
		LARGE_INTEGER                   CreateTime;
		LARGE_INTEGER                   ExitTime;
		EX_RUNDOWN_REF                  RundownProtect;
		ULONG                           UniqueProcessId;
		LIST_ENTRY                      ActiveProcessLinks;
		ULONG                           QuotaUsage[3];
		ULONG                           QuotaPeak[3];
		ULONG                           CommitCharge;
		ULONG                           PeakVirtualSize;
		ULONG                           VirtualSize;
		LIST_ENTRY                      SessionProcessLinks;
		PVOID                           DebugPort;
		PVOID                           ExceptionPort;
		PHANDLE_TABLE                   ObjectTable;
		EX_FAST_REF                     Token;
		FAST_MUTEX                      WorkingSetLock;
		ULONG                           WorkingSetPage;
		FAST_MUTEX                      AddressCreationLock;
		KSPIN_LOCK                      HyperSpaceLock;
		PETHREAD                        ForkInProgress;
		ULONG                           HardwareTrigger;
		PVOID                           VadRoot;
		PVOID                           VadHint;
		PVOID                           CloneRoot;
		ULONG                           NumberOfPrivatePages;
		ULONG                           NumberOfLockedPages;
		PVOID                           Win32Process;
		PEJOB                           Job;
		PSECTION_OBJECT                 SectionObject;
		PVOID                           SectionBaseAddress;
		PEPROCESS_QUOTA_BLOCK           QuotaBlock;
		PPAGEFAULT_HISTORY              WorkingSetWatch;
		PVOID                           Win32WindowStation;
		PVOID                           InheritedFromUniqueProcessId;
		PVOID                           LdtInformation;
		PVOID                           VadFreeHint;
		PVOID                           VdmObjects;
		PDEVICE_MAP                     DeviceMap;
		LIST_ENTRY                      PhysicalVadList;
		union {
			HARDWARE_PTE                PageDirectoryPte;
			ULONGLONG                   Filler;
		};
		PVOID                           Session;
		UCHAR                           ImageFileName[16];
		LIST_ENTRY                      JobLinks;
		PVOID                           LockedPageList;
		LIST_ENTRY                      ThreadListHead;
		PVOID                           SecurityPort;
		PVOID                           PaeTop;
		ULONG                           ActiveThreads;
		ULONG                           GrantedAccess;
		ULONG                           DefaultHardErrorProcessing;
		NTSTATUS                        LastThreadExitStatus;
		PPEB                            Peb;
		EX_FAST_REF                     PrefetchTrace;
		LARGE_INTEGER                   ReadOperationCount;
		LARGE_INTEGER                   WriteOperationCount;
		LARGE_INTEGER                   OtherOperationCount;
		LARGE_INTEGER                   ReadTransferCount;
		LARGE_INTEGER                   WriteTransferCount;
		LARGE_INTEGER                   OtherTransferCount;
		ULONG                           CommitChargeLimit;
		ULONG                           CommitChargePeek;
		PVOID                           AweInfo;
		SE_AUDIT_PROCESS_CREATION_INFO  SeAuditProcessCreationInfo;
		MMSUPPORT                       Vm;
		ULONG                           LastFaultCount;
		ULONG                           ModifiedPageCount;
		ULONG                           NumberOfVads;
		ULONG                           JobStatus;
		union {
			ULONG                       Flags;
			struct {
				ULONG                   CreateReported              : 1;
				ULONG                   NoDebugInherit              : 1;
				ULONG                   ProcessExiting              : 1;
				ULONG                   ProcessDelete               : 1;
				ULONG                   Wow64SplitPages             : 1;
				ULONG                   VmDeleted                   : 1;
				ULONG                   OutswapEnabled              : 1;
				ULONG                   Outswapped                  : 1;
				ULONG                   ForkFailed                  : 1;
				ULONG                   HasPhysicalVad              : 1;
				ULONG                   AddressSpaceInitialized     : 2;
				ULONG                   SetTimerResolution          : 1;
				ULONG                   BreakOnTermination          : 1;
				ULONG                   SessionCreationUnderway     : 1;
				ULONG                   WriteWatch                  : 1;
				ULONG                   ProcessInSession            : 1;
				ULONG                   OverrideAddressSpace        : 1;
				ULONG                   HasAddressSpace             : 1;
				ULONG                   LaunchPrefetched            : 1;
				ULONG                   InjectInpageErrors          : 1;
				ULONG                   Unused                      : 11;
			};
		};
		NTSTATUS                        ExitStatus;
		USHORT                          NextPageColor;
		union {
			struct {
				UCHAR                   SubSystemMinorVersion;
				UCHAR                   SubSystemMajorVersion;
			};
			USHORT                      SubSystemVersion;
		};
		UCHAR                           PriorityClass;
		BOOLEAN                         WorkingSetAcquiredUnsafe;
	} EPROCESS, *PEPROCESS;

#elif(VER_PRODUCTBUILD <7600)

	typedef struct _EPROCESS {
		KPROCESS                        Pcb;
		NTSTATUS                        ExitStatus;
		KEVENT                          LockEvent;
		ULONG                           LockCount;
		LARGE_INTEGER                   CreateTime;
		LARGE_INTEGER                   ExitTime;
		PKTHREAD                        LockOwner;
		ULONG                           UniqueProcessId;
		LIST_ENTRY                      ActiveProcessLinks;
		ULONGLONG                       QuotaPeakPoolUsage;
		ULONGLONG                       QuotaPoolUsage;
		ULONG                           PagefileUsage;
		ULONG                           CommitCharge;
		ULONG                           PeakPagefileUsage;
		ULONG                           PeakVirtualSize;
		ULONGLONG                       VirtualSize;
		MMSUPPORT                       Vm;
#if (VER_PRODUCTBUILD < 2195)
		ULONG                           LastProtoPteFault;
#else // (VER_PRODUCTBUILD >= 2195)
		LIST_ENTRY                      SessionProcessLinks;
#endif // (VER_PRODUCTBUILD >= 2195)
		ULONG                           DebugPort;
		ULONG                           ExceptionPort;
		PHANDLE_TABLE                   ObjectTable;
		PACCESS_TOKEN                   Token;
		FAST_MUTEX                      WorkingSetLock;
		ULONG                           WorkingSetPage;
		BOOLEAN                         ProcessOutswapEnabled;
		BOOLEAN                         ProcessOutswapped;
		BOOLEAN                         AddressSpaceInitialized;
		BOOLEAN                         AddressSpaceDeleted;
		FAST_MUTEX                      AddressCreationLock;
		KSPIN_LOCK                      HyperSpaceLock;
		PETHREAD                        ForkInProgress;
		USHORT                          VmOperation;
		BOOLEAN                         ForkWasSuccessful;
		UCHAR                           MmAgressiveWsTrimMask;
		PKEVENT                         VmOperationEvent;
#if (VER_PRODUCTBUILD < 2195)
		HARDWARE_PTE                    PageDirectoryPte;
#else // (VER_PRODUCTBUILD >= 2195)
		PVOID                           PaeTop;
#endif // (VER_PRODUCTBUILD >= 2195)
		ULONG                           LastFaultCount;
		ULONG                           ModifiedPageCount;
		PVOID                           VadRoot;
		PVOID                           VadHint;
		ULONG                           CloneRoot;
		ULONG                           NumberOfPrivatePages;
		ULONG                           NumberOfLockedPages;
		USHORT                          NextPageColor;
		BOOLEAN                         ExitProcessCalled;
		BOOLEAN                         CreateProcessReported;
		HANDLE                          SectionHandle;
		PPEB                            Peb;
		PVOID                           SectionBaseAddress;
		PEPROCESS_QUOTA_BLOCK           QuotaBlock;
		NTSTATUS                        LastThreadExitStatus;
		PPROCESS_WS_WATCH_INFORMATION   WorkingSetWatch;
		HANDLE                          Win32WindowStation;
		HANDLE                          InheritedFromUniqueProcessId;
		ACCESS_MASK                     GrantedAccess;
		ULONG                           DefaultHardErrorProcessing;
		PVOID                           LdtInformation;
		PVOID                           VadFreeHint;
		PVOID                           VdmObjects;
#if (VER_PRODUCTBUILD < 2195)
		KMUTANT                         ProcessMutant;
#else // (VER_PRODUCTBUILD >= 2195)
		PDEVICE_MAP                     DeviceMap;
		ULONG                           SessionId;
		LIST_ENTRY                      PhysicalVadList;
		HARDWARE_PTE                    PageDirectoryPte;
		ULONG                           Filler;
		ULONG                           PaePageDirectoryPage;
#endif // (VER_PRODUCTBUILD >= 2195)
		UCHAR                           ImageFileName[16];
		ULONG                           VmTrimFaultValue;
		UCHAR                           SetTimerResolution;
		UCHAR                           PriorityClass;
		union {
			struct {
				UCHAR                   SubSystemMinorVersion;
				UCHAR                   SubSystemMajorVersion;
			};
			USHORT                      SubSystemVersion;
		};
		PVOID                           Win32Process;
#if (VER_PRODUCTBUILD >= 2195)
		PEJOB                           Job;
		ULONG                           JobStatus;
		LIST_ENTRY                      JobLinks;
		PVOID                           LockedPageList;
		PVOID                           SecurityPort;
		PWOW64_PROCESS                  Wow64Process;
		LARGE_INTEGER                   ReadOperationCount;
		LARGE_INTEGER                   WriteOperationCount;
		LARGE_INTEGER                   OtherOperationCount;
		LARGE_INTEGER                   ReadTransferCount;
		LARGE_INTEGER                   WriteTransferCount;
		LARGE_INTEGER                   OtherTransferCount;
		ULONG                           CommitChargeLimit;
		ULONG                           CommitChargePeek;
		LIST_ENTRY                      ThreadListHead;
		PRTL_BITMAP                     VadPhysicalPagesBitMap;
		ULONG                           VadPhysicalPages;
		ULONG                           AweLock;
#endif // (VER_PRODUCTBUILD >= 2195)
	} EPROCESS, *PEPROCESS;

#endif

#if (VER_PRODUCTBUILD >= 2600 && VER_PRODUCTBUILD < 7600)

	typedef struct _ETHREAD {
		KTHREAD                         Tcb;
		union {
			LARGE_INTEGER               CreateTime;
			struct {
				ULONG                   NestedFaultCount    : 2;
				ULONG                   ApcNeeded           : 1;
			};
		};
		union {
			LARGE_INTEGER               ExitTime;
			LIST_ENTRY                  LpcReplyChain;
			LIST_ENTRY                  KeyedWaitChain;
		};
		union {
			NTSTATUS                    ExitStatus;
			PVOID                       OfsChain;
		};
		LIST_ENTRY                      PostBlockList;
		union {
			PTERMINATION_PORT           TerminationPort;
			PETHREAD                    ReaperLink;
			PVOID                       KeyedWaitValue;
		};
		KSPIN_LOCK                      ActiveTimerListLock;
		LIST_ENTRY                      ActiveTimerListHead;
		CLIENT_ID                       Cid;
		union {
			KSEMAPHORE                  LpcReplySemaphore;
			KSEMAPHORE                  KeyedWaitSemaphore;
		};
		union {
			PLPC_MESSAGE                LpcReplyMessage;
			PVOID                       LpcWaitingOnPort;
		};
		PPS_IMPERSONATION_INFORMATION   ImpersonationInfo;
		LIST_ENTRY                      IrpList;
		ULONG                           TopLevelIrp;
		PDEVICE_OBJECT                  DeviceToVerify;
		PEPROCESS                       ThreadsProcess;
		PKSTART_ROUTINE                 StartAddress;
		union {
			PVOID                       Win32StartAddress;
			ULONG                       LpcReceivedMessageId;
		};
		LIST_ENTRY                      ThreadListEntry;
		EX_RUNDOWN_REF                  RundownProtect;
		EX_PUSH_LOCK                    ThreadLock;
		ULONG                           LpcReplyMessageId;
		ULONG                           ReadClusterSize;
		ACCESS_MASK                     GrantedAccess;
		union {
			ULONG                       CrossThreadFlags;
			struct {
				ULONG                   Terminated              : 1;
				ULONG                   DeadThread              : 1;
				ULONG                   HideFromDebugger        : 1;
				ULONG                   ActiveImpersonationInfo : 1;
				ULONG                   SystemThread            : 1;
				ULONG                   HardErrorsAreDisabled   : 1;
				ULONG                   BreakOnTermination      : 1;
				ULONG                   SkipCreationMsg         : 1;
				ULONG                   SkipTerminationMsg      : 1;
			};
		};
		union {
			ULONG                       SameThreadPassiveFlags;
			struct {
				ULONG                   ActiveExWorker          : 1;
				ULONG                   ExWorkerCanWaitUser     : 1;
				ULONG                   MemoryMaker             : 1;
				ULONG                   KeyedEventInUse         : 1;
			};
		};
		union {
			ULONG                       SameThreadApcFlags;
			struct {
				BOOLEAN                 LpcReceivedMsgIdValid   : 1;
				BOOLEAN                 LpcExitThreadCalled     : 1;
				BOOLEAN                 AddressSpaceOwner       : 1;
			};
		};
		BOOLEAN                         ForwardClusterOnly;
		BOOLEAN                         DisablePageFaultClustering;
	} ETHREAD, *PETHREAD;

#elif(VER_PRODUCTBUILD < 7600)

	typedef struct _ETHREAD {
		KTHREAD                         Tcb;
		LARGE_INTEGER                   CreateTime;
		union {
			LARGE_INTEGER               ExitTime;
			LIST_ENTRY                  LpcReplyChain;
		};
		union {
			NTSTATUS                    ExitStatus;
			PVOID                       OfsChain;
		};
		LIST_ENTRY                      PostBlockList;
		LIST_ENTRY                      TerminationPortList;
		KSPIN_LOCK                      ActiveTimerListLock;
		LIST_ENTRY                      ActiveTimerListHead;
		CLIENT_ID                       Cid;
		KSEMAPHORE                      LpcReplySemaphore;
		PLPC_MESSAGE                    LpcReplyMessage;
		ULONG                           LpcReplyMessageId;
		ULONG                           PerformanceCountLow;
		PPS_IMPERSONATION_INFORMATION   ImpersonationInfo;
		LIST_ENTRY                      IrpList;
		PVOID                           TopLevelIrp;
		PDEVICE_OBJECT                  DeviceToVerify;
		ULONG                           ReadClusterSize;
		BOOLEAN                         ForwardClusterOnly;
		BOOLEAN                         DisablePageFaultClustering;
		BOOLEAN                         DeadThread;
#if (VER_PRODUCTBUILD >= 2195)
		BOOLEAN                         HideFromDebugger;
#endif // (VER_PRODUCTBUILD >= 2195)
#if (VER_PRODUCTBUILD < 2195)
		BOOLEAN                         HasTerminated;
#else // (VER_PRODUCTBUILD >= 2195)
		ULONG                           HasTerminated;
#endif // (VER_PRODUCTBUILD >= 2195)
#if (VER_PRODUCTBUILD < 2195)
		PKEVENT_PAIR                    EventPair;
#endif // (VER_PRODUCTBUILD < 2195)
		ACCESS_MASK                     GrantedAccess;
		PEPROCESS                       ThreadsProcess;
		PKSTART_ROUTINE                 StartAddress;
		union {
			PVOID                       Win32StartAddress;
			ULONG                       LpcReceivedMessageId;
		};
		BOOLEAN                         LpcExitThreadCalled;
		BOOLEAN                         HardErrorsAreDisabled;
		BOOLEAN                         LpcReceivedMsgIdValid;
		BOOLEAN                         ActiveImpersonationInfo;
		ULONG                           PerformanceCountHigh;
#if (VER_PRODUCTBUILD >= 2195)
		LIST_ENTRY                      ThreadListEntry;
#endif // (VER_PRODUCTBUILD >= 2195)
	} ETHREAD, *PETHREAD;

#endif

	typedef struct _EPROCESS_QUOTA_ENTRY {
		ULONG Usage;
		ULONG Limit;
		ULONG Peak;
		ULONG Return;
	} EPROCESS_QUOTA_ENTRY, *PEPROCESS_QUOTA_ENTRY;

	typedef struct _EPROCESS_QUOTA_BLOCK {
		EPROCESS_QUOTA_ENTRY    QuotaEntry[3];
		LIST_ENTRY              QuotaList;
		ULONG                   ReferenceCount;
		ULONG                   ProcessCount;
	} EPROCESS_QUOTA_BLOCK, *PEPROCESS_QUOTA_BLOCK;

	typedef struct _EXCEPTION_REGISTRATION_RECORD {
		struct _EXCEPTION_REGISTRATION_RECORD    *Next;
		PVOID                                    Handler;
	} EXCEPTION_REGISTRATION_RECORD, *PEXCEPTION_REGISTRATION_RECORD;

	/*
	* When needing these parameters cast your PIO_STACK_LOCATION to
	* PEXTENDED_IO_STACK_LOCATION
	*/
#if !defined(_ALPHA_) && !defined(_AMD64_) && !defined(_IA64_)
#include <pshpack4.h>
#endif
	typedef struct _EXTENDED_IO_STACK_LOCATION {

		/* Included for padding */
		UCHAR MajorFunction;
		UCHAR MinorFunction;
		UCHAR Flags;
		UCHAR Control;

		union {

			struct {
				PIO_SECURITY_CONTEXT              SecurityContext;
				ULONG                             Options;
				USHORT                            Reserved;
				USHORT                            ShareAccess;
				PMAILSLOT_CREATE_PARAMETERS       Parameters;
			} CreateMailslot;

			struct {
				PIO_SECURITY_CONTEXT            SecurityContext;
				ULONG                           Options;
				USHORT                          Reserved;
				USHORT                          ShareAccess;
				PNAMED_PIPE_CREATE_PARAMETERS   Parameters;
			} CreatePipe;

			struct {
				ULONG                           OutputBufferLength;
				ULONG                           InputBufferLength;
				ULONG                           FsControlCode;
				PVOID                           Type3InputBuffer;
			} FileSystemControl;

			struct {
				PLARGE_INTEGER                  Length;
				ULONG                           Key;
				LARGE_INTEGER                   ByteOffset;
			} LockControl;

			struct {
				ULONG                           Length;
				ULONG                           CompletionFilter;
			} NotifyDirectory;

			struct {
				ULONG                           Length;
				PUNICODE_STRING                 FileName;
				FILE_INFORMATION_CLASS          FileInformationClass;
				ULONG                           FileIndex;
			} QueryDirectory;

			struct {
				ULONG                           Length;
				PVOID                           EaList;
				ULONG                           EaListLength;
				ULONG                           EaIndex;
			} QueryEa;

			struct {
				ULONG                           Length;
				PSID                            StartSid;
				PFILE_GET_QUOTA_INFORMATION     SidList;
				ULONG                           SidListLength;
			} QueryQuota;

			struct {
				ULONG                           Length;
			} SetEa;

			struct {
				ULONG                           Length;
			} SetQuota;

			struct {
				ULONG                           Length;
				FS_INFORMATION_CLASS            FsInformationClass;
			} SetVolume;

		} Parameters;

	} EXTENDED_IO_STACK_LOCATION, *PEXTENDED_IO_STACK_LOCATION;
#if !defined(_ALPHA_) && !defined(_AMD64_) && !defined(_IA64_)
#include <poppack.h>
#endif
#if (VER_PRODUCTBUILD < 7600)
	typedef struct _FILE_ACCESS_INFORMATION {
		ACCESS_MASK AccessFlags;
	} FILE_ACCESS_INFORMATION, *PFILE_ACCESS_INFORMATION;

	typedef struct _FILE_ALLOCATION_INFORMATION {
		LARGE_INTEGER AllocationSize;
	} FILE_ALLOCATION_INFORMATION, *PFILE_ALLOCATION_INFORMATION;

	typedef struct _FILE_BOTH_DIR_INFORMATION {
		ULONG           NextEntryOffset;
		ULONG           FileIndex;
		LARGE_INTEGER   CreationTime;
		LARGE_INTEGER   LastAccessTime;
		LARGE_INTEGER   LastWriteTime;
		LARGE_INTEGER   ChangeTime;
		LARGE_INTEGER   EndOfFile;
		LARGE_INTEGER   AllocationSize;
		ULONG           FileAttributes;
		ULONG           FileNameLength;
		ULONG           EaSize;
		CCHAR           ShortNameLength;
		WCHAR           ShortName[12];
		WCHAR           FileName[1];
	} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

	typedef struct _FILE_COMPLETION_INFORMATION {
		HANDLE  Port;
		ULONG   Key;
	} FILE_COMPLETION_INFORMATION, *PFILE_COMPLETION_INFORMATION;

	typedef struct _FILE_COMPRESSION_INFORMATION {
		LARGE_INTEGER   CompressedFileSize;
		USHORT          CompressionFormat;
		UCHAR           CompressionUnitShift;
		UCHAR           ChunkShift;
		UCHAR           ClusterShift;
		UCHAR           Reserved[3];
	} FILE_COMPRESSION_INFORMATION, *PFILE_COMPRESSION_INFORMATION;
#endif
	typedef struct _FILE_COPY_ON_WRITE_INFORMATION {
		BOOLEAN ReplaceIfExists;
		HANDLE  RootDirectory;
		ULONG   FileNameLength;
		WCHAR   FileName[1];
	} FILE_COPY_ON_WRITE_INFORMATION, *PFILE_COPY_ON_WRITE_INFORMATION;
#if (VER_PRODUCTBUILD < 7600)
	typedef struct _FILE_DIRECTORY_INFORMATION {
		ULONG           NextEntryOffset;
		ULONG           FileIndex;
		LARGE_INTEGER   CreationTime;
		LARGE_INTEGER   LastAccessTime;
		LARGE_INTEGER   LastWriteTime;
		LARGE_INTEGER   ChangeTime;
		LARGE_INTEGER   EndOfFile;
		LARGE_INTEGER   AllocationSize;
		ULONG           FileAttributes;
		ULONG           FileNameLength;
		WCHAR           FileName[1];
	} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;

	typedef struct _FILE_EA_INFORMATION {
		ULONG EaSize;
	} FILE_EA_INFORMATION, *PFILE_EA_INFORMATION;

	typedef struct _FILE_FS_ATTRIBUTE_INFORMATION {
		ULONG   FileSystemAttributes;
		ULONG   MaximumComponentNameLength;
		ULONG   FileSystemNameLength;
		WCHAR   FileSystemName[1];
	} FILE_FS_ATTRIBUTE_INFORMATION, *PFILE_FS_ATTRIBUTE_INFORMATION;

	typedef struct _FILE_FS_CONTROL_INFORMATION {
		LARGE_INTEGER   FreeSpaceStartFiltering;
		LARGE_INTEGER   FreeSpaceThreshold;
		LARGE_INTEGER   FreeSpaceStopFiltering;
		LARGE_INTEGER   DefaultQuotaThreshold;
		LARGE_INTEGER   DefaultQuotaLimit;
		ULONG           FileSystemControlFlags;
	} FILE_FS_CONTROL_INFORMATION, *PFILE_FS_CONTROL_INFORMATION;

	typedef struct _FILE_FS_FULL_SIZE_INFORMATION {
		LARGE_INTEGER   TotalAllocationUnits;
		LARGE_INTEGER   CallerAvailableAllocationUnits;
		LARGE_INTEGER   ActualAvailableAllocationUnits;
		ULONG           SectorsPerAllocationUnit;
		ULONG           BytesPerSector;
	} FILE_FS_FULL_SIZE_INFORMATION, *PFILE_FS_FULL_SIZE_INFORMATION;

	typedef struct _FILE_FS_LABEL_INFORMATION {
		ULONG VolumeLabelLength;
		WCHAR VolumeLabel[1];
	} FILE_FS_LABEL_INFORMATION, *PFILE_FS_LABEL_INFORMATION;
#endif

#if (VER_PRODUCTBUILD >= 2195)

	typedef struct _FILE_FS_OBJECT_ID_INFORMATION {
		UCHAR ObjectId[16];
		UCHAR ExtendedInfo[48];
	} FILE_FS_OBJECT_ID_INFORMATION, *PFILE_FS_OBJECT_ID_INFORMATION;

#endif // (VER_PRODUCTBUILD >= 2195)

#if (VER_PRODUCTBUILD < 7600)
	typedef struct _FILE_FS_SIZE_INFORMATION {
		LARGE_INTEGER   TotalAllocationUnits;
		LARGE_INTEGER   AvailableAllocationUnits;
		ULONG           SectorsPerAllocationUnit;
		ULONG           BytesPerSector;
	} FILE_FS_SIZE_INFORMATION, *PFILE_FS_SIZE_INFORMATION;


	typedef struct _FILE_FS_VOLUME_INFORMATION {
		LARGE_INTEGER   VolumeCreationTime;
		ULONG           VolumeSerialNumber;
		ULONG           VolumeLabelLength;
		BOOLEAN         SupportsObjects;
		WCHAR           VolumeLabel[1];
	} FILE_FS_VOLUME_INFORMATION, *PFILE_FS_VOLUME_INFORMATION;

typedef struct _FILE_FULL_DIR_INFORMATION {
		ULONG           NextEntryOffset;
		ULONG           FileIndex;
		LARGE_INTEGER   CreationTime;
		LARGE_INTEGER   LastAccessTime;
		LARGE_INTEGER   LastWriteTime;
		LARGE_INTEGER   ChangeTime;
		LARGE_INTEGER   EndOfFile;
		LARGE_INTEGER   AllocationSize;
		ULONG           FileAttributes;
		ULONG           FileNameLength;
		ULONG           EaSize;
		WCHAR           FileName[1];
	} FILE_FULL_DIR_INFORMATION, *PFILE_FULL_DIR_INFORMATION;

	typedef struct _FILE_GET_EA_INFORMATION {
		ULONG   NextEntryOffset;
		UCHAR   EaNameLength;
		CHAR    EaName[1];
	} FILE_GET_EA_INFORMATION, *PFILE_GET_EA_INFORMATION;

	typedef struct _FILE_GET_QUOTA_INFORMATION {
		ULONG   NextEntryOffset;
		ULONG   SidLength;
		SID     Sid;
	} FILE_GET_QUOTA_INFORMATION, *PFILE_GET_QUOTA_INFORMATION;

	typedef struct _FILE_ID_BOTH_DIR_INFORMATION {
		ULONG           NextEntryOffset;
		ULONG           FileIndex;
		LARGE_INTEGER   CreationTime;
		LARGE_INTEGER   LastAccessTime;
		LARGE_INTEGER   LastWriteTime;
		LARGE_INTEGER   ChangeTime;
		LARGE_INTEGER   EndOfFile;
		LARGE_INTEGER   AllocationSize;
		ULONG           FileAttributes;
		ULONG           FileNameLength;
		ULONG           EaSize;
		CCHAR           ShortNameLength;
		WCHAR           ShortName[12];
		LARGE_INTEGER   FileId;
		WCHAR           FileName[1];
	} FILE_ID_BOTH_DIR_INFORMATION, *PFILE_ID_BOTH_DIR_INFORMATION;

	typedef struct _FILE_ID_FULL_DIR_INFORMATION {
		ULONG           NextEntryOffset;
		ULONG           FileIndex;
		LARGE_INTEGER   CreationTime;
		LARGE_INTEGER   LastAccessTime;
		LARGE_INTEGER   LastWriteTime;
		LARGE_INTEGER   ChangeTime;
		LARGE_INTEGER   EndOfFile;
		LARGE_INTEGER   AllocationSize;
		ULONG           FileAttributes;
		ULONG           FileNameLength;
		ULONG           EaSize;
		LARGE_INTEGER   FileId;
		WCHAR           FileName[1];
	} FILE_ID_FULL_DIR_INFORMATION, *PFILE_ID_FULL_DIR_INFORMATION;

	typedef struct _FILE_INTERNAL_INFORMATION {
		LARGE_INTEGER IndexNumber;
	} FILE_INTERNAL_INFORMATION, *PFILE_INTERNAL_INFORMATION;

	typedef struct _FILE_LINK_INFORMATION {
		BOOLEAN ReplaceIfExists;
		HANDLE  RootDirectory;
		ULONG   FileNameLength;
		WCHAR   FileName[1];
	} FILE_LINK_INFORMATION, *PFILE_LINK_INFORMATION;

	typedef struct _FILE_LOCK_INFO {
		LARGE_INTEGER   StartingByte;
		LARGE_INTEGER   Length;
		BOOLEAN         ExclusiveLock;
		ULONG           Key;
		PFILE_OBJECT    FileObject;
		PEPROCESS       Process;
		LARGE_INTEGER   EndingByte;
	} FILE_LOCK_INFO, *PFILE_LOCK_INFO;

	// raw internal file lock struct returned from FsRtlGetNextFileLock
	typedef struct _FILE_SHARED_LOCK_ENTRY {
		PVOID           Unknown1;
		PVOID           Unknown2;
		FILE_LOCK_INFO  FileLock;
	} FILE_SHARED_LOCK_ENTRY, *PFILE_SHARED_LOCK_ENTRY;

	// raw internal file lock struct returned from FsRtlGetNextFileLock
	typedef struct _FILE_EXCLUSIVE_LOCK_ENTRY {
		LIST_ENTRY      ListEntry;
		PVOID           Unknown1;
		PVOID           Unknown2;
		FILE_LOCK_INFO  FileLock;
	} FILE_EXCLUSIVE_LOCK_ENTRY, *PFILE_EXCLUSIVE_LOCK_ENTRY;

	typedef NTSTATUS (*PCOMPLETE_LOCK_IRP_ROUTINE) (
		__in PVOID    Context,
		__in PIRP     Irp
		);

	typedef VOID (*PUNLOCK_ROUTINE) (
		__in PVOID            Context,
		__in PFILE_LOCK_INFO  FileLockInfo
		);

	typedef struct _FILE_LOCK {
		PCOMPLETE_LOCK_IRP_ROUTINE  CompleteLockIrpRoutine;
		PUNLOCK_ROUTINE             UnlockRoutine;
		BOOLEAN                     FastIoIsQuestionable;
		BOOLEAN                     Pad[3];
		PVOID                       LockInformation;
		FILE_LOCK_INFO              LastReturnedLockInfo;
		PVOID                       LastReturnedLock;
	} FILE_LOCK, *PFILE_LOCK;

	typedef struct _FILE_MAILSLOT_PEEK_BUFFER {
		ULONG ReadDataAvailable;
		ULONG NumberOfMessages;
		ULONG MessageLength;
	} FILE_MAILSLOT_PEEK_BUFFER, *PFILE_MAILSLOT_PEEK_BUFFER;

	typedef struct _FILE_MAILSLOT_QUERY_INFORMATION {
		ULONG           MaximumMessageSize;
		ULONG           MailslotQuota;
		ULONG           NextMessageSize;
		ULONG           MessagesAvailable;
		LARGE_INTEGER   ReadTimeout;
	} FILE_MAILSLOT_QUERY_INFORMATION, *PFILE_MAILSLOT_QUERY_INFORMATION;

	typedef struct _FILE_MAILSLOT_SET_INFORMATION {
		PLARGE_INTEGER ReadTimeout;
	} FILE_MAILSLOT_SET_INFORMATION, *PFILE_MAILSLOT_SET_INFORMATION;

	typedef struct _FILE_MODE_INFORMATION {
		ULONG Mode;
	} FILE_MODE_INFORMATION, *PFILE_MODE_INFORMATION;
#endif

	// This structure is included __in the Windows 2000 DDK but is missing __in the
	// Windows NT 4.0 DDK
#if (VER_PRODUCTBUILD < 2195)
	typedef struct _FILE_NAME_INFORMATION {
		ULONG FileNameLength;
		WCHAR FileName[1];
	} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;
#endif // (VER_PRODUCTBUILD < 2195)

#if (VER_PRODUCTBUILD < 7600)
	typedef struct _FILE_ALL_INFORMATION {
		FILE_BASIC_INFORMATION      BasicInformation;
		FILE_STANDARD_INFORMATION   StandardInformation;
		FILE_INTERNAL_INFORMATION   InternalInformation;
		FILE_EA_INFORMATION         EaInformation;
		FILE_ACCESS_INFORMATION     AccessInformation;
		FILE_POSITION_INFORMATION   PositionInformation;
		FILE_MODE_INFORMATION       ModeInformation;
		FILE_ALIGNMENT_INFORMATION  AlignmentInformation;
		FILE_NAME_INFORMATION       NameInformation;
	} FILE_ALL_INFORMATION, *PFILE_ALL_INFORMATION;

	typedef struct _FILE_NAMES_INFORMATION {
		ULONG NextEntryOffset;
		ULONG FileIndex;
		ULONG FileNameLength;
		WCHAR FileName[1];
	} FILE_NAMES_INFORMATION, *PFILE_NAMES_INFORMATION;

	typedef struct _FILE_NOTIFY_INFORMATION {
		ULONG NextEntryOffset;
		ULONG Action;
		ULONG FileNameLength;
		WCHAR FileName[1];
	} FILE_NOTIFY_INFORMATION, *PFILE_NOTIFY_INFORMATION;

	typedef struct _FILE_OBJECTID_INFORMATION {
		LONGLONG        FileReference;
		UCHAR           ObjectId[16];
		union {
			struct {
				UCHAR   BirthVolumeId[16];
				UCHAR   BirthObjectId[16];
				UCHAR   DomainId[16];
			} ;
			UCHAR       ExtendedInfo[48];
		};
	} FILE_OBJECTID_INFORMATION, *PFILE_OBJECTID_INFORMATION;

	typedef struct _FILE_OLE_CLASSID_INFORMATION {
		GUID ClassId;
	} FILE_OLE_CLASSID_INFORMATION, *PFILE_OLE_CLASSID_INFORMATION;

	typedef struct _FILE_OLE_ALL_INFORMATION {
		FILE_BASIC_INFORMATION          BasicInformation;
		FILE_STANDARD_INFORMATION       StandardInformation;
		FILE_INTERNAL_INFORMATION       InternalInformation;
		FILE_EA_INFORMATION             EaInformation;
		FILE_ACCESS_INFORMATION         AccessInformation;
		FILE_POSITION_INFORMATION       PositionInformation;
		FILE_MODE_INFORMATION           ModeInformation;
		FILE_ALIGNMENT_INFORMATION      AlignmentInformation;
		USN                             LastChangeUsn;
		USN                             ReplicationUsn;
		LARGE_INTEGER                   SecurityChangeTime;
		FILE_OLE_CLASSID_INFORMATION    OleClassIdInformation;
		FILE_OBJECTID_INFORMATION       ObjectIdInformation;
		FILE_STORAGE_TYPE               StorageType;
		ULONG                           OleStateBits;
		ULONG                           OleId;
		ULONG                           NumberOfStreamReferences;
		ULONG                           StreamIndex;
		ULONG                           SecurityId;
		BOOLEAN                         ContentIndexDisable;
		BOOLEAN                         InheritContentIndexDisable;
		FILE_NAME_INFORMATION           NameInformation;
	} FILE_OLE_ALL_INFORMATION, *PFILE_OLE_ALL_INFORMATION;

	typedef struct _FILE_OLE_DIR_INFORMATION {
		ULONG               NextEntryOffset;
		ULONG               FileIndex;
		LARGE_INTEGER       CreationTime;
		LARGE_INTEGER       LastAccessTime;
		LARGE_INTEGER       LastWriteTime;
		LARGE_INTEGER       ChangeTime;
		LARGE_INTEGER       EndOfFile;
		LARGE_INTEGER       AllocationSize;
		ULONG               FileAttributes;
		ULONG               FileNameLength;
		FILE_STORAGE_TYPE   StorageType;
		GUID                OleClassId;
		ULONG               OleStateBits;
		BOOLEAN             ContentIndexDisable;
		BOOLEAN             InheritContentIndexDisable;
		WCHAR               FileName[1];
	} FILE_OLE_DIR_INFORMATION, *PFILE_OLE_DIR_INFORMATION;

	typedef struct _FILE_OLE_INFORMATION {
		LARGE_INTEGER                   SecurityChangeTime;
		FILE_OLE_CLASSID_INFORMATION    OleClassIdInformation;
		FILE_OBJECTID_INFORMATION       ObjectIdInformation;
		FILE_STORAGE_TYPE               StorageType;
		ULONG                           OleStateBits;
		BOOLEAN                         ContentIndexDisable;
		BOOLEAN                         InheritContentIndexDisable;
	} FILE_OLE_INFORMATION, *PFILE_OLE_INFORMATION;

	typedef struct _FILE_OLE_STATE_BITS_INFORMATION {
		ULONG StateBits;
		ULONG StateBitsMask;
	} FILE_OLE_STATE_BITS_INFORMATION, *PFILE_OLE_STATE_BITS_INFORMATION;

	typedef struct _FILE_PIPE_ASSIGN_EVENT_BUFFER {
		HANDLE  EventHandle;
		ULONG   KeyValue;
	} FILE_PIPE_ASSIGN_EVENT_BUFFER, *PFILE_PIPE_ASSIGN_EVENT_BUFFER;

	typedef struct _FILE_PIPE_CLIENT_PROCESS_BUFFER {
		PVOID ClientSession;
		PVOID ClientProcess;
	} FILE_PIPE_CLIENT_PROCESS_BUFFER, *PFILE_PIPE_CLIENT_PROCESS_BUFFER;

	typedef struct _FILE_PIPE_EVENT_BUFFER {
		ULONG NamedPipeState;
		ULONG EntryType;
		ULONG ByteCount;
		ULONG KeyValue;
		ULONG NumberRequests;
	} FILE_PIPE_EVENT_BUFFER, *PFILE_PIPE_EVENT_BUFFER;

	typedef struct _FILE_PIPE_INFORMATION {
		ULONG ReadMode;
		ULONG CompletionMode;
	} FILE_PIPE_INFORMATION, *PFILE_PIPE_INFORMATION;

	typedef struct _FILE_PIPE_LOCAL_INFORMATION {
		ULONG NamedPipeType;
		ULONG NamedPipeConfiguration;
		ULONG MaximumInstances;
		ULONG CurrentInstances;
		ULONG InboundQuota;
		ULONG ReadDataAvailable;
		ULONG OutboundQuota;
		ULONG WriteQuotaAvailable;
		ULONG NamedPipeState;
		ULONG NamedPipeEnd;
	} FILE_PIPE_LOCAL_INFORMATION, *PFILE_PIPE_LOCAL_INFORMATION;

	typedef struct _FILE_PIPE_PEEK_BUFFER {
		ULONG   NamedPipeState;
		ULONG   ReadDataAvailable;
		ULONG   NumberOfMessages;
		ULONG   MessageLength;
		CHAR    Data[1];
	} FILE_PIPE_PEEK_BUFFER, *PFILE_PIPE_PEEK_BUFFER;

	typedef struct _FILE_PIPE_REMOTE_INFORMATION {
		LARGE_INTEGER   CollectDataTime;
		ULONG           MaximumCollectionCount;
	} FILE_PIPE_REMOTE_INFORMATION, *PFILE_PIPE_REMOTE_INFORMATION;

	typedef struct _FILE_PIPE_WAIT_FOR_BUFFER {
		LARGE_INTEGER   Timeout;
		ULONG           NameLength;
		BOOLEAN         TimeoutSpecified;
		WCHAR           Name[1];
	} FILE_PIPE_WAIT_FOR_BUFFER, *PFILE_PIPE_WAIT_FOR_BUFFER;

	typedef struct _FILE_QUOTA_INFORMATION {
		ULONG           NextEntryOffset;
		ULONG           SidLength;
		LARGE_INTEGER   ChangeTime;
		LARGE_INTEGER   QuotaUsed;
		LARGE_INTEGER   QuotaThreshold;
		LARGE_INTEGER   QuotaLimit;
		SID             Sid;
	} FILE_QUOTA_INFORMATION, *PFILE_QUOTA_INFORMATION;

	typedef struct _FILE_RENAME_INFORMATION {
		BOOLEAN ReplaceIfExists;
		HANDLE  RootDirectory;
		ULONG   FileNameLength;
		WCHAR   FileName[1];
	} FILE_RENAME_INFORMATION, *PFILE_RENAME_INFORMATION;

	typedef struct _FILE_STREAM_INFORMATION {
		ULONG           NextEntryOffset;
		ULONG           StreamNameLength;
		LARGE_INTEGER   StreamSize;
		LARGE_INTEGER   StreamAllocationSize;
		WCHAR           StreamName[1];
	} FILE_STREAM_INFORMATION, *PFILE_STREAM_INFORMATION;

	typedef struct _FILE_TRACKING_INFORMATION {
		HANDLE  DestinationFile;
		ULONG   ObjectInformationLength;
		CHAR    ObjectInformation[1];
	} FILE_TRACKING_INFORMATION, *PFILE_TRACKING_INFORMATION;

	typedef struct _FSRTL_COMMON_FCB_HEADER {
		CSHORT          NodeTypeCode;
		CSHORT          NodeByteSize;
		UCHAR           Flags;
		UCHAR           IsFastIoPossible;
#if (VER_PRODUCTBUILD >= 1381)
		UCHAR           Flags2;
		UCHAR           Reserved : 4;
		UCHAR           Version : 4;
#endif // (VER_PRODUCTBUILD >= 1381)
		PERESOURCE      Resource;
		PERESOURCE      PagingIoResource;
		LARGE_INTEGER   AllocationSize;
		LARGE_INTEGER   FileSize;
		LARGE_INTEGER   ValidDataLength;
	} FSRTL_COMMON_FCB_HEADER, *PFSRTL_COMMON_FCB_HEADER;
#endif

#if (VER_PRODUCTBUILD >= 2600 && VER_PRODUCTBUILD < 7600)

#ifdef __cplusplus
	typedef struct _FSRTL_ADVANCED_FCB_HEADER:FSRTL_COMMON_FCB_HEADER {
#else   // __cplusplus
	typedef struct _FSRTL_ADVANCED_FCB_HEADER {
		FSRTL_COMMON_FCB_HEADER;
#endif  // __cplusplus
		PFAST_MUTEX     FastMutex;
		LIST_ENTRY      FilterContexts;
		EX_PUSH_LOCK    PushLock;
		PVOID           *FileContextSupportPointer;
	} FSRTL_ADVANCED_FCB_HEADER, *PFSRTL_ADVANCED_FCB_HEADER;

#endif // (VER_PRODUCTBUILD >= 2600)

#if (VER_PRODUCTBUILD < 7600)
	typedef struct _GENERATE_NAME_CONTEXT {
		USHORT  Checksum;
		BOOLEAN CheckSumInserted;
		UCHAR   NameLength;
		WCHAR   NameBuffer[8];
		ULONG   ExtensionLength;
		WCHAR   ExtensionBuffer[4];
		ULONG   LastIndexValue;
	} GENERATE_NAME_CONTEXT, *PGENERATE_NAME_CONTEXT;
#endif 

	typedef struct _HANDLE_INFO {       // Information about open handles
		union {
			PEPROCESS   Process;        // Pointer to PEPROCESS owning the Handle
			ULONG       Count;          // Count of HANDLE_INFO structures following this structure
		} HandleInfo;
		USHORT          HandleCount;
	} HANDLE_INFO, *PHANDLE_INFO;

	typedef struct _HANDLE_TABLE_ENTRY_INFO {
		ULONG AuditMask;
	} HANDLE_TABLE_ENTRY_INFO, *PHANDLE_TABLE_ENTRY_INFO;

	typedef struct _HANDLE_TABLE_ENTRY {
		union {
			PVOID                       Object;
			ULONG                       ObAttributes;
			PHANDLE_TABLE_ENTRY_INFO    InfoTable;
			ULONG                       Value;
		};
		union {
			ULONG                       GrantedAccess;
			USHORT                      GrantedAccessIndex;
			LONG                        NextFreeTableEntry;
		};
		USHORT                          CreatorBackTraceIndex;
	} HANDLE_TABLE_ENTRY, *PHANDLE_TABLE_ENTRY;

	typedef struct _MAPPING_PAIR {
		ULONGLONG Vcn;
		ULONGLONG Lcn;
	} MAPPING_PAIR, *PMAPPING_PAIR;

	typedef struct _GET_RETRIEVAL_DESCRIPTOR {
		ULONG           NumberOfPairs;
		ULONGLONG       StartVcn;
		MAPPING_PAIR    Pair[1];
	} GET_RETRIEVAL_DESCRIPTOR, *PGET_RETRIEVAL_DESCRIPTOR;

	typedef struct _INITIAL_TEB {
		ULONG Unknown_1;
		ULONG Unknown_2;
		PVOID StackTop;
		PVOID StackBase;
		PVOID Unknown_3;
	} INITIAL_TEB, *PINITIAL_TEB;

	typedef struct _IO_CLIENT_EXTENSION {
		struct _IO_CLIENT_EXTENSION *NextExtension;
		PVOID                       ClientIdentificationAddress;
	} IO_CLIENT_EXTENSION, *PIO_CLIENT_EXTENSION;

	typedef struct _IO_COMPLETION_BASIC_INFORMATION {
		LONG Depth;
	} IO_COMPLETION_BASIC_INFORMATION, *PIO_COMPLETION_BASIC_INFORMATION;

	typedef struct _KEVENT_PAIR {
		USHORT Type;
		USHORT Size;
		KEVENT Event1;
		KEVENT Event2;
	} KEVENT_PAIR, *PKEVENT_PAIR;

	typedef struct _KINTERRUPT {
		CSHORT              Type;
		CSHORT              Size;
		LIST_ENTRY          InterruptListEntry;
		PKSERVICE_ROUTINE   ServiceRoutine;
		PVOID               ServiceContext;
		KSPIN_LOCK          SpinLock;
		ULONG               TickCount;
		PKSPIN_LOCK         ActualLock;
		PVOID               DispatchAddress;
		ULONG               Vector;
		KIRQL               Irql;
		KIRQL               SynchronizeIrql;
		BOOLEAN             FloatingSave;
		BOOLEAN             Connected;
		CHAR                Number;
		UCHAR               ShareVector;
		KINTERRUPT_MODE     Mode;
		ULONG               ServiceCount;
		ULONG               DispatchCount;
		ULONG               DispatchCode[106];
	} KINTERRUPT, *PKINTERRUPT;

#if (VER_PRODUCTBUILD < 7600)
	typedef struct _KQUEUE {
		DISPATCHER_HEADER   Header;
		LIST_ENTRY          EntryListHead;
		ULONG               CurrentCount;
		ULONG               MaximumCount;
		LIST_ENTRY          ThreadListHead;
	} KQUEUE, *PKQUEUE, *RESTRICTED_POINTER PRKQUEUE;

	typedef struct _LARGE_MCB {
		PFAST_MUTEX FastMutex;
		ULONG       MaximumPairCount;
		ULONG       PairCount;
		POOL_TYPE   PoolType;
		PVOID       Mapping;
	} LARGE_MCB, *PLARGE_MCB;

	typedef struct _LPC_MESSAGE {
		USHORT      DataSize;
		USHORT      MessageSize;
		USHORT      MessageType;
		USHORT      VirtualRangesOffset;
		CLIENT_ID   ClientId;
		ULONG       MessageId;
		ULONG       SectionSize;
		UCHAR       Data[1];
	} LPC_MESSAGE, *PLPC_MESSAGE;

	typedef struct _LPC_SECTION_READ {
		ULONG Length;
		ULONG ViewSize;
		PVOID ViewBase;
	} LPC_SECTION_READ, *PLPC_SECTION_READ;

	typedef struct _LPC_SECTION_WRITE {
		ULONG   Length;
		HANDLE  SectionHandle;
		ULONG   SectionOffset;
		ULONG   ViewSize;
		PVOID   ViewBase;
		PVOID   TargetViewBase;
	} LPC_SECTION_WRITE, *PLPC_SECTION_WRITE;

	typedef struct _MAILSLOT_CREATE_PARAMETERS {
		ULONG           MailslotQuota;
		ULONG           MaximumMessageSize;
		LARGE_INTEGER   ReadTimeout;
		BOOLEAN         TimeoutSpecified;
	} MAILSLOT_CREATE_PARAMETERS, *PMAILSLOT_CREATE_PARAMETERS;

	typedef struct _MBCB {
		CSHORT          NodeTypeCode;
		CSHORT          NodeIsInZone;
		ULONG           PagesToWrite;
		ULONG           DirtyPages;
		ULONG           Reserved;
		LIST_ENTRY      BitmapRanges;
		LONGLONG        ResumeWritePage;
		BITMAP_RANGE    BitmapRange1;
		BITMAP_RANGE    BitmapRange2;
		BITMAP_RANGE    BitmapRange3;
	} MBCB, *PMBCB;

	typedef struct _MCB {
		LARGE_MCB LargeMcb;
	} MCB, *PMCB;
#endif

	typedef struct _MOVEFILE_DESCRIPTOR {
		HANDLE         FileHandle; 
		ULONG          Reserved;   
		LARGE_INTEGER  StartVcn; 
		LARGE_INTEGER  TargetLcn;
		ULONG          NumVcns; 
		ULONG          Reserved1;  
	} MOVEFILE_DESCRIPTOR, *PMOVEFILE_DESCRIPTOR;

	typedef struct _NAMED_PIPE_CREATE_PARAMETERS {
		ULONG           NamedPipeType;
		ULONG           ReadMode;
		ULONG           CompletionMode;
		ULONG           MaximumInstances;
		ULONG           InboundQuota;
		ULONG           OutboundQuota;
		LARGE_INTEGER   DefaultTimeout;
		BOOLEAN         TimeoutSpecified;
	} NAMED_PIPE_CREATE_PARAMETERS, *PNAMED_PIPE_CREATE_PARAMETERS;

	typedef struct _QUOTA_BLOCK {
		KSPIN_LOCK  QuotaLock;
		ULONG       ReferenceCount; // Number of processes using this block
		ULONG       PeakNonPagedPoolUsage;
		ULONG       PeakPagedPoolUsage;
		ULONG       NonPagedpoolUsage;
		ULONG       PagedPoolUsage;
		ULONG       NonPagedPoolLimit;
		ULONG       PagedPoolLimit;
		ULONG       PeakPagefileUsage;
		ULONG       PagefileUsage;
		ULONG       PageFileLimit;
	} QUOTA_BLOCK, *PQUOTA_BLOCK;

	typedef struct _OBJECT_BASIC_INFO {
		ULONG           Attributes;
		ACCESS_MASK     GrantedAccess;
		ULONG           HandleCount;
		ULONG           ReferenceCount;
		ULONG           PagedPoolUsage;
		ULONG           NonPagedPoolUsage;
		ULONG           Reserved[3];
		ULONG           NameInformationLength;
		ULONG           TypeInformationLength;
		ULONG           SecurityDescriptorLength;
		LARGE_INTEGER   CreateTime;
	} OBJECT_BASIC_INFO, *POBJECT_BASIC_INFO;

	typedef struct _OBJECT_CREATE_INFORMATION {
		ULONG                           Attributes;
		HANDLE                          RootDirectory; // 0x4
		PVOID                           ParseContext; // 0x8
		KPROCESSOR_MODE                 ProbeMode; // 0xc
		ULONG                           PagedPoolCharge; // 0x10
		ULONG                           NonPagedPoolCharge; // 0x14
		ULONG                           SecurityDescriptorCharge; // 0x18
		PSECURITY_DESCRIPTOR            SecurityDescriptor; // 0x1c
		PSECURITY_QUALITY_OF_SERVICE    SecurityQos; // 0x20
		SECURITY_QUALITY_OF_SERVICE     SecurityQualityOfService; // 0x24
	} OBJECT_CREATE_INFORMATION, *POBJECT_CREATE_INFORMATION;

	typedef struct _OBJECT_CREATOR_INFO {
		LIST_ENTRY  Creator;
		ULONG       UniqueProcessId; // Creator's Process ID
		ULONG       Reserved; // Alignment
	} OBJECT_CREATOR_INFO, *POBJECT_CREATOR_INFO;

	typedef struct _OBJECT_DIRECTORY_ITEM {
		struct _OBJECT_DIRECTORY_ITEM   *Next;
		PVOID                           Object;
	} OBJECT_DIRECTORY_ITEM, *POBJECT_DIRECTORY_ITEM;

	typedef struct _OBJECT_DIRECTORY {
		POBJECT_DIRECTORY_ITEM  HashEntries[0x25];
		POBJECT_DIRECTORY_ITEM  LastHashAccess;
		ULONG                   LastHashResult;
	} OBJECT_DIRECTORY, *POBJECT_DIRECTORY;

	typedef struct _OBJECT_HANDLE_ATTRIBUTE_INFO {
		BOOLEAN Inherit;
		BOOLEAN ProtectFromClose;
	} OBJECT_HANDLE_ATTRIBUTE_INFO, *POBJECT_HANDLE_ATTRIBUTE_INFO;

	typedef struct _OBJECT_HANDLE_DB {
		union {
			struct _EPROCESS                *Process;
			struct _OBJECT_HANDLE_DB_LIST   *HandleDBList;
		};
		ULONG                               HandleCount;
	} OBJECT_HANDLE_DB, *POBJECT_HANDLE_DB;

	typedef struct _OBJECT_HANDLE_DB_LIST {
		ULONG               Count;
		OBJECT_HANDLE_DB    Entries[1];
	} OBJECT_HANDLE_DB_LIST, *POBJECT_HANDLE_DB_LIST;

	typedef struct _OBJECT_HEADER_FLAGS {
		ULONG NameInfoOffset     : 8;
		ULONG HandleInfoOffset   : 8;
		ULONG QuotaInfoOffset    : 8;
		ULONG QuotaBlock         : 1;   // QuotaBlock/ObjectInfo
		ULONG KernelMode         : 1;   // UserMode/KernelMode
		ULONG CreatorInfo        : 1;
		ULONG Exclusive          : 1;
		ULONG Permanent          : 1;
		ULONG SecurityDescriptor : 1;
		ULONG HandleInfo         : 1;
		ULONG Reserved           : 1;
	} OBJECT_HEADER_FLAGS, *POBJECT_HEADER_FLAGS;

	typedef struct _OBJECT_HEADER {
		ULONG                           ReferenceCount;
		union {
			ULONG                       HandleCount;
			PSINGLE_LIST_ENTRY          NextToFree;
		}; // 0x4
		POBJECT_TYPE                    ObjectType; // 0x8
		OBJECT_HEADER_FLAGS             Flags; // 0xc
		union {
			POBJECT_CREATE_INFORMATION  ObjectCreateInfo;
			PQUOTA_BLOCK                QuotaBlock;
		}; // 0x10
		PSECURITY_DESCRIPTOR            SecurityDescriptor; // 0x14
		QUAD                            Body; // 0x18
	} OBJECT_HEADER, *POBJECT_HEADER;

	typedef struct _OBJECT_NAME {
		POBJECT_DIRECTORY   Directory; 
		UNICODE_STRING      ObjectName;
		ULONG               Reserved;
	} OBJECT_NAME, *POBJECT_NAME;

	typedef struct _OBJECT_NAME_INFO {
		UNICODE_STRING  ObjectName;
		WCHAR           ObjectNameBuffer[1];
	} OBJECT_NAME_INFO, *POBJECT_NAME_INFO;

	typedef struct _OBJECT_PROTECTION_INFO {
		BOOLEAN Inherit;
		BOOLEAN ProtectHandle;
	} OBJECT_PROTECTION_INFO, *POBJECT_PROTECTION_INFO;

	typedef struct _OBJECT_QUOTA_CHARGES {
		ULONG PagedPoolCharge;
		ULONG NonPagedPoolCharge;
		ULONG SecurityCharge;
		ULONG Reserved;
	} OBJECT_QUOTA_CHARGES, *POBJECT_QUOTA_CHARGES;

	typedef struct _OBJECT_QUOTA_INFO {
		ULONG       PagedPoolQuota;
		ULONG       NonPagedPoolQuota;
		ULONG       QuotaInformationSize;
		PEPROCESS   Process; // Owning process
	} OBJECT_QUOTA_INFO, *POBJECT_QUOTA_INFO;

	typedef struct _OBJECT_TYPE_INITIALIZER {
		USHORT          Length;
		BOOLEAN         UseDefaultObject;
		BOOLEAN         Reserved1;
		ULONG           InvalidAttributes;
		GENERIC_MAPPING GenericMapping;
		ACCESS_MASK     ValidAccessMask;
		BOOLEAN         SecurityRequired;
		BOOLEAN         MaintainHandleCount;  /* OBJECT_HANDLE_DB */
		BOOLEAN         MaintainTypeList;     /* OBJECT_CREATOR_INFO */
		UCHAR           Reserved2;
		BOOLEAN         PagedPool;
		ULONG           DefaultPagedPoolCharge;
		ULONG           DefaultNonPagedPoolCharge;
		PVOID           DumpProcedure;
		PVOID           OpenProcedure;
		PVOID           CloseProcedure;
		PVOID           DeleteProcedure;
		PVOID           ParseProcedure;
		PVOID           SecurityProcedure;    /* SeDefaultObjectMethod */
		PVOID           QueryNameProcedure;
		PVOID           OkayToCloseProcedure;
	} OBJECT_TYPE_INITIALIZER, *POBJECT_TYPE_INITIALIZER;

	typedef struct _OBJECT_TYPE {
		ERESOURCE               Lock;
		LIST_ENTRY              ObjectListHead; /* OBJECT_CREATOR_INFO */
		UNICODE_STRING          ObjectTypeName;
		union {
			PVOID               DefaultObject;  /* ObpDefaultObject */
			ULONG               Code; /* File: 5C, WaitablePort: A0 */
		};
		ULONG                   ObjectTypeIndex; /* OB_TYPE_INDEX_* */
		ULONG                   ObjectCount;
		ULONG                   HandleCount;
		ULONG                   PeakObjectCount;
		ULONG                   PeakHandleCount;
		OBJECT_TYPE_INITIALIZER TypeInfo;
		ULONG                   ObjectTypeTag;   /* OB_TYPE_TAG_* */
	} OBJECT_TYPE, *POBJECT_TYPE;

	typedef struct _OBJECT_TYPE_INFO {
		UNICODE_STRING  ObjectTypeName;
		UCHAR           Unknown[0x58];
		WCHAR           ObjectTypeNameBuffer[1];
	} OBJECT_TYPE_INFO, *POBJECT_TYPE_INFO;

	typedef struct _OBJECT_ALL_TYPES_INFO {
		ULONG               NumberOfObjectTypes;
		OBJECT_TYPE_INFO    ObjectsTypeInfo[1];
	} OBJECT_ALL_TYPES_INFO, *POBJECT_ALL_TYPES_INFO;

	typedef struct _PAGEFAULT_HISTORY {
		ULONG                           CurrentIndex;
		ULONG                           MaxIndex;
		KSPIN_LOCK                      SpinLock;
		PVOID                           Reserved;
		PROCESS_WS_WATCH_INFORMATION    WatchInfo[1];
	} PAGEFAULT_HISTORY, *PPAGEFAULT_HISTORY;

#if (VER_PRODUCTBUILD < 7600)
	typedef struct _PATHNAME_BUFFER {
		ULONG PathNameLength;
		WCHAR Name[1];
	} PATHNAME_BUFFER, *PPATHNAME_BUFFER;
#endif 

#if (VER_PRODUCTBUILD >= 2600)

	typedef struct _PRIVATE_CACHE_MAP_FLAGS {
		ULONG DontUse           : 16;
		ULONG ReadAheadActive   : 1;
		ULONG ReadAheadEnabled  : 1;
		ULONG Available         : 14;
	} PRIVATE_CACHE_MAP_FLAGS, *PPRIVATE_CACHE_MAP_FLAGS;

	typedef struct _PRIVATE_CACHE_MAP {
		union {
			CSHORT                  NodeTypeCode;
			PRIVATE_CACHE_MAP_FLAGS Flags;
			ULONG                   UlongFlags;
		};
		ULONG                       ReadAheadMask;
		PFILE_OBJECT                FileObject;
		LARGE_INTEGER               FileOffset1;
		LARGE_INTEGER               BeyondLastByte1;
		LARGE_INTEGER               FileOffset2;
		LARGE_INTEGER               BeyondLastByte2;
		LARGE_INTEGER               ReadAheadOffset[2];
		ULONG                       ReadAheadLength[2];
		KSPIN_LOCK                  ReadAheadSpinLock;
		LIST_ENTRY                  PrivateLinks;
	} PRIVATE_CACHE_MAP, *PPRIVATE_CACHE_MAP;

#endif

	typedef struct _PROCESS_PRIORITY_CLASS {
		BOOLEAN Foreground;
		UCHAR   PriorityClass;
	} PROCESS_PRIORITY_CLASS, *PPROCESS_PRIORITY_CLASS;

	typedef struct _PS_IMPERSONATION_INFORMATION {
		PACCESS_TOKEN                   Token;
		BOOLEAN                         CopyOnOpen;
		BOOLEAN                         EffectiveOnly;
		SECURITY_IMPERSONATION_LEVEL    ImpersonationLevel;
	} PS_IMPERSONATION_INFORMATION, *PPS_IMPERSONATION_INFORMATION;

#if (VER_PRODUCTBUILD < 7600)
	typedef struct _PUBLIC_BCB {
		CSHORT          NodeTypeCode;
		CSHORT          NodeByteSize;
		ULONG           MappedLength;
		LARGE_INTEGER   MappedFileOffset;
	} PUBLIC_BCB, *PPUBLIC_BCB;

	typedef struct _QUERY_PATH_REQUEST {
		ULONG                   PathNameLength;
		PIO_SECURITY_CONTEXT    SecurityContext;
		WCHAR                   FilePathName[1];
	} QUERY_PATH_REQUEST, *PQUERY_PATH_REQUEST;

	typedef struct _QUERY_PATH_RESPONSE {
		ULONG LengthAccepted;
	} QUERY_PATH_RESPONSE, *PQUERY_PATH_RESPONSE;
#endif 

#if (VER_PRODUCTBUILD >= 2600 && VER_PRODUCTBUILD < 7600)

	typedef struct _READ_LIST {
		PFILE_OBJECT            FileObject;
		ULONG                   NumberOfEntries;
		LOGICAL                 IsImage;
		FILE_SEGMENT_ELEMENT    List[ANYSIZE_ARRAY];
	} READ_LIST, *PREAD_LIST;

#endif // (VER_PRODUCTBUILD >= 2600)

#if (VER_PRODUCTBUILD < 7600)
	typedef struct _REPARSE_DATA_BUFFER {

		ULONG  ReparseTag;
		USHORT ReparseDataLength;
		USHORT Reserved;

		union {

			struct {
				USHORT  SubstituteNameOffset;
				USHORT  SubstituteNameLength;
				USHORT  PrintNameOffset;
				USHORT  PrintNameLength;
				WCHAR   PathBuffer[1];
			} SymbolicLinkReparseBuffer;

			struct {
				USHORT  SubstituteNameOffset;
				USHORT  SubstituteNameLength;
				USHORT  PrintNameOffset;
				USHORT  PrintNameLength;
				WCHAR   PathBuffer[1];
			} MountPointReparseBuffer;

			struct {
				UCHAR   DataBuffer[1];
			} GenericReparseBuffer;
		};

	} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;

	typedef struct _RETRIEVAL_POINTERS_BUFFER {
		ULONG               ExtentCount;
		LARGE_INTEGER       StartingVcn;
		struct {
			LARGE_INTEGER   NextVcn;
			LARGE_INTEGER   Lcn;
		} Extents[1];
	} RETRIEVAL_POINTERS_BUFFER, *PRETRIEVAL_POINTERS_BUFFER;

	typedef struct _RTL_SPLAY_LINKS {
		struct _RTL_SPLAY_LINKS *Parent;
		struct _RTL_SPLAY_LINKS *LeftChild;
		struct _RTL_SPLAY_LINKS *RightChild;
	} RTL_SPLAY_LINKS, *PRTL_SPLAY_LINKS;

	typedef struct _SE_EXPORTS {

		LUID    SeCreateTokenPrivilege;
		LUID    SeAssignPrimaryTokenPrivilege;
		LUID    SeLockMemoryPrivilege;
		LUID    SeIncreaseQuotaPrivilege;
		LUID    SeUnsolicitedInputPrivilege;
		LUID    SeTcbPrivilege;
		LUID    SeSecurityPrivilege;
		LUID    SeTakeOwnershipPrivilege;
		LUID    SeLoadDriverPrivilege;
		LUID    SeCreatePagefilePrivilege;
		LUID    SeIncreaseBasePriorityPrivilege;
		LUID    SeSystemProfilePrivilege;
		LUID    SeSystemtimePrivilege;
		LUID    SeProfileSingleProcessPrivilege;
		LUID    SeCreatePermanentPrivilege;
		LUID    SeBackupPrivilege;
		LUID    SeRestorePrivilege;
		LUID    SeShutdownPrivilege;
		LUID    SeDebugPrivilege;
		LUID    SeAuditPrivilege;
		LUID    SeSystemEnvironmentPrivilege;
		LUID    SeChangeNotifyPrivilege;
		LUID    SeRemoteShutdownPrivilege;

		PSID    SeNullSid;
		PSID    SeWorldSid;
		PSID    SeLocalSid;
		PSID    SeCreatorOwnerSid;
		PSID    SeCreatorGroupSid;

		PSID    SeNtAuthoritySid;
		PSID    SeDialupSid;
		PSID    SeNetworkSid;
		PSID    SeBatchSid;
		PSID    SeInteractiveSid;
		PSID    SeLocalSystemSid;
		PSID    SeAliasAdminsSid;
		PSID    SeAliasUsersSid;
		PSID    SeAliasGuestsSid;
		PSID    SeAliasPowerUsersSid;
		PSID    SeAliasAccountOpsSid;
		PSID    SeAliasSystemOpsSid;
		PSID    SeAliasPrintOpsSid;
		PSID    SeAliasBackupOpsSid;

		PSID    SeAuthenticatedUsersSid;

		PSID    SeRestrictedSid;
		PSID    SeAnonymousLogonSid;

		LUID    SeUndockPrivilege;
		LUID    SeSyncAgentPrivilege;
		LUID    SeEnableDelegationPrivilege;

	} SE_EXPORTS, *PSE_EXPORTS;
#endif

	typedef struct _SECTION_BASIC_INFORMATION {
		PVOID           BaseAddress;
		ULONG           Attributes;
		LARGE_INTEGER   Size;
	} SECTION_BASIC_INFORMATION, *PSECTION_BASIC_INFORMATION;

	typedef struct _SECTION_IMAGE_INFORMATION {
		PVOID   EntryPoint;
		ULONG   Unknown1;
		ULONG   StackReserve;
		ULONG   StackCommit;
		ULONG   Subsystem;
		USHORT  MinorSubsystemVersion;
		USHORT  MajorSubsystemVersion;
		ULONG   Unknown2;
		ULONG   Characteristics;
		USHORT  ImageNumber;
		BOOLEAN Executable;
		UCHAR   Unknown3;
		ULONG   Unknown4[3];
	} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;

	typedef struct _SECTION_OBJECT {
		PVOID                   StartingVa;
		PVOID                   EndingVa;
		struct _SECTION_OBJECT  *Parent;
		struct _SECTION_OBJECT  *LeftChild;
		struct _SECTION_OBJECT  *RightChild;
		PVOID                   Segment;
	} SECTION_OBJECT, *PSECTION_OBJECT;

	typedef struct _SEP_AUDIT_POLICY {
		// _SEP_AUDIT_POLICY_CATEGORIES
		ULONGLONG System                    : 4;
		ULONGLONG Logon                     : 4;
		ULONGLONG ObjectAccess              : 4;
		ULONGLONG PrivilegeUse              : 4;
		ULONGLONG DetailedTracking          : 4;
		ULONGLONG PolicyChange              : 4;
		ULONGLONG AccountManagement         : 4;
		ULONGLONG DirectoryServiceAccess    : 4;
		ULONGLONG AccountLogon              : 4;
		// _SEP_AUDIT_POLICY_OVERLAY
		ULONGLONG SetBit                    : 1;
	} SEP_AUDIT_POLICY, *PSEP_AUDIT_POLICY;

	/* size 0x1C */
	typedef struct _SEP_AUDIT_POLICY_VISTA {
		UCHAR   PerUserPolicy[25];  /* +0x000 */
		UCHAR   PolicySetStatus;    /* +0x019 */
		USHORT  Alignment;          /* +0x01A */
	} SEP_AUDIT_POLICY_VISTA, *PSEP_AUDIT_POLICY_VISTA;

	typedef struct _SERVICE_DESCRIPTOR_TABLE {
		/*
		* Table containing cServices elements of pointers to service handler
		* functions, indexed by service ID.
		*/
		PVOID   *ServiceTable;
		/*
		* Table that counts how many times each service is used. This table
		* is only updated __in checked builds.
		*/
		PULONG  CounterTable;
		/*
		* Number of services contained __in this table.
		*/
		ULONG   TableSize;
		/*
		* Table containing the number of bytes of parameters the handler
		* function takes.
		*/
		PUCHAR  ArgumentTable;
	} SERVICE_DESCRIPTOR_TABLE, *PSERVICE_DESCRIPTOR_TABLE;

#if (VER_PRODUCTBUILD >= 2600 && VER_PRODUCTBUILD < 7600)

	typedef struct _SHARED_CACHE_MAP {
		CSHORT                      NodeTypeCode;
		CSHORT                      NodeByteSize;
		ULONG                       OpenCount;
		LARGE_INTEGER               FileSize;
		LIST_ENTRY                  BcbList;
		LARGE_INTEGER               SectionSize;
		LARGE_INTEGER               ValidDataLength;
		LARGE_INTEGER               ValidDataGoal;
		PVACB                       InitialVacbs[4];
		PVACB                       *Vacbs;
		PFILE_OBJECT                FileObject;
		PVACB                       ActiveVacb;
		PVOID                       NeedToZero;
		ULONG                       ActivePage;
		ULONG                       NeedToZeroPage;
		KSPIN_LOCK                  ActiveVacbSpinLock;
		ULONG                       VacbActiveCount;
		ULONG                       DirtyPages;
		LIST_ENTRY                  SharedCacheMapLinks;
		ULONG                       Flags;
		NTSTATUS                    Status;
		PMBCB                       Mbcb;
		PVOID                       Section;
		PKEVENT                     CreateEvent;
		PKEVENT                     WaitOnActiveCount;
		ULONG                       PagesToWrite;
		LONGLONG                    BeyondLastFlush;
		PCACHE_MANAGER_CALLBACKS    Callbacks;
		PVOID                       LazyWriteContext;
		LIST_ENTRY                  PrivateList;
		PVOID                       LogHandle;
		PVOID                       FlushToLsnRoutine;
		ULONG                       DirtyPageThreshold;
		ULONG                       LazyWritePassCount;
		PCACHE_UNINITIALIZE_EVENT   UninitializeEvent;
		PVACB                       NeedToZeroVacb;
		KSPIN_LOCK                  BcbSpinLock;
		PVOID                       Reserved;
		KEVENT                      Event;
		EX_PUSH_LOCK                VacbPushLock;
		PRIVATE_CACHE_MAP           PrivateCacheMap;
	} SHARED_CACHE_MAP, *PSHARED_CACHE_MAP;

#endif

#if (VER_PRODUCTBUILD < 7600)
	typedef struct _SID_AND_ATTRIBUTES {
		PSID    Sid;
		ULONG   Attributes;
	} SID_AND_ATTRIBUTES, *PSID_AND_ATTRIBUTES;

	typedef struct _SID_AND_ATTRIBUTES_HASH {
		ULONG               SidCount;   /* +0x000 */
		PSID_AND_ATTRIBUTES SidAttr;    /* +0x004 */
		ULONG               Hash[32];   /* +0x008 */
	} SID_AND_ATTRIBUTES_HASH, *PSID_AND_ATTRIBUTES_HASH;

	typedef struct _STARTING_VCN_INPUT_BUFFER {
		LARGE_INTEGER StartingVcn;
	} STARTING_VCN_INPUT_BUFFER, *PSTARTING_VCN_INPUT_BUFFER;
#endif

	// SystemBasicInformation
	typedef struct _SYSTEM_BASIC_INFORMATION {
		ULONG Unknown;
		ULONG MaximumIncrement;
		ULONG PhysicalPageSize;
		ULONG NumberOfPhysicalPages;
		ULONG LowestPhysicalPage;
		ULONG HighestPhysicalPage;
		ULONG AllocationGranularity;
		ULONG LowestUserAddress;
		ULONG HighestUserAddress;
		ULONG ActiveProcessors;
		UCHAR NumberProcessors;
	} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

	// SystemProcessorInformation
	typedef struct _SYSTEM_PROCESSOR_INFORMATION {
		USHORT  ProcessorArchitecture;
		USHORT  ProcessorLevel;
		USHORT  ProcessorRevision;
		USHORT  Unknown;
		ULONG   FeatureBits;
	} SYSTEM_PROCESSOR_INFORMATION, *PSYSTEM_PROCESSOR_INFORMATION;

	// SystemPerformanceInformation
	typedef struct _SYSTEM_PERFORMANCE_INFORMATION {
		LARGE_INTEGER   IdleTime;
		LARGE_INTEGER   ReadTransferCount;
		LARGE_INTEGER   WriteTransferCount;
		LARGE_INTEGER   OtherTransferCount;
		ULONG           ReadOperationCount;
		ULONG           WriteOperationCount;
		ULONG           OtherOperationCount;
		ULONG           AvailablePages;
		ULONG           TotalCommittedPages;
		ULONG           TotalCommitLimit;
		ULONG           PeakCommitment;
		ULONG           PageFaults;
		ULONG           WriteCopyFaults;
		ULONG           TransistionFaults;
		ULONG           Reserved1;
		ULONG           DemandZeroFaults;
		ULONG           PagesRead;
		ULONG           PageReadIos;
		ULONG           Reserved2[2];
		ULONG           PagefilePagesWritten;
		ULONG           PagefilePageWriteIos;
		ULONG           MappedFilePagesWritten;
		ULONG           MappedFilePageWriteIos;
		ULONG           PagedPoolUsage;
		ULONG           NonPagedPoolUsage;
		ULONG           PagedPoolAllocs;
		ULONG           PagedPoolFrees;
		ULONG           NonPagedPoolAllocs;
		ULONG           NonPagedPoolFrees;
		ULONG           TotalFreeSystemPtes;
		ULONG           SystemCodePage;
		ULONG           TotalSystemDriverPages;
		ULONG           TotalSystemCodePages;
		ULONG           SmallNonPagedLookasideListAllocateHits;
		ULONG           SmallPagedLookasideListAllocateHits;
		ULONG           Reserved3;
		ULONG           MmSystemCachePage;
		ULONG           PagedPoolPage;
		ULONG           SystemDriverPage;
		ULONG           FastReadNoWait;
		ULONG           FastReadWait;
		ULONG           FastReadResourceMiss;
		ULONG           FastReadNotPossible;
		ULONG           FastMdlReadNoWait;
		ULONG           FastMdlReadWait;
		ULONG           FastMdlReadResourceMiss;
		ULONG           FastMdlReadNotPossible;
		ULONG           MapDataNoWait;
		ULONG           MapDataWait;
		ULONG           MapDataNoWaitMiss;
		ULONG           MapDataWaitMiss;
		ULONG           PinMappedDataCount;
		ULONG           PinReadNoWait;
		ULONG           PinReadWait;
		ULONG           PinReadNoWaitMiss;
		ULONG           PinReadWaitMiss;
		ULONG           CopyReadNoWait;
		ULONG           CopyReadWait;
		ULONG           CopyReadNoWaitMiss;
		ULONG           CopyReadWaitMiss;
		ULONG           MdlReadNoWait;
		ULONG           MdlReadWait;
		ULONG           MdlReadNoWaitMiss;
		ULONG           MdlReadWaitMiss;
		ULONG           ReadAheadIos;
		ULONG           LazyWriteIos;
		ULONG           LazyWritePages;
		ULONG           DataFlushes;
		ULONG           DataPages;
		ULONG           ContextSwitches;
		ULONG           FirstLevelTbFills;
		ULONG           SecondLevelTbFills;
		ULONG           SystemCalls;
	} SYSTEM_PERFORMANCE_INFORMATION, *PSYSTEM_PERFORMANCE_INFORMATION;

	// SystemTimeOfDayInformation
	typedef struct _SYSTEM_TIME_OF_DAY_INFORMATION {
		LARGE_INTEGER   BootTime;
		LARGE_INTEGER   CurrentTime;
		LARGE_INTEGER   TimeZoneBias;
		ULONG           CurrentTimeZoneId;
	} SYSTEM_TIME_OF_DAY_INFORMATION, *PSYSTEM_TIME_OF_DAY_INFORMATION;

	typedef struct _SYSTEM_THREADS_INFORMATION {
		LARGE_INTEGER   KernelTime;
		LARGE_INTEGER   UserTime;
		LARGE_INTEGER   CreateTime;
		ULONG           WaitTime;
		PVOID           StartAddress;
		CLIENT_ID       ClientId;
		KPRIORITY       Priority;
		KPRIORITY       BasePriority;
		ULONG           ContextSwitchCount;
		THREAD_STATE    State;
		KWAIT_REASON    WaitReason;
	} SYSTEM_THREADS_INFORMATION, *PSYSTEM_THREADS_INFORMATION;

	// SystemProcessesAndThreadsInformation
	typedef struct _SYSTEM_PROCESSES_INFORMATION {
		ULONG                       NextEntryDelta;
		ULONG                       ThreadCount;
		ULONG                       Reserved1[6];
		LARGE_INTEGER               CreateTime;
		LARGE_INTEGER               UserTime;
		LARGE_INTEGER               KernelTime;
		UNICODE_STRING              ProcessName;
		KPRIORITY                   BasePriority;
		ULONG                       ProcessId;
		ULONG                       InheritedFromProcessId;
		ULONG                       HandleCount;
		ULONG                       SessionId;
		ULONG                       Reserved2;
		VM_COUNTERS                 VmCounters;
#if (VER_PRODUCTBUILD >= 2195)
		IO_COUNTERS                 IoCounters;
#endif // (VER_PRODUCTBUILD >= 2195)
		SYSTEM_THREADS_INFORMATION  Threads[1];
	} SYSTEM_PROCESSES_INFORMATION, *PSYSTEM_PROCESSES_INFORMATION;

	// SystemCallCounts
	typedef struct _SYSTEM_CALL_COUNTS {
		ULONG Size;
		ULONG NumberOfDescriptorTables;
		ULONG NumberOfRoutinesInTable[1];
		// On checked build this is followed by a ULONG CallCounts[1] variable length array.
	} SYSTEM_CALL_COUNTS, *PSYSTEM_CALL_COUNTS;

	// SystemConfigurationInformation
	typedef struct _SYSTEM_CONFIGURATION_INFORMATION {
		ULONG DiskCount;
		ULONG FloppyCount;
		ULONG CdRomCount;
		ULONG TapeCount;
		ULONG SerialCount;
		ULONG ParallelCount;
	} SYSTEM_CONFIGURATION_INFORMATION, *PSYSTEM_CONFIGURATION_INFORMATION;

	// SystemProcessorTimes
	typedef struct _SYSTEM_PROCESSOR_TIMES {
		LARGE_INTEGER   IdleTime;
		LARGE_INTEGER   KernelTime;
		LARGE_INTEGER   UserTime;
		LARGE_INTEGER   DpcTime;
		LARGE_INTEGER   InterruptTime;
		ULONG           InterruptCount;
	} SYSTEM_PROCESSOR_TIMES, *PSYSTEM_PROCESSOR_TIMES;

	// SystemGlobalFlag
	typedef struct _SYSTEM_GLOBAL_FLAG {
		ULONG GlobalFlag;
	} SYSTEM_GLOBAL_FLAG, *PSYSTEM_GLOBAL_FLAG;

	// SystemModuleInformation
	typedef struct _SYSTEM_MODULE_INFORMATION {
		ULONG   Reserved[2];
		PVOID   Base;
		ULONG   Size;
		ULONG   Flags;
		USHORT  Index;
		USHORT  Unknown;
		USHORT  LoadCount;
		USHORT  ModuleNameOffset;
		CHAR    ImageName[256];
	} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

	// SystemLockInformation
	typedef struct _SYSTEM_LOCK_INFORMATION {
		PVOID   Address;
		USHORT  Type;
		USHORT  Reserved1;
		ULONG   ExclusiveOwnerThreadId;
		ULONG   ActiveCount;
		ULONG   ContentionCount;
		ULONG   Reserved2[2];
		ULONG   NumberOfSharedWaiters;
		ULONG   NumberOfExclusiveWaiters;
	} SYSTEM_LOCK_INFORMATION, *PSYSTEM_LOCK_INFORMATION;

	// SystemHandleInformation
	typedef struct _SYSTEM_HANDLE_INFORMATION {
		ULONG       ProcessId;
		UCHAR       ObjectTypeNumber;
		UCHAR       Flags;
		USHORT      Handle;
		PVOID       Object;
		ACCESS_MASK GrantedAccess;
	} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

	// SystemObjectInformation
	typedef struct _SYSTEM_OBJECT_TYPE_INFORMATION {
		ULONG           NextEntryOffset;
		ULONG           ObjectCount;
		ULONG           HandleCount;
		ULONG           TypeNumber;
		ULONG           InvalidAttributes;
		GENERIC_MAPPING GenericMapping;
		ACCESS_MASK     ValidAccessMask;
		POOL_TYPE       PoolType;
		UCHAR           Unknown;
		UNICODE_STRING  Name;
	} SYSTEM_OBJECT_TYPE_INFORMATION, *PSYSTEM_OBJECT_TYPE_INFORMATION;

	typedef struct _SYSTEM_OBJECT_INFORMATION {
		ULONG                   NextEntryOffset;
		PVOID                   Object;
		ULONG                   CreatorProcessId;
		USHORT                  Unknown;
		USHORT                  Flags;
		ULONG                   PointerCount;
		ULONG                   HandleCount;
		ULONG                   PagedPoolUsage;
		ULONG                   NonPagedPoolUsage;
		ULONG                   ExclusiveProcessId;
		PSECURITY_DESCRIPTOR    SecurityDescriptor;
		UNICODE_STRING          Name;
	} SYSTEM_OBJECT_INFORMATION, *PSYSTEM_OBJECT_INFORMATION;

	// SystemPagefileInformation
	typedef struct _SYSTEM_PAGEFILE_INFORMATION {
		ULONG           NextEntryOffset;
		ULONG           CurrentSize;
		ULONG           TotalUsed;
		ULONG           PeakUsed;
		UNICODE_STRING  FileName;
	} SYSTEM_PAGEFILE_INFORMATION, *PSYSTEM_PAGEFILE_INFORMATION;

	// SystemInstructionEmulationCounts
	typedef struct _SYSTEM_INSTRUCTION_EMULATION_COUNTS {
		ULONG GenericInvalidOpcode;
		ULONG TwoByteOpcode;
		ULONG ESprefix;
		ULONG CSprefix;
		ULONG SSprefix;
		ULONG DSprefix;
		ULONG FSPrefix;
		ULONG GSprefix;
		ULONG OPER32prefix;
		ULONG ADDR32prefix;
		ULONG INSB;
		ULONG INSW;
		ULONG OUTSB;
		ULONG OUTSW;
		ULONG PUSHFD;
		ULONG POPFD;
		ULONG INTnn;
		ULONG INTO;
		ULONG IRETD;
		ULONG FloatingPointOpcode;
		ULONG INBimm;
		ULONG INWimm;
		ULONG OUTBimm;
		ULONG OUTWimm;
		ULONG INB;
		ULONG INW;
		ULONG OUTB;
		ULONG OUTW;
		ULONG LOCKprefix;
		ULONG REPNEprefix;
		ULONG REPprefix;
		ULONG CLI;
		ULONG STI;
		ULONG HLT;
	} SYSTEM_INSTRUCTION_EMULATION_COUNTS, *PSYSTEM_INSTRUCTION_EMULATION_COUNTS;

	// SystemCacheInformation
	typedef struct _SYSTEM_CACHE_INFORMATION {
		ULONG SystemCacheWsSize;
		ULONG SystemCacheWsPeakSize;
		ULONG SystemCacheWsFaults;
		ULONG SystemCacheWsMinimum;
		ULONG SystemCacheWsMaximum;
		ULONG TransitionSharedPages;
		ULONG TransitionSharedPagesPeak;
		ULONG Reserved[2];
	} SYSTEM_CACHE_INFORMATION, *PSYSTEM_CACHE_INFORMATION;

	// SystemPoolTagInformation
	typedef struct _SYSTEM_POOL_TAG_INFORMATION {
		CHAR    Tag[4];
		ULONG   PagedPoolAllocs;
		ULONG   PagedPoolFrees;
		ULONG   PagedPoolUsage;
		ULONG   NonPagedPoolAllocs;
		ULONG   NonPagedPoolFrees;
		ULONG   NonPagedPoolUsage;
	} SYSTEM_POOL_TAG_INFORMATION, *PSYSTEM_POOL_TAG_INFORMATION;

	// SystemProcessorStatistics
	typedef struct _SYSTEM_PROCESSOR_STATISTICS {
		ULONG ContextSwitches;
		ULONG DpcCount;
		ULONG DpcRequestRate;
		ULONG TimeIncrement;
		ULONG DpcBypassCount;
		ULONG ApcBypassCount;
	} SYSTEM_PROCESSOR_STATISTICS, *PSYSTEM_PROCESSOR_STATISTICS;

	// SystemDpcInformation
	typedef struct _SYSTEM_DPC_INFORMATION {
		ULONG Reserved;
		ULONG MaximumDpcQueueDepth;
		ULONG MinimumDpcRate;
		ULONG AdjustDpcThreshold;
		ULONG IdealDpcRate;
	} SYSTEM_DPC_INFORMATION, *PSYSTEM_DPC_INFORMATION;

	// SystemLoadImage
	typedef struct _SYSTEM_LOAD_IMAGE {
		UNICODE_STRING  ModuleName;
		PVOID           ModuleBase;
		PVOID           Unknown;
		PVOID           EntryPoint;
		PVOID           ExportDirectory;
	} SYSTEM_LOAD_IMAGE, *PSYSTEM_LOAD_IMAGE;

	// SystemUnloadImage
	typedef struct _SYSTEM_UNLOAD_IMAGE {
		PVOID ModuleBase;
	} SYSTEM_UNLOAD_IMAGE, *PSYSTEM_UNLOAD_IMAGE;

	// SystemTimeAdjustment
	typedef struct _SYSTEM_QUERY_TIME_ADJUSTMENT {
		ULONG   TimeAdjustment;
		ULONG   MaximumIncrement;
		BOOLEAN TimeSynchronization;
	} SYSTEM_QUERY_TIME_ADJUSTMENT, *PSYSTEM_QUERY_TIME_ADJUSTMENT;

	// SystemTimeAdjustment
	typedef struct _SYSTEM_SET_TIME_ADJUSTMENT {
		ULONG   TimeAdjustment;
		BOOLEAN TimeSynchronization;
	} SYSTEM_SET_TIME_ADJUSTMENT, *PSYSTEM_SET_TIME_ADJUSTMENT;

	// SystemCrashDumpInformation
	typedef struct _SYSTEM_CRASH_DUMP_INFORMATION {
		HANDLE CrashDumpSectionHandle;
#if (VER_PRODUCTBUILD >= 2195)
		HANDLE Unknown;
#endif // (VER_PRODUCTBUILD >= 2195)
	} SYSTEM_CRASH_DUMP_INFORMATION, *PSYSTEM_CRASH_DUMP_INFORMATION;

	// SystemExceptionInformation
	typedef struct _SYSTEM_EXCEPTION_INFORMATION {
		ULONG AlignmentFixupCount;
		ULONG ExceptionDispatchCount;
		ULONG FloatingEmulationCount;
		ULONG Reserved;
	} SYSTEM_EXCEPTION_INFORMATION, *PSYSTEM_EXCEPTION_INFORMATION;

	// SystemCrashDumpStateInformation
	typedef struct _SYSTEM_CRASH_DUMP_STATE_INFORMATION {
		ULONG ValidCrashDump;
#if (VER_PRODUCTBUILD >= 2195)
		ULONG Unknown;
#endif // (VER_PRODUCTBUILD >= 2195)
	} SYSTEM_CRASH_DUMP_STATE_INFORMATION, *PSYSTEM_CRASH_DUMP_STATE_INFORMATION;

	// SystemKernelDebuggerInformation
	typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION {
		BOOLEAN DebuggerEnabled;
		BOOLEAN DebuggerNotPresent;
	} SYSTEM_KERNEL_DEBUGGER_INFORMATION, *PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

	// SystemContextSwitchInformation
	typedef struct _SYSTEM_CONTEXT_SWITCH_INFORMATION {
		ULONG ContextSwitches;
		ULONG ContextSwitchCounters[11];
	} SYSTEM_CONTEXT_SWITCH_INFORMATION, *PSYSTEM_CONTEXT_SWITCH_INFORMATION;

	// SystemRegistryQuotaInformation
	typedef struct _SYSTEM_REGISTRY_QUOTA_INFORMATION {
		ULONG RegistryQuota;
		ULONG RegistryQuotaInUse;
		ULONG PagedPoolSize;
	} SYSTEM_REGISTRY_QUOTA_INFORMATION, *PSYSTEM_REGISTRY_QUOTA_INFORMATION;

	// SystemLoadAndCallImage
	typedef struct _SYSTEM_LOAD_AND_CALL_IMAGE {
		UNICODE_STRING ModuleName;
	} SYSTEM_LOAD_AND_CALL_IMAGE, *PSYSTEM_LOAD_AND_CALL_IMAGE;

	// SystemPrioritySeparation
	typedef struct _SYSTEM_PRIORITY_SEPARATION {
		ULONG PrioritySeparation;
	} SYSTEM_PRIORITY_SEPARATION, *PSYSTEM_PRIORITY_SEPARATION;

	// SystemTimeZoneInformation
	typedef struct _SYSTEM_TIME_ZONE_INFORMATION {
		LONG        Bias;
		WCHAR       StandardName[32];
		TIME_FIELDS StandardDate;
		LONG        StandardBias;
		WCHAR       DaylightName[32];
		TIME_FIELDS DaylightDate;
		LONG        DaylightBias;
	} SYSTEM_TIME_ZONE_INFORMATION, *PSYSTEM_TIME_ZONE_INFORMATION;

	// SystemLookasideInformation
	typedef struct _SYSTEM_LOOKASIDE_INFORMATION {
		USHORT      Depth;
		USHORT      MaximumDepth;
		ULONG       TotalAllocates;
		ULONG       AllocateMisses;
		ULONG       TotalFrees;
		ULONG       FreeMisses;
		POOL_TYPE   Type;
		ULONG       Tag;
		ULONG       Size;
	} SYSTEM_LOOKASIDE_INFORMATION, *PSYSTEM_LOOKASIDE_INFORMATION;

	// SystemSetTimeSlipEvent
	typedef struct _SYSTEM_SET_TIME_SLIP_EVENT {
		HANDLE TimeSlipEvent;
	} SYSTEM_SET_TIME_SLIP_EVENT, *PSYSTEM_SET_TIME_SLIP_EVENT;

	// SystemCreateSession
	typedef struct _SYSTEM_CREATE_SESSION {
		ULONG Session;
	} SYSTEM_CREATE_SESSION, *PSYSTEM_CREATE_SESSION;

	// SystemDeleteSession
	typedef struct _SYSTEM_DELETE_SESSION {
		ULONG Session;
	} SYSTEM_DELETE_SESSION, *PSYSTEM_DELETE_SESSION;

	// SystemRangeStartInformation
	typedef struct _SYSTEM_RANGE_START_INFORMATION {
		PVOID SystemRangeStart;
	} SYSTEM_RANGE_START_INFORMATION, *PSYSTEM_RANGE_START_INFORMATION;

	// SystemSessionProcessesInformation
	typedef struct _SYSTEM_SESSION_PROCESS_INFORMATION {
		ULONG SessionId;
		ULONG BufferSize;
		PVOID Buffer;
	} SYSTEM_SESSION_PROCESS_INFORMATION, *PSYSTEM_SESSION_PROCESS_INFORMATION;

	typedef struct _GDI_TEB_BATCH {
		ULONG Offset;
		ULONG HDC;
		ULONG Buffer[(VER_PRODUCTBUILD >= 2195) ? 0x133 : 0x136];
	} GDI_TEB_BATCH, *PGDI_TEB_BATCH;

#if (VER_PRODUCTBUILD >= 2600)

	typedef struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME {
		struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME* Previous;
		struct _ACTIVATION_CONTEXT*                 ActivationContext; // 0x4
		ULONG                                       Flags; // 0x8
	} RTL_ACTIVATION_CONTEXT_STACK_FRAME, *PRTL_ACTIVATION_CONTEXT_STACK_FRAME;

	typedef struct _ACTIVATION_CONTEXT_STACK {
		ULONG                               Flags;
		ULONG                               NextCookieSequenceNumber;
		PRTL_ACTIVATION_CONTEXT_STACK_FRAME ActiveFrame; // 0x8
		LIST_ENTRY                          FrameListCache; // 0xc
	} ACTIVATION_CONTEXT_STACK, *PACTIVATION_CONTEXT_STACK;

#endif // (VER_PRODUCTBUILD >= 2600)

	typedef struct _Wx86ThreadState {
		PULONG  CallBx86Eip;
		PVOID   DeallocationCpu;
		UCHAR   UseKnownWx86Dll; // 0x8
		UCHAR   OleStubInvoked; // 0x9
	} Wx86ThreadState, *PWx86ThreadState;

	typedef struct _TEB_ACTIVE_FRAME_CONTEXT {
		ULONG Flags;
		PCHAR FrameName;
	} TEB_ACTIVE_FRAME_CONTEXT, *PTEB_ACTIVE_FRAME_CONTEXT;

	typedef struct _TEB_ACTIVE_FRAME {
		ULONG                       Flags;
		struct _TEB_ACTIVE_FRAME    *Previous;
		PTEB_ACTIVE_FRAME_CONTEXT   Context;
	} TEB_ACTIVE_FRAME, *PTEB_ACTIVE_FRAME;

	typedef struct _TEB // from Reactos, Native API; checked and corrected for 2003 and nt 4.0
		// should also work on XP and 2000
		// the reactos version was probably from NT 3.51 SP3
	{
		NT_TIB Tib;                         /* 00h */
		PVOID EnvironmentPointer;           /* 1Ch */
		CLIENT_ID Cid;                      /* 20h */
		HANDLE RpcHandle;                   /* 28h */
		PVOID *ThreadLocalStorage;          /* 2Ch */
		PPEB Peb;                           /* 30h */
		ULONG LastErrorValue;               /* 34h */
		ULONG CountOfOwnedCriticalSections; /* 38h */
		PVOID CsrClientThread;              /* 3Ch */
		struct _W32THREAD* Win32ThreadInfo; /* 40h */    
		ULONG User32Reserved[26];           /* 44h */
		ULONG UserReserved[5];              /* ACh */
		PVOID WOW32Reserved;                /* C0h */
		LCID CurrentLocale;                 /* C4h */
		ULONG FpSoftwareStatusRegister;     /* C8h */
		PVOID SystemReserved1[0x36];        /* CCh */
#if (VER_PRODUCTBUILD <= 1381)
		PVOID Spare1;                       /* 1A4h */
#endif
		LONG ExceptionCode;                 /* 1A4h */
#if (VER_PRODUCTBUILD >= 2600)
		ACTIVATION_CONTEXT_STACK 
			ActivationContextStack;        /* 1A8h */
		UCHAR SpareBytes1[24];              /* 1BCh */
#elif (VER_PRODUCTBUILD >= 2195)
		UCHAR SpareBytes1[0x2c];            /* 1A8h */
#else /* nt 4.0 */
		ULONG SpareBytes1[0x14];            /* 1ACh */
#endif
		GDI_TEB_BATCH GdiTebBatch;          /* 1D4h */ /* 1FC for nt 4.0 */
		ULONG gdiRgn;                       /* 6A8h */ /* 6DCh for nt 4.0 */
		ULONG gdiPen;                       /* 6ACh */
		ULONG gdiBrush;                     /* 6B0h */
		CLIENT_ID RealClientId;             /* 6B4h */ /* 6E8h for nt 4.0 */
		PVOID GdiCachedProcessHandle;       /* 6BCh */
		ULONG GdiClientPID;                 /* 6C0h */
		ULONG GdiClientTID;                 /* 6C4h */
		PVOID GdiThreadLocaleInfo;          /* 6C8h */
#if (VER_PRODUCTBUILD == 1381)
		PVOID Win32ClientInfo[5];           /* 700h */
		PVOID glDispatchTable[0x118];       /* 714h */
		ULONG glReserved1[0x1a];            /* B74h */   
#else
		PVOID Win32ClientInfo[0x3e];        /* 6CCh */
		PVOID glDispatchTable[0xe9];        /* 7C4h */
		ULONG glReserved1[0x1d];            /* B68h */
#endif
		PVOID glReserved2;                  /* BDCh */
		PVOID glSectionInfo;                /* BE0h */
		PVOID glSection;                    /* BE4h */
		PVOID glTable;                      /* BE8h */
		PVOID glCurrentRC;                  /* BECh */
		PVOID glContext;                    /* BF0h */
		NTSTATUS LastStatusValue;           /* BF4h */
		UNICODE_STRING StaticUnicodeString; /* BF8h */
		WCHAR StaticUnicodeBuffer[0x105];   /* C00h */
		PVOID DeallocationStack;            /* E0Ch */
		PVOID TlsSlots[0x40];               /* E10h */
		LIST_ENTRY TlsLinks;                /* F10h */
		PVOID Vdm;                          /* F18h */
		PVOID ReservedForNtRpc;             /* F1Ch */
		PVOID DbgSsReserved[0x2];           /* F20h */
		ULONG HardErrorDisabled;            /* F28h */
		PVOID Instrumentation[0x10];        /* F2Ch */
		PVOID WinSockData;                  /* F6Ch */
		ULONG GdiBatchCount;                /* F70h */
		BOOLEAN InDbgPrint;                 /* F74h */
		BOOLEAN FreeStackOnTermination;     /* F75h */
		BOOLEAN HasFiberData;               /* F76h */
		UCHAR IdealProcessor;               /* F77h */
		ULONG Spare3;                       /* F78h */
		ULONG ReservedForPerf;              /* F7Ch */
		PVOID ReservedForOle;               /* F80h */
		ULONG WaitingOnLoaderLock;          /* F84h */
#if (VER_PRODUCTBUILD >= 2195)
		Wx86ThreadState Wx86Thread;         /* F88h */
		PVOID* TlsExpansionSlots;           /* F94h */
		ULONG ImpersonationLocale;          /* F98h */
		ULONG IsImpersonating;              /* F9Ch */
		PVOID NlsCache;                     /* FA0h */
		PVOID pShimData;                    /* FA4h */
		ULONG HeapVirtualAffinity;          /* FA8h */
		PVOID CurrentTransactionHandle;     /* FACh */
		PTEB_ACTIVE_FRAME ActiveFrame;      /* FB0h*/
		PVOID FlsSlots;                     /* FB4h */
#endif
	} TEB, *PTEB;

	typedef struct _TERMINATION_PORT {
		struct _TERMINATION_PORT*   Next;
		PVOID                       Port;
	} TERMINATION_PORT, *PTERMINATION_PORT;

	typedef struct _THREAD_BASIC_INFORMATION {
		NTSTATUS    ExitStatus;
		PVOID       TebBaseAddress;
		ULONG       UniqueProcessId;
		ULONG       UniqueThreadId;
		KAFFINITY   AffinityMask;
		KPRIORITY   BasePriority;
		ULONG       DiffProcessPriority;
	} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

#if (VER_PRODUCTBUILD < 7600)
	typedef struct _TOKEN_SOURCE {
		CCHAR   SourceName[TOKEN_SOURCE_LENGTH];
		LUID    SourceIdentifier;
	} TOKEN_SOURCE, *PTOKEN_SOURCE;

	typedef struct _TOKEN_CONTROL {
		LUID            TokenId;
		LUID            AuthenticationId;
		LUID            ModifiedId;
		TOKEN_SOURCE    TokenSource;
	} TOKEN_CONTROL, *PTOKEN_CONTROL;

	typedef struct _TOKEN_DEFAULT_DACL {
		PACL DefaultDacl;
	} TOKEN_DEFAULT_DACL, *PTOKEN_DEFAULT_DACL;

	typedef struct _TOKEN_GROUPS {
		ULONG               GroupCount;
		SID_AND_ATTRIBUTES  Groups[1];
	} TOKEN_GROUPS, *PTOKEN_GROUPS;

	/* XP SP2 has same TOKEN_OBJECT structure as Windows Server 2003 (stucture K23 __in union). */
#include <pshpack1.h>
	typedef union
	{
		struct
		{
			TOKEN_SOURCE TokenSource;     /* 0x0: CHAR SourceName[8] = "*SYSTEM*" | "User32  " + LUID SourceIdentifier = 0x10, *SYSTEM* id == 0 */
			LUID TokenId;         /* 0x10: */
			LUID AuthenticationId;    /* 0x18: */
			LARGE_INTEGER ExpirationTime; /* 0x20: -1 no expired. *SYSTEM* has expired? */
			LUID ModifiedId;          /* 0x28: */
			ULONG UserAndGroupCount;      /* 0x30: 3 */
			ULONG PrivilegeCount;     /* 0x34: 14 */
			ULONG VariableLength;     /* 0x38: 0x37C */
			ULONG DynamicCharged;     /* 0x3C: 0x1F4 */
			ULONG DynamicAvailable;   /* 0x40: 0x1A4 */
			ULONG DefaultOwnerIndex;      /* 0x44: 1 */
			PSID_AND_ATTRIBUTES UserAndGroups;/* 0x48: TOKEN_USER Owners [UserAndGroupCount] DefaultOwnerIndex */
			PSID  PrimaryGroup;       /* 0x4C: */
			PLUID_AND_ATTRIBUTES Privileges;/* 0x50: */
			PULONG DynamicPart;       /* 0x54: */
			PACL   DefaultDacl;       /* 0x58: */
			TOKEN_TYPE TokenType;     /* 0x5C: TokenPrimary | TokenImpersonation */
			SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;/* 0x60: 0 */
			UCHAR   TokenFlags;       /* 0x64: 1 */
			BOOLEAN TokenInUse;       /* 0x65: 1 */
			USHORT  Alignment;        /* 0x66: 0 */
			PVOID   ProxyData;        /* 0x68: 0 */
			PVOID   AuditData;        /* 0x6C: 0 */
			ULONG VariablePart;       /* 0x70: */
		} NT;
		struct
		{
			TOKEN_SOURCE TokenSource;     /* 0x0: CHAR SourceName[8] = "*SYSTEM*" | "User32  " + LUID SourceIdentifier = 0x10 */
			LUID TokenId;         /* 0x10: */
			LUID AuthenticationId;    /* 0x18: */
			LUID ParentTokenId;       /* 0x20: 0 */
			LARGE_INTEGER ExpirationTime; /* 0x28: -1 no expired */
			LUID ModifiedId;          /* 0x30: */
			ULONG SessionId;          /* 0x38: 0 */
			ULONG UserAndGroupCount;      /* 0x3C: 9 */
			ULONG RestrictedSidCount;     /*+0x40: 0 */
			ULONG PrivilegeCount;     /* 0x44: 11 */
			ULONG VariableLength;     /* 0x48: 0x1F0 */
			ULONG DynamicCharged;     /* 0x4C: 0x1F4 */
			ULONG DynamicAvailable;   /* 0x50: 0x1A4 */
			ULONG DefaultOwnerIndex;      /* 0x54: 3 */
			PSID_AND_ATTRIBUTES UserAndGroups; /* 0x58: TOKEN_USER Owners [UserAndGroupCount] DefaultOwnerIndex */
			PSID_AND_ATTRIBUTES RestrictedSids;/* 0x5C: 0 */
			PSID  PrimaryGroup;       /* 0x60: */
			PLUID_AND_ATTRIBUTES Privileges;/* 0x64: */
			PULONG DynamicPart;       /* 0x68: */
			PACL   DefaultDacl;       /* 0x6C: */
			TOKEN_TYPE TokenType;     /* 0x70: TokenPrimary | TokenImpersonation */
			SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;/* 0x74: 0 */
			UCHAR   TokenFlags;       /* 0x78: 9 */
			BOOLEAN TokenInUse;       /* 0x79: 1 */
			USHORT  Alignment;        /* 0x7A: 0 */
			PVOID   ProxyData;        /* 0x7C: 0 */
			PVOID   AuditData;        /* 0x80: 0 */
			ULONG VariablePart;           /* 0x84: */
		} K2;
		struct
		{
			TOKEN_SOURCE TokenSource;     /* 0x0: CHAR SourceName[8] = "*SYSTEM*" | "User32  " + LUID SourceIdentifier = 0x10 */
			LUID TokenId;         /* 0x10: 0x6F68 */
			LUID AuthenticationId;    /* 0x18: */
			LUID ParentTokenId;       /* 0x20: 0 */
			LARGE_INTEGER ExpirationTime; /* 0x28: -1 no expired */
			PERESOURCE TokenLock;     /*+0x30: 0x8xxxxxxxx */
			LUID ModifiedId;          /* 0x34: */
			ULONG SessionId;          /* 0x3C: 0x6F6A */
			ULONG UserAndGroupCount;      /* 0x40: 4 */
			ULONG RestrictedSidCount;     /*+0x44: 0 */
			ULONG VariableLength;     /* 0x48: 0x160 */
			ULONG DynamicCharged;     /* 0x4C: 0x164 */
			ULONG DynamicAvailable;   /* 0x50: 0x1F4 */
			ULONG PrivilegeCount;     /* 0x54: 0 */
			ULONG DefaultOwnerIndex;      /* 0x58: 1 */
			PSID_AND_ATTRIBUTES UserAndGroups; /* 0x5C: TOKEN_USER Owners [UserAndGroupCount] DefaultOwnerIndex */
			PSID_AND_ATTRIBUTES RestrictedSids;/* 0x60: 0 */
			PSID  PrimaryGroup;       /* 0x64: */
			PLUID_AND_ATTRIBUTES Privileges;/* 0x68: */
			PULONG DynamicPart;       /* 0x6C: */
			PACL   DefaultDacl;       /* 0x70: */
			TOKEN_TYPE TokenType;     /* 0x74: TokenPrimary | TokenImpersonation */
			SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;/* 0x78: 0 */
			UCHAR   TokenFlags;       /* 0x7C: 9 */
			BOOLEAN TokenInUse;       /* 0x7D: 1 */
			USHORT  Alignment;        /* 0x7E: 4BB4 */
			PVOID   ProxyData;        /* 0x80: 0 */
			PVOID   AuditData;        /* 0x84: 0 */
			ULONG VariablePart;       /* 0x88: */
		} XP;
		struct
		{
			TOKEN_SOURCE TokenSource;     /* 0x0: CHAR SourceName[8] = "*SYSTEM*" | "User32  " + LUID SourceIdentifier = 0x10 */
			LUID TokenId;         /* 0x10: 0x6F68 */
			LUID AuthenticationId;    /* 0x18: */
			LUID ParentTokenId;       /* 0x20: 0 */
			LARGE_INTEGER ExpirationTime; /* 0x28: -1 no expired */
			PERESOURCE TokenLock;     /*+0x30: 0x8xxxxxxxx */
			ULONG Padding64;          /*+0x34: 0xXxxxxxxxx */
			SEP_AUDIT_POLICY AuditPolicy; /*+0x38: */
			LUID ModifiedId;          /*+0x040: 0x6F6A */
			ULONG SessionId;          /*+0x048: */
			ULONG UserAndGroupCount;      /* 0x4C: 4 */
			ULONG RestrictedSidCount;     /*+0x50: 0 */
			ULONG VariableLength;     /* 0x54: 0x18 */
			ULONG DynamicCharged;     /* 0x58: 0x17C */
			ULONG DynamicAvailable;   /* 0x5C: 0x1F4 */
			ULONG PrivilegeCount;     /* 0x60: 0 */
			ULONG DefaultOwnerIndex;      /* 0x64: 1 */
			PSID_AND_ATTRIBUTES UserAndGroups; /* 0x68: TOKEN_USER Owners [UserAndGroupCount] DefaultOwnerIndex */
			PSID_AND_ATTRIBUTES RestrictedSids;/* 0x6C: 0 */
			PSID  PrimaryGroup;       /* 0x70: */
			PLUID_AND_ATTRIBUTES Privileges;/* 0x74: */
			PULONG DynamicPart;       /* 0x78: */
			PACL   DefaultDacl;       /* 0x7C: */
			TOKEN_TYPE TokenType;     /* 0x80: TokenPrimary | TokenImpersonation */
			SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;/* 0x84: 0 */
			UCHAR   TokenFlags;       /* 0x88: 9 */
			BOOLEAN TokenInUse;       /* 0x89: 1 */
			USHORT  Alignment;        /* 0x8A: 4BB4 */
			PVOID   ProxyData;        /* 0x8C: 0x8xxxxxxxx */
			PVOID   AuditData;        /* 0x90: 0 */
			ULONG VariablePart;       /* 0x94: */
		} K23;
		struct
		{
			TOKEN_SOURCE TokenSource;       /* +0x0: CHAR SourceName[8] = "*SYSTEM*" | "User32  " + LUID SourceIdentifier = 0x10 */
			LUID     TokenId;       /* +0x10: 0x6F68 */
			LUID     AuthenticationId;  /* +0x18: */
			LUID     ParentTokenId;     /* +0x20: 0 */
			LARGE_INTEGER ExpirationTime;   /* +0x28: -1 no expired */
			PERESOURCE   TokenLock;     /* +0x30: 0x8xxxxxxxx */
			ULONG    Padding64;     /* +0x34: 0xXxxxxxxxx */
			SEP_AUDIT_POLICY AuditPolicy;   /* +0x38: */
			LUID     ModifiedId;        /* +0x040: 0x6F6A */
			ULONG    SessionId;     /* +0x048: */
			ULONG    UserAndGroupCount; /* +0x04c: 4 */
			ULONG    RestrictedSidCount;    /* +0x050: 0 */
			ULONG    PrivilegeCount;    /* +0x054: 0x18 */
			ULONG    VariableLength;    /* +0x058: 0x17C */
			ULONG    DynamicCharged;    /* +0x05c: 0x1F4 */
			ULONG    DynamicAvailable;  /* +0x060: 0 */
			ULONG    DefaultOwnerIndex; /* +0x064: 1 */
			PSID_AND_ATTRIBUTES UserAndGroups;  /* +0x68: TOKEN_USER Owners [UserAndGroupCount] DefaultOwnerIndex */
			PSID_AND_ATTRIBUTES RestrictedSids; /* +0x6C: 0 */
			PSID     PrimaryGroup;      /* +0x70: */
			PLUID_AND_ATTRIBUTES Privileges;    /* +0x74: */
			PULONG   DynamicPart;       /* +0x78: */
			PACL     DefaultDacl;       /* +0x7C: */
			TOKEN_TYPE   TokenType;     /* +0x80: TokenPrimary | TokenImpersonation */
			SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;/* +0x84: 0 */
			UCHAR    TokenFlags;        /* +0x88: 9 */
			BOOLEAN  TokenInUse;        /* +0x89: 1 */
			USHORT   Alignment;     /* +0x8A: 4BB4 */
			PVOID    ProxyData;     /* +0x8C: 0x8xxxxxxxx */
			PVOID    AuditData;     /* +0x90: 0 */
			PVOID    LogonSession;      /* +0x94: */
			LUID     OriginatingLogonSession;/* +0x98: */
			ULONG    VariablePart;      /* +0xa0: */
		} K23SP1;
		struct
		{
			TOKEN_SOURCE TokenSource;       /* +0x000 */
			LUID     TokenId;       /* +0x010 */
			LUID     AuthenticationId;  /* +0x018 */
			LUID     ParentTokenId;     /* +0x020 */
			LARGE_INTEGER ExpirationTime;   /* +0x028 */
			PERESOURCE   TokenLock;     /* +0x030 */
			LUID     ModifiedId;        /* +0x034 */
			SEP_AUDIT_POLICY_VISTA AuditPolicy; /* +0x03c */
			ULONG    SessionId;     /* +0x058 */
			ULONG    UserAndGroupCount; /* +0x05c */
			ULONG    RestrictedSidCount;    /* +0x060 */
			ULONG    PrivilegeCount;    /* +0x064 */
			ULONG    VariableLength;    /* +0x068 */
			ULONG    DynamicCharged;    /* +0x06c */
			ULONG    DynamicAvailable;  /* +0x070 */
			ULONG    DefaultOwnerIndex; /* +0x074 */
			PSID_AND_ATTRIBUTES UserAndGroups;  /* +0x078 */
			PSID_AND_ATTRIBUTES RestrictedSids; /* +0x07c */
			PSID     PrimaryGroup;      /* +0x080 */
			PLUID_AND_ATTRIBUTES Privileges;    /* +0x084 */
			PULONG   DynamicPart;       /* +0x088 */
			PACL     DefaultDacl;       /* +0x08c */
			TOKEN_TYPE   TokenType;     /* +0x090 */
			SECURITY_IMPERSONATION_LEVEL ImpersonationLevel;/* +0x094 */
			ULONG    TokenFlags;        /* +0x098 */
			BOOLEAN  TokenInUse;        /* +0x09c */
			BOOLEAN  WriterPresent;     /* +0x09d */
			USHORT   Alignment;     /* +0x09e */
			ULONG    IntegrityLevelIndex;   /* +0x0a0 */
			ULONG    DesktopIntegrityLevelIndex;/* +0x0a4 */
			ULONG    MandatoryPolicy;   /* +0x0a8 */
			PVOID    ProxyData;     /* +0x0ac */
			PVOID    AuditData;     /* +0x0b0 */
			PVOID    LogonSession;      /* +0x0b4 */
			LUID     OriginatingLogonSession;/* +0x0b8 */
			SID_AND_ATTRIBUTES_HASH SidHash;    /* +0x0c0 */
			SID_AND_ATTRIBUTES_HASH RestrictedSidHash;/* +0x148 */
			ULONG    VariablePart;      /* +0x1d0 */
		} VISTA;
		struct
		{
			TOKEN_SOURCE TokenSource;       /* +0x000 */
			LUID     TokenId;       /* +0x010 */
			LUID     AuthenticationId;  /* +0x018 */
			LUID     ParentTokenId;     /* +0x020 */
			LARGE_INTEGER ExpirationTime;   /* +0x028 */
			PERESOURCE   TokenLock;     /* +0x030 */
			SEP_AUDIT_POLICY AuditPolicy;   /* +0x038 */
			LUID     ModifiedId;        /* +0x040 */
			ULONG    SessionId;     /* +0x048 */
			ULONG    UserAndGroupCount; /* +0x04c */
			ULONG    RestrictedSidCount;    /* +0x050 */
			ULONG    PrivilegeCount;    /* +0x054 */
			ULONG    VariableLength;    /* +0x058 */
			ULONG    DynamicCharged;    /* +0x05c */
			ULONG    DynamicAvailable;  /* +0x060 */
			ULONG    DefaultOwnerIndex; /* +0x064 */
			PSID_AND_ATTRIBUTES UserAndGroups;  /* +0x068 */
			PSID_AND_ATTRIBUTES RestrictedSids; /* +0x070 */
			PSID     PrimaryGroup;      /* +0x078 */
			PLUID_AND_ATTRIBUTES Privileges;    /* +0x080 */
			PULONG   DynamicPart;       /* +0x088 */
			PACL     DefaultDacl;       /* +0x090 */
			TOKEN_TYPE   TokenType;     /* +0x098 */
			SECURITY_IMPERSONATION_LEVEL ImpersonationLevel; /* +0x09c */
			UCHAR    TokenFlags;        /* +0x0a0 */
			BOOLEAN  TokenInUse;        /* +0x0a1 */
			UCHAR    Padding64 [6];     /* +0x0a2 */
			PVOID    ProxyData;     /* +0x0a8 */
			PVOID    AuditData;     /* +0x0b0 */
			PVOID    LogonSession;      /* +0x0b8 */
			LUID     OriginatingLogonSession;/* +0x0c0 */
			ULONG    VariablePart;      /* +0x0c8 */
		} XP64; /* equial 2K3SP1x64 */
		/* VariablePart */
	} TOKEN_OBJECT, *PTOKEN_OBJECT;
#include <poppack.h>

	typedef struct _TOKEN_OWNER {
		PSID Owner;
	} TOKEN_OWNER, *PTOKEN_OWNER;

	typedef struct _TOKEN_PRIMARY_GROUP {
		PSID PrimaryGroup;
	} TOKEN_PRIMARY_GROUP, *PTOKEN_PRIMARY_GROUP;

	typedef struct _TOKEN_PRIVILEGES {
		ULONG               PrivilegeCount;
		LUID_AND_ATTRIBUTES Privileges[1];
	} TOKEN_PRIVILEGES, *PTOKEN_PRIVILEGES;

	typedef struct _TOKEN_STATISTICS {
		LUID                            TokenId;
		LUID                            AuthenticationId;
		LARGE_INTEGER                   ExpirationTime;
		TOKEN_TYPE                      TokenType;
		SECURITY_IMPERSONATION_LEVEL    ImpersonationLevel;
		ULONG                           DynamicCharged;
		ULONG                           DynamicAvailable;
		ULONG                           GroupCount;
		ULONG                           PrivilegeCount;
		LUID                            ModifiedId;
	} TOKEN_STATISTICS, *PTOKEN_STATISTICS;

	typedef struct _TOKEN_USER {
		SID_AND_ATTRIBUTES User;
	} TOKEN_USER, *PTOKEN_USER;

	typedef struct _SECURITY_CLIENT_CONTEXT {
		SECURITY_QUALITY_OF_SERVICE SecurityQos;
		PACCESS_TOKEN               ClientToken;
		BOOLEAN                     DirectlyAccessClientToken;
		BOOLEAN                     DirectAccessEffectiveOnly;
		BOOLEAN                     ServerIsRemote;
		TOKEN_CONTROL               ClientTokenControl;
	} SECURITY_CLIENT_CONTEXT, *PSECURITY_CLIENT_CONTEXT;

	typedef struct _TUNNEL {
		FAST_MUTEX          Mutex;
		PRTL_SPLAY_LINKS    Cache;
		LIST_ENTRY          TimerQueue;
		USHORT              NumEntries;
	} TUNNEL, *PTUNNEL;
#endif 

	typedef struct _VACB {
		PVOID               BaseAddress;
		PSHARED_CACHE_MAP   SharedCacheMap;
		union {
			LARGE_INTEGER   FileOffset;
			USHORT          ActiveCount;
		} Overlay;
		LIST_ENTRY          LruList;
	} VACB, *PVACB;

	typedef struct _VAD_HEADER {
		PVOID       StartVPN;
		PVOID       EndVPN;
		PVAD_HEADER ParentLink;
		PVAD_HEADER LeftLink;
		PVAD_HEADER RightLink;
		ULONG       Flags;          // LSB = CommitCharge
		PVOID       ControlArea;
		PVOID       FirstProtoPte;
		PVOID       LastPTE;
		ULONG       Unknown;
		LIST_ENTRY  Secured;
	} VAD_HEADER, *PVAD_HEADER;

	NTKERNELAPI
		BOOLEAN
		CcCanIWrite (
		__in PFILE_OBJECT FileObject,
		__in ULONG        BytesToWrite,
		__in BOOLEAN      Wait,
		__in BOOLEAN      Retrying
		);

	NTKERNELAPI
		BOOLEAN
		CcCopyRead (
		__in PFILE_OBJECT         FileObject,
		__in PLARGE_INTEGER       FileOffset,
		__in ULONG                Length,
		__in BOOLEAN              Wait,
		__out PVOID               Buffer,
		__out PIO_STATUS_BLOCK    IoStatus
		);

	NTKERNELAPI
		BOOLEAN
		CcCopyWrite (
		__in PFILE_OBJECT     FileObject,
		__in PLARGE_INTEGER   FileOffset,
		__in ULONG            Length,
		__in BOOLEAN          Wait,
		__in PVOID            Buffer
		);

#if (VER_PRODUCTBUILD<7600)
#define CcCopyWriteWontFlush(FO, FOFF, LEN) ((LEN) <= 0x10000)
#endif

	typedef VOID (*PCC_POST_DEFERRED_WRITE) (
		__in PVOID Context1,
		__in PVOID Context2
		);

	NTKERNELAPI
		VOID
		CcDeferWrite (
		__in PFILE_OBJECT             FileObject,
		__in PCC_POST_DEFERRED_WRITE  PostRoutine,
		__in PVOID                    Context1,
		__in PVOID                    Context2,
		__in ULONG                    BytesToWrite,
		__in BOOLEAN                  Retrying
		);

	NTKERNELAPI
		VOID
		CcFastCopyRead (
		__in PFILE_OBJECT         FileObject,
		__in ULONG                FileOffset,
		__in ULONG                Length,
		__in ULONG                PageCount,
		__out PVOID               Buffer,
		__out PIO_STATUS_BLOCK    IoStatus
		);

	NTKERNELAPI
		VOID
		CcFastCopyWrite (
		__in PFILE_OBJECT FileObject,
		__in ULONG        FileOffset,
		__in ULONG        Length,
		__in PVOID        Buffer
		);

	NTKERNELAPI
		VOID
		CcFlushCache (
		__in PSECTION_OBJECT_POINTERS SectionObjectPointer,
		__in PLARGE_INTEGER           FileOffset OPTIONAL,
		__in ULONG                    Length,
		__out PIO_STATUS_BLOCK        IoStatus OPTIONAL
		);

	typedef VOID (*PDIRTY_PAGE_ROUTINE) (
		__in PFILE_OBJECT     FileObject,
		__in PLARGE_INTEGER   FileOffset,
		__in ULONG            Length,
		__in PLARGE_INTEGER   OldestLsn,
		__in PLARGE_INTEGER   NewestLsn,
		__in PVOID            Context1,
		__in PVOID            Context2
		);

	NTKERNELAPI
		LARGE_INTEGER
		CcGetDirtyPages (
		__in PVOID                LogHandle,
		__in PDIRTY_PAGE_ROUTINE  DirtyPageRoutine,
		__in PVOID                Context1,
		__in PVOID                Context2
		);

	NTKERNELAPI
		PFILE_OBJECT
		CcGetFileObjectFromBcb (
		__in PVOID Bcb
		);

	NTKERNELAPI
		PFILE_OBJECT
		CcGetFileObjectFromSectionPtrs (
		__in PSECTION_OBJECT_POINTERS SectionObjectPointer
		);

#define CcGetFileSizePointer(FO) (                                     \
	((PLARGE_INTEGER)((FO)->SectionObjectPointer->SharedCacheMap) + 1) \
	)

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		LARGE_INTEGER
		CcGetFlushedValidData (
		__in PSECTION_OBJECT_POINTERS SectionObjectPointer,
		__in BOOLEAN                  BcbListHeld
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		LARGE_INTEGER
		CcGetLsnForFileObject (
		__in PFILE_OBJECT     FileObject,
		__out PLARGE_INTEGER  OldestLsn OPTIONAL
		);

	typedef BOOLEAN (*PACQUIRE_FOR_LAZY_WRITE) (
		__in PVOID    Context,
		__in BOOLEAN  Wait
		);

	typedef VOID (*PRELEASE_FROM_LAZY_WRITE) (
		__in PVOID Context
		);

	typedef BOOLEAN (*PACQUIRE_FOR_READ_AHEAD) (
		__in PVOID    Context,
		__in BOOLEAN  Wait
		);

	typedef VOID (*PRELEASE_FROM_READ_AHEAD) (
		__in PVOID Context
		);

#if (VER_PRODUCTBUILD < 7600)
	typedef struct _CACHE_MANAGER_CALLBACKS {
		PACQUIRE_FOR_LAZY_WRITE     AcquireForLazyWrite;
		PRELEASE_FROM_LAZY_WRITE    ReleaseFromLazyWrite;
		PACQUIRE_FOR_READ_AHEAD     AcquireForReadAhead;
		PRELEASE_FROM_READ_AHEAD    ReleaseFromReadAhead;
	} CACHE_MANAGER_CALLBACKS, *PCACHE_MANAGER_CALLBACKS;
#endif

	NTKERNELAPI
		VOID
		CcInitializeCacheMap (
		__in PFILE_OBJECT             FileObject,
		__in PCC_FILE_SIZES           FileSizes,
		__in BOOLEAN                  PinAccess,
		__in PCACHE_MANAGER_CALLBACKS Callbacks,
		__in PVOID                    LazyWriteContext
		);

#define CcIsFileCached(FO) (                                                         \
	((FO)->SectionObjectPointer != NULL) &&                                          \
	(((PSECTION_OBJECT_POINTERS)(FO)->SectionObjectPointer)->SharedCacheMap != NULL) \
	)

	NTKERNELAPI
		BOOLEAN
		CcIsThereDirtyData (
		__in PVPB Vpb
		);

	NTKERNELAPI
		BOOLEAN
		CcMapData (
		__in PFILE_OBJECT     FileObject,
		__in PLARGE_INTEGER   FileOffset,
		__in ULONG            Length,
#if (VER_PRODUCTBUILD >= 2600)
		__in ULONG            Flags,
#else
		__in BOOLEAN          Wait,
#endif
		__out PVOID           *Bcb,
		__out PVOID           *Buffer
		);

	NTKERNELAPI
		VOID
		CcMdlRead (
		__in PFILE_OBJECT         FileObject,
		__in PLARGE_INTEGER       FileOffset,
		__in ULONG                Length,
		__out PMDL                *MdlChain,
		__out PIO_STATUS_BLOCK    IoStatus
		);

	NTKERNELAPI
		VOID
		CcMdlReadComplete (
		__in PFILE_OBJECT FileObject,
		__in PMDL         MdlChain
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		VOID
		CcMdlWriteAbort (
		__in PFILE_OBJECT FileObject,
		__in PMDL         MdlChain
		);

#endif

	NTKERNELAPI
		VOID
		CcMdlWriteComplete (
		__in PFILE_OBJECT     FileObject,
		__in PLARGE_INTEGER   FileOffset,
		__in PMDL             MdlChain
		);

	NTKERNELAPI
		BOOLEAN
		CcPinMappedData (
		__in PFILE_OBJECT     FileObject,
		__in PLARGE_INTEGER   FileOffset,
		__in ULONG            Length,
#if (VER_PRODUCTBUILD >= 2195)
		__in ULONG            Flags,
#else
		__in BOOLEAN          Wait,
#endif
		__in __out PVOID        *Bcb
		);

	NTKERNELAPI
		BOOLEAN
		CcPinRead (
		__in PFILE_OBJECT     FileObject,
		__in PLARGE_INTEGER   FileOffset,
		__in ULONG            Length,
#if (VER_PRODUCTBUILD >= 2195)
		__in ULONG            Flags,
#else
		__in BOOLEAN          Wait,
#endif
		__out PVOID           *Bcb,
		__out PVOID           *Buffer
		);

	NTKERNELAPI
		VOID
		CcPrepareMdlWrite (
		__in PFILE_OBJECT         FileObject,
		__in PLARGE_INTEGER       FileOffset,
		__in ULONG                Length,
		__out PMDL                *MdlChain,
		__out PIO_STATUS_BLOCK    IoStatus
		);

	NTKERNELAPI
		BOOLEAN
		CcPreparePinWrite (
		__in PFILE_OBJECT     FileObject,
		__in PLARGE_INTEGER   FileOffset,
		__in ULONG            Length,
		__in BOOLEAN          Zero,
#if (VER_PRODUCTBUILD >= 2195)
		__in ULONG            Flags,
#else
		__in BOOLEAN          Wait,
#endif
		__out PVOID           *Bcb,
		__out PVOID           *Buffer
		);

#if (VER_PRODUCTBUILD < 7600)
	NTKERNELAPI
		BOOLEAN
		CcPurgeCacheSection (
		__in PSECTION_OBJECT_POINTERS SectionObjectPointer,
		__in PLARGE_INTEGER           FileOffset OPTIONAL,
		__in ULONG                    Length,
		__in BOOLEAN                  UninitializeCacheMaps
		);

#define CcReadAhead(FO, FOFF, LEN) (                \
	if ((LEN) >= 256) {                             \
	CcScheduleReadAhead((FO), (FOFF), (LEN));   \
	}                                               \
	)
#endif

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		PVOID
		CcRemapBcb (
		__in PVOID Bcb
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		CcRepinBcb (
		__in PVOID Bcb
		);

	NTKERNELAPI
		VOID
		CcScheduleReadAhead (
		__in PFILE_OBJECT     FileObject,
		__in PLARGE_INTEGER   FileOffset,
		__in ULONG            Length
		);

	NTKERNELAPI
		VOID
		CcSetAdditionalCacheAttributes (
		__in PFILE_OBJECT FileObject,
		__in BOOLEAN      DisableReadAhead,
		__in BOOLEAN      DisableWriteBehind
		);

	NTKERNELAPI
		VOID
		CcSetBcbOwnerPointer (
		__in PVOID Bcb,
		__in PVOID OwnerPointer
		);

	NTKERNELAPI
		VOID
		CcSetDirtyPageThreshold (
		__in PFILE_OBJECT FileObject,
		__in ULONG        DirtyPageThreshold
		);

	NTKERNELAPI
		VOID
		CcSetDirtyPinnedData (
		__in PVOID            BcbVoid,
		__in PLARGE_INTEGER   Lsn OPTIONAL
		);

	NTKERNELAPI
		VOID
		CcSetFileSizes (
		__in PFILE_OBJECT     FileObject,
		__in PCC_FILE_SIZES   FileSizes
		);

#if (VER_PRODUCTBUILD < 7600)
	typedef VOID (*PFLUSH_TO_LSN) (
		__in PVOID            LogHandle,
		__in PLARGE_INTEGER   Lsn
		);
#endif

	NTKERNELAPI
		VOID
		CcSetLogHandleForFile (
		__in PFILE_OBJECT     FileObject,
		__in PVOID            LogHandle,
		__in PFLUSH_TO_LSN    FlushToLsnRoutine
		);

	NTKERNELAPI
		VOID
		CcSetReadAheadGranularity (
		__in PFILE_OBJECT FileObject,
		__in ULONG        Granularity     // default: PAGE_SIZE
		// allowed: 2^n * PAGE_SIZE
		);

	NTKERNELAPI
		BOOLEAN
		CcUninitializeCacheMap (
		__in PFILE_OBJECT                 FileObject,
		__in PLARGE_INTEGER               TruncateSize OPTIONAL,
		__in PCACHE_UNINITIALIZE_EVENT    UninitializeCompleteEvent OPTIONAL
		);

	NTKERNELAPI
		VOID
		CcUnpinData (
		__in PVOID Bcb
		);

	NTKERNELAPI
		VOID
		CcUnpinDataForThread (
		__in PVOID            Bcb,
		__in ERESOURCE_THREAD ResourceThreadId
		);

	NTKERNELAPI
		VOID
		CcUnpinRepinnedBcb (
		__in PVOID                Bcb,
		__in BOOLEAN              WriteThrough,
		__out PIO_STATUS_BLOCK    IoStatus
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		CcWaitForCurrentLazyWriterActivity (
		VOID
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		BOOLEAN
		CcZeroData (
		__in PFILE_OBJECT     FileObject,
		__in PLARGE_INTEGER   StartOffset,
		__in PLARGE_INTEGER   EndOffset,
		__in BOOLEAN          Wait
		);

	NTKERNELAPI
		VOID
		ExDisableResourceBoostLite (
		__in PERESOURCE Resource
		);

#ifdef _X86_
	NTKERNELAPI
		ULONG
		ExQueryPoolBlockSize (
		__in PVOID        PoolBlock,
		__out PBOOLEAN    QuotaCharged
		);
#endif // _X86_

#define FlagOn(x, f) ((x) & (f))

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		FsRtlAcquireFileExclusive (
		__in PFILE_OBJECT FileObject
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		BOOLEAN
		FsRtlAddLargeMcbEntry (
		__in PLARGE_MCB   Mcb,
		__in LONGLONG     Vbn,
		__in LONGLONG     Lbn,
		__in LONGLONG     SectorCount
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlAddMcbEntry (
		__in PMCB     Mcb,
		__in VBN      Vbn,
		__in LBN      Lbn,
		__in ULONG    SectorCount
		);

	NTKERNELAPI
		VOID
		FsRtlAddToTunnelCache (
		__in PTUNNEL          Cache,
		__in ULONGLONG        DirectoryKey,
		__in PUNICODE_STRING  ShortName,
		__in PUNICODE_STRING  LongName,
		__in BOOLEAN          KeyByShortName,
		__in ULONG            DataLength,
		__in PVOID            Data
		);

#if (VER_PRODUCTBUILD >= 2195 && VER_PRODUCTBUILD < 7600)

	PFILE_LOCK
		FsRtlAllocateFileLock (
		__in PCOMPLETE_LOCK_IRP_ROUTINE   CompleteLockIrpRoutine OPTIONAL,
		__in PUNLOCK_ROUTINE              UnlockRoutine OPTIONAL
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		PVOID
		FsRtlAllocatePool (
		__in POOL_TYPE    PoolType,
		__in ULONG        NumberOfBytes
		);

	NTKERNELAPI
		PVOID
		FsRtlAllocatePoolWithQuota (
		__in POOL_TYPE    PoolType,
		__in ULONG        NumberOfBytes
		);

#if (VER_PRODUCTBUILD < 7600)
	NTKERNELAPI
		PVOID
		FsRtlAllocatePoolWithQuotaTag (
		__in POOL_TYPE    PoolType,
		__in ULONG        NumberOfBytes,
		__in ULONG        Tag
		);

	NTKERNELAPI
		PVOID
		FsRtlAllocatePoolWithTag (
		__in POOL_TYPE    PoolType,
		__in ULONG        NumberOfBytes,
		__in ULONG        Tag
		);

	NTKERNELAPI
		PVOID
		FsRtlAllocateResource (
		VOID
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlAreNamesEqual (
		__in PUNICODE_STRING  Name1,
		__in PUNICODE_STRING  Name2,
		__in BOOLEAN          IgnoreCase,
		__in PWCHAR           UpcaseTable OPTIONAL
		);

#define FsRtlAreThereCurrentFileLocks(FL) ( \
	((FL)->FastIoIsQuestionable)            \
	)
#endif

	NTKERNELAPI
		NTSTATUS
		FsRtlBalanceReads (
		__in PDEVICE_OBJECT TargetDevice
		);

	/*
	FsRtlCheckLockForReadAccess:

	All this really does is pick __out the lock parameters from the irp (io stack
	location?), get IoGetRequestorProcess, and pass values on to
	FsRtlFastCheckLockForRead.
	*/
	NTKERNELAPI
		BOOLEAN
		FsRtlCheckLockForReadAccess (
		__in PFILE_LOCK   FileLock,
		__in PIRP         Irp
		);

	/*
	FsRtlCheckLockForWriteAccess:

	All this really does is pick __out the lock parameters from the irp (io stack
	location?), get IoGetRequestorProcess, and pass values on to
	FsRtlFastCheckLockForWrite.
	*/
	NTKERNELAPI
		BOOLEAN
		FsRtlCheckLockForWriteAccess (
		__in PFILE_LOCK   FileLock,
		__in PIRP         Irp
		);

	typedef
		VOID
		(*POPLOCK_WAIT_COMPLETE_ROUTINE) (
		__in PVOID    Context,
		__in PIRP     Irp
		);

	typedef
		VOID
		(*POPLOCK_FS_PREPOST_IRP) (
		__in PVOID    Context,
		__in PIRP     Irp
		);

	NTKERNELAPI
		NTSTATUS
		FsRtlCheckOplock (
		__in POPLOCK                          Oplock,
		__in PIRP                             Irp,
		__in PVOID                            Context,
		__in POPLOCK_WAIT_COMPLETE_ROUTINE    CompletionRoutine OPTIONAL,
		__in POPLOCK_FS_PREPOST_IRP           PostIrpRoutine OPTIONAL
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlCopyRead (
		__in PFILE_OBJECT         FileObject,
		__in PLARGE_INTEGER       FileOffset,
		__in ULONG                Length,
		__in BOOLEAN              Wait,
		__in ULONG                LockKey,
		__out PVOID               Buffer,
		__out PIO_STATUS_BLOCK    IoStatus,
		__in PDEVICE_OBJECT       DeviceObject
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlCopyWrite (
		__in PFILE_OBJECT         FileObject,
		__in PLARGE_INTEGER       FileOffset,
		__in ULONG                Length,
		__in BOOLEAN              Wait,
		__in ULONG                LockKey,
		__in PVOID                Buffer,
		__out PIO_STATUS_BLOCK    IoStatus,
		__in PDEVICE_OBJECT       DeviceObject
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlCurrentBatchOplock (
		__in POPLOCK Oplock
		);

	NTKERNELAPI
		VOID
		FsRtlDeleteKeyFromTunnelCache (
		__in PTUNNEL      Cache,
		__in ULONGLONG    DirectoryKey
		);

	NTKERNELAPI
		VOID
		FsRtlDeleteTunnelCache (
		__in PTUNNEL Cache
		);

	NTKERNELAPI
		VOID
		FsRtlDeregisterUncProvider (
		__in HANDLE Handle
		);

	NTKERNELAPI
		VOID
		FsRtlDissectDbcs (
		__in ANSI_STRING      InputName,
		__out PANSI_STRING    FirstPart,
		__out PANSI_STRING    RemainingPart
		);

	NTKERNELAPI
		VOID
		FsRtlDissectName (
		__in UNICODE_STRING   Path,
		__out PUNICODE_STRING FirstName,
		__out PUNICODE_STRING RemainingName
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlDoesDbcsContainWildCards (
		__in PANSI_STRING Name
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlDoesNameContainWildCards (
		__in PUNICODE_STRING Name
		);

#if (VER_PRODUCTBUILD < 7600)
#define FsRtlEnterFileSystem    KeEnterCriticalRegion

#define FsRtlExitFileSystem     KeLeaveCriticalRegion

	NTKERNELAPI
		BOOLEAN
		FsRtlFastCheckLockForRead (
		__in PFILE_LOCK           FileLock,
		__in PLARGE_INTEGER       FileOffset,
		__in PLARGE_INTEGER       Length,
		__in ULONG                Key,
		__in PFILE_OBJECT         FileObject,
		__in PEPROCESS            Process
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlFastCheckLockForWrite (
		__in PFILE_LOCK           FileLock,
		__in PLARGE_INTEGER       FileOffset,
		__in PLARGE_INTEGER       Length,
		__in ULONG                Key,
		__in PFILE_OBJECT         FileObject,
		__in PEPROCESS            Process
		);

#define FsRtlFastLock(A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11) (       \
	FsRtlPrivateLock(A1, A2, A3, A4, A5, A6, A7, A8, A9, NULL, A10, A11)   \
	)
#endif 

	NTKERNELAPI
		NTSTATUS
		FsRtlFastUnlockAll (
		__in PFILE_LOCK           FileLock,
		__in PFILE_OBJECT         FileObject,
		__in PEPROCESS            Process,
		__in PVOID                Context OPTIONAL
		);
	//ret: STATUS_RANGE_NOT_LOCKED

	NTKERNELAPI
		NTSTATUS
		FsRtlFastUnlockAllByKey (
		__in PFILE_LOCK           FileLock,
		__in PFILE_OBJECT         FileObject,
		__in PEPROCESS            Process,
		__in ULONG                Key,
		__in PVOID                Context OPTIONAL
		);  
	//ret: STATUS_RANGE_NOT_LOCKED

#ifdef _X86_
	NTKERNELAPI
		NTSTATUS
		FsRtlFastUnlockSingle (
		__in PFILE_LOCK           FileLock,
		__in PFILE_OBJECT         FileObject,
		__in PLARGE_INTEGER       FileOffset,
		__in PLARGE_INTEGER       Length,
		__in PEPROCESS            Process,
		__in ULONG                Key,
		__in PVOID                Context OPTIONAL,
		__in BOOLEAN              AlreadySynchronized
		);                      
	//ret:  STATUS_RANGE_NOT_LOCKED
#endif // _X86_

	NTKERNELAPI
		BOOLEAN
		FsRtlFindInTunnelCache (
		__in PTUNNEL          Cache,
		__in ULONGLONG        DirectoryKey,
		__in PUNICODE_STRING  Name,
		__out PUNICODE_STRING ShortName,
		__out PUNICODE_STRING LongName,
		__in __out PULONG       DataLength,
		__out PVOID           Data
		);

#if (VER_PRODUCTBUILD >= 2195 && VER_PRODUCTBUILD < 7600)

	VOID
		FsRtlFreeFileLock (
		__in PFILE_LOCK FileLock
		);

#endif // (VER_PRODUCTBUILD >= 2195)

#if (VER_PRODUCTBUILD < 7600)
	NTKERNELAPI
		NTSTATUS
		FsRtlGetFileSize (
		__in PFILE_OBJECT         FileObject,
		__in __out PLARGE_INTEGER   FileSize
		);
#endif

	/*
	FsRtlGetNextFileLock:

	ret: NULL if no more locks

	Internals:
	FsRtlGetNextFileLock uses FileLock->LastReturnedLockInfo and
	FileLock->LastReturnedLock as storage.
	LastReturnedLock is a pointer to the 'raw' lock inkl. double linked
	list, and FsRtlGetNextFileLock needs this to get next lock on subsequent
	calls with Restart = FALSE.
	*/
	NTKERNELAPI
		PFILE_LOCK_INFO
		FsRtlGetNextFileLock (
		__in PFILE_LOCK   FileLock,
		__in BOOLEAN      Restart
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlGetNextLargeMcbEntry (
		__in PLARGE_MCB   Mcb,
		__in ULONG        RunIndex,
		__out PLONGLONG   Vbn,
		__out PLONGLONG   Lbn,
		__out PLONGLONG   SectorCount
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlGetNextMcbEntry (
		__in PMCB     Mcb,
		__in ULONG    RunIndex,
		__out PVBN    Vbn,
		__out PLBN    Lbn,
		__out PULONG  SectorCount
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		VOID
		FsRtlIncrementCcFastReadNotPossible (
		VOID
		);

	NTKERNELAPI
		VOID
		FsRtlIncrementCcFastReadNoWait (
		VOID
		);

	NTKERNELAPI
		VOID
		FsRtlIncrementCcFastReadResourceMiss (
		VOID
		);

	NTKERNELAPI
		VOID
		FsRtlIncrementCcFastReadWait (
		VOID
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		VOID
		FsRtlInitializeFileLock (
		__in PFILE_LOCK                   FileLock,
		__in PCOMPLETE_LOCK_IRP_ROUTINE   CompleteLockIrpRoutine OPTIONAL,
		__in PUNLOCK_ROUTINE              UnlockRoutine OPTIONAL
		);

	NTKERNELAPI
		VOID
		FsRtlInitializeLargeMcb (
		__in PLARGE_MCB   Mcb,
		__in POOL_TYPE    PoolType
		);

	NTKERNELAPI
		VOID
		FsRtlInitializeMcb (
		__in PMCB         Mcb,
		__in POOL_TYPE    PoolType
		);

	NTKERNELAPI
		VOID
		FsRtlInitializeOplock (
		__in __out POPLOCK Oplock
		);

	NTKERNELAPI
		VOID
		FsRtlInitializeTunnelCache (
		__in PTUNNEL Cache
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlIsDbcsInExpression (
		__in PANSI_STRING Expression,
		__in PANSI_STRING Name
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlIsFatDbcsLegal (
		__in ANSI_STRING  DbcsName,
		__in BOOLEAN      WildCardsPermissible,
		__in BOOLEAN      PathNamePermissible,
		__in BOOLEAN      LeadingBackslashPermissible
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlIsHpfsDbcsLegal (
		__in ANSI_STRING  DbcsName,
		__in BOOLEAN      WildCardsPermissible,
		__in BOOLEAN      PathNamePermissible,
		__in BOOLEAN      LeadingBackslashPermissible
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlIsNameInExpression (
		__in PUNICODE_STRING  Expression,
		__in PUNICODE_STRING  Name,
		__in BOOLEAN          IgnoreCase,
		__in PWCHAR           UpcaseTable OPTIONAL
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlIsNtstatusExpected (
		__in NTSTATUS Ntstatus
		);

#if (VER_PRODUCTBUILD >= 2600 && VER_PRODUCTBUILD < 7600)

	NTKERNELAPI
		BOOLEAN
		FsRtlIsPagingFile (
		__in PFILE_OBJECT FileObject
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		BOOLEAN
		FsRtlIsTotalDeviceFailure (
		__in NTSTATUS Status
		);

#if (VER_PRODUCTBUILD<7600)
#define FsRtlIsUnicodeCharacterWild(C) (                                    \
	(((C) >= 0x40) ?                                                        \
FALSE :                                                                 \
	FlagOn((*FsRtlLegalAnsiCharacterArray)[(C)], FSRTL_WILD_CHARACTER ))    \
	)
#endif

	NTKERNELAPI
		BOOLEAN
		FsRtlLookupLargeMcbEntry (
		__in PLARGE_MCB   Mcb,
		__in LONGLONG     Vbn,
		__out PLONGLONG   Lbn OPTIONAL,
		__out PLONGLONG   SectorCountFromLbn OPTIONAL,
		__out PLONGLONG   StartingLbn OPTIONAL,
		__out PLONGLONG   SectorCountFromStartingLbn OPTIONAL,
		__out PULONG      Index OPTIONAL
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlLookupLastLargeMcbEntry (
		__in PLARGE_MCB Mcb,
		__out PLONGLONG Vbn,
		__out PLONGLONG Lbn
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		BOOLEAN
		FsRtlLookupLastLargeMcbEntryAndIndex (
		__in PLARGE_MCB   OpaqueMcb,
		__out PLONGLONG   LargeVbn,
		__out PLONGLONG   LargeLbn,
		__out PULONG      Index
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		BOOLEAN
		FsRtlLookupLastMcbEntry (
		__in PMCB     Mcb,
		__out PVBN    Vbn,
		__out PLBN    Lbn
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlLookupMcbEntry (
		__in PMCB     Mcb,
		__in VBN      Vbn,
		__out PLBN    Lbn,
		__out PULONG  SectorCount OPTIONAL,
		__out PULONG  Index
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlMdlReadComplete (
		__in PFILE_OBJECT     FileObject,
		__in PMDL             MdlChain
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlMdlReadCompleteDev (
		__in PFILE_OBJECT     FileObject,
		__in PMDL             MdlChain,
		__in PDEVICE_OBJECT   DeviceObject
		);

#if (VER_PRODUCTBUILD >= 1381)

	NTKERNELAPI
		BOOLEAN
		FsRtlMdlReadDev (
		__in PFILE_OBJECT         FileObject,
		__in PLARGE_INTEGER       FileOffset,
		__in ULONG                Length,
		__in ULONG                LockKey,
		__out PMDL                *MdlChain,
		__out PIO_STATUS_BLOCK    IoStatus,
		__in PDEVICE_OBJECT       DeviceObject
		);

#endif // (VER_PRODUCTBUILD >= 1381)

	NTKERNELAPI
		BOOLEAN
		FsRtlMdlWriteComplete (
		__in PFILE_OBJECT     FileObject,
		__in PLARGE_INTEGER   FileOffset,
		__in PMDL             MdlChain
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlMdlWriteCompleteDev (
		__in PFILE_OBJECT     FileObject,
		__in PLARGE_INTEGER   FileOffset,
		__in PMDL             MdlChain,
		__in PDEVICE_OBJECT   DeviceObject
		);

	NTKERNELAPI
		NTSTATUS
		FsRtlNormalizeNtstatus (
		__in NTSTATUS Exception,
		__in NTSTATUS GenericException
		);

	NTKERNELAPI
		VOID
		FsRtlNotifyChangeDirectory (
		__in PNOTIFY_SYNC NotifySync,
		__in PVOID        FsContext,
		__in PSTRING      FullDirectoryName,
		__in PLIST_ENTRY  NotifyList,
		__in BOOLEAN      WatchTree,
		__in ULONG        CompletionFilter,
		__in PIRP         NotifyIrp
		);

	NTKERNELAPI
		VOID
		FsRtlNotifyCleanup (
		__in PNOTIFY_SYNC NotifySync,
		__in PLIST_ENTRY  NotifyList,
		__in PVOID        FsContext
		);

	typedef BOOLEAN (*PCHECK_FOR_TRAVERSE_ACCESS) (
		__in PVOID                        NotifyContext,
		__in PVOID                        TargetContext,
		__in PSECURITY_SUBJECT_CONTEXT    SubjectContext
		);

#if (VER_PRODUCTBUILD >= 2600)

	typedef BOOLEAN (*PFILTER_REPORT_CHANGE) (
		__in PVOID NotifyContext,
		__in PVOID FilterContext
		);

	NTKERNELAPI
		VOID
		FsRtlNotifyFilterChangeDirectory (
		__in PNOTIFY_SYNC                 NotifySync,
		__in PLIST_ENTRY                  NotifyList,
		__in PVOID                        FsContext,
		__in PSTRING                      FullDirectoryName,
		__in BOOLEAN                      WatchTree,
		__in BOOLEAN                      IgnoreBuffer,
		__in ULONG                        CompletionFilter,
		__in PIRP                         NotifyIrp,
		__in PCHECK_FOR_TRAVERSE_ACCESS   TraverseCallback OPTIONAL,
		__in PSECURITY_SUBJECT_CONTEXT    SubjectContext OPTIONAL,
		__in PFILTER_REPORT_CHANGE        FilterCallback OPTIONAL
		);

	NTKERNELAPI
		VOID
		FsRtlNotifyFilterReportChange (
		__in PNOTIFY_SYNC NotifySync,
		__in PLIST_ENTRY  NotifyList,
		__in PSTRING      FullTargetName,
		__in USHORT       TargetNameOffset,
		__in PSTRING      StreamName OPTIONAL,
		__in PSTRING      NormalizedParentName OPTIONAL,
		__in ULONG        FilterMatch,
		__in ULONG        Action,
		__in PVOID        TargetContext,
		__in PVOID        FilterContext
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		VOID
		FsRtlNotifyFullChangeDirectory (
		__in PNOTIFY_SYNC                 NotifySync,
		__in PLIST_ENTRY                  NotifyList,
		__in PVOID                        FsContext,
		__in PSTRING                      FullDirectoryName,
		__in BOOLEAN                      WatchTree,
		__in BOOLEAN                      IgnoreBuffer,
		__in ULONG                        CompletionFilter,
		__in PIRP                         NotifyIrp,
		__in PCHECK_FOR_TRAVERSE_ACCESS   TraverseCallback OPTIONAL,
		__in PSECURITY_SUBJECT_CONTEXT    SubjectContext OPTIONAL
		);

	NTKERNELAPI
		VOID
		FsRtlNotifyFullReportChange (
		__in PNOTIFY_SYNC NotifySync,
		__in PLIST_ENTRY  NotifyList,
		__in PSTRING      FullTargetName,
		__in USHORT       TargetNameOffset,
		__in PSTRING      StreamName OPTIONAL,
		__in PSTRING      NormalizedParentName OPTIONAL,
		__in ULONG        FilterMatch,
		__in ULONG        Action,
		__in PVOID        TargetContext
		);

	NTKERNELAPI
		VOID
		FsRtlNotifyInitializeSync (
		__in PNOTIFY_SYNC *NotifySync
		);

	NTKERNELAPI
		VOID
		FsRtlNotifyReportChange (
		__in PNOTIFY_SYNC NotifySync,
		__in PLIST_ENTRY  NotifyList,
		__in PSTRING      FullTargetName,
		__in PUSHORT      FileNamePartLength,
		__in ULONG        FilterMatch
		);

	NTKERNELAPI
		VOID
		FsRtlNotifyUninitializeSync (
		__in PNOTIFY_SYNC *NotifySync
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		FsRtlNotifyVolumeEvent (
		__in PFILE_OBJECT FileObject,
		__in ULONG        EventCode
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		ULONG
		FsRtlNumberOfRunsInLargeMcb (
		__in PLARGE_MCB Mcb
		);

	NTKERNELAPI
		ULONG
		FsRtlNumberOfRunsInMcb (
		__in PMCB Mcb
		);

	NTKERNELAPI
		NTSTATUS
		FsRtlOplockFsctrl (
		__in POPLOCK  Oplock,
		__in PIRP     Irp,
		__in ULONG    OpenCount
		);

	NTKERNELAPI
		BOOLEAN
		FsRtlOplockIsFastIoPossible (
		__in POPLOCK Oplock
		);

	typedef
		VOID
		(*PFSRTL_STACK_OVERFLOW_ROUTINE) (
		__in PVOID    Context,
		__in PKEVENT  Event
		);

	NTKERNELAPI
		VOID
		FsRtlPostPagingFileStackOverflow (
		__in PVOID                            Context,
		__in PKEVENT                          Event,
		__in PFSRTL_STACK_OVERFLOW_ROUTINE    StackOverflowRoutine
		);

	NTKERNELAPI
		VOID
		FsRtlPostStackOverflow (
		__in PVOID                            Context,
		__in PKEVENT                          Event,
		__in PFSRTL_STACK_OVERFLOW_ROUTINE    StackOverflowRoutine
		);

#if (VER_PRODUCTBUILD >= 1381)

	NTKERNELAPI
		BOOLEAN
		FsRtlPrepareMdlWriteDev (
		__in PFILE_OBJECT         FileObject,
		__in PLARGE_INTEGER       FileOffset,
		__in ULONG                Length,
		__in ULONG                LockKey,
		__out PMDL                *MdlChain,
		__out PIO_STATUS_BLOCK    IoStatus,
		__in PDEVICE_OBJECT       DeviceObject
		);

#endif // (VER_PRODUCTBUILD >= 1381)

	/*
	FsRtlPrivateLock:

	ret: IoStatus->Status: STATUS_PENDING, STATUS_LOCK_NOT_GRANTED

	Internals: 
	-Calls IoCompleteRequest if Irp
	-Uses exception handling / ExRaiseStatus with STATUS_INSUFFICIENT_RESOURCES
	*/
	NTKERNELAPI
		BOOLEAN
		FsRtlPrivateLock (
		__in PFILE_LOCK           FileLock,
		__in PFILE_OBJECT         FileObject,
		__in PLARGE_INTEGER       FileOffset,
		__in PLARGE_INTEGER       Length,
		__in PEPROCESS            Process,
		__in ULONG                Key,
		__in BOOLEAN              FailImmediately, 
		__in BOOLEAN              ExclusiveLock,
		__out PIO_STATUS_BLOCK    IoStatus, 
		__in PIRP                 Irp OPTIONAL,
		__in PVOID                Context,
		__in BOOLEAN              AlreadySynchronized
		);

	/*
	FsRtlProcessFileLock:

	ret:
	-STATUS_INVALID_DEVICE_REQUEST
	-STATUS_RANGE_NOT_LOCKED from unlock routines.
	-STATUS_PENDING, STATUS_LOCK_NOT_GRANTED from FsRtlPrivateLock
	(redirected IoStatus->Status).

	Internals: 
	-switch ( Irp->CurrentStackLocation->MinorFunction )
	lock: return FsRtlPrivateLock;
	unlocksingle: return FsRtlFastUnlockSingle;
	unlockall: return FsRtlFastUnlockAll;
	unlockallbykey: return FsRtlFastUnlockAllByKey;
	default: IofCompleteRequest with STATUS_INVALID_DEVICE_REQUEST;
	return STATUS_INVALID_DEVICE_REQUEST;

	-'AllwaysZero' is passed thru as 'AllwaysZero' to lock / unlock routines.
	-'Irp' is passet thru as 'Irp' to FsRtlPrivateLock.
	*/
	NTKERNELAPI
		NTSTATUS
		FsRtlProcessFileLock (
		__in PFILE_LOCK   FileLock,
		__in PIRP         Irp,
		__in PVOID        Context OPTIONAL
		);

	NTKERNELAPI
		NTSTATUS
		FsRtlRegisterUncProvider (
		__in __out PHANDLE      MupHandle,
		__in PUNICODE_STRING  RedirectorDeviceName,
		__in BOOLEAN          MailslotsSupported
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		FsRtlReleaseFile (
		__in PFILE_OBJECT FileObject
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		FsRtlRemoveLargeMcbEntry (
		__in PLARGE_MCB   Mcb,
		__in LONGLONG     Vbn,
		__in LONGLONG     SectorCount
		);

	NTKERNELAPI
		VOID
		FsRtlRemoveMcbEntry (
		__in PMCB     Mcb,
		__in VBN      Vbn,
		__in ULONG    SectorCount
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		FsRtlResetLargeMcb (
		__in PLARGE_MCB   Mcb,
		__in BOOLEAN      SelfSynchronized
		);

#endif // (VER_PRODUCTBUILD >= 2195)

#if (VER_PRODUCTBUILD >= 2600)

#define FsRtlSetupAdvancedHeader( _advhdr, _fmutx )                         \
	{                                                                           \
	SetFlag( (_advhdr)->Flags, FSRTL_FLAG_ADVANCED_HEADER );                \
	SetFlag( (_advhdr)->Flags2, FSRTL_FLAG2_SUPPORTS_FILTER_CONTEXTS );     \
	(_advhdr)->Version = FSRTL_FCB_HEADER_V1;                               \
	InitializeListHead( &(_advhdr)->FilterContexts );                       \
	if ((_fmutx) != NULL) {                                                 \
	(_advhdr)->FastMutex = (_fmutx);                                    \
	}                                                                       \
	*((PULONG_PTR)(&(_advhdr)->PushLock)) = 0;                              \
	(_advhdr)->FileContextSupportPointer = NULL;                            \
	}

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		BOOLEAN
		FsRtlSplitLargeMcb (
		__in PLARGE_MCB   Mcb,
		__in LONGLONG     Vbn,
		__in LONGLONG     Amount
		);

	NTKERNELAPI
		VOID
		FsRtlTruncateLargeMcb (
		__in PLARGE_MCB   Mcb,
		__in LONGLONG     Vbn
		);

	NTKERNELAPI
		VOID
		FsRtlTruncateMcb (
		__in PMCB Mcb,
		__in VBN  Vbn
		);

	NTKERNELAPI
		VOID
		FsRtlUninitializeFileLock (
		__in PFILE_LOCK FileLock
		);

	NTKERNELAPI
		VOID
		FsRtlUninitializeLargeMcb (
		__in PLARGE_MCB Mcb
		);

	NTKERNELAPI
		VOID
		FsRtlUninitializeMcb (
		__in PMCB Mcb
		);

	NTKERNELAPI
		VOID
		FsRtlUninitializeOplock (
		__in __out POPLOCK Oplock
		);

	//
	// If using HalDisplayString during boot on Windows 2000 or later you must
	// first call InbvEnableDisplayString.
	//
	NTSYSAPI
		VOID
		NTAPI
		HalDisplayString (
		__in PCHAR String
		);

	NTSYSAPI
		VOID
		NTAPI
		HalQueryRealTimeClock (
		__in __out PTIME_FIELDS TimeFields
		);

	NTSYSAPI
		VOID
		NTAPI
		HalSetRealTimeClock (
		__in PTIME_FIELDS TimeFields
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		InbvAcquireDisplayOwnership (
		VOID
		);

	NTKERNELAPI
		BOOLEAN
		InbvCheckDisplayOwnership (
		VOID
		);

	NTKERNELAPI
		BOOLEAN
		InbvDisplayString (
		__in PCHAR String
		);

	NTKERNELAPI
		VOID
		InbvEnableBootDriver (
		__in BOOLEAN Enable
		);

	NTKERNELAPI
		BOOLEAN
		InbvEnableDisplayString (
		__in BOOLEAN Enable
		);

	NTKERNELAPI
		VOID
		InbvInstallDisplayStringFilter (
		__in PVOID Unknown
		);

	NTKERNELAPI
		BOOLEAN
		InbvIsBootDriverInstalled (
		VOID
		);

	NTKERNELAPI
		VOID
		InbvNotifyDisplayOwnershipLost (
		__in PVOID Callback
		);

	NTKERNELAPI
		BOOLEAN
		InbvResetDisplay (
		VOID
		);

	NTKERNELAPI
		VOID
		InbvSetScrollRegion (
		__in ULONG Left,
		__in ULONG Top,
		__in ULONG Width,
		__in ULONG Height
		);

	NTKERNELAPI
		VOID
		InbvSetTextColor (
		__in ULONG Color
		);

	NTKERNELAPI
		VOID
		InbvSolidColorFill (
		__in ULONG Left,
		__in ULONG Top,
		__in ULONG Width,
		__in ULONG Height,
		__in ULONG Color
		);

#endif // (VER_PRODUCTBUILD >= 2195)

#define InitializeMessageHeader(m, l, t) {                  \
	(m)->Length = (USHORT)(l);                              \
	(m)->DataLength = (USHORT)(l - sizeof( LPC_MESSAGE ));  \
	(m)->MessageType = (USHORT)(t);                         \
	(m)->DataInfoOffset = 0;                                \
	}

	NTKERNELAPI
		VOID
		IoAcquireVpbSpinLock (
		__out PKIRQL Irql
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		NTSTATUS
		IoAttachDeviceToDeviceStackSafe (
		__in PDEVICE_OBJECT   SourceDevice,
		__in PDEVICE_OBJECT   TargetDevice,
		__out PDEVICE_OBJECT  *AttachedToDeviceObject
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		NTSTATUS
		IoCheckDesiredAccess (
		__in __out PACCESS_MASK DesiredAccess,
		__in ACCESS_MASK      GrantedAccess
		);

	NTKERNELAPI
		NTSTATUS
		IoCheckEaBufferValidity (
		__in PFILE_FULL_EA_INFORMATION    EaBuffer,
		__in ULONG                        EaLength,
		__out PULONG                      ErrorOffset
		);

#if (VER_PRODUCTBUILD < 7600)
	NTKERNELAPI
		NTSTATUS
		IoCheckFunctionAccess (
		__in ACCESS_MASK              GrantedAccess,
		__in UCHAR                    MajorFunction,
		__in UCHAR                    MinorFunction,
		__in ULONG                    IoControlCode,
		__in PFILE_INFORMATION_CLASS  FileInformationClass OPTIONAL,
		__in PFS_INFORMATION_CLASS    FsInformationClass OPTIONAL
		);
#endif

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		IoCheckQuerySetFileInformation (
		__in FILE_INFORMATION_CLASS   FileInformationClass,
		__in ULONG                    Length,
		__in BOOLEAN                  SetOperation
		);

	NTKERNELAPI
		NTSTATUS
		IoCheckQuerySetVolumeInformation (
		__in FS_INFORMATION_CLASS     FsInformationClass,
		__in ULONG                    Length,
		__in BOOLEAN                  SetOperation
		);

	NTKERNELAPI
		NTSTATUS
		IoCheckQuotaBufferValidity (
		__in PFILE_QUOTA_INFORMATION  QuotaBuffer,
		__in ULONG                    QuotaLength,
		__out PULONG                  ErrorOffset
		);

#endif // (VER_PRODUCTBUILD >= 2195)

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		NTSTATUS
		IoCreateFileSpecifyDeviceObjectHint (
		__out PHANDLE             FileHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes,
		__out PIO_STATUS_BLOCK    IoStatusBlock,
		__in PLARGE_INTEGER       AllocationSize OPTIONAL,
		__in ULONG                FileAttributes,
		__in ULONG                ShareAccess,
		__in ULONG                Disposition,
		__in ULONG                CreateOptions,
		__in PVOID                EaBuffer OPTIONAL,
		__in ULONG                EaLength,
		__in CREATE_FILE_TYPE     CreateFileType,
		__in PVOID                ExtraCreateParameters OPTIONAL,
		__in ULONG                Options,
		__in PVOID                DeviceObject
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		PFILE_OBJECT
		IoCreateStreamFileObject (
		__in PFILE_OBJECT     FileObject OPTIONAL,
		__in PDEVICE_OBJECT   DeviceObject OPTIONAL
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		PFILE_OBJECT
		IoCreateStreamFileObjectEx (
		__in PFILE_OBJECT     FileObject OPTIONAL,
		__in PDEVICE_OBJECT   DeviceObject OPTIONAL,
		__out PHANDLE         FileObjectHandle OPTIONAL
		);

#endif // (VER_PRODUCTBUILD >= 2600)

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		PFILE_OBJECT
		IoCreateStreamFileObjectLite (
		__in PFILE_OBJECT     FileObject OPTIONAL,
		__in PDEVICE_OBJECT   DeviceObject OPTIONAL
		);

#endif // (VER_PRODUCTBUILD >= 2195)

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		NTSTATUS
		IoEnumerateDeviceObjectList (
		__in PDRIVER_OBJECT   DriverObject,
		__in PDEVICE_OBJECT   *DeviceObjectList,
		__in ULONG            DeviceObjectListSize,
		__out PULONG          ActualNumberDeviceObjects
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		BOOLEAN
		IoFastQueryNetworkAttributes (
		__in POBJECT_ATTRIBUTES               ObjectAttributes,
		__in ACCESS_MASK                      DesiredAccess,
		__in ULONG                            OpenOptions,
		__out PIO_STATUS_BLOCK                IoStatus,
		__out PFILE_NETWORK_OPEN_INFORMATION  Buffer
		);

	NTKERNELAPI
		PDEVICE_OBJECT
		IoGetAttachedDevice (
		__in PDEVICE_OBJECT DeviceObject
		);

	NTKERNELAPI
		PDEVICE_OBJECT
		IoGetBaseFileSystemDeviceObject (
		__in PFILE_OBJECT FileObject
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		PDEVICE_OBJECT
		IoGetDeviceAttachmentBaseRef (
		__in PDEVICE_OBJECT DeviceObject
		);

	NTKERNELAPI
		NTSTATUS
		IoGetDiskDeviceObject (
		__in PDEVICE_OBJECT   FileSystemDeviceObject,
		__out PDEVICE_OBJECT  *DiskDeviceObject
		);

	NTKERNELAPI
		PDEVICE_OBJECT
		IoGetLowerDeviceObject (
		__in PDEVICE_OBJECT DeviceObject
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		PEPROCESS
		IoGetRequestorProcess (
		__in PIRP Irp
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		ULONG
		IoGetRequestorProcessId (
		__in PIRP Irp
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		PIRP
		IoGetTopLevelIrp (
		VOID
		);

#if (VER_PRODUCTBUILD<7600)
#define IoIsFileOpenedExclusively(FileObject) ( \
	(BOOLEAN) !(                                \
	(FileObject)->SharedRead ||                 \
	(FileObject)->SharedWrite ||                \
	(FileObject)->SharedDelete                  \
	)                                           \
	)
#endif

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		BOOLEAN
		IoIsFileOriginRemote (
		__in PFILE_OBJECT FileObject
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		BOOLEAN
		IoIsOperationSynchronous (
		__in PIRP Irp
		);

	NTKERNELAPI
		BOOLEAN
		IoIsSystemThread (
		__in PETHREAD Thread
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		BOOLEAN
		IoIsValidNameGraftingBuffer (
		__in PIRP                 Irp,
		__in PREPARSE_DATA_BUFFER ReparseBuffer
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		IoPageRead (
		__in PFILE_OBJECT         FileObject,
		__in PMDL                 Mdl,
		__in PLARGE_INTEGER       Offset,
		__in PKEVENT              Event,
		__out PIO_STATUS_BLOCK    IoStatusBlock
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		NTSTATUS
		IoQueryFileDosDeviceName (
		__in PFILE_OBJECT                 FileObject,
		__out POBJECT_NAME_INFORMATION    *ObjectNameInformation
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		NTSTATUS
		IoQueryFileInformation (
		__in PFILE_OBJECT             FileObject,
		__in FILE_INFORMATION_CLASS   FileInformationClass,
		__in ULONG                    Length,
		__out PVOID                   FileInformation,
		__out PULONG                  ReturnedLength
		);

	NTKERNELAPI
		NTSTATUS
		IoQueryVolumeInformation (
		__in PFILE_OBJECT         FileObject,
		__in FS_INFORMATION_CLASS FsInformationClass,
		__in ULONG                Length,
		__out PVOID               FsInformation,
		__out PULONG              ReturnedLength
		);

#if (VER_PRODUCTBUILD >= 1381)

	NTKERNELAPI
		VOID
		IoQueueThreadIrp (
		__in PIRP Irp
		);

#endif // (VER_PRODUCTBUILD >= 1381)

	NTKERNELAPI
		VOID
		IoRegisterFileSystem (
		__in __out PDEVICE_OBJECT DeviceObject
		);

#if (VER_PRODUCTBUILD >= 1381)

	typedef VOID (*PDRIVER_FS_NOTIFICATION) (
		__in PDEVICE_OBJECT DeviceObject,
		__in BOOLEAN        DriverActive
		);

	NTKERNELAPI
		NTSTATUS
		IoRegisterFsRegistrationChange (
		__in PDRIVER_OBJECT           DriverObject,
		__in PDRIVER_FS_NOTIFICATION  DriverNotificationRoutine
		);

#endif // (VER_PRODUCTBUILD >= 1381)

	NTKERNELAPI
		VOID
		IoReleaseVpbSpinLock (
		__in KIRQL Irql
		);

	NTKERNELAPI
		VOID
		IoSetDeviceToVerify (
		__in PETHREAD         Thread,
		__in PDEVICE_OBJECT   DeviceObject
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		IoSetFileOrigin (
		__in PFILE_OBJECT FileObject,
		__in BOOLEAN      Remote
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		IoSetInformation (
		__in PFILE_OBJECT             FileObject,
		__in FILE_INFORMATION_CLASS   FileInformationClass,
		__in ULONG                    Length,
		__in PVOID                    FileInformation
		);

	NTKERNELAPI
		VOID
		IoSetTopLevelIrp (
		__in PIRP Irp
		);

	NTKERNELAPI
		NTSTATUS
		IoSynchronousPageWrite (
		__in PFILE_OBJECT         FileObject,
		__in PMDL                 Mdl,
		__in PLARGE_INTEGER       FileOffset,
		__in PKEVENT              Event,
		__out PIO_STATUS_BLOCK    IoStatusBlock
		);

	NTKERNELAPI
		PEPROCESS
		IoThreadToProcess (
		__in PETHREAD Thread
		);

	NTKERNELAPI
		VOID
		IoUnregisterFileSystem (
		__in __out PDEVICE_OBJECT DeviceObject
		);

#if (VER_PRODUCTBUILD >= 1381 && VER_PRODUCTBUILD < 7600)

	NTKERNELAPI
		NTSTATUS
		IoUnregisterFsRegistrationChange (
		__in PDRIVER_OBJECT           DriverObject,
		__in PDRIVER_FS_NOTIFICATION  DriverNotificationRoutine
		);

#endif // (VER_PRODUCTBUILD >= 1381)

	NTKERNELAPI
		NTSTATUS
		IoVerifyVolume (
		__in PDEVICE_OBJECT   DeviceObject,
		__in BOOLEAN          AllowRawMount
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		KIRQL
		FASTCALL
		KeAcquireQueuedSpinLock (
		__in KSPIN_LOCK_QUEUE_NUMBER Number
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		KeAttachProcess (
		__in PEPROCESS Process
		);

	NTKERNELAPI
		VOID
		KeDetachProcess (
		VOID
		);

	NTKERNELAPI
		VOID
		KeInitializeApc (
		PKAPC               Apc,
		PKTHREAD            Thread,
		UCHAR               StateIndex,
		PKKERNEL_ROUTINE    KernelRoutine,
		PKRUNDOWN_ROUTINE   RundownRoutine,
		PKNORMAL_ROUTINE    NormalRoutine,
		KPROCESSOR_MODE     ApcMode,
		PVOID               NormalContext
		);

	NTKERNELAPI
		VOID
		KeInitializeMutant (
		__in PRKMUTANT    Mutant,
		__in BOOLEAN      InitialOwner
		);

	NTKERNELAPI
		VOID
		KeInitializeQueue (
		__in PRKQUEUE Queue,
		__in ULONG    Count OPTIONAL
		);

	NTKERNELAPI
		LONG
		KeInsertHeadQueue (
		__in PRKQUEUE     Queue,
		__in PLIST_ENTRY  Entry
		);

	NTKERNELAPI
		LONG
		KeInsertQueue (
		__in PRKQUEUE     Queue,
		__in PLIST_ENTRY  Entry
		);

	NTKERNELAPI
		BOOLEAN
		KeInsertQueueApc (
		__in PKAPC        Apc,
		__in PVOID        SystemArgument1,
		__in PVOID        SystemArgument2,
		__in KPRIORITY    Increment
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		BOOLEAN
		KeIsAttachedProcess (
		VOID
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		BOOLEAN
		KeIsExecutingDpc (
		VOID
		);

	NTKERNELAPI
		LONG
		KeReadStateMutant (
		__in PRKMUTANT Mutant
		);

	NTKERNELAPI
		LONG
		KeReadStateQueue (
		__in PRKQUEUE Queue
		);

	NTKERNELAPI
		LONG
		KeReleaseMutant (
		__in PRKMUTANT    Mutant,
		__in KPRIORITY    Increment,
		__in BOOLEAN      Abandoned,
		__in BOOLEAN      Wait
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		FASTCALL
		KeReleaseQueuedSpinLock (
		__in KSPIN_LOCK_QUEUE_NUMBER  Number,
		__in KIRQL                    OldIrql
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		PLIST_ENTRY
		KeRemoveQueue (
		__in PRKQUEUE         Queue,
		__in KPROCESSOR_MODE  WaitMode,
		__in PLARGE_INTEGER   Timeout OPTIONAL
		);

#if (VER_PRODUCTBUILD >= 2195 && VER_PRODUCTBUILD < 7600)

	NTKERNELAPI
		NTSTATUS
		KeRevertToUserAffinityThread (
		VOID
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		PLIST_ENTRY
		KeRundownQueue (
		__in PRKQUEUE Queue
		);

#if (VER_PRODUCTBUILD >= 1381 && VER_PRODUCTBUILD < 7600)

	NTKERNELAPI
		CCHAR
		KeSetIdealProcessorThread (
		__in PKTHREAD Thread,
		__in CCHAR    Processor
		);

	NTKERNELAPI
		BOOLEAN
		KeSetKernelStackSwapEnable (
		__in BOOLEAN Enable
		);

#endif // (VER_PRODUCTBUILD >= 1381)

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		KeStackAttachProcess (
		__in PKPROCESS    Process,
		__out PKAPC_STATE ApcState
		);

	NTKERNELAPI
		LOGICAL
		FASTCALL
		KeTryToAcquireQueuedSpinLock (
		__in KSPIN_LOCK_QUEUE_NUMBER  Number,
		__in PKIRQL                   OldIrql
		);

	NTKERNELAPI
		VOID
		KeUnstackDetachProcess (
		__in PKAPC_STATE ApcState
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		KeUpdateSystemTime (
		VOID
		);

	NTKERNELAPI
		BOOLEAN
		MmCanFileBeTruncated (
		__in PSECTION_OBJECT_POINTERS     SectionObjectPointer,
		__in PLARGE_INTEGER               NewFileSize
		);

	NTKERNELAPI
		NTSTATUS
		MmCreateSection (
		__out PVOID               *SectionObject,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
		__in PLARGE_INTEGER       MaximumSize,
		__in ULONG                SectionPageProtection,
		__in ULONG                AllocationAttributes,
		__in HANDLE               FileHandle OPTIONAL,
		__in PFILE_OBJECT         FileObject OPTIONAL
		);

	NTKERNELAPI
		BOOLEAN
		MmFlushImageSection (
		__in PSECTION_OBJECT_POINTERS     SectionObjectPointer,
		__in MMFLUSH_TYPE                 FlushType
		);

	NTKERNELAPI
		BOOLEAN
		MmForceSectionClosed (
		__in PSECTION_OBJECT_POINTERS SectionObjectPointer,
		__in BOOLEAN                  DelayClose
		);

#if (VER_PRODUCTBUILD >= 1381)

	NTKERNELAPI
		BOOLEAN
		MmIsRecursiveIoFault (
		VOID
		);

#else

#define MmIsRecursiveIoFault() (                            \
	(PsGetCurrentThread()->DisablePageFaultClustering) |    \
	(PsGetCurrentThread()->ForwardClusterOnly)              \
	)

#endif

	NTKERNELAPI
		NTSTATUS
		MmMapViewOfSection (
		__in PVOID                SectionObject,
		__in PEPROCESS            Process,
		__in __out PVOID            *BaseAddress,
		__in ULONG                ZeroBits,
		__in ULONG                CommitSize,
		__in __out PLARGE_INTEGER   SectionOffset OPTIONAL,
		__in __out PULONG           ViewSize,
		__in SECTION_INHERIT      InheritDisposition,
		__in ULONG                AllocationType,
		__in ULONG                Protect
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		NTSTATUS
		MmPrefetchPages (
		__in ULONG        NumberOfLists,
		__in PREAD_LIST   *ReadLists
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		BOOLEAN
		MmSetAddressRangeModified (
		__in PVOID    Address,
		__in SIZE_T   Length
		);

	NTKERNELAPI
		NTSTATUS
		ObCreateObject (
		__in KPROCESSOR_MODE      ObjectAttributesAccessMode OPTIONAL,
		__in POBJECT_TYPE         ObjectType,
		__in POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
		__in KPROCESSOR_MODE      AccessMode,
		__in __out PVOID            ParseContext OPTIONAL,
		__in ULONG                ObjectSize,
		__in ULONG                PagedPoolCharge OPTIONAL,
		__in ULONG                NonPagedPoolCharge OPTIONAL,
		__out PVOID               *Object
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		VOID
		ObDereferenceSecurityDescriptor (
		__in PSECURITY_DESCRIPTOR SecurityDescriptor,
		__in ULONG                Count
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		ULONG
		ObGetObjectPointerCount (
		__in PVOID Object
		);

	NTKERNELAPI
		NTSTATUS
		ObInsertObject (
		__in PVOID            Object,
		__in PACCESS_STATE    PassedAccessState OPTIONAL,
		__in ACCESS_MASK      DesiredAccess,
		__in ULONG            AdditionalReferences,
		__out PVOID           *ReferencedObject OPTIONAL,
		__out PHANDLE         Handle
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		NTSTATUS
		ObLogSecurityDescriptor (
		__in PSECURITY_DESCRIPTOR     InputSecurityDescriptor,
		__out PSECURITY_DESCRIPTOR    *OutputSecurityDescriptor,
		__in ULONG                    RefBias
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		VOID
		ObMakeTemporaryObject (
		__in PVOID Object
		);

	NTKERNELAPI
		NTSTATUS
		ObOpenObjectByPointer (
		__in PVOID            Object,
		__in ULONG            HandleAttributes,
		__in PACCESS_STATE    PassedAccessState OPTIONAL,
		__in ACCESS_MASK      DesiredAccess OPTIONAL,
		__in POBJECT_TYPE     ObjectType OPTIONAL,
		__in KPROCESSOR_MODE  AccessMode,
		__out PHANDLE         Handle
		);

	NTKERNELAPI
		NTSTATUS
		ObQueryNameString (
		__in PVOID                        Object,
		__out POBJECT_NAME_INFORMATION    ObjectNameInfo,
		__in ULONG                        Length,
		__out PULONG                      ReturnLength
		);

	NTKERNELAPI
		NTSTATUS
		ObQueryObjectAuditingByHandle (
		__in HANDLE       Handle,
		__out PBOOLEAN    GenerateOnClose
		);

	NTKERNELAPI
		NTSTATUS
		ObReferenceObjectByName (
		__in PUNICODE_STRING  ObjectName,
		__in ULONG            Attributes,
		__in PACCESS_STATE    PassedAccessState OPTIONAL,
		__in ACCESS_MASK      DesiredAccess OPTIONAL,
		__in POBJECT_TYPE     ObjectType,
		__in KPROCESSOR_MODE  AccessMode,
		__in __out PVOID        ParseContext OPTIONAL,
		__out PVOID           *Object
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		VOID
		ObReferenceSecurityDescriptor (
		__in PSECURITY_DESCRIPTOR SecurityDescriptor,
		__in ULONG                Count
		);

	NTKERNELAPI
		NTSTATUS
		PoQueueShutdownWorkItem (
		__in PWORK_QUEUE_ITEM WorkItem
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		NTSTATUS
		PsAssignImpersonationToken (
		__in PETHREAD Thread,
		__in HANDLE   Token
		);

#ifdef _X86_
	NTKERNELAPI
		VOID
		PsChargePoolQuota (
		__in PEPROCESS    Process,
		__in POOL_TYPE    PoolType,
		__in ULONG        Amount
		);
#endif // _X86_

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		NTSTATUS
		PsChargeProcessNonPagedPoolQuota (
		__in PEPROCESS Process,
		__in ULONG_PTR Amount
		);

	NTKERNELAPI
		NTSTATUS
		PsChargeProcessPagedPoolQuota (
		__in PEPROCESS Process,
		__in ULONG_PTR Amount
		);

	NTKERNELAPI
		NTSTATUS
		PsChargeProcessPoolQuota (
		__in PEPROCESS Process,
		__in POOL_TYPE PoolType,
		__in ULONG_PTR Amount
		);

#endif // (VER_PRODUCTBUILD >= 2600)

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		VOID
		PsDereferenceImpersonationToken (
		__in PACCESS_TOKEN ImpersonationToken
		);

	NTKERNELAPI
		VOID
		PsDereferencePrimaryToken (
		__in PACCESS_TOKEN PrimaryToken
		);

#else

#define PsDereferenceImpersonationToken(T)  \
	{if (ARGUMENT_PRESENT(T)) {     \
	(ObDereferenceObject((T))); \
	} else {                        \
	;                           \
	}                               \
	}

#define PsDereferencePrimaryToken(T) (ObDereferenceObject((T)))

#endif

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		BOOLEAN
		PsDisableImpersonation (
		__in PETHREAD                 Thread,
		__in PSE_IMPERSONATION_STATE  ImpersonationState
		);

#endif // (VER_PRODUCTBUILD >= 2195)

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		ULONG
		PsGetCurrentProcessSessionId (
		VOID
		);

	NTKERNELAPI
		KPROCESSOR_MODE
		PsGetCurrentThreadPreviousMode (
		VOID
		);

	NTKERNELAPI
		PVOID
		PsGetCurrentThreadStackBase (
		VOID
		);

	NTKERNELAPI
		PVOID
		PsGetCurrentThreadStackLimit (
		VOID
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		LARGE_INTEGER
		PsGetProcessExitTime (
		VOID
		);

	NTKERNELAPI
		NTSTATUS
		PsImpersonateClient (
		__in PETHREAD                     Thread,
		__in PACCESS_TOKEN                Token,
		__in BOOLEAN                      CopyOnOpen,
		__in BOOLEAN                      EffectiveOnly,
		__in SECURITY_IMPERSONATION_LEVEL ImpersonationLevel
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		BOOLEAN
		PsIsSystemThread (
		__in PETHREAD Thread
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		BOOLEAN
		PsIsThreadTerminating (
		__in PETHREAD Thread
		);

	//
	// PsLookupProcessByProcessId returns a referenced pointer to the process
	// that should be dereferenced after use with a call to ObDereferenceObject.
	//
	NTKERNELAPI
		NTSTATUS
		PsLookupProcessByProcessId (
		__in PVOID        ProcessId,
		__out PEPROCESS   *Process
		);

	NTKERNELAPI
		NTSTATUS
		PsLookupProcessThreadByCid (
		__in PCLIENT_ID   Cid,
		__out PEPROCESS   *Process OPTIONAL,
		__out PETHREAD    *Thread
		);

	NTKERNELAPI
		NTSTATUS
		PsLookupThreadByThreadId (
		__in PVOID        UniqueThreadId,
		__out PETHREAD    *Thread
		);

	NTKERNELAPI
		PACCESS_TOKEN
		PsReferenceImpersonationToken (
		__in PETHREAD                         Thread,
		__out PBOOLEAN                        CopyOnOpen,
		__out PBOOLEAN                        EffectiveOnly,
		__out PSECURITY_IMPERSONATION_LEVEL   ImpersonationLevel
		);

	NTKERNELAPI
		PACCESS_TOKEN
		PsReferencePrimaryToken (
		__in PEPROCESS Process
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		PsRestoreImpersonation (
		__in PETHREAD                 Thread,
		__in PSE_IMPERSONATION_STATE  ImpersonationState
		);

#endif // (VER_PRODUCTBUILD >= 2195)

#ifdef _X86_
	NTKERNELAPI
		VOID
		PsReturnPoolQuota (
		__in PEPROCESS    Process,
		__in POOL_TYPE    PoolType,
		__in ULONG        Amount
		);
#endif // _X86_

#if (VER_PRODUCTBUILD >= 1381)

	NTKERNELAPI
		VOID
		PsRevertToSelf (
		VOID
		);

#endif // (VER_PRODUCTBUILD >= 1381)

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlAbsoluteToSelfRelativeSD (
		__in PSECURITY_DESCRIPTOR     AbsoluteSecurityDescriptor,
		__in __out PSECURITY_DESCRIPTOR SelfRelativeSecurityDescriptor,
		__in PULONG                   BufferLength
		);

#ifdef _X86_
	NTSYSAPI
		PVOID
		NTAPI
		RtlAllocateHeap (
		__in HANDLE  HeapHandle,
		__in ULONG   Flags,
		__in ULONG   Size
		);
#endif // _X86_

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlCompressBuffer (
		__in USHORT   CompressionFormatAndEngine,
		__in PUCHAR   UncompressedBuffer,
		__in ULONG    UncompressedBufferSize,
		__out PUCHAR  CompressedBuffer,
		__in ULONG    CompressedBufferSize,
		__in ULONG    UncompressedChunkSize,
		__out PULONG  FinalCompressedSize,
		__in PVOID    WorkSpace
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlCompressChunks (
		__in PUCHAR                       UncompressedBuffer,
		__in ULONG                        UncompressedBufferSize,
		__out PUCHAR                      CompressedBuffer,
		__in ULONG                        CompressedBufferSize,
		__in __out PCOMPRESSED_DATA_INFO    CompressedDataInfo,
		__in ULONG                        CompressedDataInfoLength,
		__in PVOID                        WorkSpace
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlConvertSidToUnicodeString (
		__out PUNICODE_STRING DestinationString,
		__in PSID             Sid,
		__in BOOLEAN          AllocateDestinationString
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlCopySid (
		__in ULONG   Length,
		__in PSID    Destination,
		__in PSID    Source
		);

#if (VER_PRODUCTBUILD < 7600)
	NTSYSAPI
		HANDLE
		NTAPI
		RtlCreateHeap (
		__in ULONG Flags,
		__in PVOID Base,
		__in ULONG Reserve, 
		__in ULONG Commit, 
		__in ULONG Lock, 
		__in PVOID RtlHeapParams
		);
#endif

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlDecompressBuffer (
		__in USHORT   CompressionFormat,
		__out PUCHAR  UncompressedBuffer,
		__in ULONG    UncompressedBufferSize,
		__in PUCHAR   CompressedBuffer,
		__in ULONG    CompressedBufferSize,
		__out PULONG  FinalUncompressedSize
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlDecompressChunks (
		__out PUCHAR                  UncompressedBuffer,
		__in ULONG                    UncompressedBufferSize,
		__in PUCHAR                   CompressedBuffer,
		__in ULONG                    CompressedBufferSize,
		__in PUCHAR                   CompressedTail,
		__in ULONG                    CompressedTailSize,
		__in PCOMPRESSED_DATA_INFO    CompressedDataInfo
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlDecompressFragment (
		__in USHORT   CompressionFormat,
		__out PUCHAR  UncompressedFragment,
		__in ULONG    UncompressedFragmentSize,
		__in PUCHAR   CompressedBuffer,
		__in ULONG    CompressedBufferSize,
		__in ULONG    FragmentOffset,
		__out PULONG  FinalUncompressedSize,
		__in PVOID    WorkSpace
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlDescribeChunk (
		__in USHORT       CompressionFormat,
		__in __out PUCHAR   *CompressedBuffer,
		__in PUCHAR       EndOfCompressedBufferPlus1,
		__out PUCHAR      *ChunkBuffer,
		__out PULONG      ChunkSize
		);

#if (VER_PRODUCTBUILD < 7600)
	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlDestroyHeap (
		__in HANDLE  HeapHandle
		);
#endif

	NTSYSAPI
		BOOLEAN
		NTAPI
		RtlEqualSid (
		__in PSID Sid1,
		__in PSID Sid2
		);

#ifdef _X86_
	NTSYSAPI
		VOID
		NTAPI
		RtlFillMemoryUlong (
		__in PVOID    Destination,
		__in ULONG    Length,
		__in ULONG    Fill
		);
#endif // _X86_

	NTSYSAPI
		BOOLEAN
		NTAPI
		RtlFreeHeap (
		__in HANDLE  HeapHandle,
		__in ULONG   Flags,
		__in PVOID   P
		);

#if (VER_PRODUCTBUILD < 7600)
	NTSYSAPI
		VOID
		NTAPI
		RtlGenerate8dot3Name (
		__in PUNICODE_STRING              Name,
		__in BOOLEAN                      AllowExtendedCharacters,
		__in __out PGENERATE_NAME_CONTEXT   Context,
		__out PUNICODE_STRING             Name8dot3
		);
#endif

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlGetCompressionWorkSpaceSize (
		__in USHORT   CompressionFormatAndEngine,
		__out PULONG  CompressBufferWorkSpaceSize,
		__out PULONG  CompressFragmentWorkSpaceSize
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlGetDaclSecurityDescriptor (
		__in PSECURITY_DESCRIPTOR SecurityDescriptor,
		__out PBOOLEAN            DaclPresent,
		__out PACL                *Dacl,
		__out PBOOLEAN            DaclDefaulted
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlGetGroupSecurityDescriptor (
		__in PSECURITY_DESCRIPTOR SecurityDescriptor,
		__out PSID                *Group,
		__out PBOOLEAN            GroupDefaulted
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		ULONG
		NTAPI
		RtlGetNtGlobalFlags (
		VOID
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlGetOwnerSecurityDescriptor (
		__in PSECURITY_DESCRIPTOR SecurityDescriptor,
		__out PSID                *Owner,
		__out PBOOLEAN            OwnerDefaulted
		);

	//
	// This function returns a PIMAGE_NT_HEADERS,
	// see the standard include file winnt.h
	//
	NTSYSAPI
		PVOID
		NTAPI
		RtlImageNtHeader (
		__in PVOID BaseAddress
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlInitializeSid (
		__in __out PSID                     Sid,
		__in PSID_IDENTIFIER_AUTHORITY    IdentifierAuthority,
		__in UCHAR                        SubAuthorityCount
		);

#if (VER_PRODUCTBUILD < 7600)
	NTSYSAPI
		BOOLEAN
		NTAPI
		RtlIsNameLegalDOS8Dot3 (
		__in PUNICODE_STRING UnicodeName,
		__in PANSI_STRING    AnsiName,
		PBOOLEAN           Unknown
		);

	NTSYSAPI
		ULONG
		NTAPI
		RtlLengthRequiredSid (
		__in UCHAR SubAuthorityCount
		);
#endif

	NTSYSAPI
		ULONG
		NTAPI
		RtlLengthSid (
		__in PSID Sid
		);

	NTSYSAPI
		ULONG
		NTAPI
		RtlNtStatusToDosError (
		__in NTSTATUS Status
		);

#define RtlOemStringToCountedUnicodeSize(STRING) (                    \
	(ULONG)(RtlOemStringToUnicodeSize(STRING) - sizeof(UNICODE_NULL)) \
	)

#define RtlOemStringToUnicodeSize(STRING) (                \
	NLS_MB_OEM_CODE_PAGE_TAG ?                             \
	RtlxOemStringToUnicodeSize(STRING) :                   \
	((STRING)->Length + sizeof(ANSI_NULL)) * sizeof(WCHAR) \
	)

#if (VER_PRODUCTBUILD < 7600)
	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlOemStringToUnicodeString (
		__out PUNICODE_STRING DestinationString,
		__in POEM_STRING      SourceString,
		__in BOOLEAN          AllocateDestinationString
		);
#endif

	NTSYSAPI
		ULONG
		NTAPI
		RtlRandom (
		__in PULONG Seed
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTSYSAPI
		ULONG
		NTAPI
		RtlRandomEx (
		__in PULONG Seed
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlReserveChunk (
		__in USHORT       CompressionFormat,
		__in __out PUCHAR   *CompressedBuffer,
		__in PUCHAR       EndOfCompressedBufferPlus1,
		__out PUCHAR      *ChunkBuffer,
		__in ULONG        ChunkSize
		);

	NTSYSAPI
		VOID
		NTAPI
		RtlSecondsSince1970ToTime (
		__in ULONG            SecondsSince1970,
		__out PLARGE_INTEGER  Time
		);

	NTSYSAPI
		VOID
		NTAPI
		RtlSecondsSince1980ToTime (
		__in ULONG            SecondsSince1980,
		__out PLARGE_INTEGER  Time
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlSelfRelativeToAbsoluteSD (
		__in PSECURITY_DESCRIPTOR     SelfRelativeSD,
		__out PSECURITY_DESCRIPTOR    AbsoluteSD,
		__in PULONG                   AbsoluteSDSize,
		__in PACL                     Dacl,
		__in PULONG                   DaclSize,
		__in PACL                     Sacl,
		__in PULONG                   SaclSize,
		__in PSID                     Owner,
		__in PULONG                   OwnerSize,
		__in PSID                     PrimaryGroup,
		__in PULONG                   PrimaryGroupSize
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlSetGroupSecurityDescriptor (
		__in __out PSECURITY_DESCRIPTOR SecurityDescriptor,
		__in PSID                     Group,
		__in BOOLEAN                  GroupDefaulted
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlSetOwnerSecurityDescriptor (
		__in __out PSECURITY_DESCRIPTOR SecurityDescriptor,
		__in PSID                     Owner,
		__in BOOLEAN                  OwnerDefaulted
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlSetSaclSecurityDescriptor (
		__in __out PSECURITY_DESCRIPTOR SecurityDescriptor,
		__in BOOLEAN                  SaclPresent,
		__in PACL                     Sacl,
		__in BOOLEAN                  SaclDefaulted
		);

	NTSYSAPI
		PUCHAR
		NTAPI
		RtlSubAuthorityCountSid (
		__in PSID Sid
		);

	NTSYSAPI
		PULONG
		NTAPI
		RtlSubAuthoritySid (
		__in PSID    Sid,
		__in ULONG   SubAuthority
		);

	NTSYSAPI
		BOOLEAN
		NTAPI
		RtlTimeToSecondsSince1970 (
		__in PLARGE_INTEGER   Time,
		__out PULONG          SecondsSince1970
		);

	NTSYSAPI
		BOOLEAN
		NTAPI
		RtlTimeToSecondsSince1980 (
		__in PLARGE_INTEGER   Time,
		__out PULONG          SecondsSince1980
		);

#define RtlUnicodeStringToOemSize(STRING) (                   \
	NLS_MB_OEM_CODE_PAGE_TAG ?                                \
	RtlxUnicodeStringToOemSize(STRING) :                      \
	((STRING)->Length + sizeof(UNICODE_NULL)) / sizeof(WCHAR) \
	)

#if (VER_PRODUCTBUILD < 7600)
	NTSYSAPI
		NTSTATUS
		NTAPI
		RtlUnicodeStringToOemString (
		__out POEM_STRING     DestinationString,
		__in PUNICODE_STRING  SourceString,
		__in BOOLEAN          AllocateDestinationString
		);

	NTSYSAPI
		BOOLEAN
		NTAPI
		RtlValidSid (
		__in PSID Sid
		);

	NTSYSAPI
		ULONG
		NTAPI
		RtlxOemStringToUnicodeSize (
		__in POEM_STRING OemString
		);

	//NTSYSAPI
	//	ULONG
	//	NTAPI
	//	RtlxUnicodeStringToAnsiSize (
	//	__in PUNICODE_STRING UnicodeString
	//	);

	NTSYSAPI
		ULONG
		NTAPI
		RtlxUnicodeStringToOemSize (
		__in PUNICODE_STRING UnicodeString
		);
#endif

	NTKERNELAPI
		NTSTATUS
		SeAppendPrivileges (
		PACCESS_STATE   AccessState,
		PPRIVILEGE_SET  Privileges
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		SeAuditHardLinkCreation (
		__in PUNICODE_STRING  FileName,
		__in PUNICODE_STRING  LinkName,
		__in BOOLEAN          Success
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		BOOLEAN
		SeAuditingFileEvents (
		__in BOOLEAN              AccessGranted,
		__in PSECURITY_DESCRIPTOR SecurityDescriptor
		);

	NTKERNELAPI
		BOOLEAN
		SeAuditingFileOrGlobalEvents (
		__in BOOLEAN                      AccessGranted,
		__in PSECURITY_DESCRIPTOR         SecurityDescriptor,
		__in PSECURITY_SUBJECT_CONTEXT    SubjectContext
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		BOOLEAN
		SeAuditingHardLinkEvents (
		__in BOOLEAN              AccessGranted,
		__in PSECURITY_DESCRIPTOR SecurityDescriptor
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		SeCaptureSubjectContext (
		__out PSECURITY_SUBJECT_CONTEXT SubjectContext
		);

	NTKERNELAPI
		NTSTATUS
		SeCreateAccessState (
		__out PACCESS_STATE   AccessState,
		__in PVOID            AuxData,
		__in ACCESS_MASK      AccessMask,
		__in PGENERIC_MAPPING Mapping
		);

	NTKERNELAPI
		NTSTATUS
		SeCreateClientSecurity (
		__in PETHREAD                     Thread,
		__in PSECURITY_QUALITY_OF_SERVICE QualityOfService,
		__in BOOLEAN                      RemoteClient,
		__out PSECURITY_CLIENT_CONTEXT    ClientContext
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		SeCreateClientSecurityFromSubjectContext (
		__in PSECURITY_SUBJECT_CONTEXT    SubjectContext,
		__in PSECURITY_QUALITY_OF_SERVICE QualityOfService,
		__in BOOLEAN                      ServerIsRemote,
		__out PSECURITY_CLIENT_CONTEXT    ClientContext
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		SeDeleteAccessState (
		__in PACCESS_STATE AccessState
		);

#define SeDeleteClientSecurity(C)  {                                           \
	if (SeTokenType((C)->ClientToken) == TokenPrimary) {               \
	PsDereferencePrimaryToken( (C)->ClientToken );                 \
	} else {                                                           \
	PsDereferenceImpersonationToken( (C)->ClientToken );           \
	}                                                                  \
	}

	NTKERNELAPI
		VOID
		SeDeleteObjectAuditAlarm (
		__in PVOID    Object,
		__in HANDLE   Handle
		);

#define SeEnableAccessToExports() SeExports = *(PSE_EXPORTS *)SeExports;

#if (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		NTSTATUS
		SeFilterToken (
		__in PACCESS_TOKEN        ExistingToken,
		__in ULONG                Flags,
		__in PTOKEN_GROUPS        SidsToDisable OPTIONAL,
		__in PTOKEN_PRIVILEGES    PrivilegesToDelete OPTIONAL,
		__in PTOKEN_GROUPS        RestrictedSids OPTIONAL,
		__out PACCESS_TOKEN       *FilteredToken
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTKERNELAPI
		VOID
		SeFreePrivileges (
		__in PPRIVILEGE_SET Privileges
		);

	NTKERNELAPI
		VOID
		SeImpersonateClient (
		__in PSECURITY_CLIENT_CONTEXT ClientContext,
		__in PETHREAD                 ServerThread OPTIONAL
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		SeImpersonateClientEx (
		__in PSECURITY_CLIENT_CONTEXT ClientContext,
		__in PETHREAD                 ServerThread OPTIONAL
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		VOID
		SeLockSubjectContext (
		__in PSECURITY_SUBJECT_CONTEXT SubjectContext
		);

	NTKERNELAPI
		NTSTATUS
		SeMarkLogonSessionForTerminationNotification (
		__in PLUID LogonId
		);

	NTKERNELAPI
		VOID
		SeOpenObjectAuditAlarm (
		__in PUNICODE_STRING      ObjectTypeName,
		__in PVOID                Object OPTIONAL,
		__in PUNICODE_STRING      AbsoluteObjectName OPTIONAL,
		__in PSECURITY_DESCRIPTOR SecurityDescriptor,
		__in PACCESS_STATE        AccessState,
		__in BOOLEAN              ObjectCreated,
		__in BOOLEAN              AccessGranted,
		__in KPROCESSOR_MODE      AccessMode,
		__out PBOOLEAN            GenerateOnClose
		);

	NTKERNELAPI
		VOID
		SeOpenObjectForDeleteAuditAlarm (
		__in PUNICODE_STRING      ObjectTypeName,
		__in PVOID                Object OPTIONAL,
		__in PUNICODE_STRING      AbsoluteObjectName OPTIONAL,
		__in PSECURITY_DESCRIPTOR SecurityDescriptor,
		__in PACCESS_STATE        AccessState,
		__in BOOLEAN              ObjectCreated,
		__in BOOLEAN              AccessGranted,
		__in KPROCESSOR_MODE      AccessMode,
		__out PBOOLEAN            GenerateOnClose
		);

	NTKERNELAPI
		BOOLEAN
		SePrivilegeCheck (
		__in __out PPRIVILEGE_SET           RequiredPrivileges,
		__in PSECURITY_SUBJECT_CONTEXT    SubjectContext,
		__in KPROCESSOR_MODE              AccessMode
		);

	NTKERNELAPI
		NTSTATUS
		SeQueryAuthenticationIdToken (
		__in PACCESS_TOKEN    Token,
		__out PLUID           LogonId
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		SeQueryInformationToken (
		__in PACCESS_TOKEN           Token,
		__in TOKEN_INFORMATION_CLASS TokenInformationClass,
		__out PVOID                  *TokenInformation
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		SeQuerySecurityDescriptorInfo (
		__in PSECURITY_INFORMATION    SecurityInformation,
		__out PSECURITY_DESCRIPTOR    SecurityDescriptor,
		__in __out PULONG               Length,
		__in PSECURITY_DESCRIPTOR     *ObjectsSecurityDescriptor
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		SeQuerySessionIdToken (
		__in PACCESS_TOKEN    Token,
		__in PULONG           SessionId
		);

#endif // (VER_PRODUCTBUILD >= 2195)

#if (VER_PRODUCTBUILD<7600)
#define SeQuerySubjectContextToken( SubjectContext )                \
	( ARGUMENT_PRESENT(                                             \
	((PSECURITY_SUBJECT_CONTEXT) SubjectContext)->ClientToken   \
	) ?                                                         \
	((PSECURITY_SUBJECT_CONTEXT) SubjectContext)->ClientToken :     \
	((PSECURITY_SUBJECT_CONTEXT) SubjectContext)->PrimaryToken )
#endif

	typedef NTSTATUS (*PSE_LOGON_SESSION_TERMINATED_ROUTINE) (
		__in PLUID LogonId
		);

	NTKERNELAPI
		NTSTATUS
		SeRegisterLogonSessionTerminatedRoutine (
		__in PSE_LOGON_SESSION_TERMINATED_ROUTINE CallbackRoutine
		);

	NTKERNELAPI
		VOID
		SeReleaseSubjectContext (
		__in PSECURITY_SUBJECT_CONTEXT SubjectContext
		);

#if (VER_PRODUCTBUILD < 7600)
	NTKERNELAPI
		VOID
		SeSetAccessStateGenericMapping (
		PACCESS_STATE       AccessState,
		PGENERIC_MAPPING    GenericMapping
		);
#endif

	NTKERNELAPI
		NTSTATUS
		SeSetSecurityDescriptorInfo (
		__in PVOID                    Object OPTIONAL,
		__in PSECURITY_INFORMATION    SecurityInformation,
		__in PSECURITY_DESCRIPTOR     SecurityDescriptor,
		__in __out PSECURITY_DESCRIPTOR *ObjectsSecurityDescriptor,
		__in POOL_TYPE                PoolType,
		__in PGENERIC_MAPPING         GenericMapping
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		NTSTATUS
		SeSetSecurityDescriptorInfoEx (
		__in PVOID                    Object OPTIONAL,
		__in PSECURITY_INFORMATION    SecurityInformation,
		__in PSECURITY_DESCRIPTOR     ModificationDescriptor,
		__in __out PSECURITY_DESCRIPTOR *ObjectsSecurityDescriptor,
		__in ULONG                    AutoInheritFlags,
		__in POOL_TYPE                PoolType,
		__in PGENERIC_MAPPING         GenericMapping
		);

	NTKERNELAPI
		BOOLEAN
		SeTokenIsAdmin (
		__in PACCESS_TOKEN Token
		);

	NTKERNELAPI
		BOOLEAN
		SeTokenIsRestricted (
		__in PACCESS_TOKEN Token
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTKERNELAPI
		TOKEN_TYPE
		SeTokenType (
		__in PACCESS_TOKEN Token
		);

	NTKERNELAPI
		VOID
		SeUnlockSubjectContext (
		__in PSECURITY_SUBJECT_CONTEXT SubjectContext
		);

	NTKERNELAPI
		NTSTATUS
		SeUnregisterLogonSessionTerminatedRoutine (
		__in PSE_LOGON_SESSION_TERMINATED_ROUTINE CallbackRoutine
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwAdjustPrivilegesToken (
		__in HANDLE               TokenHandle,
		__in BOOLEAN              DisableAllPrivileges,
		__in PTOKEN_PRIVILEGES    NewState,
		__in ULONG                BufferLength,
		__out PTOKEN_PRIVILEGES   PreviousState OPTIONAL,
		__out PULONG              ReturnLength
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwAlertThread (
		__in HANDLE ThreadHandle
		);

#ifdef _X86_
	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwAllocateVirtualMemory (
		__in HANDLE       ProcessHandle,
		__in __out PVOID    *BaseAddress,
		__in ULONG        ZeroBits,
		__in __out PSIZE_T  RegionSize,
		__in ULONG        AllocationType,
		__in ULONG        Protect
		);
#endif // _X86_

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwAccessCheckAndAuditAlarm (
		__in PUNICODE_STRING      SubsystemName,
		__in PVOID                HandleId,
		__in PUNICODE_STRING      ObjectTypeName,
		__in PUNICODE_STRING      ObjectName,
		__in PSECURITY_DESCRIPTOR SecurityDescriptor,
		__in ACCESS_MASK          DesiredAccess,
		__in PGENERIC_MAPPING     GenericMapping,
		__in BOOLEAN              ObjectCreation,
		__out PACCESS_MASK        GrantedAccess,
		__out PBOOLEAN            AccessStatus,
		__out PBOOLEAN            GenerateOnClose
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwCancelIoFile (
		__in HANDLE               FileHandle,
		__out PIO_STATUS_BLOCK    IoStatusBlock
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwClearEvent (
		__in HANDLE EventHandle
		);

#if (VER_PRODUCTBUILD < 7600)
	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwConnectPort (
		__out PHANDLE                     ClientPortHandle,
		__in PUNICODE_STRING              ServerPortName,
		__in PSECURITY_QUALITY_OF_SERVICE SecurityQos,
		__in __out PLPC_SECTION_WRITE       ClientSharedMemory OPTIONAL,
		__in __out PLPC_SECTION_READ        ServerSharedMemory OPTIONAL,
		__out PULONG                      MaximumMessageLength OPTIONAL,
		__in __out PVOID                    ConnectionInfo OPTIONAL,
		__in __out PULONG                   ConnectionInfoLength OPTIONAL
		);
#endif

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwCloseObjectAuditAlarm (
		__in PUNICODE_STRING  SubsystemName,
		__in PVOID            HandleId,
		__in BOOLEAN          GenerateOnClose
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwCreateEvent (
		__out PHANDLE             EventHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
		__in EVENT_TYPE           EventType,
		__in BOOLEAN              InitialState
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwCreateSection (
		__out PHANDLE             SectionHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
		__in PLARGE_INTEGER       MaximumSize OPTIONAL,
		__in ULONG                SectionPageProtection,
		__in ULONG                AllocationAttributes,
		__in HANDLE               FileHandle OPTIONAL
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwCreateSymbolicLinkObject (
		__out PHANDLE             SymbolicLinkHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes,
		__in PUNICODE_STRING      TargetName
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwDeleteFile (
		__in POBJECT_ATTRIBUTES ObjectAttributes
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwDeleteValueKey (
		__in HANDLE           Handle,
		__in PUNICODE_STRING  Name
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwDeviceIoControlFile (
		__in HANDLE               FileHandle,
		__in HANDLE               Event OPTIONAL,
		__in PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
		__in PVOID                ApcContext OPTIONAL,
		__out PIO_STATUS_BLOCK    IoStatusBlock,
		__in ULONG                IoControlCode,
		__in PVOID                InputBuffer OPTIONAL,
		__in ULONG                InputBufferLength,
		__out PVOID               OutputBuffer OPTIONAL,
		__in ULONG                OutputBufferLength
		);

	//
	// If using ZwDisplayString during boot on Windows 2000 or later you must
	// first call InbvEnableDisplayString.
	//
	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwDisplayString (
		__in PUNICODE_STRING String
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwDuplicateObject (
		__in HANDLE       SourceProcessHandle,
		__in HANDLE       SourceHandle,
		__in HANDLE       TargetProcessHandle OPTIONAL,
		__out PHANDLE     TargetHandle OPTIONAL,
		__in ACCESS_MASK  DesiredAccess,
		__in ULONG        HandleAttributes,
		__in ULONG        Options
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwDuplicateToken (
		__in HANDLE               ExistingTokenHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes,
		__in BOOLEAN              EffectiveOnly,
		__in TOKEN_TYPE           TokenType,
		__out PHANDLE             NewTokenHandle
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwFlushInstructionCache (
		__in HANDLE   ProcessHandle,
		__in PVOID    BaseAddress OPTIONAL,
		__in ULONG    FlushSize
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwFlushVirtualMemory (
		__in HANDLE               ProcessHandle,
		__in __out PVOID            *BaseAddress,
		__in __out PSIZE_T          RegionSize,
		__out PIO_STATUS_BLOCK    IoStatusBlock
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwFreeVirtualMemory (
		__in HANDLE       ProcessHandle,
		__in __out PVOID    *BaseAddress,
		__in __out PSIZE_T  RegionSize,
		__in ULONG        FreeType
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwFsControlFile (
		__in HANDLE               FileHandle,
		__in HANDLE               Event OPTIONAL,
		__in PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
		__in PVOID                ApcContext OPTIONAL,
		__out PIO_STATUS_BLOCK    IoStatusBlock,
		__in ULONG                FsControlCode,
		__in PVOID                InputBuffer OPTIONAL,
		__in ULONG                InputBufferLength,
		__out PVOID               OutputBuffer OPTIONAL,
		__in ULONG                OutputBufferLength
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwInitiatePowerAction (
		__in POWER_ACTION         SystemAction,
		__in SYSTEM_POWER_STATE   MinSystemState,
		__in ULONG                Flags,
		__in BOOLEAN              Asynchronous
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwLoadDriver (
		// "\\Registry\\Machine\\System\\CurrentControlSet\\Services\\<DriverName>"
		__in PUNICODE_STRING RegistryPath
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwLoadKey (
		__in POBJECT_ATTRIBUTES KeyObjectAttributes,
		__in POBJECT_ATTRIBUTES FileObjectAttributes
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwNotifyChangeKey (
		__in HANDLE               KeyHandle,
		__in HANDLE               EventHandle OPTIONAL,
		__in PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
		__in PVOID                ApcContext OPTIONAL,
		__out PIO_STATUS_BLOCK    IoStatusBlock,
		__in ULONG                NotifyFilter,
		__in BOOLEAN              WatchSubtree,
		__in PVOID                Buffer,
		__in ULONG                BufferLength,
		__in BOOLEAN              Asynchronous
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenDirectoryObject (
		__out PHANDLE             DirectoryHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenEvent (
		__out PHANDLE             EventHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenProcess (
		__out PHANDLE             ProcessHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes,
		__in PCLIENT_ID           ClientId OPTIONAL
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenProcessToken (
		__in HANDLE       ProcessHandle,
		__in ACCESS_MASK  DesiredAccess,
		__out PHANDLE     TokenHandle
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenProcessTokenEx (
		__in HANDLE       ProcessHandle,
		__in ACCESS_MASK  DesiredAccess,
		__in ULONG        HandleAttributes,
		__out PHANDLE     TokenHandle
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenThread (
		__out PHANDLE             ThreadHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes,
		__in PCLIENT_ID           ClientId
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenThreadToken (
		__in HANDLE       ThreadHandle,
		__in ACCESS_MASK  DesiredAccess,
		__in BOOLEAN      OpenAsSelf,
		__out PHANDLE     TokenHandle
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenThreadTokenEx (
		__in HANDLE       ThreadHandle,
		__in ACCESS_MASK  DesiredAccess,
		__in BOOLEAN      OpenAsSelf,
		__in ULONG        HandleAttributes,
		__out PHANDLE     TokenHandle
		);

#endif // (VER_PRODUCTBUILD >= 2600)

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwPowerInformation (
		__in POWER_INFORMATION_LEVEL  PowerInformationLevel,
		__in PVOID                    InputBuffer OPTIONAL,
		__in ULONG                    InputBufferLength,
		__out PVOID                   OutputBuffer OPTIONAL,
		__in ULONG                    OutputBufferLength
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwPulseEvent (
		__in HANDLE   EventHandle,
		__out PULONG  PreviousState OPTIONAL
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryDefaultLocale (
		__in BOOLEAN  ThreadOrSystem,
		__out PLCID   Locale
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryDefaultUILanguage (
		__out LANGID *LanguageId
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryDirectoryFile (
		__in HANDLE                   FileHandle,
		__in HANDLE                   Event OPTIONAL,
		__in PIO_APC_ROUTINE          ApcRoutine OPTIONAL,
		__in PVOID                    ApcContext OPTIONAL,
		__out PIO_STATUS_BLOCK        IoStatusBlock,
		__out PVOID                   FileInformation,
		__in ULONG                    Length,
		__in FILE_INFORMATION_CLASS   FileInformationClass,
		__in BOOLEAN                  ReturnSingleEntry,
		__in PUNICODE_STRING          FileName OPTIONAL,
		__in BOOLEAN                  RestartScan
		);

#if (VER_PRODUCTBUILD >= 2195 && VER_PRODUCTBUILD < 7600)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryDirectoryObject (
		__in HANDLE       DirectoryHandle,
		__out PVOID       Buffer,
		__in ULONG        Length,
		__in BOOLEAN      ReturnSingleEntry,
		__in BOOLEAN      RestartScan,
		__in __out PULONG   Context,
		__out PULONG      ReturnLength OPTIONAL
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryEaFile (
		__in HANDLE               FileHandle,
		__out PIO_STATUS_BLOCK    IoStatusBlock,
		__out PVOID               Buffer,
		__in ULONG                Length,
		__in BOOLEAN              ReturnSingleEntry,
		__in PVOID                EaList OPTIONAL,
		__in ULONG                EaListLength,
		__in PULONG               EaIndex OPTIONAL,
		__in BOOLEAN              RestartScan
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryInformationProcess (
		__in HANDLE           ProcessHandle,
		__in PROCESSINFOCLASS ProcessInformationClass,
		__out PVOID           ProcessInformation,
		__in ULONG            ProcessInformationLength,
		__out PULONG          ReturnLength OPTIONAL
		);

#if (VER_PRODUCTBUILD >= 2600)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryInformationThread (
		__in HANDLE           ThreadHandle,
		__in THREADINFOCLASS  ThreadInformationClass,
		__out PVOID           ThreadInformation,
		__in ULONG            ThreadInformationLength,
		__out PULONG          ReturnLength OPTIONAL
		);

#endif // (VER_PRODUCTBUILD >= 2600)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryInformationToken (
		__in HANDLE                   TokenHandle,
		__in TOKEN_INFORMATION_CLASS  TokenInformationClass,
		__out PVOID                   TokenInformation,
		__in ULONG                    TokenInformationLength,
		__out PULONG                  ReturnLength
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryInstallUILanguage (
		__out LANGID *LanguageId
		);

#endif // (VER_PRODUCTBUILD >= 2195)

#if (VER_PRODUCTBUILD < 7600)
	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryObject (
		__in HANDLE               ObjectHandle,
		__in OBJECT_INFO_CLASS    ObjectInformationClass,
		__out PVOID               ObjectInformation,
		__in ULONG                Length,
		__out PULONG              ResultLength
		);
#endif

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQuerySection (
		__in HANDLE                       SectionHandle,
		__in SECTION_INFORMATION_CLASS    SectionInformationClass,
		__out PVOID                       SectionInformation,
		__in ULONG                        SectionInformationLength,
		__out PULONG                      ResultLength OPTIONAL
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQuerySecurityObject (
		__in HANDLE                   FileHandle,
		__in SECURITY_INFORMATION     SecurityInformation,
		__out PSECURITY_DESCRIPTOR    SecurityDescriptor,
		__in ULONG                    Length,
		__out PULONG                  ResultLength
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQuerySystemInformation (
		__in SYSTEM_INFORMATION_CLASS SystemInformationClass,
		__out PVOID                   SystemInformation,
		__in ULONG                    Length,
		__out PULONG                  ReturnLength
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQueryVolumeInformationFile (
		__in HANDLE               FileHandle,
		__out PIO_STATUS_BLOCK    IoStatusBlock,
		__out PVOID               FsInformation,
		__in ULONG                Length,
		__in FS_INFORMATION_CLASS FsInformationClass
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwReplaceKey (
		__in POBJECT_ATTRIBUTES   NewFileObjectAttributes,
		__in HANDLE               KeyHandle,
		__in POBJECT_ATTRIBUTES   OldFileObjectAttributes
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwRequestWaitReplyPort (
		__in HANDLE           PortHandle,
		__in PLPC_MESSAGE     Request,
		__out PLPC_MESSAGE    Reply
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwResetEvent (
		__in HANDLE   EventHandle,
		__out PULONG  PreviousState OPTIONAL
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwRestoreKey (
		__in HANDLE   KeyHandle,
		__in HANDLE   FileHandle,
		__in ULONG    Flags
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSaveKey (
		__in HANDLE KeyHandle,
		__in HANDLE FileHandle
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetDefaultLocale (
		__in BOOLEAN  ThreadOrSystem,
		__in LCID     Locale
		);

#if (VER_PRODUCTBUILD >= 2195 && VER_PRODUCTBUILD < 7600)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetDefaultUILanguage (
		__in LANGID LanguageId
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetEaFile (
		__in HANDLE               FileHandle,
		__out PIO_STATUS_BLOCK    IoStatusBlock,
		__out PVOID               Buffer,
		__in ULONG                Length
		);

#endif // (VER_PRODUCTBUILD >= 2195)

#if (VER_PRODUCTBUILD < 7600)
	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetEvent (
		__in HANDLE   EventHandle,
		__out PULONG  PreviousState OPTIONAL
		);
#endif

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetInformationObject (
		__in HANDLE               ObjectHandle,
		__in OBJECT_INFO_CLASS    ObjectInformationClass,
		__in PVOID                ObjectInformation,
		__in ULONG                ObjectInformationLength
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetInformationProcess (
		__in HANDLE           ProcessHandle,
		__in PROCESSINFOCLASS ProcessInformationClass,
		__in PVOID            ProcessInformation,
		__in ULONG            ProcessInformationLength
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetSecurityObject (
		__in HANDLE               Handle,
		__in SECURITY_INFORMATION SecurityInformation,
		__in PSECURITY_DESCRIPTOR SecurityDescriptor
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetSystemInformation (
		__in SYSTEM_INFORMATION_CLASS SystemInformationClass,
		__in PVOID                    SystemInformation,
		__in ULONG                    Length
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetSystemTime (
		__in PLARGE_INTEGER   NewTime,
		__out PLARGE_INTEGER  OldTime OPTIONAL
		);

#if (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwSetVolumeInformationFile (
		__in HANDLE               FileHandle,
		__out PIO_STATUS_BLOCK    IoStatusBlock,
		__in PVOID                FsInformation,
		__in ULONG                Length,
		__in FS_INFORMATION_CLASS FsInformationClass
		);

#endif // (VER_PRODUCTBUILD >= 2195)

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwTerminateProcess (
		__in HANDLE   ProcessHandle OPTIONAL,
		__in NTSTATUS ExitStatus
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwUnloadDriver (
		// "\\Registry\\Machine\\System\\CurrentControlSet\\Services\\<DriverName>"
		__in PUNICODE_STRING RegistryPath
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwUnloadKey (
		__in POBJECT_ATTRIBUTES KeyObjectAttributes
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwWaitForSingleObject (
		__in HANDLE           Handle,
		__in BOOLEAN          Alertable,
		__in PLARGE_INTEGER   Timeout OPTIONAL
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwWaitForMultipleObjects (
		__in ULONG            HandleCount,
		__in PHANDLE          Handles,
		__in WAIT_TYPE        WaitType,
		__in BOOLEAN          Alertable,
		__in PLARGE_INTEGER   Timeout OPTIONAL
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwYieldExecution (
		VOID
		);

	//
	// Below is stuff that is included __in the Windows 2000 DDK but is missing __in
	// the Windows NT 4.0 DDK
	//

#if (VER_PRODUCTBUILD < 2195)

	NTSYSAPI
		VOID
		NTAPI
		HalMakeBeep (
		__in ULONG Frequency
		);

#ifndef IoCopyCurrentIrpStackLocationToNext
#define IoCopyCurrentIrpStackLocationToNext( Irp ) { \
	PIO_STACK_LOCATION irpSp; \
	PIO_STACK_LOCATION nextIrpSp; \
	irpSp = IoGetCurrentIrpStackLocation( (Irp) ); \
	nextIrpSp = IoGetNextIrpStackLocation( (Irp) ); \
	RtlCopyMemory( \
	nextIrpSp, \
	irpSp, \
	FIELD_OFFSET(IO_STACK_LOCATION, CompletionRoutine) \
	); \
	nextIrpSp->Control = 0; }
#endif

	NTKERNELAPI
		NTSTATUS
		IoCreateFile (
		__out PHANDLE             FileHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes,
		__out PIO_STATUS_BLOCK    IoStatusBlock,
		__in PLARGE_INTEGER       AllocationSize OPTIONAL,
		__in ULONG                FileAttributes,
		__in ULONG                ShareAccess,
		__in ULONG                CreateDisposition,
		__in ULONG                CreateOptions,
		__in PVOID                EaBuffer OPTIONAL,
		__in ULONG                EaLength,
		__in CREATE_FILE_TYPE     CreateFileType,
		__in PVOID                ExtraCreateParameters,
		__in ULONG                Options
		);

#ifndef IoSkipCurrentIrpStackLocation
#define IoSkipCurrentIrpStackLocation( Irp ) \
	(Irp)->CurrentLocation++; \
	(Irp)->Tail.Overlay.CurrentStackLocation++;
#endif

	NTSYSAPI
		VOID
		NTAPI
		ProbeForWrite (
		__in PVOID Address,
		__in ULONG Length,
		__in ULONG Alignment
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenFile (
		__out PHANDLE             FileHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes,
		__out PIO_STATUS_BLOCK    IoStatusBlock,
		__in ULONG                ShareAccess,
		__in ULONG                OpenOptions
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwOpenSymbolicLinkObject (
		__out PHANDLE             SymbolicLinkHandle,
		__in ACCESS_MASK          DesiredAccess,
		__in POBJECT_ATTRIBUTES   ObjectAttributes
		);

	NTSYSAPI
		NTSTATUS
		NTAPI
		ZwQuerySymbolicLinkObject (
		__in HANDLE               LinkHandle,
		__in __out PUNICODE_STRING  LinkTarget,
		__out PULONG              ReturnedLength OPTIONAL
		);

#endif // (VER_PRODUCTBUILD < 2195)

	extern POBJECT_TYPE *PsProcessType;
#ifdef __cplusplus
}
#endif