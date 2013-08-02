#if !defined(AFX_CONTACTDLG_H__994642C0_E6BE_46B6_83BD_E93699C6C974__INCLUDED_)
#define AFX_CONTACTDLG_H__994642C0_E6BE_46B6_83BD_E93699C6C974__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContactDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog

class CContactDlg : public CDialog
{
// Construction
public:
	void OnCancel();
	BOOL Create();
	CContactDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContactDlg)
	enum { IDD = IDD_CONTACT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContactDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContactDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkTreeContact(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CView* m_pView;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTDLG_H__994642C0_E6BE_46B6_83BD_E93699C6C974__INCLUDED_)
