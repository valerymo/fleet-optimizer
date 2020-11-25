// PortDialog.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "PortDialog.h"
#include "..\FleetOptimizer\Port.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortDialog dialog


CPortDialog::CPortDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPortDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortDialog)
	//}}AFX_DATA_INIT
}


void CPortDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortDialog)
	DDX_Control(pDX, IDC_PORT_LIST, m_CtrlPortList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortDialog, CDialog)
	//{{AFX_MSG_MAP(CPortDialog)
	ON_BN_CLICKED(IDREFRESH, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortDialog message handlers

BOOL CPortDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPortDialog::OnRefresh() 
{
	// TODO: Add your control notification handler code here
	m_CtrlPortList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,(LPARAM)LVS_EX_GRIDLINES);

	int coal, general, oil, vegetables_fruits;
		int BasePortNum;
	char str[20];

	CString line("");


	char* pFileName = "port_log.fl";
	CFile f;
	if (!f.Open( pFileName, CFile::modeRead)){
		MessageBox("Ошибка открытия/чтения файла","CPortDialog::OnRefresh()",MB_OK);
	}
	
	CArchive archive(&f, CArchive::load);

	CString PortName;
	CString PNBName;
	archive >> BasePortNum;
	archive >> PortName;

	sprintf(str,"%d",BasePortNum);
	m_CtrlPortList.AddString(str); 
	for (int port_num = 1; port_num <= BasePortNum; port_num++){
		//archive >> PortName;
		line = "";
		for (int i = 0; i < 2; i++){
			archive >> PNBName;
			line = (PNBName + "  " + PortName);
			m_CtrlPortList.AddString(line);

			archive >> coal;
			archive >> general;
			archive >> oil;
			archive >> vegetables_fruits;
		}
	}


	archive.Close();
	f.Close();

}
