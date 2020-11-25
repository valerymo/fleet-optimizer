#if !defined(AFX_MAINDETAILSPPAGE3_H__079DE611_DBEE_4441_B3C2_76130ADB7A2D__INCLUDED_)
#define AFX_MAINDETAILSPPAGE3_H__079DE611_DBEE_4441_B3C2_76130ADB7A2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDetailsPPage3.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CMainDetailsPPage3 dialog

class CMainDetailsPPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMainDetailsPPage3)

// Construction
public:
	CMainDetailsPPage3();
	~CMainDetailsPPage3();

// Dialog Data
	//{{AFX_DATA(CMainDetailsPPage3)
	enum { IDD = IDD_PORT_MAINDETAILS3 };
	CListCtrl	m_CargoList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMainDetailsPPage3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMainDetailsPPage3)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDETAILSPPAGE3_H__079DE611_DBEE_4441_B3C2_76130ADB7A2D__INCLUDED_)
