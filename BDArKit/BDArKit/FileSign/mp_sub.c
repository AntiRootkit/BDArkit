#pragma once


#include "StdAfx.h"
#include "mp.h"

BOOL BN_SUB(BIGNUM * y, CONST BIGNUM * n)
{
	BOOL bRet = FALSE;
	DWORD i = 0;
	ULARGE_INTEGER sub;
	DWORD carry = 0;
    INT nResult = 0;

	nResult = BN_CMP(y, n);
	CONDITION_ASSERT(0 <= nResult);

	for (i = 0; i < y->dwCount; ++i)
	{
		sub.u.HighPart = 1;
		sub.u.LowPart = y->dwDigits[i];
		sub.QuadPart -= n->dwDigits[i];
		sub.QuadPart -= carry;

		y->dwDigits[i] = sub.u.LowPart;
		carry = !sub.u.HighPart;
	}

	for (i = y->dwCount; 0 < i; --i)
	{
		if (y->dwDigits[i - 1])
		{
			break;
		}

		if (1 < y->dwCount)
		{
			--y->dwCount;
		}
	}

    bRet = TRUE;
Exit0:
	return bRet;
}

BOOL BN_SUB_I(BIGNUM * y, DWORD n)
{
	BOOL bRet=FALSE;
    BIGNUM *temp;

	temp = GetPool(sizeof(BIGNUM));
	if (temp){
		BN_SET_I(temp, n);

		bRet = BN_SUB(y, temp);
		ExFreePool(temp);
	}
	return bRet;
}