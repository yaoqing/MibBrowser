// NewGuard.h: interface for the CNewGuard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWGUARD_H__8806B931_4E7D_4FE1_AB10_263DFBCE8867__INCLUDED_)
#define AFX_NEWGUARD_H__8806B931_4E7D_4FE1_AB10_263DFBCE8867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNewGuard  
{
public:
	CNewGuard(char *ptr);
	virtual ~CNewGuard();
	
protected:
	char * m_ptr;

};

#endif // !defined(AFX_NEWGUARD_H__8806B931_4E7D_4FE1_AB10_263DFBCE8867__INCLUDED_)
