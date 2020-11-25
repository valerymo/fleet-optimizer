#if !defined(AFX_EASYSTARTDLG_H__9462745E_3CD9_49D1_A54A_56C1D70E4325__INCLUDED_)
#define AFX_EASYSTARTDLG_H__9462745E_3CD9_49D1_A54A_56C1D70E4325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EasyStartDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEasyStartDlg dialog

class CEasyStartDlg : public CDialog
{
// Construction
public:
	CEasyStartDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEasyStartDlg)
	enum { IDD = IDD_EASYSTART_DIALOG };
	CButton	m_ctrlShowDlgCheck;
	BOOL	m_bShowDlg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasyStartDlg)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEasyStartDlg)
	afx_msg void OnCreateScenarioButton();
	afx_msg void OnLoadScenarioButton();
	afx_msg void OnRunOptimizButton();
	afx_msg void OnShowResultsButton();
	afx_msg void OnHelpButton();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CEasyStartDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private:
	void SaveToRegistry();
	void ReadFromRegistry();
	UINT m_nID;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASYSTARTDLG_H__9462745E_3CD9_49D1_A54A_56C1D70E4325__INCLUDED_)
