// RegKeyEx.h: interface for the CRegKeyEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGKEYEX_H__FC21585B_5538_4C7E_8136_68D35A879BE2__INCLUDED_)
#define AFX_REGKEYEX_H__FC21585B_5538_4C7E_8136_68D35A879BE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegKeyEx  
{
public:
	CRegKeyEx();
	virtual ~CRegKeyEx();

	HKEY OpenReg(CString szSubKey);
	void CloseReg(HKEY hKey);
	void DeleteReg(CString szSubKey);
	void SetKeyValue(HKEY hKey, CString szKeyName, CString szKeyValue);
	CString GetKeyValue(HKEY hKey, CString szKeyName);

};

#endif // !defined(AFX_REGKEYEX_H__FC21585B_5538_4C7E_8136_68D35A879BE2__INCLUDED_)
