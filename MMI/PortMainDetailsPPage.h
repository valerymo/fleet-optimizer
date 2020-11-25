//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_PORTMAINDETAILSPPAGE_H__73581D19_D6A4_40D9_A7B9_D26C17BC3405__INCLUDED_)
#define AFX_PORTMAINDETAILSPPAGE_H__73581D19_D6A4_40D9_A7B9_D26C17BC3405__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortMainDetailsPPage.h : header file
//
#include "resource.h"
#include "EditGrid.h"
#include "PortMainInfo.h"
//#include "PortDetailsPSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CPortMainDetailsPPage dialog

class CPortMainDetailsPPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPortMainDetailsPPage)

// Construction
public:
	CPropertySheet* m_pParentPtr;
	void SetParentPtr(CPropertySheet *p);
	BOOL m_ISLoadDetails;
	CPortMainDetailsPPage();
	~CPortMainDetailsPPage();

	void SetCargoTableTitles(void);
	void Test1(void);

	BOOL m_ISModifiable;
	BOOL m_bInitialized;
	CPortMainInfo* m_SelectedPortMainInfo;
	int m_OpenStat; // 0 - undef; 1 - read; 2 - edit; 3 - new
	int m_RecordsNumber;  // for m_OpenStat == 3 only


// Dialog Data
	//{{AFX_DATA(CPortMainDetailsPPage)
	enum { IDD = IDD_PORT_MAINDETAILS };
	CEdit	m_ctrlPortID;
	CEdit	m_ctrlPortName;
	CComboBox	m_ctrlPortRegion;
	CComboBox	m_ctrlPortType;
	CEdit	m_ctrlBasePortName;
	CString	m_strPortRegion;
	CString	m_strPortName;
	CString	m_strPortType;
	CString	m_strBasePortName;
	CEditGrid	m_CargoInfoGrid;
	CString	m_strPortID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPortMainDetailsPPage)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPortMainDetailsPPage)
	afx_msg void OnChangePortnameText();
	afx_msg void OnDblClickCargoinfoGrid();
	afx_msg void OnScrollCargoinfoGrid();
	afx_msg void OnClickCargoinfoGrid();
	afx_msg void OnSelChangeCargoinfoGrid();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	void FillGridsTitles();
	void DisableControls(void);
	void FillPortDetails(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTMAINDETAILSPPAGE_H__73581D19_D6A4_40D9_A7B9_D26C17BC3405__INCLUDED_)
