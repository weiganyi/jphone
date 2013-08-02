// JLogServerView.cpp : implementation of the CJLogServerView class
//

#include "stdafx.h"
#include "JLogServer.h"

#include "JFrameWork.h"

using namespace JFrameWork;

#include "JLogSrv.h"
#include "JLogSrvAgent.h"

#include "MainFrm.h"
#include "JLogServerDoc.h"
#include "JLogServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJLogServerView

IMPLEMENT_DYNCREATE(CJLogServerView, CScrollView)

BEGIN_MESSAGE_MAP(CJLogServerView, CScrollView)
	//{{AFX_MSG_MAP(CJLogServerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJLogServerView construction/destruction

CJLogServerView::CJLogServerView()
{
	// TODO: add construction code here

}

CJLogServerView::~CJLogServerView()
{
}

BOOL CJLogServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CJLogServerView drawing

void CJLogServerView::OnDraw(CDC* pDC)
{
	CJLogServerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	JUINT32 uiStart = 0;
	JUINT32 uiEnd = 0;
	JUINT32 uiOffset = 0;
	JUINT32 uiLine = 0;
	JUINT32 uiLen = 0;

    JUINT32 uiCurrLogSrv = 0;
    JCHAR strCurrLogSrv[3] = {0};
    CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
    JCHAR logMessage[JLOGMSG_MAX_MSG][JMAX_BUFFER_LEN] = {0};
    JUINT32 uiLogOffset = 0;

    if (!pMainFrame)
    {
        return ;
    }

    EnterCriticalSection(&g_criticalSection);
    uiCurrLogSrv = pMainFrame->GetCurrLogSrv();
    LeaveCriticalSection(&g_criticalSection);

    sprintf(strCurrLogSrv, "%u", uiCurrLogSrv);
    pMainFrame->GetAgent()->GetLogMsg(strCurrLogSrv, reinterpret_cast<JCHAR*>(logMessage), 
        JLOGMSG_MAX_MSG*JMAX_BUFFER_LEN, &uiLogOffset);

    uiLen = strlen(logMessage[uiLogOffset]);
    if (uiLen)
    {
        uiStart = uiLogOffset;
        if (uiLogOffset == 0)
        {
            uiEnd = JLOGMSG_MAX_MSG-1;
        }
        else
        {
            uiEnd = uiLogOffset-1;
        }
    }
    else
    {
        uiStart = 0;
        if (uiLogOffset == 0)
        {
            uiEnd = 0;
        }
        else
        {
            uiEnd = uiLogOffset-1;
        }
    }

    uiOffset = uiStart;

    while(1)
    {
        if (uiOffset == uiEnd)
        {
            uiLen = strlen(logMessage[uiOffset]);
            if (uiLen)
            {
	            pDC->TextOut(0, JLOGSERVERVIEW_CHAR_PIXEL*uiLine, logMessage[uiOffset]);
            }
            break;
        }

	    pDC->TextOut(0, JLOGSERVERVIEW_CHAR_PIXEL*uiLine, logMessage[uiOffset]);

        if (uiOffset == JLOGMSG_MAX_MSG-1)
        {
            uiOffset = 0;
        }
        else
        {
            uiOffset++;
        }

	    uiLine++;
    };

    return;
}

void CJLogServerView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: calculate the total size of this view
	CSize sizeTotal;
	CSize sizePage;
	CSize sizeLine;

	sizeTotal.cx = JLOGSERVERVIEW_CHAR_PIXEL/3*JMAX_BUFFER_LEN;
	sizeTotal.cy = JLOGSERVERVIEW_CHAR_PIXEL*JLOGMSG_MAX_MSG;
	sizePage.cx = JLOGSERVERVIEW_CHAR_PIXEL*10;
	sizePage.cy = JLOGSERVERVIEW_CHAR_PIXEL*10;
	sizeLine.cx = JLOGSERVERVIEW_CHAR_PIXEL;
	sizeLine.cy = JLOGSERVERVIEW_CHAR_PIXEL;
	SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);

    return;
}

/////////////////////////////////////////////////////////////////////////////
// CJLogServerView diagnostics

#ifdef _DEBUG
void CJLogServerView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CJLogServerView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CJLogServerDoc* CJLogServerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJLogServerDoc)));
	return (CJLogServerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CJLogServerView message handlers
