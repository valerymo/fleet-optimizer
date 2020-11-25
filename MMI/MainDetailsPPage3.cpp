// MainDetailsPPage3.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "MainDetailsPPage3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDetailsPPage3 property page

IMPLEMENT_DYNCREATE(CMainDetailsPPage3, CPropertyPage)

CMainDetailsPPage3::CMainDetailsPPage3() : CPropertyPage(CMainDetailsPPage3::IDD)
{
	//{{AFX_DATA_INIT(CMainDetailsPPage3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMainDetailsPPage3::~CMainDetailsPPage3()
{
}

void CMainDetailsPPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDetailsPPage3)
	DDX_Control(pDX, IDC_CARGO_LISTCNTRL1, m_CargoList);
	//}}AFX_DATA_MAP
	m_CargoList.InsertColumn(0, _T("XXXXXX"), LVCFMT_LEFT, 50 ,0);

}


BEGIN_MESSAGE_MAP(CMainDetailsPPage3, CPropertyPage)
	//{{AFX_MSG_MAP(CMainDetailsPPage3)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDetailsPPage3 message handlers
