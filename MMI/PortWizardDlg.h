//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_PORTWIZARDDLG_H__90E575E2_9100_4CF1_98D5_449256DA92E9__INCLUDED_)
#define AFX_PORTWIZARDDLG_H__90E575E2_9100_4CF1_98D5_449256DA92E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortWizardDlg.h : header file
//
#include "resource.h"
#include "EditGrid.h"
//#include "PortDetailsPSheet.h"
#include "ScenarioWizardDlg.h"

#define WM_GOODBYE_PORTWIZARD WM_USER + 5

//class CScenarioWizardDlg;
/////////////////////////////////////////////////////////////////////////////
// CPortWizardDlg dialog

class AFX_EXT_CLASS CPortWizardDlg : public CDialog
{
// Construction

public:	
	int GetSelectedPortID(void);
	CPortWizardDlg(CWnd* pParent = NULL);   // standard constructor
//public:
//	CPortWizardDlg(CScenarioWizardDlg* pScenarioWizardDlg);


// Dialog Data
	//{{AFX_DATA(CPortWizardDlg)
	enum { IDD = IDD_PORTWIZ_DIALOG };
	CEditGrid	m_PortGenInfoGrid;
	CEditGrid	m_PortCargoInfoGrid;
	CEditGrid	m_PortsDistMatrixInfoGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortWizardDlg)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPortWizardDlg)
	afx_msg void OnPortWizDetails();
	afx_msg void OnPortWizNew();
	afx_msg void OnPortWizEdit();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickPortwizList();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnPortwizDelete();
	afx_msg void OnPortWizRefresh();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CleanGenInfoGrid();
	int  GetIntFromArchive(CArchive&);
	void PutIntToArchive(int, CArchive&);
	void UpdateEditPortDistMatrixInDB();
	int m_SelectedPortID;
	void FillSelectedPortInfo();
	CPortMainInfo* m_SelectedPortMainInfo;
	CPortLandStructInfo* m_SelectedPortLandStructInfo;
	//CScenarioWizardDlg* m_pScenarioWizardDlg;
	void FillGridsTitles(void);
	void FillGenInfoGrid(void);
	int m_RecordsNumber;
	char* m_pPortsDBFile;
	char* m_pPortDistMatrixFile;
	UINT m_nID;
	int m_ActionFlag;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTWIZARDDLG_H__90E575E2_9100_4CF1_98D5_449256DA92E9__INCLUDED_)
