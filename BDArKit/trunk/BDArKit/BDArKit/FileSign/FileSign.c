#pragma once


#include "StdAfx.h"
#include "filesign.h"    
#include "md5.h"
#include "mp.h"
//#include "public.h"


void Md5HashBuffer( BYTE *outBuffer, const BYTE *inBuffer, int length)
{
	IMS_MD5_CTX ctx;

	IMSMD5Init(&ctx);
	IMSMD5Update(&ctx, (BYTE*)inBuffer, length);
	IMSMD5Final(outBuffer, &ctx);
}

ULONG_PTR RvToVa(IN PIMAGE_NT_HEADERS pNtHeaders,IN ULONG_PTR Offset)
{
	ULONG_PTR VirtualAddress=0;
	ULONG i;
	PIMAGE_SECTION_HEADER pSectionHeader;

	pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);

	for (i=0;i<pNtHeaders->FileHeader.NumberOfSections;i++,pSectionHeader++){
		if (Offset>pSectionHeader->PointerToRawData && Offset<pSectionHeader->PointerToRawData+pSectionHeader->SizeOfRawData){
			VirtualAddress = pSectionHeader->VirtualAddress+(Offset-pSectionHeader->PointerToRawData);
			break;
		}
	}
	return VirtualAddress;
}

NTSTATUS VerifyFile(IN LPWSTR lpszFileName)
{
	NTSTATUS Status;
	HANDLE hFile;
	//DWORD dwBytesRet;
	//DWORD dwBytesWrite;
	IMAGE_DOS_HEADER ImageDosHeader;
	DWORD dwFileSize;
	VERIFY_INFO VerifyInfo;
	//BYTE Context[CONTEXT_LENGTH];
	BYTE Hash[MD5LEN];
	BYTE Encrypt[MAX_ENCRYPT_BUFFER_SIZE];
	DWORD dwEncryptSize = MAX_ENCRYPT_BUFFER_SIZE;
	BYTE Decrypt[MAX_DECRYPT_BUFFER_SZIE]={0};
	DWORD dwDecryptSize;
	PBYTE pContext = NULL;
	UNICODE_STRING ustrFileName;
	OBJECT_ATTRIBUTES ObjectAttributes;
	IO_STATUS_BLOCK IoStatusBlock;
	PFILE_OBJECT FileObject;
	FILE_STANDARD_INFORMATION FileStandardInfo;
	ULONG ReturnLength;

	RtlInitUnicodeString(&ustrFileName,lpszFileName);
	InitializeObjectAttributes(&ObjectAttributes,
							   &ustrFileName,
							   OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
							   NULL,
							   NULL 
							   );

	Status = IoCreateFile(&hFile,
						  FILE_READ_DATA | SYNCHRONIZE,
						  &ObjectAttributes,
						  &IoStatusBlock,
						  NULL,
						  0,
						  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
						  FILE_OPEN,
						  FILE_OPEN_FOR_BACKUP_INTENT | FILE_SYNCHRONOUS_IO_ALERT,
						  (PVOID) NULL,
						  0L,
						  CreateFileTypeNone,
						  (PVOID)NULL,
						  IO_NO_PARAMETER_CHECKING
						  );
	if (!NT_SUCCESS(Status)){
		hFile = NULL;
		FileObject = NULL;
		goto Exit0;
	}

	Status = ObReferenceObjectByHandle(hFile,
									   0,
									   *IoFileObjectType,
									   KernelMode,
									   &FileObject,
									   NULL
									   );

	if (!NT_SUCCESS(Status)){
		FileObject = NULL;
		goto Exit0;
	}
	FileObject->CurrentByteOffset.QuadPart = 0;
	Status = IoQueryFileInformation(FileObject,
									FileStandardInformation,
									sizeof(FileStandardInfo),
									&FileStandardInfo,
									&ReturnLength
									);
	if (!NT_SUCCESS(Status)){
		goto Exit0;
	}

	dwFileSize = FileStandardInfo.EndOfFile.LowPart;

	Status = ZwReadFile(hFile,NULL,NULL,NULL,&IoStatusBlock,&ImageDosHeader,sizeof(ImageDosHeader),NULL,NULL);
	if (!NT_SUCCESS(Status)){
		goto Exit0;
	}

	if (ImageDosHeader.e_magic != IMAGE_DOS_SIGNATURE){
		Status = STATUS_INVALID_IMAGE_NOT_MZ;
		goto Exit0;
	}
	
	dwFileSize = *(DWORD *)&ImageDosHeader.e_res2[2];
	if (0 == dwFileSize 
		|| dwFileSize > MAX_PE_FILE_SIZE
		|| (~dwFileSize) != *(DWORD *)&ImageDosHeader.e_res2[0]
		)
	{
		//KdPrint1("文件长度验证出错\n");
		Status = STATUS_UNSUCCESSFUL;
		goto Exit0;
	}

	FileObject->CurrentByteOffset.QuadPart = 0;
	FileObject->CurrentByteOffset.LowPart = dwFileSize;

	Status = ZwReadFile(hFile,NULL,NULL,NULL,&IoStatusBlock,Encrypt,sizeof(Encrypt),NULL,NULL);
	if (!NT_SUCCESS(Status)){
		goto Exit0;
	}

	dwDecryptSize = sizeof(VerifyInfo);
	if (!RSA_Decrypt(Encrypt, dwEncryptSize, Decrypt, &dwDecryptSize)){
		//KdPrint1("验证信息签名失败\n");
		Status=STATUS_UNSUCCESSFUL;
		goto Exit0;
	}

	memcpy(&VerifyInfo, Decrypt, sizeof(VerifyInfo));

	FileObject->CurrentByteOffset.QuadPart = 0;
	FileObject->CurrentByteOffset.LowPart = VerifyInfo.Offset;

	pContext = (PBYTE)GetPool(VerifyInfo.Length);
	if (!pContext){
		pContext = NULL;
		//KdPrint1("申请内存出错\n");
		Status = STATUS_NO_MEMORY;
		goto Exit0;
	}

	Status = ZwReadFile(hFile,NULL,NULL,NULL,&IoStatusBlock,pContext,VerifyInfo.Length,NULL,NULL);
	if (!NT_SUCCESS(Status)){
		goto Exit0;
	}

	Md5HashBuffer(Hash, pContext, VerifyInfo.Length);

	if (!memcmp(Hash, VerifyInfo.Hash, MD5LEN)){
		//KdPrint1("文件校验正确\n");
	}else{
		//KdPrint1("文件校验失败\n");
		Status = STATUS_UNSUCCESSFUL;
	}


Exit0:
	if (pContext){
		ExFreePool(pContext);
		pContext = NULL;
	}
	if (FileObject){
		ObDereferenceObject(FileObject);
	}
	if (hFile){
		ZwClose(hFile);
		hFile = NULL;
	}
	return Status;
}

