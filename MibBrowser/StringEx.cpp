// StringEx.cpp: implementation of the CStringEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mibbrowser.h"
#include "StringEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringEx::CStringEx()
{
}

CStringEx::CStringEx(CString &sss):m_sss(sss)
{
}

CStringEx::~CStringEx()
{
}

char * CStringEx::toChar()
{
	int len;
	char *buff = NULL;
#ifdef _UNICODE
	len = WideCharToMultiByte(CP_ACP, 0, m_sss, m_sss.GetLength(), NULL, 0, 0, 0);
#else
	len = m_sss.GetLength();
#endif
	if (len < 0)
		return buff;
	buff = new char[len+1];
#ifdef _UNICODE
	WideCharToMultiByte(CP_ACP, 0, m_sss, m_sss.GetLength(), buff, len, 0, 0);
#else
	memcpy(buff, m_sss, len);
#endif
	buff[len] = 0;
	return buff;
}

CStringEx & CStringEx::operator = (char *ptr)
{
	m_sss = ptr;
	return *this;
}

CStringEx & CStringEx::operator = (wchar_t *ptr)
{
	m_sss = ptr;
	return *this;
}

CStringEx & CStringEx::operator = (CString &sss)
{
	m_sss = sss;
	return *this;
}