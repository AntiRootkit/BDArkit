#pragma once


#include "StdAfx.h"
#include "mp.h"

VOID BN_SET(BIGNUM * y, CONST BIGNUM * x)
{
	DWORD i;

	y->dwCount = x->dwCount;

	for (i = 0; i < BN_MAX_BIT; ++i)
	{
		y->dwDigits[i] = x->dwDigits[i];
	}

	return ;
}

VOID BN_SET_I(BIGNUM * y, DWORD x)
{
	DWORD i = 1;

	y->dwCount = 1;

	y->dwDigits[0] = x;

	for (i = 1; i < BN_MAX_BIT; ++i)
	{
		y->dwDigits[i] = 0;
	}

	return ;
}

