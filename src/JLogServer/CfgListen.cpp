// CfgListen.cpp : implementation file
//

#include "stdafx.h"
#include "JLogServer.h"
#include "CfgListen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCfgListen dialog


CCfgListen::CCfgListen(CWnd* pParent /*=NULL*/)
	: CDialog(CCfgListen::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCfgListen)
	m_address = _T("");
	m_port = _T("");
	//}}AFX_DATA_INIT
}


void CCfgListen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCfgListen)
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_address);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCfgListen, CDialog)
	//{{AFX_MSG_MAP(CCfgListen)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCfgListen message handlers
