#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "Utility.h"
#include "NtosKrnl.h"
#include "BDKitUtils.h"
//#include "BDKitProcess.h"


VOID  KernelKillThreadRoutine(
							  __in PKAPC					Apc,
							  __in __out PKNORMAL_ROUTINE*	NormalRoutine,
							  __in __out PVOID*				NormalContext,
							  __in __out PVOID*				SystemArgument1,
							  __in __out PVOID*				SystemArgument2
							  )
{
	PULONG	ThreadFlags = NULL;

	UNREFERENCED_PARAMETER(Apc);
	UNREFERENCED_PARAMETER(NormalRoutine);
	UNREFERENCED_PARAMETER(NormalContext);
	UNREFERENCED_PARAMETER(SystemArgument1);
	UNREFERENCED_PARAMETER(SystemArgument2);

	BDKitFreePool(Apc);

	//ETHREAD中CrossThreadFlags的偏移量为0x248 
	ThreadFlags=(PULONG)((ULONG)PsGetCurrentThread()+0x248);  
	if( MmIsAddressValid(ThreadFlags) )
	{
		*ThreadFlags |=  PS_CROSS_THREAD_FLAGS_SYSTEM;
		//(*PspExitThread_XP)(STATUS_SUCCESS);//PspExitThread不可用,需要自己定位
		PsTerminateSystemThread (STATUS_SUCCESS);
	}
}

NTSTATUS BDKitTerminateProcessByClearMemroy(__in PEPROCESS EProcess)
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	ULONG_PTR	MemIndex	= 0;
	KAPC_STATE	kApcState	= {0x00};

	do 
	{
		_KeStackAttachProcess ((PKPROCESS)EProcess, &kApcState);
		{
			for (	MemIndex = LOWEST_USER_MEM_ADDRESS; 
				MemIndex < HIGHEST_USER_MEM_ADDRESS - 1;
				MemIndex += PAGE_SIZE
				)
			{
				__try
				{
					ProbeForWrite ((PVOID)MemIndex, PAGE_SIZE, 4L);
					RtlFillMemory ((PVOID)MemIndex, PAGE_SIZE, 0x00);
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					break;
				}
			}
		}
		_KeUnstackDetachProcess ((PKPROCESS)EProcess, &kApcState);

		nsStatus = STATUS_SUCCESS;
	} while (FALSE);

	return nsStatus;
}

NTSTATUS BDKitTerminateProcessByAPC (__in PEPROCESS EProcess)
{
	NTSTATUS	nsStatus	= STATUS_SUCCESS;
	PETHREAD	Thread		= NULL;
	PKAPC		ExitApc		= NULL;
	ULONG num;   //线程数量
	ULONG Head;  //链表头
	ULONG address;//地址
	ULONG i;
	do 
	{
		num=*(ULONG *)((ULONG)EProcess+0x1a0);   //EPROCESS中ActiveThreads的数量  0x1a0是EPROCESS中ActiveThread的偏移量

		Head=(ULONG)EProcess+ 0x190;              //List_entry第一个节点地址	
		for(i=0;i<num;i++)
		{
			//记录线程地址
			Head=(ULONG)((PLIST_ENTRY)Head)->Flink;
			address=Head-0x22c;

			Thread=(PETHREAD)address;                                       //转换成线程指针 
			ExitApc=(PKAPC)ExAllocatePoolWithTag(NonPagedPool,sizeof(KAPC),'apc');
			if(ExitApc==NULL)
			{

				return STATUS_UNSUCCESSFUL;
			}
			KeInitializeApc(ExitApc,
				(PKTHREAD)&Thread,                         //线程
				OriginalApcEnvironment,
				KernelKillThreadRoutine,
				NULL,
				NULL,
				KernelMode,
				NULL);//为线程初始化APC
			nsStatus=KeInsertQueueApc(ExitApc,ExitApc,NULL,2);   //插入Apc到线程队列
		}


		nsStatus = STATUS_SUCCESS;
	} while (FALSE);

	return nsStatus;
}

NTSTATUS BDKitTerminateProcessByAPI( __in PEPROCESS EProcess, __in ULONG ExitCode )
{
	NTSTATUS	nsStatus	= STATUS_UNSUCCESSFUL;
	HANDLE		hProcess	= NULL;
	BOOLEAN		bAttach		= FALSE;
	KAPC_STATE	kApcState	= {0x00};

	do 
	{
		if ( bAttach == TRUE )
		{
			_KeStackAttachProcess ((PKPROCESS)EProcess, &kApcState);

			nsStatus = ZwTerminateProcess (0, STATUS_SUCCESS);
		}
		else
		{
			nsStatus = ObOpenObjectByPointer (
				EProcess,
				OBJ_KERNEL_HANDLE, 
				NULL,
				PROCESS_ALL_ACCESS,
				NULL,
				KernelMode,
				&hProcess
				);
			BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

			nsStatus = ZwTerminateProcess (hProcess, ExitCode);
		}
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

	} while (FALSE);

	if ( IsKernelHandle(hProcess, KernelMode) )
	{
		BDKitCloseHandle(hProcess);
	}
	//BDKitCloseObject(EProcess);

	return nsStatus;
}