#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "Utility.h"
#include "NtosKrnl.h"
#include "BDKitUtils.h"



/*
////////////////////Shudown Notification callback/////////////////////////
*/
NTSTATUS BDKitGetShutdownNotifyQueryHead (__out PVOID* pShutdownQueueHead)
{
	NTSTATUS		nsStatus							= STATUS_UNSUCCESSFUL;
	ULONG			ulOffset							= 0;
	PUCHAR			_KeRaiseIrqlToDcpLevel				= NULL;
	PUCHAR			_KeAcquireQueuedSpinLock			= NULL;
	PUCHAR			_IopInterlockedInsertHeadList		= NULL;
	PUCHAR			_IoRegisterShutdownNotification		= NULL;
	PUCHAR			pIopNotifyShutdownQueueHead			= NULL;
	PUCHAR			pIopInterlockedInsertHeadList		= NULL;
	PUCHAR			pIoRegisterShutdownNotification		= NULL;
	UNICODE_STRING	uszKeRaiseIrqlToDpcLevel			= {0x00};
	UNICODE_STRING	uszKeAcquireQueuedSpinLock			= {0x00};
	UNICODE_STRING	uszIoRegisterShutdownNotification	= {0x00};

#ifdef _X86_
	do 
	{
		BDKit_If_Not_Break_With_Reason(pShutdownQueueHead != NULL, STATUS_INVALID_PARAMETER);
		//*pShutdownQueueHead = NULL;

		RtlInitUnicodeString (&uszIoRegisterShutdownNotification, L"IoRegisterShutdownNotification");
		RtlInitUnicodeString (&uszKeAcquireQueuedSpinLock, L"KeAcquireQueuedSpinLock");
		RtlInitUnicodeString (&uszKeRaiseIrqlToDpcLevel, L"KeRaiseIrqlToDpcLevel");

		_KeRaiseIrqlToDcpLevel		= (PUCHAR)MmGetSystemRoutineAddress (&uszKeRaiseIrqlToDpcLevel);
		_KeAcquireQueuedSpinLock	= (PUCHAR)MmGetSystemRoutineAddress (&uszKeAcquireQueuedSpinLock);
		_IoRegisterShutdownNotification	= (PUCHAR)MmGetSystemRoutineAddress (&uszIoRegisterShutdownNotification);
		BDKit_If_Not_Break_With_Reason(_KeRaiseIrqlToDcpLevel != NULL && _KeAcquireQueuedSpinLock != NULL && _IoRegisterShutdownNotification != NULL,
			STATUS_CALLBACK_RETURNED_WHILE_IMPERSONATING);

		pIoRegisterShutdownNotification = _IoRegisterShutdownNotification;
		for (; pIoRegisterShutdownNotification < (_IoRegisterShutdownNotification + 0x60); pIoRegisterShutdownNotification++)
		{
			BDKit_If_Not_Break(
				MmIsAddressValid ((PVOID)pIoRegisterShutdownNotification[0]) && 
				MmIsAddressValid ((PVOID)pIoRegisterShutdownNotification[5])
				);
			BDKit_If_Not_Continue(
				(pIoRegisterShutdownNotification[0]==0xB9 || pIoRegisterShutdownNotification[0]==0xBF) && 
				pIoRegisterShutdownNotification[5]==0xE8
				);

			// ÕÒµ½ÁËÌØÕ÷Âë
			_IopInterlockedInsertHeadList = 
				(PUCHAR)((ULONG_PTR)(*(PULONG)(pIoRegisterShutdownNotification + 0x06)) + 
				(ULONG_PTR)(pIoRegisterShutdownNotification + 0x0A));
			pIopNotifyShutdownQueueHead	  = (PUCHAR)(*(PULONG)(pIoRegisterShutdownNotification + 0x01));
			if (!MmIsAddressValid ((PVOID)_IopInterlockedInsertHeadList) || !MmIsAddressValid ((PVOID)pIopNotifyShutdownQueueHead))
			{
				_IopInterlockedInsertHeadList	= NULL;
				pIopNotifyShutdownQueueHead		= NULL;
			}
		}
		BDKit_If_Not_Break(_IopInterlockedInsertHeadList != NULL && pIopNotifyShutdownQueueHead != NULL);

		pIopInterlockedInsertHeadList = _IopInterlockedInsertHeadList;
		for (; pIopInterlockedInsertHeadList < (_IopInterlockedInsertHeadList + 0x20); pIopInterlockedInsertHeadList++)
		{
			PUCHAR pIatJmpAddr = NULL;

			BDKit_If_Not_Break(
				MmIsAddressValid ((PVOID)pIopInterlockedInsertHeadList[0]) &&
				MmIsAddressValid ((PVOID)pIopInterlockedInsertHeadList[1])
				);
			
			if (pIopInterlockedInsertHeadList[0] == 0xFF && pIopInterlockedInsertHeadList[1] == 0x15)
			{
				BDKit_If_Not_Break(MmIsAddressValid ((PVOID)(pIopInterlockedInsertHeadList + 0x02)));
				pIatJmpAddr = (PUCHAR)(*(PULONG)(pIopInterlockedInsertHeadList + 0x02));

			} else if (pIopInterlockedInsertHeadList[0] == 0x59 && pIopInterlockedInsertHeadList[1]== 0xE8)
			{
				BDKit_If_Not_Break(MmIsAddressValid ((PVOID)(pIopInterlockedInsertHeadList + 0x02)));
				pIatJmpAddr = 
					(PUCHAR)((ULONG_PTR)(*(PULONG)(pIopInterlockedInsertHeadList + 0x02)) + 
					(ULONG_PTR)(pIoRegisterShutdownNotification + 0x06));
			}
			BDKit_If_Not_Break(MmIsAddressValid ((PVOID)pIatJmpAddr));

			if (pIatJmpAddr == (PUCHAR)KeAcquireQueuedSpinLock || pIatJmpAddr == (PUCHAR)KeRaiseIrqlToDpcLevel)
			{
				(*pShutdownQueueHead) = pIopNotifyShutdownQueueHead;
				nsStatus = STATUS_SUCCESS;
			}
		}
	} while (FALSE);

	return nsStatus;

#else
	return STATUS_NOT_SUPPORTED;
#endif
}
/*
////////////////////Shudown Notification callback/////////////////////////
*/