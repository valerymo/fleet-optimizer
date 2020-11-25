#if !defined(AFX_EDITGRID_H__3ACFE259_D8EA_4884_AB18_F2A875421823__INCLUDED_)
#define AFX_EDITGRID_H__3ACFE259_D8EA_4884_AB18_F2A875421823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditGrid.h : header file
//
#include "msflexgrid.h"
#include "EditWnd.h"
/////////////////////////////////////////////////////////////////////////////
// CEditGrid window

class CEditGrid : public CMSFlexGrid  
{
public:
	CEditGrid();
	//virtual ~CEditGrid();
	
	void PreSubclassWindow();

	afx_msg void OnKeyPressGrid(short FAR* KeyAscii);
	afx_msg void OnDblClickGrid();
	afx_msg void OnUpdateGrid();
	DECLARE_EVENTSINK_MAP()

	UINT OnGetDlgCode();
	void OnSetFocus(CWnd* pOldWnd);

public:
	CEditWnd m_edit;
	long m_lBorderWidth;
	long m_lBorderHeight;

	int m_nLogX;
	int m_nLogY;

};

/*
/////////////******************

class CEditGrid : public CMSFlexGrid
{
// Construction
public:
	CEditGrid();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
*/
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITGRID_H__3ACFE259_D8EA_4884_AB18_F2A875421823__INCLUDED_)
