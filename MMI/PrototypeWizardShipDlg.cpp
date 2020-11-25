// PrototypeWizardShipDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "PrototypeWizardShipDlg.h"
#include "..\FleetOptimizer\FleetOptimizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrototypeWizardShipDlg dialog


CPrototypeWizardShipDlg::CPrototypeWizardShipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrototypeWizardShipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrototypeWizardShipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//m_pDBFile = "proto_ship_db.fd";
	CString	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbProtoShip");
	m_pDBFile = _strdup(str.GetBuffer(20));

	m_nID = CPrototypeWizardShipDlg::IDD;
	CDialog::Create(m_nID, pParent);

	m_ISModifiable = 0;
	SetTableTitles();
	m_OpenStat = 0;
}


void CPrototypeWizardShipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrototypeWizardShipDlg)
	DDX_Control(pDX, IDC_PROTOWIZSHIP_LIST, m_PrototypeShipGrid);
	//}}AFX_DATA_MAP

	FillGenInfoGrid();
}


BEGIN_MESSAGE_MAP(CPrototypeWizardShipDlg, CDialog)
	//{{AFX_MSG_MAP(CPrototypeWizardShipDlg)
	ON_BN_CLICKED(IDC_PROTOWIZSHIP_EDIT, OnProtoShipEdit)
	ON_BN_CLICKED(IDC_PROTOWIZSHIP_NEW, OnProtoShipNew)
	ON_BN_CLICKED(IDC_PROTOWIZSHIP_REFRESH, OnProtoShipRefresh)
	ON_BN_CLICKED(IDC_PROTOWIZSHIP_APPLY, OnProtoShipApply)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CPrototypeWizardShipDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrototypeWizardShipDlg message handlers



void CPrototypeWizardShipDlg::CleanProtoShipGrid()
{
	int i,j,max;

	max = MAX_PROTOTYPES_RECORDS_IN_SHIPDB;
	//max = m_RecordsNumber+1;
	for (i = 1; i < max; i++)
		for (j = 0; j <= SHIP_DB_FIELDS_NUM; j++)
			m_PrototypeShipGrid.SetTextMatrix(i,j,"");

}

void CPrototypeWizardShipDlg::SetTableTitles()
{
			m_PrototypeShipGrid.SetTextMatrix(0,1,"Name ");
			m_PrototypeShipGrid.SetTextMatrix(0,2,"Project ID ");
			m_PrototypeShipGrid.SetTextMatrix(0,3,"Classification");
			m_PrototypeShipGrid.SetTextMatrix(0,4,"Length LOA, m");
			m_PrototypeShipGrid.SetTextMatrix(0,5,"Length LPP, m");
			m_PrototypeShipGrid.SetTextMatrix(0,6,"Beam BOA, m");
			m_PrototypeShipGrid.SetTextMatrix(0,7,"Depth, m");
			m_PrototypeShipGrid.SetTextMatrix(0,8,"Draft, m)");
			m_PrototypeShipGrid.SetTextMatrix(0,9,"Displacement loaded, t");
			m_PrototypeShipGrid.SetTextMatrix(0,10,"DW, t");
			m_PrototypeShipGrid.SetTextMatrix(0,11,"Load, t");
			m_PrototypeShipGrid.SetTextMatrix(0,12,"Speed loaded, knots");
			m_PrototypeShipGrid.SetTextMatrix(0,13,"Speed in ballast, knots");
			m_PrototypeShipGrid.SetTextMatrix(0,14,"Dist, miles. Sailing range ");
			m_PrototypeShipGrid.SetTextMatrix(0,15,"Wb, m3. Bale Capacity");
			m_PrototypeShipGrid.SetTextMatrix(0,16,"Wg, m3. Grain Capacity");
			m_PrototypeShipGrid.SetTextMatrix(0,17,"Hatches, count");
			m_PrototypeShipGrid.SetTextMatrix(0,18,"Jib, count*load");
			m_PrototypeShipGrid.SetTextMatrix(0,19,"Jib radius, m");
			m_PrototypeShipGrid.SetTextMatrix(0,20,"Cranes, count*load");
			m_PrototypeShipGrid.SetTextMatrix(0,21,"Cranes Jib radius, m");
			m_PrototypeShipGrid.SetTextMatrix(0,23,"Full cost of vessel");
			m_PrototypeShipGrid.SetTextMatrix(0,24,"Hull cost/full cost");
			m_PrototypeShipGrid.SetTextMatrix(0,25,"Cost of Power/Full cost");
			m_PrototypeShipGrid.SetTextMatrix(0,27,"Daily operating costs on sail");
			m_PrototypeShipGrid.SetTextMatrix(0,28,"Daily operating costs in unloading in port");
			m_PrototypeShipGrid.SetTextMatrix(0,29,"Daily operating costs in unloading waiting");
			m_PrototypeShipGrid.SetTextMatrix(0,30,"Daily operating costs in Winter, out of work");
			m_PrototypeShipGrid.SetTextMatrix(0,31,"Daily operating costs in Repair");
			m_PrototypeShipGrid.SetTextMatrix(0,32,"Daily Fuel cost/Daily costs on sail");
			m_PrototypeShipGrid.SetTextMatrix(0,33,"p01, Hull cost");
			
}




