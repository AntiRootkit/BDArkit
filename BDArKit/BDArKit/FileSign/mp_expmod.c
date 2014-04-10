#pragma once


#include "StdAfx.h"
#include "mp.h"

BOOL BN_EXPMOD(BIGNUM * y, CONST BIGNUM * x, CONST BIGNUM * e, CONST BIGNUM * n)
{
    BOOL bRet = FALSE;
    BOOL bResult = FALSE;
	DWORD i;
	DWORD bits;
	DWORD e0;
	DWORD mask;
	BIGNUM *D=NULL;
	BIGNUM *mul=NULL;
	BIGNUM *div=NULL;

	D = GetPool(sizeof(BIGNUM));
	mul = GetPool(sizeof(BIGNUM));
	div = GetPool(sizeof(BIGNUM));
	if (!D || !mul || !div){
		CONDITION_ASSERT(bResult);
	}

	bits = e->dwCount * 32;
	e0 = e->dwDigits[e->dwCount - 1];
	for (mask = 1 << 31; 0 < mask; mask >>= 1)
	{
		if (e0 & mask)
		{
			break;
		}
		--bits;
	}

	BN_SET_I(D, 1);

	for (i = bits; 0 < i; --i)
	{
		bResult = BN_MUL(mul, D, D);
		CONDITION_ASSERT(bResult);

		bResult = BN_DIV(div, mul, n, D);
		CONDITION_ASSERT(bResult);

		e0 = e->dwDigits[(i-1) / 32];
		mask = 1 << ((i-1) % 32);

		if (e0 & mask)
		{
			bResult = BN_MUL(mul, D, x);
			CONDITION_ASSERT(bResult);

			bResult = BN_DIV(div, mul, n, D);
			CONDITION_ASSERT(bResult);
		}
	}

	BN_SET(y, D);

    bRet = TRUE;
Exit0:
	if (D){
		ExFreePool(D);
	}
	if (div){
		ExFreePool(div);
	}
	if (mul){
		ExFreePool(mul);
	}
	return bRet;
}