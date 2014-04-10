#ifndef _FILE_SIGN_H
#define _FILE_SIGN_H 1

#include <windef.h>
#include <stdio.h>
#include <devioctl.h>
#include "ntimage.h"


#define BD_MAX_INDEX 128
#define BD_MAX_BLOCK_INDEX 32
#define BD_MAX_RULES_INDEX 32
#define BD_MAX_RULES_TYPE 2

#pragma pack(push)
#pragma pack(1)
typedef struct _tagVERIFY_INFO
{
	DWORD Offset;
	DWORD Length;
	BYTE  Hash[16];
}VERIFY_INFO,*PVERIFY_INFO;
#pragma pack(pop)

#define CONTEXT_LENGTH 32
#define MAX_PE_FILE_SIZE 300*1024*1024

NTSTATUS VerifyFile(IN LPWSTR lpszFileName);
NTSTATUS VerifyFileByProcess(IN PEPROCESS Process);

NTSYSAPI
PPEB
NTAPI
PsGetProcessPeb(
				IN PEPROCESS Process
				);
#endif
