// BDKitUtils.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "KernelUtils.h"
#include "BDKitUtils\BDKitUtils.h"


CKernelUtils g_KernelUtils;

extern "C" DLL_DEFINE IKernelUtils* GetBDKitInterface(VOID)
{
	return &g_KernelUtils;
}