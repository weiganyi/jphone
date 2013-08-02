// CfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JPhone.h"
#include "CfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCfgDlg dialog


CCfgDlg::CCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCfgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCfgDlg)
	m_number = _T("");
	m_authname = _T("");
	m_authpasswd = _T("");
	m_proxyaddr = _T("");
	m_logaddr = _T("");
	m_logport = _T("");
	m_logfile = FALSE;
	m_logremote = FALSE;
	m_saveMethod = -1;
	//}}AFX_DATA_INIT
}


void CCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCfgDlg)
	DDX_Text(pDX, IDC_EDIT1, m_number);
	DDX_Text(pDX, IDC_EDIT2, m_authname);
	DDX_Text(pDX, IDC_EDIT3, m_authpasswd);
	DDX_Text(pDX, IDC_EDIT4, m_proxyaddr);
	DDX_Text(pDX, IDC_EDIT5, m_logaddr);
	DDX_Text(pDX, IDC_EDIT6, m_logport);
	DDX_Check(pDX, IDC_CHECK1, m_logfile);
	DDX_Check(pDX, IDC_CHECK2, m_logremote);
	DDX_Radio(pDX, IDC_RADIO3, m_saveMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCfgDlg, CDialog)
	//{{AFX_MSG_MAP(CCfgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCfgDlg message handlers
