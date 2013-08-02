// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__915B3757_FFA6_4A2E_951D_49337A045110__INCLUDED_)
#define AFX_MAINFRM_H__915B3757_FFA6_4A2E_951D_49337A045110__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

    friend void CALLBACK lpStatusTimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);

// Implementation
public:
	JUINT32 GetCurrLogSrv();
	JLogServerAgent* GetAgent();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCfgListen();
	afx_msg void OnCleanLog();
	afx_msg void OnServer1();
	afx_msg void OnServer2();
	afx_msg void OnServer10();
	afx_msg void OnServer3();
	afx_msg void OnServer4();
	afx_msg void OnServer5();
	afx_msg void OnServer6();
	afx_msg void OnServer7();
	afx_msg void OnServer8();
	afx_msg void OnServer9();
	afx_msg void OnUpdateServer1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServer2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServer10(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServer3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServer4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServer5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServer6(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServer7(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServer8(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServer9(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
   	JLogServerAgent* m_pAgent;
	JUINT32 m_statusTimer;

	JUINT32 m_oldLogSrv;
	JUINT32 m_currLogSrv;

	JString m_remoteAddr[JLOGSRV_MAX_MSG_SRV];
};

#define JFRAME_GET_MSG_TIMER_PERIOD    500

extern CRITICAL_SECTION g_criticalSection;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__915B3757_FFA6_4A2E_951D_49337A045110__INCLUDED_)
