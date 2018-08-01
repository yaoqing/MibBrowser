#if !defined(AFX_ADDIPCONFWND_H__C21C0BCC_B833_4114_82CB_E768D919BDCB__INCLUDED_)
#define AFX_ADDIPCONFWND_H__C21C0BCC_B833_4114_82CB_E768D919BDCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddIpConfWnd.h : header file
//
#include "EditEx.h"
/////////////////////////////////////////////////////////////////////////////
// CAddIpConfWnd window

class CAddIpConfWnd : public CWnd
{
// Construction
public:
	CAddIpConfWnd(CString *name, CString *ip, CString *community, CString *remark, int *port, bool *isokexit);

// Attributes
public:

// Operations
public:

protected:
	CEditEx m_ctrlName;
	CEditEx m_ctrlIp;
	CEditEx m_ctrlPort;
	CEditEx m_ctrlCommunity;
	CEditEx m_ctrlRemark;
	CButton m_ctrlOk;
	CButton m_ctrlCancel;
	CFont *font;

	int *m_port;
	bool *m_isokexit;
	CString *m_name, *m_ip, *m_community, *m_remark;	

	void DrawLabel(CDC *pDC, CRect &rect, CString szText);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddIpConfWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAddIpConfWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAddIpConfWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDIPCONFWND_H__C21C0BCC_B833_4114_82CB_E768D919BDCB__INCLUDED_)
