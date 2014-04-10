#pragma once


#include "StdAfx.h"
#include "mp.h"

BOOL BN_LSHIFT_D(BIGNUM * x, DWORD n)
{
    BOOL bRet = FALSE;
	int nResult;
	DWORD i;
	
    if (x->dwCount == 0)
        goto Exit1;

	nResult = BN_CMP_I(x, 0);
	if (0 != nResult)
	{
		x->dwCount += n;
	}

	CONDITION_ASSERT(x->dwCount <= BN_MAX_BIT);
	
	for (i = x->dwCount; n < i; --i)
	{
		x->dwDigits[i - 1] = x->dwDigits[i - n - 1];
	}
	
	for (i = n; 0 < i; --i)
	{
		x->dwDigits[i - 1] = 0;
	}
	
Exit1:
    bRet = TRUE;
Exit0:
	return bRet;
}

BOOL BN_LSHIFT_B(BIGNUM * x, DWORD n)
{
    BOOL bRet = FALSE;
    BOOL bResult = FALSE;
	DWORD d;
	DWORD b;
	DWORD i;
	ULARGE_INTEGER shft;

	d = n / 32;
	bResult = BN_LSHIFT_D(x, d);
	CONDITION_ASSERT(bResult);
	
	CONDITION_ASSERT(x->dwCount + 1 <= BN_MAX_BIT);

	b = n % 32;
	for (i = x->dwCount; 0 < i; --i)
	{
		shft.u.HighPart = 0;
		shft.u.LowPart = x->dwDigits[i - 1];
		shft.QuadPart <<= b;
		//
		x->dwDigits[i] += shft.u.HighPart;
		x->dwDigits[i - 1] = shft.u.LowPart;
	}

	if (x->dwDigits[x->dwCount])
	{
		++x->dwCount;
	}
	
    bRet = TRUE;
Exit0:
	return bRet;
}

VOID BN_RSHIFT_D(BIGNUM * x, DWORD n)
{
	DWORD i;
	
    if (n > x->dwCount)
    {
        x->dwCount = 0;
        BN_SET_I(x, 0);
        return;
    }

	for (i = n; i < x->dwCount; ++i)
	{
		x->dwDigits[i - n] = x->dwDigits[i];
	}
	x->dwCount -= n;
	
	for (i = x->dwCount; i < x->dwCount + n; ++i)
	{
		x->dwDigits[i] = 0;
	}
	
	return ;
}

VOID BN_RSHIFT_B(BIGNUM * x, DWORD n)
{
	DWORD d = 0;
	DWORD b = 0;
	DWORD i = 0;
	ULARGE_INTEGER shft;
	
	d = n / 32;
	BN_RSHIFT_D(x, d);
	
	b = n % 32;
	for (i = 0; i < x->dwCount; ++i)
	{
		shft.u.HighPart = x->dwDigits[i + 1];
		shft.u.LowPart = x->dwDigits[i];
		shft.QuadPart >>= b;
		
		x->dwDigits[i] = shft.u.LowPart;
	}
	
	if (!x->dwDigits[x->dwCount - 1])
	{
		if (1 < x->dwCount)
		{
			--x->dwCount;
		}
	}
	
	return;
}