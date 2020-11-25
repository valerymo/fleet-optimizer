//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_SHIPTYPEWIZARDDLG_H__FEF661EA_1441_437A_90BA_2BB1C25F25D6__INCLUDED_)
#define AFX_SHIPTYPEWIZARDDLG_H__FEF661EA_1441_437A_90BA_2BB1C25F25D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShipTypeWizardDlg.h : header file
//
#include "resource.h"
#include "EditGrid.h"
#include "ShipMainInfo.h"
#include "PrototypeWizardShipDlg.h"
#include "PrototypeWizardACVDlg.h"


#define WM_GOODBYE_SHIPWIZARD WM_USER + 6

/////////////////////////////////////////////////////////////////////////////
// CShipTypeWizardDlg dialog

class AFX_EXT_CLASS CShipTypeWizardDlg : public CDialog
{
// Construction
public:
	int GetSelectedShipID();
	CShipTypeWizardDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShipTypeWizardDlg)
	enum { IDD = IDD_SHIPTYPEWIZ_DIALOG };
	CEdit	m_ctrlSpeed;
	CButton	m_ctrlCargoCompatiblVegFruits;
	CButton	m_ctrlCargoCompatiblCoal;
	CButton	m_ctrlCargoCompatiblOil;
	CButton	m_ctrlCargoCompatiblGeneral;
	CEdit	m_ctrlT;
	CEdit	m_ctrlL;
	CEdit	m_ctrlB;
	CEdit	m_ctrlPrototypeName;
	CEdit	m_ctrlLoad;
	CEdit	m_ctrlRegisterClass;
	CEdit	m_ctrlShipTypeName;
	CEditGrid	m_ShipGenInfoGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShipTypeWizardDlg)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShipTypeWizardDlg)
	afx_msg void OnShiptypeWizEdit();
	afx_msg void OnShiptypeWizDetails();
	afx_msg void OnShiptypeWizNew();
	afx_msg void OnClickShiptypewizList();
	afx_msg void OnShiptypewizDelete();
	afx_msg void OnShiptypeWizRefresh();
	afx_msg void OnShipPrototypeWizButton();
	afx_msg void OnACVPrototypeWizButton();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CleanGenInfoGrid(void);
	int m_SelectedShipID;
	CShipMainInfo* m_SelectedShipMainInfo;
	void FillSelectedShipInfo();
	char* m_pShipsDBFile;
	int m_RecordsNumber;
	void FillGenInfoGrid(void);
	void PutIntToArchive(int, CArchive& ar);
	int GetIntFromArchive(CArchive& ar);
	UINT m_nID;
	CPrototypeWizardShipDlg* m_pShipProtoWizDlg;
	CPrototypeWizardACVDlg* m_pACVProtoWizDlg;


public:
	afx_msg void OnStnClickedStaticText2();
	afx_msg void OnStnClickedStaticText5();
	afx_msg void OnBnClickedCheck3();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHIPTYPEWIZARDDLG_H__FEF661EA_1441_437A_90BA_2BB1C25F25D6__INCLUDED_)
