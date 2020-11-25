//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_PROTOTYPEWIZARDSHIPDLG_H__23901F0A_3A52_4B0C_8C1B_7D973F03F9F1__INCLUDED_)
#define AFX_PROTOTYPEWIZARDSHIPDLG_H__23901F0A_3A52_4B0C_8C1B_7D973F03F9F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrototypeWizardShipDlg.h : header file
//

#include "resource.h"
#include "EditGrid.h"

#define MAX_PROTOTYPES_RECORDS_IN_SHIPDB 50
#define SHIP_DB_FIELDS_NUM 35


/////////////////////////////////////////////////////////////////////////////
// CPrototypeWizardShipDlg dialog

class AFX_EXT_CLASS CPrototypeWizardShipDlg : public CDialog
{
// Construction
public:
	CPrototypeWizardShipDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrototypeWizardShipDlg)
	enum { IDD = IDD_PROTOTYPEWIZ_SHIP_DIALOG };
	CEditGrid	m_PrototypeShipGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrototypeWizardShipDlg)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrototypeWizardShipDlg)
	afx_msg void OnDblClickProtoShipList();
	afx_msg void OnProtoShipEdit();
	afx_msg void OnProtoShipNew();
	afx_msg void OnProtoShipRefresh();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnScrollProtoShipList();
	afx_msg void OnProtoShipApply();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetTableTitles(void);
	void CleanProtoShipGrid(void);
	void FillGenInfoGrid(void);
	void RecordsNumIncrement(void);
	void Apply();
	UINT m_nID;
	int m_RecordsNumber;
	char* m_pDBFile;
	int m_OpenStat;
	BOOL m_ISModifiable;

public:
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROTOTYPEWIZARDSHIPDLG_H__23901F0A_3A52_4B0C_8C1B_7D973F03F9F1__INCLUDED_)
