//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_SCENARIOWIZARDDLG_H__03FE8BE6_5804_4667_AB17_97A425344A11__INCLUDED_)
#define AFX_SCENARIOWIZARDDLG_H__03FE8BE6_5804_4667_AB17_97A425344A11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScenarioWizardDlg.h : header file
//
#include "resource.h" 
#include "PortDetailsPSheet.h"
#include "ShipTypePropertySheet.h"
#include "InvestDlg.h"


class CPortWizardDlg;
/////////////////////////////////////////////////////////////////////////////
// CScenarioWizardDlg dialog

#include "PortMainInfo.h"	// Added by ClassView
#include "PortLandStructInfo.h"
#include "ShipMainInfo.h"


class AFX_EXT_CLASS CScenarioWizardDlg : public CDialog
{
friend class CFleetOptimizerApp;
// Construction
public:
	void ClearScenPortsGrid();
	void SaveScenToFile();
	CScenarioWizardDlg(CWnd* pParent = NULL);   // standard constructor
	char* m_pScenarioFile;
	BOOL m_bAutoLoad;

// Dialog Data
	//{{AFX_DATA(CScenarioWizardDlg)
	enum { IDD = IDD_SCENARIOWIZ_DIALOG };
	CEdit	m_ctrInvestLimitEdit;
	CButton	m_ctrInvestLimitGroupBox;
	CEditGrid	m_PortDBGrid;
	CEditGrid	m_ScenPortGrid;
	CEditGrid	m_ShipsDBGrid;
	CEditGrid	m_ScenShipsGrid;
	//}}AFX_DATA

	int m_test;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScenarioWizardDlg)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScenarioWizardDlg)
	afx_msg void OnPortwizardButton();
	afx_msg void OnScenarioPortView();
	afx_msg void OnShipWizardButton();
	afx_msg void OnGetPort();
	afx_msg void OnScenFileSave();
	afx_msg void OnScenFileSaveAs();
	afx_msg void OnScenFileLoad();
	afx_msg void OnGetShip();
	afx_msg void OnScenarioShipView();
	afx_msg void OnScenarioRefreshButton();
	virtual void OnOK();
	afx_msg void OnScenarioShipDeleteButton();
	afx_msg void OnScenarioPortDeleteButton();
	afx_msg void OnDblClickScenShipDBGrid();
	afx_msg void OnDblClickScenShipScenGrid();
	afx_msg void OnDblClickScenPortDBGrid();
	afx_msg void OnDblClickScenPortScenGrid();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnContextMenuShipsDBView();
	afx_msg void OnContextMenuShipsDBGet();
	afx_msg void OnContextMenuShipScenView();
	afx_msg void OnContextMenuShipsScenDelete();
	afx_msg void OnContextMenuPortsDBView();
	afx_msg void OnContextMenuPortsDBGet();
	afx_msg void OnContextMunuPortScenView();
	afx_msg void OnContextMenuPortScenDelete();
	afx_msg void OnScenarioPortEditButton();
	afx_msg void OnScenarioShipEditButton();
	afx_msg void OnContextMenuShipScenEdit();
	afx_msg void OnContextMenuPortScenEdit();
	afx_msg void OnInvestmentButton();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg long CScenarioWizardDlg::OnGoodbyePortWizard(UINT wParam, LONG lParam);
	afx_msg long CScenarioWizardDlg::OnGoodbyeShipWizard(UINT wParam, LONG lParam);
	DECLARE_MESSAGE_MAP()
private:
	void ReadFromRegistry();
	void SaveToRegistry();
	void FillInvestInfo();
	void ClearScenShipsGrid();
	void SetTitlesPortGrids();
	void SetTitlesShipsGrid(void);
	void LoadDefaultScenario(void);
	void CopyScenarioFileToDefault();
	void CleanShipDBGrid(void);
	void CleanPortDBGrid(void);
	void FillSelectedShipScenInfo();
	void FillSelectedShipDBInfo();
	void FillSelectedPortScenInfo();
	void FillSelectedPortDBInfo();
	void LoadScenarioData();
	void LoadScenShipData(CArchive& ar);
	void LoadScenPortData(CArchive& ar);
	void LoadScenInvestData(CArchive& ar);
	void SaveScenPortDataToFile(CArchive& ar);
	void SaveScenShipDataToFile(CArchive& ar);
	void SaveScenInvestDataToFile(CArchive &ar);
	CPortMainInfo* m_SelectedPortMainInfo;
	CShipMainInfo* m_SelectedShipMainInfo;
	CPortLandStructInfo* m_SelectedPortLandStructInfo;
	char* m_pShipsDBFile;
	char* m_pPortDistMatrixFile;
	char* m_pPortsDBFile;
	char* m_pScenDefaultFile;
	int m_ShipsDBRecordsNum;
	int m_PortDBRecordsNum;
	void FillShipDBGrid();
	void FillPortDBGrid(void);
	CPortWizardDlg* m_pPortWizDlg;
	CShipTypeWizardDlg* m_pShipWizDlg;
	int m_flagEditPort;
	int m_nEditPortID;
	int m_nEditPortNum;
	int m_flagEditShip;
	int m_nEditShipID;
	int m_nEditShipNum;
	CInvestDlg* m_pInvestDlg;
	long m_InvestmentSize;
	BOOL m_bIncludeLandStructCalc;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENARIOWIZARDDLG_H__03FE8BE6_5804_4667_AB17_97A425344A11__INCLUDED_)
