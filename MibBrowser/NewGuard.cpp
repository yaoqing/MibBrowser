// NewGuard.cpp: implementation of the CNewGuard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mibbrowser.h"
#include "NewGuard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewGuard::CNewGuard(char *ptr)
{
	m_ptr = ptr;
}

CNewGuard::~CNewGuard()
{
	delete [] m_ptr;
}
