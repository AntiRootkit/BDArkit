#pragma once


#include "StdAfx.h"
#include "Macro.h"
#include "List.h"


typedef struct _LIST_DATA_CONTEXT
{
	LIST_ENTRY		listHead;
	LONG			listCount;
	ULONG			listTag;
	ERESOURCE		listResourceLock;
	NPAGED_LOOKASIDE_LIST  listLookaside;
} LIST_DATA_CONTEXT, *PLIST_DATA_CONTEXT;

typedef struct _LIST_NODE
{
	LIST_ENTRY		listEntry;
	PVOID			pData;
} LIST_NODE, *PLIST_NODE;

LIST_CMP_HANDLER	DefaultCompare;
LIST_CLEAR_HANDLER	DefaultClear;

HANDLE BDKitInitializeList( ULONG listTag )
{
	PLIST_DATA_CONTEXT	pCtx		= NULL;
	NTSTATUS			nsStatus	= STATUS_UNSUCCESSFUL;

	do 
	{
		
		BDKitAllocateNonpagePool(pCtx, sizeof(LIST_DATA_CONTEXT));
		BDKit_If_Not_Break_With_Reason(pCtx != NULL, STATUS_INSUFF_SERVER_RESOURCES);

		nsStatus = ExInitializeResourceLite (&pCtx->listResourceLock);
		BDKit_If_Not_Break(NT_SUCCESS(nsStatus));
		
		InitializeListHead(&pCtx->listHead);
		ExInitializeNPagedLookasideList(
			&pCtx->listLookaside, 
			NULL, 
			NULL, 
			0, 
			sizeof(LIST_NODE), 
			'TLDB', 
			0
			);

		pCtx->listTag = listTag;
		pCtx->listCount = 0;
	} while (FALSE);

	BDKit_If_DoAction(!NT_SUCCESS(nsStatus), BDKitFreePool(pCtx));

	return (HANDLE)pCtx;
}

VOID BDKitCloseListHandleWithHandler( 
									 __in HANDLE hListCtx, 
									 __in LIST_CLEAR_HANDLER pfnHandler 
									 )
{
	if ( hListCtx != NULL )
	{
		PLIST_DATA_CONTEXT	pListCtx = (PLIST_DATA_CONTEXT)hListCtx;

		if ( pfnHandler == NULL )
		{
			(PVOID)pfnHandler = (PVOID)DefaultClear;
		}

		BDKitClearListNodeWithHandler (hListCtx, pfnHandler);
		ExDeleteResourceLite (&pListCtx->listResourceLock);
		ExDeleteNPagedLookasideList(&pListCtx->listLookaside);
		BDKitFreePool(hListCtx);
	}
}

VOID BDKitCloseListHandle(__in HANDLE hListCtx)
{
	BDKitCloseListHandleWithHandler (hListCtx, NULL);
}

VOID BDKitInsertListNode(__in HANDLE hListCtx, __in PVOID pNode)
{
	PLIST_DATA_CONTEXT	pListCtx	= (PLIST_DATA_CONTEXT)hListCtx;
	PLIST_NODE			pListNode	= NULL;

	do 
	{
		BDKit_If_Not_Break(pListCtx != NULL && pNode != NULL);

		pListNode = ExAllocateFromNPagedLookasideList(&pListCtx->listLookaside);
		BDKit_If_Not_Break(pListNode != NULL);

		pListNode->pData = pNode;

		FltAcquireResourceExclusive (&pListCtx->listResourceLock);
		{
			InsertTailList(&pListCtx->listHead, &pListNode->listEntry);
			pListCtx->listCount++;
		}
		FltReleaseResource (&pListCtx->listResourceLock);

	} while (FALSE);
}

PVOID BDKitFindListNode(__in HANDLE hListCtx, __in PVOID pNode)
{
	return BDKitFindListNodeWithHandler (hListCtx, pNode, DefaultCompare);
}

