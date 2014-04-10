#pragma once


typedef BOOLEAN LIST_CMP_HANDLER(
								 __in PVOID pNode1, 
								 __in PVOID pNode2
								 );
typedef VOID LIST_CLEAR_HANDLER(__in PVOID pNode);

HANDLE BDKitInitializeList(ULONG listTag);

VOID BDKitInsertListNode(
						 __in HANDLE	hListCtx, 
						 __in PVOID		pNode
						 );

PVOID BDKitFindListNode(
						__in HANDLE		hListCtx, 
						__in PVOID		pNode
						);

PVOID BDKitFindListNodeWithHandler(
								   __in HANDLE				hListCtx,
								   __in PVOID				pNode, 
								   __in LIST_CMP_HANDLER	pfnHandler
								   );

BOOLEAN BDKitRemoveListNodeWithHandler( 
									__in HANDLE				hListCtx, 
									__in PVOID				pNode, 
									__in LIST_CMP_HANDLER	pfnHandler 
									);

BOOLEAN BDKitRemoveListNode(__in HANDLE hListCtx, __in PVOID pNode);

VOID BDKitCloseListHandle(__in HANDLE hListCtx);

VOID BDKitCloseListHandleWithHandler(
									 __in HANDLE hListCtx, 
									 __in LIST_CLEAR_HANDLER pfnHandler
									 );

VOID BDKitClearListNodeWithHandler(
								   __in HANDLE hListCtx, 
								   __in LIST_CLEAR_HANDLER pfnHandler
								   );