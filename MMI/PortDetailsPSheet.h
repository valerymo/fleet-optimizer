#if !defined(AFX_PORTDETAILSPSHEET_H__74C9867A_B821_49AC_BD85_6F8DD376977A__INCLUDED_)
#define AFX_PORTDETAILSPSHEET_H__74C9867A_B821_49AC_BD85_6F8DD376977A__INCLUDED_

#include "MainDetailsPPage3.h"	// Added by ClassView
#include "PortMainDetailsPPage.h"	// Added by ClassView
#include "PortDistMatrixPPage.h"
#include "PortLandStructPPage.h"
#include "ScenarioWizardDlg.h"
#include "PortMainInfo.h"
#include "PortLandStructInfo.h"

#define MAX_PORT_RECORDS_IN_DB 30

	// Added by ClassView


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortDetailsPSheet.h : header file
//
class CScenarioWizardDlg;
//class CPortMainDetailsPPage;

/////////////////////////////////////////////////////////////////////////////
// CPortDetailsPSheet

class CPortDetailsPSheet : public CPropertySheet
{
friend class CPortWizardDlg;
friend class CScenarioWizardDlg;
friend class CPortLandStructPPage;

	DECLARE_DYNAMIC(CPortDetailsPSheet)

// Construction
public:
	CPortDetailsPSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPortDetailsPSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

protected:
	void AddControlPages(void);
// Attributes
public:
//	CEditGrid	m_PortDistMatrixGrid;
//	CEditGrid	m_CargoInfoGrid;
	CPortMainDetailsPPage m_CPortMainDetailsPPage;
	CPortDistMatrixPPage m_CPortDistMatrixPPage;
	CMainDetailsPPage3 m_CMainDetailsPPage3;
	CPortLandStructPPage m_PortLandStructPPage;

	


// Operations
public:
	void RecordsNumIncrement(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortDetailsPSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSelectedPortMainInfo(CPortMainInfo*  portInfo);
	void SetSelectedPortLandStructInfo(CPortLandStructInfo*  portLandStructInfo);
	CWnd* GetParentWndPoint();
	int GetRecordsNumber(void);
	BOOL m_ISOK;
	void Test1();
	virtual ~CPortDetailsPSheet();

	int m_OpenStat; // 0 - undef; 1 - read; 2 - edit; 3 - new
	
private:
	void PutIntToArchive(int, CArchive& ar);
	int GetIntFromArchive(CArchive& ar);
	void SetUpdatedPortInfo(void);
	void UpdatePortInDB(void);
	void UpdateEditPortDistMatrixInDB(void);
	int CheckPPagesData(void);
	char* m_pPortsDBFile;
	char* m_pProjectFile1;
	char* m_pPortDistMatrixFile;
	int m_RecordsNumber;
	CWnd* m_pParentWnd;
	CPortMainInfo* m_SelectedPortMainInfo;
	CPortLandStructInfo* m_SelectedPortLandStructInfo;
	CPortMainInfo m_UpdatedPortInfo;
	CPortLandStructInfo m_UpdatedPortLandInfo;



	// Generated message map functions
protected:
	int m_SelectedPortID;


	//{{AFX_MSG(CPortDetailsPSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTDETAILSPSHEET_H__74C9867A_B821_49AC_BD85_6F8DD376977A__INCLUDED_)
