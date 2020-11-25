// EditGrid.cpp: implementation of the CEditGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EditGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_EVENTSINK_MAP(CEditGrid, CMSFlexGrid)
      // {{AFX_EVENTSINK_MAP(CEditGrid)
      // }}AFX_EVENTSINK_MAP
         //ON_EVENT_REFLECT(CEditGrid, -603 /* KeyPress */, OnKeyPressGrid,
          //                VTS_PI2)
         //ON_EVENT_REFLECT(CEditGrid, -601 /* DblClick */, OnDblClickGrid,
          //                VTS_NONE)
         ON_EVENT_REFLECT(CEditGrid, 72 /* LeaveCell */, OnUpdateGrid,
                          VTS_NONE)
END_EVENTSINK_MAP()

CEditGrid::CEditGrid()
{

}

//CEditGrid::~CEditGrid()
//{

//}

void CEditGrid::PreSubclassWindow()
{
	// Calculate border size.
	SetRow(0);
	SetCol(0);
	m_lBorderWidth  = GetCellLeft();
	m_lBorderHeight = GetCellTop();

	// To convert grid rect from twips to DC units you need
	// pixels per inch.
	CDC* pDC = GetDC();
	m_nLogX = pDC->GetDeviceCaps(LOGPIXELSX);
	m_nLogY = pDC->GetDeviceCaps(LOGPIXELSY);
	ReleaseDC(pDC);

	// Create invisible edit control.
	m_edit.Create(WS_CHILD|ES_MULTILINE|ES_WANTRETURN,
				  CRect(0,0,0,0), this, GetDlgCtrlID());
}  

void CEditGrid::OnDblClickGrid()
{
	short i = 13;
	OnKeyPressGrid(&i); // Simulate a return.
}

void CEditGrid::OnKeyPressGrid(short FAR* KeyAscii)
{
	ASSERT (KeyAscii != NULL);

	m_edit.SetWindowText(GetText());

	if (*KeyAscii == 13)
	{
		m_edit.SetSel(0,-1);
	}
	else
	{
		char buf[] = " ";
		buf[0] = (char)*KeyAscii;
		m_edit.SetSel(-1,-1);
		m_edit.ReplaceSel(buf);
	}

	// Adjust for border height and convert from twips to screen
	// units.
	m_edit.MoveWindow(((GetCellLeft() - m_lBorderWidth) * m_nLogX)/1440,
					  ((GetCellTop() - m_lBorderHeight) * m_nLogY)/1440,
					  (GetCellWidth()* m_nLogX)/1440,
					  (GetCellHeight()* m_nLogY)/1440, FALSE);

	m_edit.ShowWindow(SW_SHOW);
	m_edit.SetFocus();
}

void CEditGrid::OnUpdateGrid()
{
	// Check to see if edit is visible.
	BOOL bVisible = ::GetWindowLong(m_edit.GetSafeHwnd(), GWL_STYLE)
				   & WS_VISIBLE;
	if (bVisible)
	{
		CString cStr;
		m_edit.GetWindowText(cStr);

		cStr.TrimLeft();
		cStr = " " + cStr;

		SetText(cStr);
		m_edit.SetWindowText("");
		m_edit.ShowWindow(SW_HIDE);
	}
} 


UINT CEditGrid::OnGetDlgCode()
{
	return DLGC_WANTALLKEYS;
}

void CEditGrid::OnSetFocus(CWnd* pOldWnd)
{
	CEditGrid::OnSetFocus(pOldWnd);

	OnUpdateGrid();
} 
