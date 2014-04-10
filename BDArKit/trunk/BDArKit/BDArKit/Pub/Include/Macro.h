#pragma once


#define PAGE_TAG							'DSDB'
#define MAX_HANDLE_TABLE_SIZE				0x00FFFFFF
#define MAX_OBJECT_PATH_LENGTH				0x00000800
#define MEM_SIZE_STEP						0x00001000
#define LOWEST_USER_MEM_ADDRESS				0x00000000
#define HIGHEST_USER_MEM_ADDRESS			0x80000000

//// For Macron ////

#define METHOD_FROM_CTL_CODE(ctrlCode)          ((ULONG)(ctrlCode & 3))
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))

#define OBJECT_TO_OBJECT_HEADER( o ) \
	CONTAINING_RECORD( (o), OBJECT_HEADER, Body )

#define KERNEL_HANDLE_MASK					((ULONG_PTR)((LONG)0x80000000))
#define IsKernelHandle(H,M)                                \
	(((KERNEL_HANDLE_MASK & (ULONG_PTR)(H)) == KERNEL_HANDLE_MASK) && \
	((M) == KernelMode) &&                                \
	((H) != NtCurrentThread()) &&                         \
	((H) != NtCurrentProcess()))
#define EncodeKernelHandle(H) (HANDLE)(KERNEL_HANDLE_MASK | (ULONG_PTR)(H))
#define DecodeKernelHandle(H) (HANDLE)(KERNEL_HANDLE_MASK ^ (ULONG_PTR)(H))

#define DBG
#ifdef DBG 
#define BDbgPrint DbgPrint
#else
#define BDbgPrint
#endif // DBG

#define BDKit_If_DoAction(condition, action)	\
{												\
	if ( (condition) ) { action; }				\
}

#define BDKit_If_Not_Break(condition)			\
{												\
	BDKit_If_DoAction( !(condition), break)		\
}

#define BDKit_If_Not_Continue(condition)		\
{												\
	BDKit_If_DoAction( !(condition), continue)	\
}

#define BDKit_If_Not_Break_With_Reason(condition, reason)	\
{															\
	if ( !(condition) ) { nsStatus = reason; break; }		\
}

#define BDKit_If_Not_Return(condition, status)		\
{													\
	BDKit_If_DoAction( !(condition), return status)	\
}

#define BDKitCloseHandle(hHandle)				\
{												\
	if ( hHandle != NULL )						\
	{											\
		ZwClose(hHandle);						\
		hHandle = NULL;							\
	}											\
}

#define BDKitCloseObject(pObject)				\
{												\
	if ( (pObject) != NULL )					\
	{											\
		ObfDereferenceObject(pObject);			\
		(pObject) = NULL;						\
	}											\
}

#define BDKitAllocateNonpagePool(ptr, size)		\
{												\
	ptr = NULL;									\
	if( size != 0 )								\
		ptr = ExAllocatePoolWithTag (NonPagedPool, size, PAGE_TAG);	\
	if (ptr != NULL)							\
		RtlZeroMemory(ptr, size);				\
}

#define BDKitAllocatePagePool(ptr, size)		\
{												\
	ptr = NULL;									\
	if( size != 0 )								\
		ptr = ExAllocatePoolWithTag (PagedPool, size, PAGE_TAG);\
	if (ptr != NULL)							\
		RtlZeroMemory(ptr, size);				\
}

#define BDKitAllocateSysNonPageQuotaPool(ptr, size)				\
{																\
	ptr = NULL;													\
	if( size != 0 )												\
		ptr = ExAllocatePoolWithQuota (NonPagedPool, size);		\
	if (ptr != NULL)											\
		RtlZeroMemory(ptr, size);								\
}

// Should be protected by try-catch
#define BDKitAllocateNonPageQuotaPool(ptr, size)				\
{																\
	ptr = NULL;													\
	if( size != 0 )												\
		ptr = ExAllocatePoolWithQuotaTag  (NonPagedPool, size, PAGE_TAG);	\
	if (ptr != NULL)											\
		RtlZeroMemory(ptr, size);								\
}

#define BDKitFreePool(ptr)						\
{												\
	if ( ptr != NULL )							\
		ExFreePoolWithTag (ptr, PAGE_TAG);		\
	ptr = NULL;									\
}

#define BDKitFreeSysPool(ptr)					\
{												\
	if ( ptr != NULL )							\
		ExFreePool (ptr);						\
	ptr = NULL;									\
}

//////////////////////////////////////////////////////////////////////////