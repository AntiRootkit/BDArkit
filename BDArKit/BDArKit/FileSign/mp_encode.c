#pragma once

#include "StdAfx.h"
#include "mp.h"

BOOL BN_REVERT(BIGNUM* src, PUCHAR uTarget, DWORD dwTargetSize)
{
	BOOL bRet = FALSE;
	DWORD i = 0;
	UCHAR d = 0;
	PUCHAR p = NULL;
    PUCHAR q = NULL;

	for (i = 0; 0 != BN_CMP_I(src, 0) && i < MAX_ENCRYPT_BUFFER_SIZE; ++i)
	{
		uTarget[i] = (UCHAR)(src->dwDigits[0]&0xff);

		BN_RSHIFT_B(src, 8);
	}

    //reverse uTarget byte
	for (p = uTarget, q = p + dwTargetSize - 1; p < q; ++p, --q)
	{
		d = *p; 
        *p = *q; 
        *q = d;
	}

    bRet = TRUE;
//Exit0:
	return bRet;
}