NTSTATUS VerifyFileByProcess(IN PEPROCESS Process)
{
	NTSTATUS Status=STATUS_INVALID_IMAGE_FORMAT;
	BOOLEAN bAttach=FALSE;
	KAPC_STATE ApcState;
	PPEB Peb;
	PVOID BaseAddress;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeaders;
	PBYTE pEncrypt;
	ULONG_PTR VirtualAddress;
	DWORD dwFileSize;
	VERIFY_INFO VerifyInfo;
	BYTE Hash[MD5LEN];
	DWORD dwEncryptSize = MAX_ENCRYPT_BUFFER_SIZE;
	BYTE Decrypt[MAX_DECRYPT_BUFFER_SZIE]={0};
	DWORD dwDecryptSize;
	PBYTE pContext;

	if (!Process){
		return STATUS_INVALID_PARAMETER;
	}
	Peb = PsGetProcessPeb(Process);
	if (!Peb){
		return Status;
	}
	if (PsGetCurrentProcess()!=Process){
		bAttach = TRUE;
		KeStackAttachProcess((PKPROCESS)Process, &ApcState);
	}
	__try{
		BaseAddress = Peb->ImageBaseAddress;
		if (!BaseAddress){
			goto Exit0;
		}

		pDosHeader = (PIMAGE_DOS_HEADER)BaseAddress;
		if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE){
			Status = STATUS_INVALID_IMAGE_NOT_MZ;
			goto Exit0;
		}

		pNtHeaders = RtlImageNtHeader(BaseAddress);
		if (!pNtHeaders || pNtHeaders->Signature!=IMAGE_NT_SIGNATURE){
			goto Exit0;
		}

		dwFileSize = *(DWORD *)&pDosHeader->e_res2[2];
		if (0 == dwFileSize 
			|| dwFileSize > MAX_PE_FILE_SIZE
			|| (~dwFileSize) != *(DWORD *)&pDosHeader->e_res2[0]
			)
		{
			Status = STATUS_UNSUCCESSFUL;
			goto Exit0;
		}

		VirtualAddress=RvToVa(pNtHeaders,dwFileSize);
		if (!VirtualAddress){
			Status = STATUS_UNSUCCESSFUL;
			goto Exit0;
		}
		VirtualAddress+=(ULONG_PTR)BaseAddress;
		pEncrypt = (PBYTE)VirtualAddress;
		dwDecryptSize = sizeof(VerifyInfo);
		if (!RSA_Decrypt(pEncrypt, dwEncryptSize, Decrypt, &dwDecryptSize)){
			Status=STATUS_UNSUCCESSFUL;
			goto Exit0;
		}

		memcpy(&VerifyInfo, Decrypt, sizeof(VerifyInfo));
		
		VirtualAddress=RvToVa(pNtHeaders,VerifyInfo.Offset);
		if (!VirtualAddress){
			Status = STATUS_UNSUCCESSFUL;
			goto Exit0;
		}
		
		VirtualAddress+=(ULONG_PTR)BaseAddress;
		pContext = (PBYTE)VirtualAddress;

		Md5HashBuffer(Hash, pContext, VerifyInfo.Length);
		
		if (!memcmp(Hash, VerifyInfo.Hash, MD5LEN)){
			Status = STATUS_SUCCESS;
		}else{
			Status = STATUS_UNSUCCESSFUL;
		}
	}  __except(EXCEPTION_EXECUTE_HANDLER) {
		Status = GetExceptionCode();
	}

Exit0:
	if (bAttach){
		KeUnstackDetachProcess(&ApcState);
	}
	return Status;
}
