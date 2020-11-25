// EditWnd.cpp : implementation file
//

#include "stdafx.h"
#include "EditWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditWnd

CEditWnd::CEditWnd()
{
}

//CEditWnd::~CEditWnd()
//{
//}


BEGIN_MESSAGE_MAP(CEditWnd, CEdit)
	//{{AFX_MSG_MAP(CEditWnd)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditWnd message handlers

void CEditWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar != 13) //Return
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CEditWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == 27) //Esc
	{
		SetWindowText("");
		ShowWindow(SW_HIDE);		
		GetParent()->SetFocus();
	}
	else if (nChar == 13)  //Return 
	{
		GetParent()->SetFocus();
	}
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
