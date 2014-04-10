#pragma once


#include "StdAfx.h"
#include "mp.h"


//x = y * n + m
BOOL BN_DIV(BIGNUM * y, CONST BIGNUM * x, CONST BIGNUM * n, BIGNUM * m)
{
    BOOL bRet = FALSE;
    BOOL bResult = FALSE;
    INT nResult= 0;
	DWORD i = 0;
	ULARGE_INTEGER m0;
	ULONGLONG n0 = 0;
	ULONGLONG min = 0, div = 0, max = 0;
	BIGNUM *mul=NULL;
	BIGNUM *sub=NULL;
	ULONGLONG ld = 0, d = 0, rd = 0;
	//

	mul = GetPool(sizeof(BIGNUM));
	sub = GetPool(sizeof(BIGNUM));
	if (!sub || !mul){
		CONDITION_ASSERT(bResult);
	}
    BN_SET_I(sub, 0);
    BN_SET_I(mul, 0);
	BN_SET_I(y, 0);
	BN_SET(m, x);
	//
	for (i = x->dwCount; n->dwCount <= i; --i)
	{
		CONDITION_ASSERT(i == m->dwCount || i + 1 == m->dwCount);
		
		m0.QuadPart = m->dwDigits[i - 1];
		if (i < m->dwCount)
		{
			m0.u.HighPart = m->dwDigits[m->dwCount - 1];
		}
		n0 = n->dwDigits[n->dwCount - 1];
		max = (m0.QuadPart / n0);
		min = (m0.QuadPart / (n0 + 1));
		CONDITION_ASSERT(min <= max);
		//
		for ( ld = 0, rd = max - min; ; )
		{
			d = ld + (rd - ld) / 2;
			div = max - d;
			if (ld == rd) break;
			//
			bResult = BN_MUL_I(mul, n, div);
			CONDITION_ASSERT(bResult);
			//
			bResult = BN_LSHIFT_D(mul, i - n->dwCount);
			CONDITION_ASSERT(bResult);
			//
			nResult = BN_CMP(m, mul);
			if (0 < nResult)
			{
				BN_SET(sub, m);
				//
				bResult = BN_SUB(sub, mul);
				CONDITION_ASSERT(bResult);
				//
				BN_RSHIFT_D(sub, i - n->dwCount);
				//
				nResult = BN_CMP(n, sub);
				if (0 < nResult) 
                    break;
				//
				rd = d;
				continue;
			}
			if (nResult < 0)
			{
				ld = d + 1;
				continue;
			}
			// if (0 == r)
			{
				break;
			}
		}
		//
		bResult = BN_MUL_I(mul, n, div);
		CONDITION_ASSERT(bResult);
		//
		bResult = BN_LSHIFT_D(mul, i - n->dwCount);
		CONDITION_ASSERT(bResult);
		//
		bResult = BN_SUB(m, mul);
		CONDITION_ASSERT(bResult);
		//
		y->dwDigits[i - n->dwCount] = (DWORD)div;
	}
	//
	if (n->dwCount < x->dwCount)
	{
		y->dwCount = x->dwCount - n->dwCount + 1;
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
	if (mul){
		ExFreePool(mul);
	}
	if (sub){
		ExFreePool(sub);
	}
	return bRet;
}