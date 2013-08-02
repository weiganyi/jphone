// JLogServerView.h : interface of the CJLogServerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_JLOGSERVERVIEW_H__F8786D26_E5A0_46E6_9A4D_1D10BC28B441__INCLUDED_)
#define AFX_JLOGSERVERVIEW_H__F8786D26_E5A0_46E6_9A4D_1D10BC28B441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CJLogServerView : public CScrollView
{
protected: // create from serialization only
	CJLogServerView();
	DECLARE_DYNCREATE(CJLogServerView)

// Attributes
public:
	CJLogServerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJLogServerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CJLogServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CJLogServerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#define JLOGSERVERVIEW_CHAR_PIXEL   15

#ifndef _DEBUG  // debug version in JLogServerView.cpp
inline CJLogServerDoc* CJLogServerView::GetDocument()
   { return (CJLogServerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JLOGSERVERVIEW_H__F8786D26_E5A0_46E6_9A4D_1D10BC28B441__INCLUDED_)
