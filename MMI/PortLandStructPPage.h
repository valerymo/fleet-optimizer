//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_PORTLANDSTRUCTPPAGE_H__53B2B77C_AB84_46FA_9D5B_13EAB269E7AA__INCLUDED_)
#define AFX_PORTLANDSTRUCTPPAGE_H__53B2B77C_AB84_46FA_9D5B_13EAB269E7AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortLandStructPPage.h : header file
//

#include "resource.h"
#include "EditGrid.h"
#include "PortLandStructInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CPortLandStructPPage dialog

class CPortLandStructPPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPortLandStructPPage)

// Construction
public:
	CPortLandStructPPage();
	~CPortLandStructPPage();

	void SetParentPtr(CPropertySheet* p);
	CPropertySheet* m_pParentPtr;
	BOOL m_ISLoadDetails;
	BOOL m_ISModifiable;
	BOOL m_bInitialized;

	CPortLandStructInfo* m_SelectedPortLandStructInfo;
	int m_OpenStat; // 0 - undef; 1 - read; 2 - edit; 3 - new



// Dialog Data
	//{{AFX_DATA(CPortLandStructPPage)
	enum { IDD = IDD_PORT_LANDSTRUCT_DIALOG };
	CButton	m_ctrlLandStructCalcActivate;
	CButton	m_ctrlShipCourtAvailable;
	CButton	m_ctrlACVCourtAvailable;
	CEdit	m_ctrlShipCourtInvestPP;
	CEdit	m_ctrlMoorElemPrice;
	CEdit	m_ctrlMoorLength;
	CEdit	m_ctrlACVSpaceElemPrice;
	CEdit	m_ctrlACVSpace;
	CEdit	m_ctrlACVCourtInvestPP;
	CEditGrid	m_ctrlRepairDetailsGrid;
	BOOL	m_bACVCourtAvailable;
	BOOL	m_bShipCourtAvailable;
	BOOL	m_bLandStructCalcActivate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPortLandStructPPage)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPortLandStructPPage)
	afx_msg void OnClickMsflexGridLandRepair();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClickMsflexgridLandRepair();
	afx_msg void OnLandStructActivateCheck();
	afx_msg void OnLandStructPPACVCourtCheck();
	afx_msg void OnLandStructPPShipCourtCheck();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillPortLandDetails();
	void FillGridsTitles();
	void DisableControls();
	void EnableLandStructSettings(int Num, BOOL Flag);

public:
	afx_msg void OnBnClickedLandPointAcvcourtStatic();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTLANDSTRUCTPPAGE_H__53B2B77C_AB84_46FA_9D5B_13EAB269E7AA__INCLUDED_)
