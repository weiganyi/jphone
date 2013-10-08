// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "JLogServer.h"

#include "JFrameWork.h"

using namespace JFrameWork;

#include "JLogSrv.h"
#include "JLogSrvAgent.h"
#include "JLogSrvThread.h"

#include "MainFrm.h"
#include "CfgListen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_CFG_LISTEN, OnCfgListen)
	ON_COMMAND(ID_CLEAN_LOG, OnCleanLog)
	ON_COMMAND(ID_SERVER1, OnServer1)
	ON_COMMAND(ID_SERVER2, OnServer2)
	ON_COMMAND(ID_SERVER10, OnServer10)
	ON_COMMAND(ID_SERVER3, OnServer3)
	ON_COMMAND(ID_SERVER4, OnServer4)
	ON_COMMAND(ID_SERVER5, OnServer5)
	ON_COMMAND(ID_SERVER6, OnServer6)
	ON_COMMAND(ID_SERVER7, OnServer7)
	ON_COMMAND(ID_SERVER8, OnServer8)
	ON_COMMAND(ID_SERVER9, OnServer9)
	ON_UPDATE_COMMAND_UI(ID_SERVER1, OnUpdateServer1)
	ON_UPDATE_COMMAND_UI(ID_SERVER2, OnUpdateServer2)
	ON_UPDATE_COMMAND_UI(ID_SERVER10, OnUpdateServer10)
	ON_UPDATE_COMMAND_UI(ID_SERVER3, OnUpdateServer3)
	ON_UPDATE_COMMAND_UI(ID_SERVER4, OnUpdateServer4)
	ON_UPDATE_COMMAND_UI(ID_SERVER5, OnUpdateServer5)
	ON_UPDATE_COMMAND_UI(ID_SERVER6, OnUpdateServer6)
	ON_UPDATE_COMMAND_UI(ID_SERVER7, OnUpdateServer7)
	ON_UPDATE_COMMAND_UI(ID_SERVER8, OnUpdateServer8)
	ON_UPDATE_COMMAND_UI(ID_SERVER9, OnUpdateServer9)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CRITICAL_SECTION g_criticalSection;

void CALLBACK lpStatusTimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
    JUINT32 uiIdx = 0;
    JUINT8 bHasMsg = JFALSE;
    CMainFrame *pMainFrame = reinterpret_cast<CMainFrame*>(dwUser);
	JLogSrvNumber clsLogSrvNumber;
	JLogSrvHasNewMsg clsLogSrvHasNewMsg;
	JString strCurrSrv;
	JCHAR currSrv[3] = {0};

    EnterCriticalSection(&g_criticalSection);
    if (pMainFrame->m_currLogSrv != pMainFrame->m_oldLogSrv)
    {
        bHasMsg = JTRUE;
        pMainFrame->m_oldLogSrv = pMainFrame->m_currLogSrv;
    }
    else
    {
        //check whether new message coming
        sprintf(currSrv, "%u", pMainFrame->m_currLogSrv);
        strCurrSrv = currSrv;
        clsLogSrvNumber.SetSrvNumber(strCurrSrv);
        pMainFrame->m_pAgent->GetHasNewMsg(&clsLogSrvNumber);
        pMainFrame->m_pAgent->GetHasNewMsgRsp(&clsLogSrvHasNewMsg);

        if (clsLogSrvHasNewMsg.GetHasNewMsg().GetLength())
        {
            bHasMsg = atoi(clsLogSrvHasNewMsg.GetHasNewMsg().c_str());
        }

        //save the remote address that already connected
        for (uiIdx=0; uiIdx<JLOGSRV_MAX_MSG_SRV; uiIdx++)
        {
            if (pMainFrame->m_remoteAddr[uiIdx] != clsLogSrvHasNewMsg.GetRmtAddr(uiIdx))
            {
                pMainFrame->m_remoteAddr[uiIdx] = clsLogSrvHasNewMsg.GetRmtAddr(uiIdx);
            }
        }
    }
    LeaveCriticalSection(&g_criticalSection);

    //let client to repaint
    if (bHasMsg)
    {
        pMainFrame->Invalidate();
    }

    //start a timer to check new message again
    pMainFrame->m_statusTimer = 
        timeSetEvent(JFRAME_GET_MSG_TIMER_PERIOD, 0, lpStatusTimeProc, dwUser, TIME_ONESHOT);

	return;
}

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	InitializeCriticalSection(&g_criticalSection);

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    // TODO: Add your command handler code here
	HANDLE pThreadId = JNULL;
    MMRESULT ret;
    JUINT32 uiIdx = 0;

    WORD wVersion;
    WSADATA WSAData;
    wVersion=MAKEWORD(2,0);

    //initialize socket library
    WSAStartup(wVersion,&WSAData);

    //create JAgent object
	m_pAgent = new JLogServerAgent;

    //create main thread
    pThreadId = CreateThread(JNULL, JNULL, (LPTHREAD_START_ROUTINE)JLogSrvThreadProc, 
        JNULL, 0, (LPDWORD)JNULL);
    if (!pThreadId)
    {
        cout << "CMainFrame::OnCreate call CreateThread return failure" << endl;
        return -1;
    }

    //create status query timer
    ret = timeBeginPeriod(JFRAME_GET_MSG_TIMER_PERIOD);
    if (ret == TIMERR_NOERROR)
    {
        m_statusTimer = 
            timeSetEvent(JFRAME_GET_MSG_TIMER_PERIOD, 0, lpStatusTimeProc, 
            reinterpret_cast<DWORD>(this), TIME_ONESHOT);
    }

	m_oldLogSrv = 1;
    m_currLogSrv = 1;

    for (uiIdx=0; uiIdx<JLOGSRV_MAX_MSG_SRV; uiIdx++)
    {
        m_remoteAddr[uiIdx].Clear();
    }

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

    cs.style &= ~FWS_ADDTOTITLE;    //clean the name of document

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnCfgListen() 
{
	// TODO: Add your command handler code here
	CCfgListen listenCfg;
	JLogSrvCfg clsLogSrvCfg;

    //get log server config
    EnterCriticalSection(&g_criticalSection);
    m_pAgent->GetLogSrvCfg();
    m_pAgent->GetLogSrvCfgRsp(&clsLogSrvCfg);
    LeaveCriticalSection(&g_criticalSection);

    //save the config
    listenCfg.m_address = clsLogSrvCfg.GetListenAddr().c_str();
    listenCfg.m_port = clsLogSrvCfg.GetListenPort().c_str();

    //show the dialog box
	listenCfg.DoModal();

    //if the config had changed, save them to the main thread
    if (clsLogSrvCfg.GetListenAddr() != listenCfg.m_address.GetBuffer(JMAX_STRING_LEN) ||
        clsLogSrvCfg.GetListenPort() != listenCfg.m_port.GetBuffer(JMAX_STRING_LEN))
    {
    	JString address;
    	address = listenCfg.m_address.GetBuffer(JMAX_STRING_LEN);
	    clsLogSrvCfg.SetListenAddr(address);

    	JString listenport;
    	listenport = listenCfg.m_port.GetBuffer(JMAX_STRING_LEN);
	    clsLogSrvCfg.SetListenPort(listenport);

        EnterCriticalSection(&g_criticalSection);
        m_pAgent->SetLogSrvCfg(&clsLogSrvCfg);
        LeaveCriticalSection(&g_criticalSection);
    }

    return;
	
}

