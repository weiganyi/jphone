// JLogServerDoc.cpp : implementation of the CJLogServerDoc class
//

#include "stdafx.h"
#include "JLogServer.h"

#include "JLogServerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJLogServerDoc

IMPLEMENT_DYNCREATE(CJLogServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CJLogServerDoc, CDocument)
	//{{AFX_MSG_MAP(CJLogServerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJLogServerDoc construction/destruction

CJLogServerDoc::CJLogServerDoc()
{
	// TODO: add one-time construction code here

}

CJLogServerDoc::~CJLogServerDoc()
{
}

BOOL CJLogServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CJLogServerDoc serialization

void CJLogServerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CJLogServerDoc diagnostics

#ifdef _DEBUG
void CJLogServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CJLogServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CJLogServerDoc commands
