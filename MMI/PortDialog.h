#if !defined(AFX_PORTDIALOG_H__3673B1BB_7032_4368_B4B0_60972AC408D8__INCLUDED_)
#define AFX_PORTDIALOG_H__3673B1BB_7032_4368_B4B0_60972AC408D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortDialog.h : header file
//

#include "resource.h"     

/////////////////////////////////////////////////////////////////////////////
// CPortDialog dialog

class AFX_EXT_CLASS CPortDialog : public CDialog
{
// Construction
public:
	CPortDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPortDialog)
	enum { IDD = IDD_PORT_DIALOG };
	CListBox	m_CtrlPortList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPortDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTDIALOG_H__3673B1BB_7032_4368_B4B0_60972AC408D8__INCLUDED_)
