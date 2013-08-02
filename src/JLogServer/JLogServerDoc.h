// JLogServerDoc.h : interface of the CJLogServerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_JLOGSERVERDOC_H__F38C1A22_63FF_46FD_AE0F_0D5356A38453__INCLUDED_)
#define AFX_JLOGSERVERDOC_H__F38C1A22_63FF_46FD_AE0F_0D5356A38453__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CJLogServerDoc : public CDocument
{
protected: // create from serialization only
	CJLogServerDoc();
	DECLARE_DYNCREATE(CJLogServerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJLogServerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CJLogServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CJLogServerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JLOGSERVERDOC_H__F38C1A22_63FF_46FD_AE0F_0D5356A38453__INCLUDED_)
