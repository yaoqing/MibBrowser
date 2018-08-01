// StringEx.h: interface for the CStringEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGEX_H__30D76843_B469_41BA_85E1_FD8D89033B90__INCLUDED_)
#define AFX_STRINGEX_H__30D76843_B469_41BA_85E1_FD8D89033B90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStringEx
{
public:
	CStringEx();
	CStringEx(CString &sss);
	virtual ~CStringEx();

	char * toChar();
	CStringEx & operator = (char *ptr);
	CStringEx & operator = (wchar_t *ptr);
	CStringEx & operator = (CString &sss);

protected:
	CString m_sss;
};

#endif // !defined(AFX_STRINGEX_H__30D76843_B469_41BA_85E1_FD8D89033B90__INCLUDED_)