void CPrototypeWizardShipDlg::FillGenInfoGrid()
{
	CFile f( m_pDBFile, CFile::modeRead );
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);
	Serialize(ar);
	ar.Close();
	f.Close();
}

void CPrototypeWizardShipDlg::RecordsNumIncrement(void){

	CFile f;
	CFileException e;
	char strBuf[3];

	f.Open( m_pDBFile, CFile::modeWrite, &e);
	f.SeekToBegin();
	CArchive ar(&f, CArchive::store);
	m_RecordsNumber++;
	_itoa( m_RecordsNumber, strBuf, 10 );
	CString strNum = strBuf;
	ar << strNum;
	ar.Close();
	f.Close();

}



void CPrototypeWizardShipDlg::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
		CString	str;
		if (m_OpenStat == 3){ //New Prototype. 
							  //Add latest line from Table to DB.
			int i = m_RecordsNumber;
			for (int j = 1; j< SHIP_DB_FIELDS_NUM; j++){ 
				str = m_PrototypeShipGrid.GetTextMatrix(i,j);
				str.TrimLeft(); ///???? del left space
				ar << str;
			}
			str = "\n";
			ar << str;
		}
		if (m_OpenStat == 2){ //Edit Prototype Table
			char strBuf[3];
			_itoa( m_RecordsNumber, strBuf, 10 );
			str = strBuf;
			ar << str;
			for (int i = 1; i <= m_RecordsNumber; i++){
				for (int j = 1; j < SHIP_DB_FIELDS_NUM; j++){
					str = m_PrototypeShipGrid.GetTextMatrix(i,j);
					str.TrimLeft();
					ar << str;
					//ar << m_PrototypeShipGrid.GetTextMatrix(i,j);
				}
				str = "\n";
				ar << str;
			}
		}
	}
	else
	{	// loading code
		CString	str, strNum;
		ULONGLONG positionEnd, position;
		char *stopstring;
		
		CFile* fp = ar.GetFile();
		fp->SeekToEnd();
		positionEnd = fp->GetPosition();
		fp->SeekToBegin();
		position = fp->GetPosition();
		
		if (positionEnd == 0)
			m_RecordsNumber = 0;
		else {
			ar >> strNum;
			m_RecordsNumber  = (int)strtod(strNum,&stopstring);
		}

		int i = 1;
		char strRowNum[5];
   
		while (i <= m_RecordsNumber){
			_itoa(i, strRowNum, 10);
			m_PrototypeShipGrid.SetTextMatrix(i,0,strRowNum);
			for (int j = 1; j< SHIP_DB_FIELDS_NUM; j++){		//j???
				ar >> str;
				m_PrototypeShipGrid.SetTextMatrix(i,j,str);
			}
			ar >> str; //end of line
			i++;
		}//end of while (i <= m_RecordsNumber)

	}//end of else //loading code
}

