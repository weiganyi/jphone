#if !defined(AFX_CFGDLG_H__9222E05D_EF6B_4E0C_81A4_09337195DA06__INCLUDED_)
#define AFX_CFGDLG_H__9222E05D_EF6B_4E0C_81A4_09337195DA06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CfgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCfgDlg dialog

class CCfgDlg : public CDialog
{
// Construction
public:
	CCfgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCfgDlg)
	enum { IDD = IDD_CFG_DIALOG };
	CString	m_number;
	CString	m_authname;
	CString	m_authpasswd;
	CString	m_proxyaddr;
	CString	m_logaddr;
	CString	m_logport;
	BOOL	m_logfile;
	BOOL	m_logremote;
	int		m_saveMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCfgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCfgDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFGDLG_H__9222E05D_EF6B_4E0C_81A4_09337195DA06__INCLUDED_)
