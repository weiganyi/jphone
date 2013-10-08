// ContactDlg.cpp : implementation file
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
// CContactDlg dialog


CContactDlg::CContactDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContactDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContactDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CContactDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactDlg, CDialog)
	//{{AFX_MSG_MAP(CContactDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_CONTACT, OnDblclkTreeContact)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactDlg message handlers

BOOL CContactDlg::Create()
{
	return CDialog::Create(CContactDlg::IDD);
}

void CContactDlg::OnCancel()
{
	DestroyWindow();

	return;
}

BOOL CContactDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    unsigned int uiIdx = 0;
    JPjSipUaContactList clsContactList;
    HTREEITEM hRootItem = 0, hSubItem = 0;
    JPhoneAgent* pAgent = 0;
    CJPhoneDlg* pCJPhoneDlg = GetCJPhoneDlg();
    CTreeCtrl* pTree = static_cast<CTreeCtrl*>(GetDlgItem(IDC_TREE_CONTACT));

    if (pCJPhoneDlg)
    {
	    pAgent = pCJPhoneDlg->GetAgent();
        EnterCriticalSection(&g_criticalSection);
        //get the contact number list
        pAgent->GetContact();
        pAgent->GetContactRsp(&clsContactList);
        LeaveCriticalSection(&g_criticalSection);

        if (pTree)
        {
            pTree->SetImageList(&pCJPhoneDlg->m_imageList, TVSIL_NORMAL);

            hRootItem = pTree->InsertItem("Contact", TVI_ROOT);
            //insert contact number into the tree one by one
            for (uiIdx=0; uiIdx<JPJSIP_MAX_NUMBER; uiIdx++)
            {
                if (clsContactList.GetContactNumber(uiIdx).GetLength())
                {
                    hSubItem = 
                        pTree->InsertItem(clsContactList.GetContactNumber(uiIdx).c_str(), 0, 0, hRootItem);
                }
                else
                {
                    break;
                }
            }
            pTree->Expand(hRootItem, TVE_EXPAND);
        }
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactDlg::OnDblclkTreeContact(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    CJPhoneDlg* pCJPhoneDlg = GetCJPhoneDlg();
    CTreeCtrl* pTree = static_cast<CTreeCtrl*>(GetDlgItem(IDC_TREE_CONTACT));
    JPjSipUaClickContact clsClickContact;
    JPhoneAgent* pAgent = 0;
    HTREEITEM hSelectedItem;
    CString clsNumber;

	if (pCJPhoneDlg)
	{
        if (pTree)
        {
            //get the selected number
    	    hSelectedItem = pTree->GetSelectedItem();
            clsNumber = pTree->GetItemText(hSelectedItem);

    	    JString number;
    	    number = clsNumber.GetBuffer(JMAX_STRING_LEN);
    	    clsClickContact.SetContactNumber(number);

    	    pAgent = pCJPhoneDlg->GetAgent();
            if (pAgent)
            {
                EnterCriticalSection(&g_criticalSection);
                //start click to call
                pAgent->ClickContact(&clsClickContact);
                LeaveCriticalSection(&g_criticalSection);
            }
        }
	}

	CContactDlg::OnCancel();

	*pResult = 0;

    return;
}
