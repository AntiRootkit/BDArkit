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

	//ETHREAD��CrossThreadFlags��ƫ����Ϊ0x248 
	ThreadFlags=(PULONG)((ULONG)PsGetCurrentThread()+0x248);  
	if( MmIsAddressValid(ThreadFlags) )
	{
		*ThreadFlags |=  PS_CROSS_THREAD_FLAGS_SYSTEM;
		//(*PspExitThread_XP)(STATUS_SUCCESS);//PspExitThread������,��Ҫ�Լ���λ
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
	ULONG num;   //�߳�����
	ULONG Head;  //����ͷ
	ULONG address;//��ַ
	ULONG i;
	do 
	{
		num=*(ULONG *)((ULONG)EProcess+0x1a0);   //EPROCESS��ActiveThreads������  0x1a0��EPROCESS��ActiveThread��ƫ����

		Head=(ULONG)EProcess+ 0x190;              //List_entry��һ���ڵ��ַ	
		for(i=0;i<num;i++)
		{
			//��¼�̵߳�ַ
			Head=(ULONG)((PLIST_ENTRY)Head)->Flink;
			address=Head-0x22c;

			Thread=(PETHREAD)address;                                       //ת�����߳�ָ�� 
			ExitApc=(PKAPC)ExAllocatePoolWithTag(NonPagedPool,sizeof(KAPC),'apc');
			if(ExitApc==NULL)
			{

				return STATUS_UNSUCCESSFUL;
			}
			KeInitializeApc(ExitApc,
				(PKTHREAD)&Thread,                         //�߳�
				OriginalApcEnvironment,
				KernelKillThreadRoutine,
				NULL,
				NULL,
				KernelMode,
				NULL);//Ϊ�̳߳�ʼ��APC
			nsStatus=KeInsertQueueApc(ExitApc,ExitApc,NULL,2);   //����Apc���̶߳���
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