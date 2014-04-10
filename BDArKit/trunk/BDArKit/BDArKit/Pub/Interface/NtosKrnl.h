#pragma once


typedef struct _SYSTEM_SERVICE_TABLE 
{ 
	PVOID*     ServiceTable;    // array of entry points 
	PULONG     CounterTable;    // array of usage counters 
	DWORD      ServiceLimit;    // number of table entries 
	PBYTE      ArgumentTable;   // array of byte counts 
} SYSTEM_SERVICE_TABLE, *PSYSTEM_SERVICE_TABLE, **PPSYSTEM_SERVICE_TABLE; 

//typedef struct _SERVICE_DESCRIPTOR_TABLE 
//{ 
//	SYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe ( native api ) 
//	SYSTEM_SERVICE_TABLE win32k;    // win32k.sys (gdi/user support) 
//	SYSTEM_SERVICE_TABLE Table3;    // not used 
//	SYSTEM_SERVICE_TABLE Table4;    // not used 
//} SYSTEM_DESCRIPTOR_TABLE, *PSYSTEM_DESCRIPTOR_TABLE, **PPSYSTEM_DESCRIPTOR_TABLE;

//typedef struct _SYSTEM_BASE_INFO
//{
//	ULONG						Os_NtBuildNumber;
//	ULONG						Os_ServicePack;
//	ULONG_PTR					Os_PsLoadedModuleList;
//	ULONG_PTR					Os_PsActiveProcessLinks;
//	ULONG_PTR					Os_NtoskrnlBase;
//	ULONG						Os_NtoskrnlSize;
//	PUNICODE_STRING				Os_ntoskrnlfullpath;
//	ULONG_PTR					Os_NtoskrnlBase_New;
//	ULONG						Os_NtoskrnlSize_New;
//	BOOL						Os_DriverVerifiering;
//	ULONG						TimeStamp;
//	PSYSTEM_DESCRIPTOR_TABLE	NewPsdt;
//} SYSTEM_BASE_INFO, *PSYSTEM_BASE_INFO;
//
//extern SYSTEM_BASE_INFO				g_stSystemBaseInfo;
//extern PSYSTEM_DESCRIPTOR_TABLE		KeServiceDescriptorTable;

typedef struct _LOADED_MOD_INFO
{
	ULONG ulCount;
	SYSTEM_MODULE_INFORMATION pmi[1];
} LOADED_MOD_INFO, *PLOADED_MOD_INFO;

typedef enum _KAPC_ENVIRONMENT
{
	OriginalApcEnvironment,
	AttachedApcEnvironment,
	CurrentApcEnvironment,
	InsertApcEnvironment
} KAPC_ENVIRONMENT;

//
// Kernel Object Types
//
typedef enum _KOBJECTS
{
	EventNotificationObject = 0,
	EventSynchronizationObject = 1,
	MutantObject = 2,
	ProcessObject = 3,
	QueueObject = 4,
	SemaphoreObject = 5,
	ThreadObject = 6,
	GateObject = 7,
	TimerNotificationObject = 8,
	TimerSynchronizationObject = 9,
	Spare2Object = 10,
	Spare3Object = 11,
	Spare4Object = 12,
	Spare5Object = 13,
	Spare6Object = 14,
	Spare7Object = 15,
	Spare8Object = 16,
	Spare9Object = 17,
	ApcObject = 18,
	DpcObject = 19,
	DeviceQueueObject = 20,
	EventPairObject = 21,
	InterruptObject = 22,
	ProfileObject = 23,
	ThreadedDpcObject = 24,
	MaximumKernelObject = 25
} KOBJECTS;

//
// This is a system thread
//
#define PS_CROSS_THREAD_FLAGS_SYSTEM               0x00000010UL

VOID NTAPI _KeInitializeApc( 	
							__in PKAPC  				Apc,
							__in PKTHREAD  			Thread,
							__in KAPC_ENVIRONMENT  	TargetEnvironment,
							__in PKKERNEL_ROUTINE  	KernelRoutine,
							__in PKRUNDOWN_ROUTINE	RundownRoutine OPTIONAL,
							__in PKNORMAL_ROUTINE  	NormalRoutine,
							__in KPROCESSOR_MODE  	Mode,
							__in PVOID  				Context 
							); 

NTSTATUS _KeStackAttachProcess(__in PKPROCESS KProcess, __out PRKAPC_STATE ApcState);

VOID _KeUnstackDetachProcess(__in PKPROCESS pProcess, __in PRKAPC_STATE SavedApcState);