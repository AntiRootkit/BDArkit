#pragma once


#include "StdAfx.h"
#include "mp.h"

BOOL BN_MUL_I(BIGNUM * y, CONST BIGNUM * x, ULONGLONG n)
{
    BOOL bRet = TRUE;
	DWORD i;
	ULARGE_INTEGER mul;
	DWORD carry = 0;

    if (n == 0 || (x->dwCount == 1 && x->dwDigits[0] == 0))
    {
        BN_SET_I(y, 0);
        goto Exit1;
    }

	y->dwCount = x->dwCount;

	for (i = 0; i < y->dwCount; ++i)
	{
		mul.QuadPart = x->dwDigits[i];
		mul.QuadPart *= n;
		mul.QuadPart += carry;
		
		y->dwDigits[i] = mul.u.LowPart;
		carry = mul.u.HighPart;
	}
	
	if (carry)
	{
		CONDITION_ASSERT(y->dwCount < BN_MAX_BIT);
		++y->dwCount;
		y->dwDigits[y->dwCount - 1] = carry;
	}
	for (i = y->dwCount; i < BN_MAX_BIT; ++i)
	{
		y->dwDigits[i] = 0;
	}
	
Exit1:
    bRet = TRUE;
Exit0:
	return bRet;
}

BOOL BN_MUL(BIGNUM * y, CONST BIGNUM * x, CONST BIGNUM * n)
{
    BOOL bRet = FALSE;
    BOOL bResult = FALSE;
	DWORD i;
	BIGNUM *mul=NULL;
    DWORD temp;

	mul = GetPool(sizeof(BIGNUM)) ;
	if (!mul){
		CONDITION_ASSERT(bResult);
	}
	BN_SET_I(y, 0);
    BN_SET_I(mul, 0);

	for (i = 0; i < n->dwCount; ++i)
	{
		temp = n->dwDigits[i];

		bResult = BN_MUL_I(mul, x, temp);
		CONDITION_ASSERT(bResult);

		bResult = BN_LSHIFT_D(mul, i);
		CONDITION_ASSERT(bResult);

		bResult = BN_ADD(y, mul);
		CONDITION_ASSERT(bResult);
	}
	
    bRet = TRUE;
Exit0:
	if (mul){
		ExFreePool(mul);
	}
	return bRet;
}