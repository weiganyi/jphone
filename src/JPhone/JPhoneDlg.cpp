// JPhoneDlg.cpp : implementation file
//

#include "stdafx.h"

#include "JFrameWork.h"

using namespace JFrameWork;

#include "JSipUa.h"
#include "JPhoneAgent.h"
#include "JPhoneThread.h"

#include "JPhone.h"
#include "CfgDlg.h"
#include "ContactDlg.h"
#include "JPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJPhoneDlg dialog

CRITICAL_SECTION g_criticalSection;
CJPhoneDlg*  g_CJPhoneDlg;

void CALLBACK lpStatusTimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    CJPhoneDlg *pDlg = reinterpret_cast<CJPhoneDlg*>(dwUser);
    char displayBuffer[JDLG_STATUS_BUFFER_LEN] = {0};

    //get call status
    EnterCriticalSection(&g_criticalSection);
    pDlg->m_pAgent->GetStatus();
    pDlg->m_pAgent->GetStatusRsp(&pDlg->m_clsCallStatus);
    LeaveCriticalSection(&g_criticalSection);

    if (pDlg->m_clsCallStatus.GetStatus().GetLength())
    {
        strcpy(displayBuffer, pDlg->m_clsCallStatus.GetStatus().c_str());

        if (pDlg->m_clsCallStatus.GetNumber().GetLength())
        {
            strcat(displayBuffer, ": ");
            strcat(displayBuffer, pDlg->m_clsCallStatus.GetNumber().c_str());
        }
    }

    //display the call status
    if (strlen(displayBuffer) != 0)
    {
        pDlg->SetDlgItemText(IDC_STATIC_DISPLAY, displayBuffer);
    }

    //start the timer again
    pDlg->m_statusTimer = 
        timeSetEvent(JDLG_STATUS_TIMER_PERIOD, 0, lpStatusTimeProc, dwUser, TIME_ONESHOT);

	return;
}

void SetCJPhoneDlg(CJPhoneDlg* pCJPhoneDlg)
{
    g_CJPhoneDlg = pCJPhoneDlg;

	return;
}

CJPhoneDlg* GetCJPhoneDlg()
{
    return g_CJPhoneDlg;
}

CJPhoneDlg::CJPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJPhoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJPhoneDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	InitializeCriticalSection(&g_criticalSection);

	SetCJPhoneDlg(this);

	return;
}

void CJPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJPhoneDlg)
	//}}AFX_DATA_MAP

	return;
}

BEGIN_MESSAGE_MAP(CJPhoneDlg, CDialog)
	//{{AFX_MSG_MAP(CJPhoneDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_0, OnButton0)
	ON_BN_CLICKED(IDC_BUTTON_1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON_5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON_6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON_7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON_8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON_9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON_OFFHOOK, OnButtonOffhook)
	ON_BN_CLICKED(IDC_BUTTON_ONHOOK, OnButtonOnhook)
	ON_BN_CLICKED(IDC_BUTTON_POUND, OnButtonPound)
	ON_BN_CLICKED(IDC_BUTTON_STAR, OnButtonStar)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POP_CFG, OnPopCfg)
	ON_COMMAND(ID_POP_CONTACT, OnPopContact)
	ON_COMMAND(ID_POP_ABOUT, OnPopAbout)
	ON_COMMAND(ID_POP_EXIT, OnPopExit)
	ON_COMMAND(ID_POP_DUMP_MEMORY, OnPopDumpMemory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJPhoneDlg message handlers

BOOL CJPhoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	HANDLE pThreadId = JNULL;
    HICON hIcon = 0;

    WORD wVersion;
    WSADATA WSAData;
    MMRESULT ret;

    wVersion=MAKEWORD(2,0);

    //initialize socket library
    WSAStartup(wVersion,&WSAData);

    //create JPhoneAgent object
	m_pAgent = new JPhoneAgent;

    //create main thread
    pThreadId = CreateThread(JNULL, JNULL, (LPTHREAD_START_ROUTINE)JPhoneThreadProc, 
        JNULL, 0, (LPDWORD)JNULL);
    if (!pThreadId)
    {
        cout << "CJPhoneDlg::OnInitDialog call CreateThread return failure" << endl;
        return FALSE;
    }

    //load tel icon
    m_imageList.Create(16, 16, 0, 1, 1);
    hIcon = AfxGetApp()->LoadIcon(IDI_ICON_TEL);
    m_imageList.Add(hIcon);

    //create status query timer
    ret = timeBeginPeriod(JDLG_STATUS_TIMER_PERIOD);
    if (ret == TIMERR_NOERROR)
    {
        m_statusTimer = 
            timeSetEvent(JDLG_STATUS_TIMER_PERIOD, 0, lpStatusTimeProc, 
            reinterpret_cast<DWORD>(this), TIME_ONESHOT);
    }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CJPhoneDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}

	return;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CJPhoneDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	return;
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CJPhoneDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CJPhoneDlg::OnButton0() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_0);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_1);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_2);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_3);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_4);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_5);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_6);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_7);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_8);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButton9() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_9);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButtonOffhook() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_OFFHOOK);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButtonOnhook() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_ONHOOK);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButtonPound() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_POUND);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnButtonStar() 
{
	// TODO: Add your control notification handler code here
    EnterCriticalSection(&g_criticalSection);
	m_pAgent->PressKey(JSIPUA_KEY_STAR);
    LeaveCriticalSection(&g_criticalSection);

	return;
}

void CJPhoneDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu clsPopMenu;

	clsPopMenu.LoadMenu(IDR_MENU_POP);
	clsPopMenu.GetSubMenu(0)
		->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);

	return;
}

void CJPhoneDlg::OnPopCfg() //model dialog
{
	// TODO: Add your command handler code here
	CCfgDlg dlgCfg;
	JPjSipUaCfg clsSipUaCfg;
	JLogCfg clsLogCfg;
	JDaemonCfg clsDaemonCfg;
	BOOL bLogFile = false;
	BOOL bLogRemote = false;
	unsigned int uiSaveMethod = 0;

    EnterCriticalSection(&g_criticalSection);
    m_pAgent->GetSipUaCfg();
    m_pAgent->GetSipUaCfgRsp(&clsSipUaCfg);
    m_pAgent->GetLogCfg();
    m_pAgent->GetLogCfgRsp(&clsLogCfg);
    m_pAgent->GetDaemonCfg();
    m_pAgent->GetDaemonCfgRsp(&clsDaemonCfg);
    LeaveCriticalSection(&g_criticalSection);

    //get the sipua config
    dlgCfg.m_number = clsSipUaCfg.GetNumber().c_str();
    dlgCfg.m_authname = clsSipUaCfg.GetAuthName().c_str();
    dlgCfg.m_authpasswd = clsSipUaCfg.GetAuthPasswd().c_str();
    dlgCfg.m_proxyaddr = clsSipUaCfg.GetProxyAddr().c_str();

    //get the log config
    dlgCfg.m_logaddr = clsLogCfg.GetLogAddress().c_str();
    dlgCfg.m_logport = clsLogCfg.GetLogPort().c_str();

    if (clsLogCfg.GetOutputFile() == JLOG_FILE_DISABLE)
    {
        dlgCfg.m_logfile = 0;
        bLogFile = 0;
    }
    else if (clsLogCfg.GetOutputFile() == JLOG_FILE_ENABLE)
    {
        dlgCfg.m_logfile = 1;
        bLogFile = 1;
    }
    if (clsLogCfg.GetOutputRemote() == JLOG_REMOTE_DISABLE)
    {
        dlgCfg.m_logremote = 0;
        bLogRemote = 0;
    }
    else if (clsLogCfg.GetOutputRemote() == JLOG_REMOTE_ENABLE)
    {
        dlgCfg.m_logremote = 1;
        bLogRemote = 1;
    }

    //get the daemon config
    dlgCfg.m_saveMethod = 0;
    if (clsDaemonCfg.GetSaveMethod() == JDAEMON_SAVE_SQL_ENABLED)
    {
        dlgCfg.m_saveMethod = 0;
        uiSaveMethod = 0;
    }
    else if (clsDaemonCfg.GetSaveMethod() == JDAEMON_SAVE_XML_ENABLED)
    {
        dlgCfg.m_saveMethod = 1;
        uiSaveMethod = 1;
    }

    //show the cfg dialog
	dlgCfg.DoModal();

    //set the sipua config
    if (clsSipUaCfg.GetNumber() != dlgCfg.m_number.GetBuffer(JMAX_STRING_LEN) ||
        clsSipUaCfg.GetAuthName() != dlgCfg.m_authname.GetBuffer(JMAX_STRING_LEN) ||
        clsSipUaCfg.GetAuthPasswd() != dlgCfg.m_authpasswd.GetBuffer(JMAX_STRING_LEN) ||
        clsSipUaCfg.GetProxyAddr() != dlgCfg.m_proxyaddr.GetBuffer(JMAX_STRING_LEN))
    {
    	JString number;
    	number = dlgCfg.m_number.GetBuffer(JMAX_STRING_LEN);
	    clsSipUaCfg.SetNumber(number);

    	JString authname;
    	authname = dlgCfg.m_authname.GetBuffer(JMAX_STRING_LEN);
	    clsSipUaCfg.SetAuthName(authname);

    	JString authpasswd;
    	authpasswd = dlgCfg.m_authpasswd.GetBuffer(JMAX_STRING_LEN);
	    clsSipUaCfg.SetAuthPasswd(authpasswd);

    	JString proxyaddr;
    	proxyaddr = dlgCfg.m_proxyaddr.GetBuffer(JMAX_STRING_LEN);
	    clsSipUaCfg.SetProxyAddr(proxyaddr);

        EnterCriticalSection(&g_criticalSection);
        m_pAgent->SetSipUaCfg(&clsSipUaCfg);
        LeaveCriticalSection(&g_criticalSection);
    }

    //set the log config
    if (clsLogCfg.GetLogAddress() != dlgCfg.m_logaddr.GetBuffer(JMAX_STRING_LEN) ||
        clsLogCfg.GetLogPort() != dlgCfg.m_logport.GetBuffer(JMAX_STRING_LEN) ||
        bLogFile != dlgCfg.m_logfile ||
        bLogRemote != dlgCfg.m_logremote)
    {
    	JString address, port;
    	address = dlgCfg.m_logaddr.GetBuffer(JMAX_STRING_LEN);
	    clsLogCfg.SetLogAddress(address);

    	port = dlgCfg.m_logport.GetBuffer(JMAX_STRING_LEN);
	    clsLogCfg.SetLogPort(port);

        if (dlgCfg.m_logfile == 0)
        {
	    	JString outputfile;
	    	outputfile = JLOG_FILE_DISABLE;
		    clsLogCfg.SetOutputFile(outputfile);
        }
        else if (dlgCfg.m_logfile == 1)
        {
	    	JString outputfile;
	    	outputfile = JLOG_FILE_ENABLE;
		    clsLogCfg.SetOutputFile(outputfile);
        }
        if (dlgCfg.m_logremote == 0)
        {
	    	JString outputremote;
	    	outputremote = JLOG_REMOTE_DISABLE;
		    clsLogCfg.SetOutputRemote(outputremote);
        }
        else if (dlgCfg.m_logremote == 1)
        {
	    	JString outputremote;
	    	outputremote = JLOG_REMOTE_ENABLE;
		    clsLogCfg.SetOutputRemote(outputremote);
        }

        EnterCriticalSection(&g_criticalSection);
        m_pAgent->SetLogCfg(&clsLogCfg);
        LeaveCriticalSection(&g_criticalSection);
    }

    //set the daemon config
    if (uiSaveMethod != dlgCfg.m_saveMethod)
    {
        if (dlgCfg.m_saveMethod == 0)
        {
        	JString saveMethod;
        	saveMethod = JDAEMON_SAVE_SQL_ENABLED;
    	    clsDaemonCfg.SetSaveMethod(saveMethod);
        }
        else if (dlgCfg.m_saveMethod == 1)
        {
        	JString saveMethod;
        	saveMethod = JDAEMON_SAVE_XML_ENABLED;
    	    clsDaemonCfg.SetSaveMethod(saveMethod);
        }

        EnterCriticalSection(&g_criticalSection);
        m_pAgent->SetDaemonCfg(&clsDaemonCfg);
        LeaveCriticalSection(&g_criticalSection);
    }

    return;
}

void CJPhoneDlg::OnPopContact() //unmodel dialog
{
	// TODO: Add your command handler code here
	m_pContactDlg = new CContactDlg(this);

	m_pContactDlg->Create();

    m_pContactDlg->ShowWindow(SW_SHOW);

	return;
}

void CJPhoneDlg::OnPopAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;

	dlgAbout.DoModal();

	return;
}

JPhoneAgent* CJPhoneDlg::GetAgent()
{
    return m_pAgent;
}

void CJPhoneDlg::OnPopExit() 
{
	// TODO: Add your command handler code here
	CDialog::OnCancel();

	return;
}

void CJPhoneDlg::OnPopDumpMemory() 
{
	// TODO: Add your command handler code here
    EnterCriticalSection(&g_criticalSection);
    m_pAgent->DumpMemory();
    LeaveCriticalSection(&g_criticalSection);

	return;
}
