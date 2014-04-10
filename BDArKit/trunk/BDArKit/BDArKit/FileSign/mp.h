#ifndef SECLIB_MPI_H_
#define SECLIB_MPI_H_
#pragma once


#ifdef USER_MODE
#include <windows.h>
#include <stdio.h>
#else
//#include <ntddk.h>
#include <stdio.h>
#include "windef.h"
//#include "public.h"
#define CONST const
#define VOID void
#endif

#ifndef CONDITION_ASSERT
#if 0
#
#	define CONDITION_ASSERT(x) { if (!(x)){printf("error %s %d\n", __FILE__, __LINE__);goto Exit0;}; }
#
#else
#
#
#	define CONDITION_ASSERT(x) { if (!(x)){goto Exit0;};}
#
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ENCRYPT_BUFFER_SIZE 0x80
#define MAX_DECRYPT_BUFFER_SZIE 0x80

enum MPI_UINT
{
    BN_MAX_BIT = 0x100
};

typedef struct _tagBIGNUM
{
    DWORD dwCount;
    DWORD dwDigits[BN_MAX_BIT];
} BIGNUM;

//
BOOL BN_ASSIGN(BIGNUM* target, PUCHAR uSrc, DWORD dwSrcSize);
BOOL BN_REVERT(BIGNUM* src, PUCHAR uTarget, DWORD dwTargetSize);
//
VOID BN_SET(BIGNUM * y, CONST BIGNUM *x);
VOID BN_SET_I(BIGNUM * y, DWORD x);
//
INT BN_CMP(CONST BIGNUM * x1, CONST BIGNUM * x2);
INT BN_CMP_I(CONST BIGNUM * x1, DWORD x2);
//
BOOL BN_ADD(BIGNUM * x, CONST BIGNUM * n);
BOOL BN_ADD_I(BIGNUM * x, DWORD n);
BOOL BN_SUB(BIGNUM * x, CONST BIGNUM * n);
BOOL BN_SUB_I(BIGNUM * x, DWORD n);
BOOL BN_MUL(BIGNUM * y, CONST BIGNUM * x, CONST BIGNUM * n);
BOOL BN_MUL_I(BIGNUM * y, CONST BIGNUM * x, ULONGLONG n);
BOOL BN_DIV(BIGNUM * y, CONST BIGNUM * x, CONST BIGNUM * n, BIGNUM * m);
BOOL BN_DIV_I(BIGNUM * y, CONST BIGNUM * x, DWORD n, BIGNUM * m);
//
BOOL BN_LSHIFT_D(BIGNUM * x, DWORD n);
BOOL BN_LSHIFT_B(BIGNUM * x, DWORD n);
VOID BN_RSHIFT_D(BIGNUM * x, DWORD n);
VOID BN_RSHIFT_B(BIGNUM * x, DWORD n);
//
BOOL BN_EXPMOD(BIGNUM * , CONST BIGNUM * x, CONST BIGNUM * e, CONST BIGNUM * n);

BOOL RSA_Encrypt(
    /*IN*/CONST UCHAR* src, 
    /*IN*/DWORD dwSrcSize, 
    /*IN*/UCHAR *pCipher, 
    /*IN\OUT*/DWORD *pdwCipherSize
);

BOOL RSA_Decrypt(
    /*IN*/CONST UCHAR* pCipher, 
    /*IN*/DWORD dwCipherSize, 
    /*IN*/UCHAR *pPlain, 
    /*IN\OUT*/DWORD *pPlainSize
);

typedef struct _PEB_LDR_DATA {
	ULONG Length;
	BOOLEAN Initialized;
	HANDLE SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID EntryInProgress;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _CURDIR {
	UNICODE_STRING DosPath;
	HANDLE Handle;
} CURDIR, *PCURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	ULONG MaximumLength;
	ULONG Length;

	ULONG Flags;
	ULONG DebugFlags;

	HANDLE ConsoleHandle;
	ULONG  ConsoleFlags;
	HANDLE StandardInput;
	HANDLE StandardOutput;
	HANDLE StandardError;

	CURDIR CurrentDirectory;        // ProcessParameters
	UNICODE_STRING DllPath;         // ProcessParameters
	UNICODE_STRING ImagePathName;   // ProcessParameters
	UNICODE_STRING CommandLine;     // ProcessParameters
}RTL_USER_PROCESS_PARAMETERS,*PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB {
	BOOLEAN InheritedAddressSpace;      // These four fields cannot change unless the
	BOOLEAN ReadImageFileExecOptions;   //
	BOOLEAN BeingDebugged;              //
	BOOLEAN SpareBool;                  //
	HANDLE Mutant;                      // INITIAL_PEB structure is also updated.

	PVOID ImageBaseAddress;
	PPEB_LDR_DATA Ldr;
	struct _RTL_USER_PROCESS_PARAMETERS *ProcessParameters;
}PEB,*PPEB;


#define GetPool(x) ExAllocatePoolWithTag(NonPagedPool,x,'20db')
#ifdef __cplusplus
}
#endif

#endif