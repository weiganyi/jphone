#if !defined(AFX_CFGLISTEN_H__8B7A57E1_C703_4EC2_A5C7_53AF9B0E9A0F__INCLUDED_)
#define AFX_CFGLISTEN_H__8B7A57E1_C703_4EC2_A5C7_53AF9B0E9A0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CfgListen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCfgListen dialog

class CCfgListen : public CDialog
{
// Construction
public:
	CCfgListen(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCfgListen)
	enum { IDD = IDD_CFG_LISTEN };
	CString	m_address;
	CString	m_port;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCfgListen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCfgListen)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFGLISTEN_H__8B7A57E1_C703_4EC2_A5C7_53AF9B0E9A0F__INCLUDED_)
