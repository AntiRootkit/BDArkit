#pragma once


#define IRP_MJ_UNREGISTER		(IRP_MJ_MAXIMUM_FUNCTION + 1)
NTSTATUS BDKitInitializeFilter(__in PDRIVER_OBJECT pDriverObject);
NTSTATUS BDKitUnInitializeFilter(VOID);
NTSTATUS BDKitRegisterFileOperationCallback(__in PFLT_PRE_OPERATION_CALLBACK pCallbackFunc);
NTSTATUS BDKitUnregisterFileOperationCallback(PFLT_PRE_OPERATION_CALLBACK pCallbackFunc);