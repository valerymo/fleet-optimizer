// FleetOptimizerDoc.h : interface of the CFleetOptimizerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLEETOPTIMIZERDOC_H__F98A7ADA_BECF_4F47_9C95_EFA9B49E20EB__INCLUDED_)
#define AFX_FLEETOPTIMIZERDOC_H__F98A7ADA_BECF_4F47_9C95_EFA9B49E20EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputView;

class CFleetOptimizerDoc : public CDocument
{
protected: // create from serialization only
	CFleetOptimizerDoc();
	DECLARE_DYNCREATE(CFleetOptimizerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFleetOptimizerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	COutputView* GetOutputView();
	virtual ~CFleetOptimizerDoc();
	CDC* GetOutputViewDC(void);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFleetOptimizerDoc)
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLEETOPTIMIZERDOC_H__F98A7ADA_BECF_4F47_9C95_EFA9B49E20EB__INCLUDED_)