void CMainFrame::OnCleanLog() 
{
	// TODO: Add your command handler code here
	JLogSrvNumber clsLogSrvNumber;
	JString strCurrSrv;
	JCHAR currSrv[3] = {0};

    EnterCriticalSection(&g_criticalSection);
    sprintf(currSrv, "%u", m_currLogSrv);
    strCurrSrv = currSrv;
    clsLogSrvNumber.SetSrvNumber(strCurrSrv);
    m_pAgent->CleanLog(&clsLogSrvNumber);
    LeaveCriticalSection(&g_criticalSection);

    Invalidate();
}

void CMainFrame::OnServer1() 
{
	// TODO: Add your command handler code here
    EnterCriticalSection(&g_criticalSection);
	m_currLogSrv = 1;
    LeaveCriticalSection(&g_criticalSection);
}

void CMainFrame::OnServer2() 
{
	// TODO: Add your command handler code here
    EnterCriticalSection(&g_criticalSection);
	m_currLogSrv = 2;
    LeaveCriticalSection(&g_criticalSection);
}

void CMainFrame::OnServer10() 
{
	// TODO: Add your command handler code here
    EnterCriticalSection(&g_criticalSection);
	m_currLogSrv = 10;
    LeaveCriticalSection(&g_criticalSection);
}

void CMainFrame::OnServer3() 
{
	// TODO: Add your command handler code here
    EnterCriticalSection(&g_criticalSection);
	m_currLogSrv = 3;
    LeaveCriticalSection(&g_criticalSection);
}

void CMainFrame::OnServer4() 
{
	// TODO: Add your command handler code here
    EnterCriticalSection(&g_criticalSection);
	m_currLogSrv = 4;
    LeaveCriticalSection(&g_criticalSection);
}

void CMainFrame::OnServer5() 
{
	// TODO: Add your command handler code here
    EnterCriticalSection(&g_criticalSection);
	m_currLogSrv = 5;
    LeaveCriticalSection(&g_criticalSection);
}

void CMainFrame::OnServer6() 
{
	// TODO: Add your command handler code here
    EnterCriticalSection(&g_criticalSection);
	m_currLogSrv = 6;
    LeaveCriticalSection(&g_criticalSection);
}

void CMainFrame::OnServer7() 
{
	// TODO: Add your command handler code here
    EnterCriticalSection(&g_criticalSection);
	m_currLogSrv = 7;
    LeaveCriticalSection(&g_criticalSection);
}

void CMainFrame::OnServer8() 
{
	// TODO: Add your command handler code here
    EnterCriticalSection(&g_criticalSection);
	m_currLogSrv = 8;
    LeaveCriticalSection(&g_criticalSection);
}

