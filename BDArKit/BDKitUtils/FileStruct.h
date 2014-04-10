#pragma once


#include "tstring.h"


#define FIND_DATA_SIZE      0x4000
#define FIND_DEVICE_HANDLE  ((HANDLE)0x1)

typedef enum _FIND_DATA_TYPE
{
	FindFile   = 1,
	FindStream = 2
} FIND_DATA_TYPE;

typedef struct _FIND_FILE_DATA
{
	HANDLE				Handle;
	FINDEX_INFO_LEVELS	InfoLevel;
	FINDEX_SEARCH_OPS	SearchOp;
	tstring				DirectoryPath;
	tstring				FilePattern;
} FIND_FILE_DATA, *PFIND_FILE_DATA;

typedef struct _FIND_DATA_HANDLE
{
	FIND_DATA_TYPE		Type;
	FIND_FILE_DATA		FindFileData;
} FIND_DATA_HANDLE, *PFIND_DATA_HANDLE;