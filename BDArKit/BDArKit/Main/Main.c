#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "BDArKit\KitHelper.h"
#include "Utility.h"
#include "BDKitUtils.h"
#include "PELoader.h"
#include "miniFilter.h"
#include "RebootActions.h"
#include "DriverHelper.h"


PBDKIT_DEVICE_EXTENSION g_DeviceExtension	= NULL;
PDRIVER_OBJECT			g_DriverObject		= NULL;
PUNICODE_STRING			g_DriverRegistry	= NULL;
#ifdef _X86_
CONST ULONG32			g_bRunningX86		= TRUE;
#else
CONST ULONG32			g_bRunningX86		= FALSE;
#endif // _X86_

VOID DriverUnload (__in PDRIVER_OBJECT pDriverObject);

NTSTATUS DriverEntry (__in PDRIVER_OBJECT pDriverObject, __in PUNICODE_STRING puszRegistry)
{
	NTSTATUS		nsStatus		= STATUS_UNSUCCESSFUL;
	UNICODE_STRING	uszDeviceName	= RTL_CONSTANT_STRING(BDArKit_DEVICE_NAMEW);
	UNICODE_STRING	uszSymbolLink	= RTL_CONSTANT_STRING(BDArKit_SYMBOL_NAMEW);
	PDEVICE_OBJECT	pDeviceObject	= NULL;
	PBDKIT_DEVICE_EXTENSION pDevExt = NULL;
	ULONG			MajorIndex		= 0;
	DebugFlag		DebugFlag		= emNormalRun;

	do 
	{
		pDriverObject->DriverUnload = NULL;

		DebugFlag = BDKitCheckRunMode(puszRegistry);
		if (IS_SPECIFIC_RUN_MODE(DebugFlag, emAllowUnload)) {
			pDriverObject->DriverUnload = DriverUnload;
		}
		if (IS_SPECIFIC_RUN_MODE(DebugFlag, emEnableKDDisable)) {
			KdDisableDebugger ();
		}

		nsStatus = IoCreateDevice (
			pDriverObject,
			sizeof(BDKIT_DEVICE_EXTENSION),
			&uszDeviceName,
			FILE_DEVICE_UNKNOWN,
			0,
			FALSE,
			&pDeviceObject);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = IoCreateSymbolicLink (&uszSymbolLink, &uszDeviceName);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		//nsStatus = IoRegisterShutdownNotification (pDeviceObject);
		//BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		////TODO ALL INITIALIZE
		for (MajorIndex = 0; MajorIndex <= IRP_MJ_MAXIMUM_FUNCTION; ++MajorIndex)
			pDriverObject->MajorFunction[MajorIndex] = DriverDispatchHandler;

		pDevExt = (PBDKIT_DEVICE_EXTENSION)pDeviceObject->DeviceExtension;
		pDevExt->pFunDeviceObject = pDeviceObject;
		g_DeviceExtension = pDevExt;
		BDKit_If_Not_Break_With_Reason(BDKitCopyUnicodeStringW (&pDevExt->uszDeviceName, &uszDeviceName),
			STATUS_INVALID_VARIANT);
		BDKit_If_Not_Break_With_Reason(BDKitCopyUnicodeStringW (&pDevExt->uszSymbolLink, &uszSymbolLink),
			STATUS_INVALID_VARIANT);

		pDeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

		g_DriverObject	= pDriverObject;
		BDKitCopyPUnicodeStringW (&g_DriverRegistry, puszRegistry);
		BDKit_If_Not_Break_With_Reason(g_DriverRegistry != NULL, STATUS_INVALID_VARIANT);

		nsStatus = BDKitGetOSVersion ();
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = BDKitInitializeUtility ();
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = BDKitInitializeFilter (pDriverObject);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));

		nsStatus = BDKitExecuteRebootHandler (pDeviceObject);
		
		nsStatus = STATUS_SUCCESS;

	} while (FALSE);

	if ( !NT_SUCCESS(nsStatus) )
	{
		DriverUnload (pDriverObject);
	}

	BDbgPrint ("[BDKIT]Install Kit Status:%p RunMode:%p\r\n", nsStatus, DebugFlag);
	return nsStatus;
}

VOID DriverUnload (__in PDRIVER_OBJECT pDriverObject)
{
	PBDKIT_DEVICE_EXTENSION	pDevExt = g_DeviceExtension;

	UNREFERENCED_PARAMETER(pDriverObject);

	BDKitDisableIATHook ();

	do 
	{
		BDKit_If_Not_Break(pDevExt != NULL);
		BDKit_If_DoAction(
			pDevExt->uszSymbolLink.Buffer != NULL,
			IoDeleteSymbolicLink (&pDevExt->uszSymbolLink));
		BDKit_If_DoAction(
			pDevExt->uszDeviceName.Buffer != NULL,
			BDKitFreePool(pDevExt->uszDeviceName.Buffer));
		BDKit_If_DoAction(
			pDevExt->uszSymbolLink.Buffer != NULL,
			BDKitFreePool(pDevExt->uszSymbolLink.Buffer));
		BDKit_If_DoAction(
			pDevExt->pFunDeviceObject != NULL,
			IoDeleteDevice (pDevExt->pFunDeviceObject));
	} while (FALSE);

	BDKitUnInitializeUtility ();
	BDKitUnInitializeFilter ();
	BDKitUnloadRebootHandler ();

	BDKitFreePool(g_DriverRegistry);
	g_DriverObject = NULL;

	BDbgPrint ("[BDKIT]Uninstall Kit Complete\r\n");
}
