// CSGuard.h: interface for the CCSGuard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSGUARD_H__83D4C90B_25B3_4ACB_95AB_526D732A6704__INCLUDED_)
#define AFX_CSGUARD_H__83D4C90B_25B3_4ACB_95AB_526D732A6704__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCSGuard  
{
public:
	CCSGuard(LPCRITICAL_SECTION pcs);
	~CCSGuard();

private:
	LPCRITICAL_SECTION m_pcs;
};

#endif // !defined(AFX_CSGUARD_H__83D4C90B_25B3_4ACB_95AB_526D732A6704__INCLUDED_)
