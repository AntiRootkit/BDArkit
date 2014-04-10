#pragma once


#include "StdAfx.h"
#include <assert.h>
#include "NtosKrnl.h"


//
// We should break __in when this thread is terminated
//
#define PS_CROSS_THREAD_FLAGS_BREAK_ON_TERMINATION 0x00000040UL

//
// Flags for cross thread access. Use interlocked operations
// via PS_SET_BITS etc.
//

//
// Used to signify that the delete APC has been queued or the
// thread has called PspExitThread itself.
//
#define PS_CROSS_THREAD_FLAGS_TERMINATED           0x00000001UL

#define PS_SET_BITS(Flags, Flag) \
	RtlInterlockedSetBitsDiscardReturn (Flags, Flag)

#define PS_TEST_SET_BITS(Flags, Flag) \
	RtlInterlockedSetBits (Flags, Flag)

#define PS_CLEAR_BITS(Flags, Flag) \
	RtlInterlockedClearBitsDiscardReturn (Flags, Flag)

#define PS_TEST_CLEAR_BITS(Flags, Flag) \
	RtlInterlockedClearBits (Flags, Flag)

VOID NTAPI _KeInitializeApc( 	
							__in PKAPC  				Apc,
							__in PKTHREAD  			Thread,
							__in KAPC_ENVIRONMENT  	TargetEnvironment,
							__in PKKERNEL_ROUTINE  	KernelRoutine,
							__in PKRUNDOWN_ROUTINE	RundownRoutine OPTIONAL,
							__in PKNORMAL_ROUTINE  	NormalRoutine,
							__in KPROCESSOR_MODE  	Mode,
							__in PVOID  				Context 
							) 		
{
	/* Sanity check */
	ASSERT(TargetEnvironment <= InsertApcEnvironment);

	/* Set up the basic APC Structure Data */
	Apc->Type = ApcObject;
	Apc->Size = sizeof(KAPC);

	/* Set the Environment */
	if (TargetEnvironment == CurrentApcEnvironment)
	{
		/* Use the current one for the thread */
		Apc->ApcStateIndex = Thread->ApcStateIndex;
	}
	else
	{
		/* Sanity check */
		ASSERT((TargetEnvironment <= Thread->ApcStateIndex) ||
			(TargetEnvironment == InsertApcEnvironment));

		/* Use the one that was given */
		Apc->ApcStateIndex = TargetEnvironment;
	}

	/* Set the Thread and Routines */
	Apc->Thread = Thread;
	Apc->KernelRoutine = KernelRoutine;
	Apc->RundownRoutine = RundownRoutine;
	Apc->NormalRoutine = NormalRoutine;

	/* Check if this is a special APC */
	if (NormalRoutine)
	{
		/* It's a normal one. Set the context and mode */
		Apc->ApcMode = Mode;
		Apc->NormalContext = Context;
	}
	else
	{
		/* It's a special APC, which can only be kernel mode */
		Apc->ApcMode = KernelMode;
		Apc->NormalContext = NULL;
	}

	/* The APC is not inserted */
	Apc->Inserted = FALSE;
}

typedef NTSTATUS (*PFN_PsAcquireProcessExitSynchronization)(PKPROCESS);
static UNICODE_STRING uniPsAcquireProcessExitSynchronization = RTL_CONSTANT_STRING(L"PsAcquireProcessExitSynchronization");
static PFN_PsAcquireProcessExitSynchronization pPsAcquireProcessExitSynchronization = NULL;
NTSTATUS _KeStackAttachProcess(__in PKPROCESS KProcess, __out PRKAPC_STATE ApcState)
{
	NTSTATUS nsStatus = STATUS_SUCCESS;

	pPsAcquireProcessExitSynchronization = 
		(PFN_PsAcquireProcessExitSynchronization)MmGetSystemRoutineAddress(&uniPsAcquireProcessExitSynchronization);

	if( pPsAcquireProcessExitSynchronization )
	{
		nsStatus = pPsAcquireProcessExitSynchronization(KProcess);
		if( !NT_SUCCESS(nsStatus) )
		{
			return nsStatus;
		}
	}

	KeStackAttachProcess(KProcess, ApcState);
	return nsStatus;
}

typedef VOID (*PFN_PsReleaseProcessExitSynchronization)(PKPROCESS);
static UNICODE_STRING uniPsReleaseProcessExitSynchronization = RTL_CONSTANT_STRING(L"PsReleaseProcessExitSynchronization");
static PFN_PsReleaseProcessExitSynchronization pPsReleaseProcessExitSynchronization = NULL;
VOID _KeUnstackDetachProcess(__in PKPROCESS pProcess, __in PRKAPC_STATE SavedApcState)
{
	pPsReleaseProcessExitSynchronization = 
		(PFN_PsReleaseProcessExitSynchronization)MmGetSystemRoutineAddress(&uniPsReleaseProcessExitSynchronization);

	KeUnstackDetachProcess(SavedApcState);

	if( pPsReleaseProcessExitSynchronization )
	{
		pPsReleaseProcessExitSynchronization(pProcess);
	}
}