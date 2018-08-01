#if !defined(AFX_PROMPTWND_H__4AFAC73A_B4EE_4C2E_9BA2_512EC50F36DA__INCLUDED_)
#define AFX_PROMPTWND_H__4AFAC73A_B4EE_4C2E_9BA2_512EC50F36DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PromptWnd.h : header file
//
#include "EditEx.h"
#include "ButtonEx.h"
/////////////////////////////////////////////////////////////////////////////
// CPromptWnd window

class CPromptWnd : public CWnd
{
// Construction
public:
	CPromptWnd(CString ip, int port, CString community);

// Attributes
public:

// Operations
public:
	void SetOID(CString szOID);

protected:
	CFont *font;
	CEditEx m_ctrlIp;
	CEditEx m_ctrlPort;
	CEditEx m_ctrlCommunity;
	CEditEx m_ctrlTimeout;
	CEditEx m_ctrlOID;

	CButton m_ctrlClose;
	CButton m_ctrlGet;
	CButton m_ctrlGetNext;
	CButtonEx m_ctrlUseResult;
	CButtonEx m_ctrlRecursive;

	int m_nPort;
	CString m_szIp;
	CString m_szCommunity;
	CString m_szOID;

	int m_get_type; 

	void DrawLabel(CDC *pDC, CRect &rect, CString szText);
	void OnQueryOID();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPromptWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPromptWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPromptWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMPTWND_H__4AFAC73A_B4EE_4C2E_9BA2_512EC50F36DA__INCLUDED_)
