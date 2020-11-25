#if !defined(AFX_SHIPTYPEMAINPPAGE_H__383E904F_FE88_475A_8058_AC4E14E17D37__INCLUDED_)
#define AFX_SHIPTYPEMAINPPAGE_H__383E904F_FE88_475A_8058_AC4E14E17D37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShipTypeMainPPage.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CShipTypeMainPPage dialog

class CShipTypeMainPPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CShipTypeMainPPage)

// Construction
public:
	BOOL m_ISModifiable;
	void SetParentPtr(CPropertySheet *p);
	CShipTypeMainPPage();
	~CShipTypeMainPPage();
	CPropertySheet* m_pParentPtr;
	

// Dialog Data
	//{{AFX_DATA(CShipTypeMainPPage)
	enum { IDD = IDD_SHIPTYPE_MAINDETAILS };
	CEdit	m_ctrRange;
	CEdit	m_ctr_h3;
	CEdit	m_ctrlSpeed;
	CEdit	m_ctrlT;
	CEdit	m_ctrlL;
	CEdit	m_ctrlB;
	CEdit	m_ctrDW;
	CButton	m_ctrlCargoCompatibleVegFruits;
	CButton	m_ctrlCargoCompatiblCoal;
	CButton	m_ctrlCargoCompatiblOil;
	CButton	m_ctrlCargoCompatiblGeneral;
	CEdit	m_ctrShipRegisterClass;
	CEdit	m_ctrPrototypeName;
	CEdit	m_ctrlShipTypeName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CShipTypeMainPPage)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CShipTypeMainPPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DisableControls();
	void FillShipDetails();

public:
	BOOL m_bInitialized;
	int m_OpenStat; // 0 - undef; 1 - read; 2 - edit; 3 - new

	afx_msg void OnStnClickedShiptypePrototypeStText();
	afx_msg void OnStnClickedMaindimenStText();
	afx_msg void OnBnClickedCheck4();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHIPTYPEMAINPPAGE_H__383E904F_FE88_475A_8058_AC4E14E17D37__INCLUDED_)
