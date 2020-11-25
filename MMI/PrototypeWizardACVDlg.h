//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_PROTOTYPEWIZARDACVDLG_H__04F24521_27CD_48D1_93E5_D056E9FEAE19__INCLUDED_)
#define AFX_PROTOTYPEWIZARDACVDLG_H__04F24521_27CD_48D1_93E5_D056E9FEAE19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrototypeWizardACVDlg.h : header file
//

#include "resource.h"
#include "EditGrid.h"

#define MAX_PROTOTYPES_RECORDS_IN_ACVDB 30
#define ACV_DB_FIELDS_NUM 60


/////////////////////////////////////////////////////////////////////////////
// CPrototypeWizardACVDlg dialog

class AFX_EXT_CLASS CPrototypeWizardACVDlg : public CDialog
{
// Construction
public:
	CPrototypeWizardACVDlg(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CPrototypeWizardACVDlg)
	enum { IDD = IDD_PROTOTYPEWIZ_ACV_DIALOG };
	CEditGrid	m_PrototypeACVGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrototypeWizardACVDlg)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrototypeWizardACVDlg)
	afx_msg void OnDblClickProtoACVList();
	afx_msg void OnScrollProtoACVList();
	afx_msg void OnProtoACVEdit();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnProtoACVRefresh();
	afx_msg void OnProtoACVNew();
	afx_msg void OnProtoACVApply();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetTableTitles(void);
	void CleanProtoACVGrid(void);
	void FillGenInfoGrid(void);
	void RecordsNumIncrement(void);
	void Apply(void);
	UINT m_nID;
	int m_RecordsNumber;
	char* m_pDBFile;
	int m_OpenStat;
	BOOL m_ISModifiable;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROTOTYPEWIZARDACVDLG_H__04F24521_27CD_48D1_93E5_D056E9FEAE19__INCLUDED_)
