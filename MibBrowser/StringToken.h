// StringToken.h: interface for the CStringToken class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGTOKEN_H__5C9C92C1_97ED_49C2_A860_CA56CB3682C3__INCLUDED_)
#define AFX_STRINGTOKEN_H__5C9C92C1_97ED_49C2_A860_CA56CB3682C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStringToken
{
public:
	CStringToken(TCHAR delim = _T(' '));
	CStringToken(CString str, TCHAR delim = _T(' '));
	virtual ~CStringToken();

	int CountToke();
	void SetString(char * ptr);
	void SetString(CString str);
	BOOL HasKey(CString key, int &value);
	CString GetAt(int index);

protected:
	int m_nCount;
	CString m_clBuffer;
	TCHAR m_cDelim;
	CMap<CString, CString&, int, int> map;
	CMap<int, int, CString, CString&> rmap;

	void SpitAll();
};

#endif // !defined(AFX_STRINGTOKEN_H__5C9C92C1_97ED_49C2_A860_CA56CB3682C3__INCLUDED_)
