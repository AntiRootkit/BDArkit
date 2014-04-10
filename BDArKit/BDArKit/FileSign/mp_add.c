#pragma once

#include "StdAfx.h"
#include "mp.h"

BOOL BN_ADD(BIGNUM * y, CONST BIGNUM * n)
{
    BOOL bRet = FALSE;
	DWORD i = 0;
	ULARGE_INTEGER sum;
	DWORD carry = 0;

	if (y->dwCount < n->dwCount) 
        y->dwCount = n->dwCount;

	for (i = 0; i < y->dwCount; ++i)
	{
		sum.u.HighPart = 0;
		sum.u.LowPart = y->dwDigits[i];
		sum.QuadPart += n->dwDigits[i];
		sum.QuadPart += carry;

		y->dwDigits[i] = sum.u.LowPart;
		carry = sum.u.HighPart;
	}

	if (carry)
	{
		CONDITION_ASSERT(y->dwCount < BN_MAX_BIT);
		++y->dwCount;
		y->dwDigits[y->dwCount - 1] = carry;
	}

    bRet = TRUE;
Exit0:
	return bRet;
}

BOOL BN_ADD_I( BIGNUM * x, DWORD n )
{
	BOOL bRet=FALSE;
    BIGNUM *temp;

	temp = GetPool(sizeof(BIGNUM));
	if (temp){
		BN_SET_I(temp, n);

		bRet = BN_ADD(x, temp);
		ExFreePool(temp);
	}
	return bRet;
}