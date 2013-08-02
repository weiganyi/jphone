// JPhoneDlg.h : header file
//

#if !defined(AFX_JPHONEDLG_H__269DB38C_476C_413B_BB58_E9AF50B02FFD__INCLUDED_)
#define AFX_JPHONEDLG_H__269DB38C_476C_413B_BB58_E9AF50B02FFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CJPhoneDlg dialog

class CJPhoneDlg : public CDialog
{
// Construction
public:
	CImageList m_imageList;
	JPhoneAgent* GetAgent();
	CJPhoneDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CJPhoneDlg)
	enum { IDD = IDD_JPHONE_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJPhoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

    friend void CALLBACK lpStatusTimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CJPhoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton0();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButtonOffhook();
	afx_msg void OnButtonOnhook();
	afx_msg void OnButtonPound();
	afx_msg void OnButtonStar();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPopCfg();
	afx_msg void OnPopContact();
	afx_msg void OnPopAbout();
	afx_msg void OnPopExit();
	afx_msg void OnPopDumpMemory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JPhoneAgent* m_pAgent;
	JUINT32 m_statusTimer;
	JPjSipUaCallStatus m_clsCallStatus;
	CContactDlg* m_pContactDlg;
};

#define JDLG_STATUS_BUFFER_LEN      128
#define JDLG_STATUS_TIMER_PERIOD    500

extern CJPhoneDlg* GetCJPhoneDlg();

extern CRITICAL_SECTION g_criticalSection;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JPHONEDLG_H__269DB38C_476C_413B_BB58_E9AF50B02FFD__INCLUDED_)
