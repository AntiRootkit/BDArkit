#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "BDArKit\KitHelper.h"
#include "DriverHelper.h"
#include "Utility.h"
#include "NtosKrnl.h"
#include "BDKitUtils.h"
#include "BDKitHandler.h"


typedef struct _SHUTDOWN_PACKET
{
	LIST_ENTRY		ListEntry;
	PDEVICE_OBJECT	DeviceObject;
}SHUTDOWN_PACKET, *PSHUTDOWN_PACKET;

NTSTATUS BDKitFocusEnumShutdownList(
									__in_bcount(ulInBufferSize)	PVOID pInBuffer,
									__in		ULONG			ulInBufferSize,
									__out_bcount(ulOutBufferSize) PVOID pOutBuffer,
									__in		ULONG			ulOutBufferSize,
									__out		PULONG			information,
									__in		ULONG			level
									)
{
	NTSTATUS	nsStatus		= STATUS_UNSUCCESSFUL;
	PVOID		pShutdownQueue	= NULL;
	KIRQL		oldIRQL;

	do 
	{
		BDKit_If_Not_Break(nsStatus = BDKitGetShutdownNotifyQueryHead((PVOID*)&pShutdownQueue));

		oldIRQL=KeAcquireQueuedSpinLock(LockQueueIoDatabaseLock);
		{
			PSHUTDOWN_PACKET	pShutdownPacket = (PSHUTDOWN_PACKET)pShutdownQueue;
		}
		KeReleaseQueuedSpinLock(LockQueueIoDatabaseLock, oldIRQL);
	} while (FALSE);
}