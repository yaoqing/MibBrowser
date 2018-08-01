// CSGuard.cpp: implementation of the CCSGuard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mibbrowser.h"
#include "CSGuard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCSGuard::CCSGuard(LPCRITICAL_SECTION pcs)
{
	m_pcs = pcs;
	EnterCriticalSection(m_pcs);
}

CCSGuard::~CCSGuard()
{
	LeaveCriticalSection(m_pcs);
	m_pcs = NULL;
}
