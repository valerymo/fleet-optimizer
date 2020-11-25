#if !defined(AFX_PORTDIALOG1_H__5AE30FF4_243B_4582_9102_2CFFCB1B476C__INCLUDED_)
#define AFX_PORTDIALOG1_H__5AE30FF4_243B_4582_9102_2CFFCB1B476C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortDialog1.h : header file
//
#include "resource.h" 
/////////////////////////////////////////////////////////////////////////////
// CPortDialog1 dialog

class AFX_EXT_CLASS CPortDialog1 : public CDialog
{
// Construction
public:
	CPortDialog1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPortDialog1)
	enum { IDD = IDD_PORT_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortDialog1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPortDialog1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTDIALOG1_H__5AE30FF4_243B_4582_9102_2CFFCB1B476C__INCLUDED_)
