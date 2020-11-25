#if !defined(AFX_SHIPTYPEOPTIMIZPARAMPPAGE_H__497187E3_8C15_4BBC_95BD_30421350422E__INCLUDED_)
#define AFX_SHIPTYPEOPTIMIZPARAMPPAGE_H__497187E3_8C15_4BBC_95BD_30421350422E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShipTypeOptimizParamPPage.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CShipTypeOptimizParamPPage dialog

class CShipTypeOptimizParamPPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CShipTypeOptimizParamPPage)

// Construction
public:
	BOOL m_ISModifiable;
	void SetParentPtr(CPropertySheet *p);
	CShipTypeOptimizParamPPage();
	~CShipTypeOptimizParamPPage();
	CPropertySheet* m_pParentPtr;

	void GetUpdatePPageDataStruct(void);


// Dialog Data
	//{{AFX_DATA(CShipTypeOptimizParamPPage)
	enum { IDD = IDD_SHIP_OPTIMIZATION_PARAM };
	CEdit	m_ctrlShipExistDesignPrice;
	CEdit	m_ctrlShipExistDesignAdvantPrice;
	CEdit	m_ctrlShipExistBuildPrice;
	CEdit	m_ctrlShipExistBuildAdvantPrice;
	CEdit	m_ctrlShipExistPrice;
	CEdit	m_ctrlShipExistAdvantPrice;
	CComboBox	m_ctrlShipExistBuildNumComboBox;
	CComboBox	m_ctrlShipExistNumComboBox;
	CButton	m_ctrlShipExistDesignCheckBox;
	CButton	m_ctrlShipExistBuildCheckBox;
	CButton	m_ctrlShipExistCheckBox;
	CButton	m_ctrlShipExistSetCheckBox;
	CButton	m_ctrlShipExistForceCheckBox;
	CEdit	m_ctrlShipTariffGenVF;
	CEdit	m_ctrlShipTariffOil;
	CEdit	m_ctrlShipTariffCoal;
	CEdit	m_ctrlShipLoadMin;
	CEdit	m_ctrlShipLoadMax;
	CEdit	m_ctrlShipLoadDelta;
	BOOL	m_bUseShipExistSet;
	BOOL	m_bShipExist;
	BOOL	m_bShipExistBuild;
	BOOL	m_bShipExistDesign;
	BOOL	m_bShipExistForce;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CShipTypeOptimizParamPPage)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CShipTypeOptimizParamPPage)
	afx_msg void OnChangeShiptypeLoadMin();
	afx_msg void OnUseShipExistSettings();
	afx_msg void OnShipExist();
	afx_msg void OnShipExistBuild();
	afx_msg void OnShipExistDesign();
	afx_msg void OnShipExistForce();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void EnableShipExistingSettings(int subset, BOOL flag);
	void DisableControls();
	void FillShipDetails();

public:
	BOOL m_bInitialized;
	int m_OpenStat; // 0 - undef; 1 - read; 2 - edit; 3 - new
	struct PPageDataStruct{
		CString	strShipTariffGenVF;
		CString	strShipTariffOil;
		CString	strShipTariffCoal;
		CString	strShipLoadMin;
		CString	strShipLoadMax;
		CString	strShipLoadDelta;
	};

	struct PPageDataStruct m_ppage_data;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHIPTYPEOPTIMIZPARAMPPAGE_H__497187E3_8C15_4BBC_95BD_30421350422E__INCLUDED_)
