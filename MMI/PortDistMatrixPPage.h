//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_PORTDISTMATRIXPPAGE_H__8031B71B_62E1_451A_81B2_C4A22AA87AA6__INCLUDED_)
#define AFX_PORTDISTMATRIXPPAGE_H__8031B71B_62E1_451A_81B2_C4A22AA87AA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortDistMatrixPPage.h : header file
//
#include "resource.h"
#include "EditGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CPortDistMatrixPPage dialog

class CPortDistMatrixPPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPortDistMatrixPPage)

// Construction
public:
	BOOL m_ISLoadDetails;
	void SetDistMatrixTitles(void);
	void SetParentPtr(CPropertySheet* p);
	CPropertySheet* m_pParentPtr;
	BOOL m_ISModifiable;
	CPortDistMatrixPPage();
	~CPortDistMatrixPPage();

	BOOL m_bInitialized;

// Dialog Data
	//{{AFX_DATA(CPortDistMatrixPPage)
	enum { IDD = IDD_PORT_DISTMATRIX };
	CEditGrid	m_PortDistMatrixGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPortDistMatrixPPage)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPortDistMatrixPPage)
	afx_msg void OnDblClickDistmatrixGrid();
	afx_msg void OnScrollDistmatrixGrid();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void FillDistMatrix();
	char* m_pPortDistMatrixFile;
	int m_PortsNumber;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTDISTMATRIXPPAGE_H__8031B71B_62E1_451A_81B2_C4A22AA87AA6__INCLUDED_)
