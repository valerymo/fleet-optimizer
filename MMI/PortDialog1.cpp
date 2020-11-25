// PortDialog1.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "PortDialog1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortDialog1 dialog


CPortDialog1::CPortDialog1(CWnd* pParent /*=NULL*/)
	: CDialog(CPortDialog1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortDialog1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPortDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortDialog1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortDialog1, CDialog)
	//{{AFX_MSG_MAP(CPortDialog1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortDialog1 message handlers

BOOL CPortDialog1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    CListCtrl* pList =
        (CListCtrl*) GetDlgItem(IDC_PORT_LIST_CTRL);
		
	//pList->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_GRIDLINES);
	pList->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0,
						  (LPARAM)LVS_EX_FULLROWSELECT|(LPARAM)LVS_EX_GRIDLINES);
	pList->InsertColumn(0,_T("PNB Name"),LVCFMT_LEFT,100,0);
	pList->InsertColumn(1,_T("Port Name"),LVCFMT_LEFT,100,1);
	pList->InsertColumn(2,_T("Region"),LVCFMT_LEFT,100,2);

	int numberOfLines = 0;

	CString fieldName = "NAME1";
	CString fieldValue = "1111";
	pList->InsertItem(0, fieldName);
	pList->SetItemText(0, 1, fieldValue);

	fieldName = "NAME2";
	fieldValue = "2222";
	pList->InsertItem(1, fieldName);
	pList->SetItemText(1, 1, fieldValue);
	pList->SetItemText(1, 2, fieldValue);

	fieldName = "NAME3";
	fieldValue = "3333";
	CString fieldValue1 = "JJJJ";
	pList->InsertItem(2, fieldName);
	pList->SetItemText(2, 2, fieldValue);
	pList->SetItemText(2, 1, fieldValue1);


						



	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
