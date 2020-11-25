#if !defined(AFX_LOGSDLG_H__F79199A4_AE7F_4911_8F14_3038B31566A2__INCLUDED_)
#define AFX_LOGSDLG_H__F79199A4_AE7F_4911_8F14_3038B31566A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogsDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CLogsDlg dialog

class AFX_EXT_CLASS CLogsDlg : public CDialog
{
// Construction
public:
	CLogsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogsDlg)
	enum { IDD = IDD_LOGS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogsDlg)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogsDlg)
	afx_msg void OnClose();
	afx_msg void OnOpenFileVehiclesList();
	afx_msg void OnOpenFileInitDBMVehiclesAll();
	afx_msg void OnOpenFileErrorsLog();
	afx_msg void OnOpenFileInitialBasePlan();
	afx_msg void OnOpenFileOptimizStep1Plan();
	afx_msg void OnOpenFileOptimizStep2Plan();
	afx_msg void OnOpenFileOptimizStep3Plan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CLogsDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

private:
	void ReadDataFromRegistry();
	UINT m_nID;
	CString m_sLogDir;
	char* m_pLogFileVehiclesList; 
	char* m_pLogFileInitDBMVehiclesAll;
	char* m_pLogFileErrors;
	char* m_pLogFileInitialBasePlan;
	char* m_pLogFileOptimizStep1Plan;
	char* m_pLogFileOptimizStep2Plan;
	char* m_pLogFileOptimizStep3Plan;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGSDLG_H__F79199A4_AE7F_4911_8F14_3038B31566A2__INCLUDED_)
