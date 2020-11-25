// FleetOptimizerView.h : interface of the CFleetOptimizerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLEETOPTIMIZERVIEW_H__04CBB99E_3B59_410E_9866_6DA3E1DF568D__INCLUDED_)
#define AFX_FLEETOPTIMIZERVIEW_H__04CBB99E_3B59_410E_9866_6DA3E1DF568D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FleetOptimizerDoc.h"

class CFleetOptimizerView : public CView
{
protected: // create from serialization only
	CFleetOptimizerView();
	DECLARE_DYNCREATE(CFleetOptimizerView)

// Attributes
public:
	CFleetOptimizerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFleetOptimizerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFleetOptimizerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFleetOptimizerView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FleetOptimizerView.cpp
inline CFleetOptimizerDoc* CFleetOptimizerView::GetDocument()
   { return (CFleetOptimizerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLEETOPTIMIZERVIEW_H__04CBB99E_3B59_410E_9866_6DA3E1DF568D__INCLUDED_)
