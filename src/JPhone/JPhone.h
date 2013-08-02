// JPhone.h : main header file for the JPHONE application
//

#if !defined(AFX_JPHONE_H__9B62E1E8_284B_434B_8880_F18A96CE7015__INCLUDED_)
#define AFX_JPHONE_H__9B62E1E8_284B_434B_8880_F18A96CE7015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CJPhoneApp:
// See JPhone.cpp for the implementation of this class
//

class CJPhoneApp : public CWinApp
{
public:
	CJPhoneApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJPhoneApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CJPhoneApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JPHONE_H__9B62E1E8_284B_434B_8880_F18A96CE7015__INCLUDED_)
