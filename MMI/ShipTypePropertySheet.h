#if !defined(AFX_SHIPTYPEPROPERTYSHEET_H__25856E58_583F_4097_BD8B_611FB2B4F426__INCLUDED_)
#define AFX_SHIPTYPEPROPERTYSHEET_H__25856E58_583F_4097_BD8B_611FB2B4F426__INCLUDED_

#include "ShipTypeOptimizParamPPage.h"	// Added by ClassView
#include "ShipTypeMainPPage.h"	// Added by ClassView
#include "ShipMainInfo.h"
#include "..\FleetOptimizer\FleetOptimizer.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShipTypePropertySheet.h : header file
//

#define MAX_SHIP_RECORDS_IN_DB 500


/////////////////////////////////////////////////////////////////////////////
// CShipTypePropertySheet

class AFX_EXT_CLASS CShipTypePropertySheet : public CPropertySheet
{
friend class CShipTypeWizardDlg;
friend class CScenarioWizardDlg;
friend class CShipTypeMainPPage;
friend class CShipTypeOptimizParamPPage;
friend class CFleetOptimizerApp;

	DECLARE_DYNAMIC(CShipTypePropertySheet)

// Construction
public:
	CShipTypePropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CShipTypePropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShipTypePropertySheet)
	public:
	virtual BOOL OnInitDialog();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	CShipMainInfo* m_SelectedShipMainInfo;
	CShipMainInfo m_UpdatedShipInfo;
	CShipTypeOptimizParamPPage m_CShipTypeOptimizParamPPage;
	CShipTypeMainPPage m_CShipTypeMainPPage;
	virtual ~CShipTypePropertySheet();


	// Generated message map functions
protected:
	void AddControlPages();
	//{{AFX_MSG(CShipTypePropertySheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnOK();
	DECLARE_MESSAGE_MAP()
private:
	int CheckPPagesData(void);
	void SetUpdatedShipInfo();
	void UpdateShipInDB();
	char* m_pShipsDBFile;
	int m_RecordsNumber;
	int m_OpenStat;
	void RecordsNumIncrement(void);
protected:
	int m_SelectedShipID;
	void PutIntToArchive(int, CArchive& ar);
	int GetIntFromArchive(CArchive& ar);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHIPTYPEPROPERTYSHEET_H__25856E58_583F_4097_BD8B_611FB2B4F426__INCLUDED_)
