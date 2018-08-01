// LangLoader.h: interface for the CLangLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANGLOADER_H__318F98EC_CB63_46D9_A22B_EA4A8BF8E8C5__INCLUDED_)
#define AFX_LANGLOADER_H__318F98EC_CB63_46D9_A22B_EA4A8BF8E8C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLangLoader  
{
private:
	CLangLoader();
	static CLangLoader *instance;
	CMap<int, int, CString, CString> lang_res;
	CRITICAL_SECTION m_cs;

	int m_lang_id;
	CStringArray order;
	CMap<CString, CString&, CString, CString&> string_res;

	LANGID GetDefaultLanguage();
	void EnumLangRes();

public:
	virtual ~CLangLoader();
	static CLangLoader * GetInstance();

	CString LoadString(CString key);
	bool InitStringRes(LANGID lang_id);
	void SaveToFile();
	void GetLangID(CArray<int, int> &array);
	CString GetLangName(int lang_id);
	int GetCurrLangID();
};

#endif // !defined(AFX_LANGLOADER_H__318F98EC_CB63_46D9_A22B_EA4A8BF8E8C5__INCLUDED_)
