#if !defined(AFX_OUTPUTVIEW_H__CF1C4BA8_8186_4187_BA01_5BA32E6D8CBE__INCLUDED_)
#define AFX_OUTPUTVIEW_H__CF1C4BA8_8186_4187_BA01_5BA32E6D8CBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputView.h : header file
//

class CFleetOptimizerDoc;

class COutputView : public CScrollView
{
protected: // create from serialization only
	COutputView();
	DECLARE_DYNCREATE(COutputView)

// Attributes
public:
	CFleetOptimizerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutputView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CStringList m_ListOfOutputStrings;
	void TextOut(CString text);
	virtual ~COutputView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COutputView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_strOutputText;
};

#ifndef _DEBUG  // debug version in FleetOptimizerView.cpp
inline CFleetOptimizerDoc* COutputView::GetDocument()
   { return (CFleetOptimizerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTVIEW_H__CF1C4BA8_8186_4187_BA01_5BA32E6D8CBE__INCLUDED_)
