#pragma once


#include <fltkernel.h>
#include <ntddk.h>
#include <ntverp.h>
#include <WinDef.h>


extern BYTE *ImageModuleBase;
extern ULONG SystemKernelModuleBase;
extern ULONG SystemKernelModuleSize;

#ifdef _X86_
typedef ULONG32	OFFSET_VALUE;
typedef ULONG32	_ULONG_PTR;
typedef ULONG32 _ULONG;
#else
typedef ULONG64	OFFSET_VALUE;
typedef ULONG64	_ULONG_PTR;
typedef ULONG64 _ULONG;
#endif // _X86_


#if (VER_PRODUCTBUILD >= 7600)
#pragma warning (disable: 4055) //'type cast' : from data pointer 'PVOID' to function pointer 'PFN_RtlGetVersion'
#pragma warning (disable: 4214) //bit field types other than int
#pragma warning (disable: 4201) //warning C4201: nonstandard extension used : nameless struct/union
#pragma warning (disable: 4127) //conditional expression is constant

#pragma warning (disable: 28101) //The Drivers module has inferred that the current function is a DRIVER_INITIALIZE function
#pragma warning (disable: 28112) //A variable which is accessed via an Interlocked function must always be accessed via an Interlocked function
#pragma warning (disable: 28155) //The function being assigned or passed should be a IO_COMPLETION_ROUTINE function
#pragma warning (disable: 28159) //Consider using 'RtlGetVersion' instead of 'PsGetVersion'. Reason: Obsolete.
#pragma warning (disable: 28169) //The dispatch function 'DriverDispatchHandler' does not have any __drv_dispatchType annotations
#pragma warning (disable: 28175) //The 'xxxx' member of _DRIVER_OBJECT should not be accessed by a driver
#pragma warning (disable: 28196) //The result '*POINT' is 'NULL', which is inconsistent with the Results specified by the annotations.
#pragma warning (disable: 28719) //Banned API Usage: wcscat is a Banned API as listed __in dontuse.h for security purposes
#endif

#include "BDNtifs.h"