void CMainFrame::OnServer9() 
{
	// TODO: Add your command handler code here
    EnterCriticalSection(&g_criticalSection);
	m_currLogSrv = 9;
    LeaveCriticalSection(&g_criticalSection);
}


JLogServerAgent* CMainFrame::GetAgent()
{
	return m_pAgent;
}

JUINT32 CMainFrame::GetCurrLogSrv()
{
	return m_currLogSrv;
}

void CMainFrame::OnUpdateServer1(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    JCHAR strMenuText[JMAX_STRING_LEN] = {0};

    strcpy(strMenuText, "Server 1 ");
    if (m_remoteAddr[0].GetLength())
    {
	    strcat(strMenuText, "[");
	    strcat(strMenuText, m_remoteAddr[0].c_str());
	    strcat(strMenuText, "]");
    }
    pCmdUI->SetText(strMenuText);

	if (m_currLogSrv == 1)
	{
	    pCmdUI->SetCheck(1);
	}
	else
	{
	    pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnUpdateServer2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    JCHAR strMenuText[JMAX_STRING_LEN] = {0};

    strcpy(strMenuText, "Server 2 ");
    if (m_remoteAddr[1].GetLength())
    {
	    strcat(strMenuText, "[");
	    strcat(strMenuText, m_remoteAddr[1].c_str());
	    strcat(strMenuText, "]");
    }
    pCmdUI->SetText(strMenuText);

	if (m_currLogSrv == 2)
	{
    	pCmdUI->SetCheck(1);
	}
	else
	{
	    pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnUpdateServer10(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    JCHAR strMenuText[JMAX_STRING_LEN] = {0};

    strcpy(strMenuText, "Server 10 ");
    if (m_remoteAddr[9].GetLength())
    {
	    strcat(strMenuText, "[");
	    strcat(strMenuText, m_remoteAddr[9].c_str());
	    strcat(strMenuText, "]");
    }
    pCmdUI->SetText(strMenuText);

	if (m_currLogSrv == 10)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnUpdateServer3(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    JCHAR strMenuText[JMAX_STRING_LEN] = {0};

    strcpy(strMenuText, "Server 3 ");
    if (m_remoteAddr[2].GetLength())
    {
	    strcat(strMenuText, "[");
	    strcat(strMenuText, m_remoteAddr[2].c_str());
	    strcat(strMenuText, "]");
    }
    pCmdUI->SetText(strMenuText);

	if (m_currLogSrv == 3)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnUpdateServer4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    JCHAR strMenuText[JMAX_STRING_LEN] = {0};

    strcpy(strMenuText, "Server 4 ");
    if (m_remoteAddr[3].GetLength())
    {
	    strcat(strMenuText, "[");
	    strcat(strMenuText, m_remoteAddr[3].c_str());
	    strcat(strMenuText, "]");
    }
    pCmdUI->SetText(strMenuText);

	if (m_currLogSrv == 4)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnUpdateServer5(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    JCHAR strMenuText[JMAX_STRING_LEN] = {0};

    strcpy(strMenuText, "Server 5 ");
    if (m_remoteAddr[4].GetLength())
    {
	    strcat(strMenuText, "[");
	    strcat(strMenuText, m_remoteAddr[4].c_str());
	    strcat(strMenuText, "]");
    }
    pCmdUI->SetText(strMenuText);

	if (m_currLogSrv == 5)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnUpdateServer6(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    JCHAR strMenuText[JMAX_STRING_LEN] = {0};

    strcpy(strMenuText, "Server 6 ");
    if (m_remoteAddr[5].GetLength())
    {
        strcat(strMenuText, "[");
        strcat(strMenuText, m_remoteAddr[5].c_str());
        strcat(strMenuText, "]");
    }
    pCmdUI->SetText(strMenuText);

	if (m_currLogSrv == 6)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnUpdateServer7(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    JCHAR strMenuText[JMAX_STRING_LEN] = {0};

    strcpy(strMenuText, "Server 7 ");
    if (m_remoteAddr[6].GetLength())
    {
	    strcat(strMenuText, "[");
	    strcat(strMenuText, m_remoteAddr[6].c_str());
	    strcat(strMenuText, "]");
    }
    pCmdUI->SetText(strMenuText);

	if (m_currLogSrv == 7)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnUpdateServer8(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    JCHAR strMenuText[JMAX_STRING_LEN] = {0};

    strcpy(strMenuText, "Server 8 ");
    if (m_remoteAddr[7].GetLength())
    {
	    strcat(strMenuText, "[");
	    strcat(strMenuText, m_remoteAddr[7].c_str());
	    strcat(strMenuText, "]");
    }
    pCmdUI->SetText(strMenuText);

	if (m_currLogSrv == 8)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnUpdateServer9(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    JCHAR strMenuText[JMAX_STRING_LEN] = {0};

    strcpy(strMenuText, "Server 9 ");
    if (m_remoteAddr[8].GetLength())
    {
	    strcat(strMenuText, "[");
	    strcat(strMenuText, m_remoteAddr[8].c_str());
	    strcat(strMenuText, "]");
    }
    pCmdUI->SetText(strMenuText);

	if (m_currLogSrv == 9)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

