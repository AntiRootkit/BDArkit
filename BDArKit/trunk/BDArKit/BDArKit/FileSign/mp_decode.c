#pragma once


#include "StdAfx.h"
#include "mp.h"

BOOL BN_ASSIGN(BIGNUM* target, PUCHAR uSrc, DWORD dwSrcSize)
{
	BOOL bRet = 0;
    BOOL bResult = FALSE;
	DWORD i = 0;

	BN_SET_I(target, 0);

	CONDITION_ASSERT(dwSrcSize <= BN_MAX_BIT * 4);

	for (i = 0; i < dwSrcSize; ++i)
	{
		bResult = BN_LSHIFT_B(target, 8);
		CONDITION_ASSERT(bResult);

		target->dwDigits[0] += uSrc[i];
	}

    bRet = TRUE;
Exit0:
	return bRet;
}