BEGIN_EVENTSINK_MAP(CPrototypeWizardShipDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CPrototypeWizardShipDlg)
	ON_EVENT(CPrototypeWizardShipDlg, IDC_PROTOWIZSHIP_LIST, -601 /* DblClick */, OnDblClickProtoShipList, VTS_NONE)
	ON_EVENT(CPrototypeWizardShipDlg, IDC_PROTOWIZSHIP_LIST, 73 /* Scroll */, OnScrollProtoShipList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPrototypeWizardShipDlg::OnDblClickProtoShipList() 
{
	// TODO: Add your control notification handler code here
	if (m_ISModifiable == 1){
		short i = 13;
		m_PrototypeShipGrid.OnKeyPressGrid(&i);// Simulate a return.
	}	
	
}

void CPrototypeWizardShipDlg::OnProtoShipEdit() 
{
	// TODO: Add your control notification handler code here
	m_OpenStat = 2;
	m_ISModifiable = 1;
	m_PrototypeShipGrid.SetFixedCols(1);
}


void CPrototypeWizardShipDlg::OnProtoShipNew() 
{
	// TODO: Add your control notification handler code here
	m_OpenStat = 3;
	m_ISModifiable = 1;
	m_PrototypeShipGrid.SetFixedCols(1);
}

void CPrototypeWizardShipDlg::OnProtoShipRefresh() 
{
	// TODO: Add your control notification handler code here
	CleanProtoShipGrid();
	FillGenInfoGrid();
	m_ISModifiable = 0;
	m_PrototypeShipGrid.SetFixedCols(2);
	m_OpenStat = 0;
}


void CPrototypeWizardShipDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_ISModifiable = 0;
	m_PrototypeShipGrid.SetFixedCols(2);
	m_OpenStat = 0;
	
	CDialog::OnCancel();
}

void CPrototypeWizardShipDlg::OnOK() 
{
	// TODO: Add extra validation here
/*	CFile f;
	CFileException e;

	if (m_OpenStat == 3){ //New Ship
		RecordsNumIncrement();
		f.Open( m_pDBFile, CFile::modeWrite, &e);
		f.SeekToEnd();
		CArchive ar(&f, CArchive::store);
		Serialize(ar);
		ar.Close();
		f.Close();
	}
	if (m_OpenStat == 2) { //Edit Ship
		f.Open( m_pDBFile, CFile::modeWrite, &e);
		CArchive ar(&f, CArchive::store);
		Serialize(ar);
		ar.Close();
		f.Close();
	}	
	
	m_ISModifiable = 0;
	m_PrototypeShipGrid.SetFixedCols(2);
	m_OpenStat = 0;
*/
	Apply();
	CDialog::OnOK();
}


void CPrototypeWizardShipDlg::OnScrollProtoShipList() 
{
	// TODO: Add your control notification handler code here
	m_PrototypeShipGrid.OnUpdateGrid();
	
}

void CPrototypeWizardShipDlg::OnProtoShipApply() 
{
	// TODO: Add your control notification handler code here
	Apply();
}



void CPrototypeWizardShipDlg::Apply() 
{
	CFile f;
	CFileException e;

	if (m_OpenStat == 3){ //New Ship
		RecordsNumIncrement();
		f.Open( m_pDBFile, CFile::modeWrite, &e);
		f.SeekToEnd();
		CArchive ar(&f, CArchive::store);
		Serialize(ar);
		ar.Close();
		f.Close();
	}
	if (m_OpenStat == 2) { //Edit Ship
		f.Open( m_pDBFile, CFile::modeWrite, &e);
		CArchive ar(&f, CArchive::store);
		Serialize(ar);
		ar.Close();
		f.Close();
	}	
	
	m_ISModifiable = 0;
	m_PrototypeShipGrid.SetFixedCols(2);
	m_OpenStat = 0;
}

void CPrototypeWizardShipDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
