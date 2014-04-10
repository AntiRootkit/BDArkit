#pragma once

#include "StdAfx.h"
#include "mp.h"

INT BN_CMP_I(CONST BIGNUM * x1, DWORD x2)
{
	if (x1->dwCount > 1) 
        return 1; 

	if (x1->dwCount < 1) 
        return -1;

    if (x1->dwDigits[0] > x2)
        return 1;

    if (x1->dwDigits[0] < x2) 
        return -1;

	return 0;
}

int BN_CMP(CONST BIGNUM * x1, CONST BIGNUM * x2)
{
	DWORD i;

	if (x1->dwCount > x2->dwCount) 
        return 1;

	if (x1->dwCount < x2->dwCount) 
        return -1;

	for (i = x1->dwCount; 0 < i; --i)
	{
		if (x1->dwDigits[i - 1] > x2->dwDigits[i - 1])
            return 1; 

		if (x1->dwDigits[i - 1] < x2->dwDigits[i - 1]) 
            return -1; 
	}

	return 0;
}