PVOID BDKitFindListNodeWithHandler( 
								   __in HANDLE				hListCtx, 
								   __in PVOID				pNode, 
								   __in LIST_CMP_HANDLER	pfnHandler 
								   )
{
	PLIST_DATA_CONTEXT	pListCtx	= (PLIST_DATA_CONTEXT)hListCtx;
	PLIST_NODE			pListNode	= NULL;
	PLIST_ENTRY			pListInter	= NULL;
	PLIST_NODE			pFindNode	= NULL;

	do 
	{
		BDKit_If_Not_Break(pListCtx != NULL && pNode != NULL && pfnHandler != NULL);

		FltAcquireResourceShared (&pListCtx->listResourceLock);	
		{
			if ( pListCtx->listCount > 0 )
			{
				for(
					pListInter	= pListCtx->listHead.Flink;
					pListInter != &pListCtx->listHead;
				pListInter	= pListInter->Flink
					) 
				{
					pListNode = (PLIST_NODE)CONTAINING_RECORD(pListInter, LIST_NODE, listEntry);
					if ( pfnHandler != NULL && pfnHandler(pListNode->pData, pNode) )
					{
						pFindNode = pListNode;
						break;
					}
				}
			}
		}
		FltReleaseResource (&pListCtx->listResourceLock);
	} while (FALSE);

	return pFindNode;	
}

BOOLEAN FindEntryList(__in PLIST_ENTRY ListHead, PLIST_ENTRY pListNode)
{
	PLIST_ENTRY	pListInter	= NULL;
	BOOLEAN		bFind		= FALSE;

	for(
		pListInter	= ListHead->Flink;
		pListInter != ListHead;
		pListInter	= pListInter->Flink
		) 
	{
		if (pListInter == pListNode)
		{
			break;
			bFind = TRUE;
		}
	}

	return bFind;
}

BOOLEAN BDKitRemoveListNodeWithHandler(
									__in HANDLE				hListCtx, 
									__in PVOID				pNode, 
									__in LIST_CMP_HANDLER	pfnHandler
									)
{
	PLIST_DATA_CONTEXT	pListCtx	= (PLIST_DATA_CONTEXT)hListCtx;
	PLIST_NODE			pListNode	= NULL;
	PLIST_ENTRY			pListInter	= NULL;
	BOOLEAN				bFind		= FALSE;

	do 
	{
		BDKit_If_Not_Break(pListCtx != NULL && pNode != NULL && pfnHandler != NULL);

		FltAcquireResourceExclusive (&pListCtx->listResourceLock);
		{
			if ( pListCtx->listCount > 0 )
			{
				for(
					pListInter	= pListCtx->listHead.Flink;
					pListInter != &pListCtx->listHead;
				pListInter	= pListInter->Flink
					) 
				{
					pListNode = (PLIST_NODE)CONTAINING_RECORD(pListInter, LIST_NODE, listEntry);
					if ( pfnHandler != NULL && pfnHandler(pListNode->pData, pNode) )
					{
						RemoveEntryList (&pListNode->listEntry);
						pListCtx->listCount--;
						ExFreeToNPagedLookasideList(&pListCtx->listLookaside, pListNode);
						bFind = TRUE;
						break;
					}
				}
			}
		}
		FltReleaseResource (&pListCtx->listResourceLock);
	} while (FALSE);

	return bFind;
}

BOOLEAN BDKitRemoveListNode(__in HANDLE hListCtx, __in PVOID pNode)
{
	return BDKitRemoveListNodeWithHandler (hListCtx, pNode, DefaultCompare);
}

VOID BDKitClearListNodeWithHandler(__in HANDLE hListCtx, __in LIST_CLEAR_HANDLER pfnHandler)
{
	PLIST_DATA_CONTEXT	pListCtx	= (PLIST_DATA_CONTEXT)hListCtx;
	PLIST_ENTRY			pListInter	= NULL;
	PLIST_NODE			pListNode	= NULL;

	FltAcquireResourceExclusive (&pListCtx->listResourceLock);
	{
		while(!IsListEmpty(&pListCtx->listHead))
		{
			pListInter= RemoveHeadList(&pListCtx->listHead);
			pListNode = (PLIST_NODE)CONTAINING_RECORD(pListInter, LIST_NODE, listEntry);
			if (pfnHandler != NULL)
			{
				pfnHandler(pListNode->pData);
			}
			ExFreeToNPagedLookasideList(&pListCtx->listLookaside, pListNode);
			pListCtx->listCount--;
		}
	}
	FltReleaseResource (&pListCtx->listResourceLock);
}

VOID BDKitClearListNode(__in HANDLE hListCtx)
{
	BDKitClearListNodeWithHandler (hListCtx, DefaultClear);
}

VOID DefaultClear(__in PVOID pNode)
{
	UNREFERENCED_PARAMETER(pNode);
}

BOOLEAN DefaultCompare(__in PVOID pNode1, __in PVOID pNode2)
{
	return pNode1 == pNode2;
}