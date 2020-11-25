#if !defined(AFX_OPTIMIZSETTINGSDLG_H__F6DB2E6C_602C_4BB8_B13B_E79A12F3D991__INCLUDED_)
#define AFX_OPTIMIZSETTINGSDLG_H__F6DB2E6C_602C_4BB8_B13B_E79A12F3D991__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptimizSettingsDlg.h : header file
//

#include "resource.h"
#include "..\FleetOptimizer\FleetOptimizer.h"


/////////////////////////////////////////////////////////////////////////////
// COptimizSettingsDlg dialog

class AFX_EXT_CLASS COptimizSettingsDlg : public CDialog
{
// Construction
public:
	COptimizSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	int m_nSenceCheckRate;

// Dialog Data
	//{{AFX_DATA(COptimizSettingsDlg)
	enum { IDD = IDD_OPTIMIZSETTINGS_DIALOG };
	CEdit	m_ctrlSenceCheckRate;
	CButton	m_ctrlUseSensitivityCheck;
	CButton	m_ctrlUseOptSpecific;
	CButton	m_ctrlUseOptAll;
	CButton	m_ctrOptStep3Check;
	CButton	m_ctrOptStep2Check;
	CButton	m_ctrOptStep1Check;
	BOOL m_bUseOptAll;
	BOOL	m_bUseOptStep1;
	BOOL	m_bUseOptStep2;
	BOOL	m_bUseOptStep3;
	BOOL	m_bUseSensitivityCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptimizSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptimizSettingsDlg)
	afx_msg void OnUseAllOptimizRadio();
	afx_msg void OnUseSpecificOptimizRadio();
	virtual void OnOK();
	afx_msg void OnSensitivityCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void EnableOptimizSettings(BOOL Flag);
	void LoadSettings();
	void SaveSelectedOptions();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIMIZSETTINGSDLG_H__F6DB2E6C_602C_4BB8_B13B_E79A12F3D991__INCLUDED_)
