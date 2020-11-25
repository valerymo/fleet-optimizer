#if !defined(AFX_INVESTDLG_H__5041DC80_9499_4657_951E_C0EA8931726D__INCLUDED_)
#define AFX_INVESTDLG_H__5041DC80_9499_4657_951E_C0EA8931726D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InvestDlg.h : header file
//

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CInvestDlg dialog

class CInvestDlg : public CDialog
{
// Construction
public:
	CInvestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInvestDlg)
	enum { IDD = IDD_INVEST_DIALOG };
	CButton	m_ctrlIncludeLandStructCalcInScen;
	CEdit	m_ctrInvestmentSize;
	long	m_InvestmentSize;
	BOOL	m_bIncludeLandStructCalc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

private:
	void FillDialogDetails(void);
	void ReadFromRegistry();
	UINT m_nID;
	void SaveToRegistry();


	// Generated message map functions
	//{{AFX_MSG(CInvestDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticInvestNotes();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INVESTDLG_H__5041DC80_9499_4657_951E_C0EA8931726D__INCLUDED_)
