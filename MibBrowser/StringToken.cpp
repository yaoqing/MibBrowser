// StringToken.cpp: implementation of the CStringToken class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StringToken.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringToken::CStringToken(TCHAR delim):m_nCount(0),m_cDelim(delim)
{
	SpitAll();
}

CStringToken::CStringToken(CString str, TCHAR delim):m_nCount(0),m_clBuffer(str),m_cDelim(delim)
{
	SpitAll();
}

CStringToken::~CStringToken()
{
}

void CStringToken::SetString(char * ptr)
{
	m_clBuffer = ptr;
	SpitAll();
}

void CStringToken::SetString(CString str)
{
	m_clBuffer = str;
	SpitAll();
}

void CStringToken::SpitAll()
{
	m_nCount = 0;
	map.RemoveAll();
	rmap.RemoveAll();
	if (m_clBuffer.GetLength() <= 0)
		return;

	CString s;
	int offset, pos;
	offset = pos = 0;
	while (true)
	{
		pos = m_clBuffer.Find(m_cDelim, offset);
		if (pos == -1)
		{
			int len = m_clBuffer.GetLength();
			if (offset >= len)
				break;
			else
				pos = len;
		}
		else if (pos == offset)
		{
			offset++;
			continue;
		}
		s = m_clBuffer.Mid(offset, pos-offset);
		offset = ++pos;
		map.SetAt(s, m_nCount);
		rmap.SetAt(m_nCount, s);
		m_nCount += 1;
	}	
}

BOOL CStringToken::HasKey(CString key, int &value)
{
	return map.Lookup(key, value);
}

int CStringToken::CountToke()
{
	return m_nCount;
}

CString CStringToken::GetAt(int index)
{
	CString value;
	if (index >= 0 && index < m_nCount)
		rmap.Lookup(index, value);
	return value;
}