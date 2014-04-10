#pragma once


#include "stdafx.h"
#include "Utility.h"


static void* GetProcAddress( LPCWSTR szLoadedModuleName, LPCSTR szProcName )
{
	HMODULE h( GetModuleHandle(szLoadedModuleName) );
	if( h == NULL )
		return h;

	return ::GetProcAddress( h, szProcName );
}

#define DEFINE_LOADED_PROC( ModuleName, RetType, CallType, ProcName, Params )  \
	typedef RetType (CallType *ProcName##Type) Params;      \
	static const ProcName##Type p##ProcName( reinterpret_cast<ProcName##Type>(::GetProcAddress(ModuleName,#ProcName)) )

DEFINE_LOADED_PROC( L"kernel32.dll", BOOL, WINAPI, IsWow64Process, (HANDLE,PBOOL) );

BOOL OSInfo::Is32Bit()
{
	return !Is64Bit ();
}

BOOL OSInfo::Is64Bit()
{
#ifdef _WIN64
	return TRUE;
#else
	static bool g_bWow64Initialized( false );
	static bool g_bIs64Bit( false );

	if( g_bWow64Initialized )
		return g_bIs64Bit;

	if( pIsWow64Process == NULL )
		return FALSE;

	BOOL bIsWow64( FALSE );
	if( pIsWow64Process(GetCurrentProcess(),&bIsWow64) == FALSE )
		return FALSE;

	g_bIs64Bit = bIsWow64 != FALSE;
	g_bWow64Initialized = true;
	return g_bIs64Bit;
#endif
}

DEFINE_LOADED_PROC( L"kernel32.dll", BOOL, WINAPI, Wow64DisableWow64FsRedirection, (PVOID*) );
DEFINE_LOADED_PROC( L"kernel32.dll", BOOL, WINAPI, Wow64RevertWow64FsRedirection, (PVOID) );

File::CWow64RedirDisable::CWow64RedirDisable( bool bAutoDisable /*= false*/ )
: m_OldValue(NULL)
{
	if( bAutoDisable )
		DisableRedirection();
}

File::CWow64RedirDisable::~CWow64RedirDisable()
{
	 Revert();
}

bool File::CWow64RedirDisable::DisableRedirection()
{
	if( pWow64DisableWow64FsRedirection == NULL )
		return false;
	return pWow64DisableWow64FsRedirection(&m_OldValue) != FALSE;
}

bool File::CWow64RedirDisable::Revert()
{
	if( pWow64RevertWow64FsRedirection == NULL )
		return false;
	return pWow64RevertWow64FsRedirection(m_OldValue) != FALSE;
}
