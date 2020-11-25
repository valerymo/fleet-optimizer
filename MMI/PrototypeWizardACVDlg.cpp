// PrototypeWizardACVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PrototypeWizardACVDlg.h"
#include "..\FleetOptimizer\FleetOptimizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrototypeWizardACVDlg dialog


CPrototypeWizardACVDlg::CPrototypeWizardACVDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrototypeWizardACVDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrototypeWizardACVDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//m_pDBFile = "proto_acv_db.fd";
	CString str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbProtoACV");
	m_pDBFile = _strdup(str.GetBuffer(20));


	m_nID = CPrototypeWizardACVDlg::IDD;
	CDialog::Create(m_nID, pParent);
	
	m_ISModifiable = 0;
	SetTableTitles();

	m_OpenStat = 0;
//	m_RecordsNumber = 30; //stab
}


void CPrototypeWizardACVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrototypeWizardACVDlg)
	DDX_Control(pDX, IDC_PROTOWIZACV_LIST, m_PrototypeACVGrid);
	//}}AFX_DATA_MAP

	//m_PrototypeACVGrid.SetFixedCols(2);
	FillGenInfoGrid();

}


BEGIN_MESSAGE_MAP(CPrototypeWizardACVDlg, CDialog)
	//{{AFX_MSG_MAP(CPrototypeWizardACVDlg)
	ON_BN_CLICKED(IDC_PROTOWIZACV_EDIT, OnProtoACVEdit)
	ON_BN_CLICKED(IDC_PROTOWIZACV_REFRESH, OnProtoACVRefresh)
	ON_BN_CLICKED(IDC_PROTOWIZACV_NEW, OnProtoACVNew)
	ON_BN_CLICKED(IDC_PROTOWIZACV_APPLY, OnProtoACVApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrototypeWizardACVDlg message handlers

BEGIN_EVENTSINK_MAP(CPrototypeWizardACVDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CPrototypeWizardACVDlg)
	ON_EVENT(CPrototypeWizardACVDlg, IDC_PROTOWIZACV_LIST, -601 /* DblClick */, OnDblClickProtoACVList, VTS_NONE)
	ON_EVENT(CPrototypeWizardACVDlg, IDC_PROTOWIZACV_LIST, 73 /* Scroll */, OnScrollProtoACVList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPrototypeWizardACVDlg::OnDblClickProtoACVList() 
{
	// TODO: Add your control notification handler code here
	if (m_ISModifiable == 1){
		short i = 13;
		m_PrototypeACVGrid.OnKeyPressGrid(&i);// Simulate a return.
	}	
}

void CPrototypeWizardACVDlg::OnScrollProtoACVList() 
{
	// TODO: Add your control notification handler code here
	m_PrototypeACVGrid.OnUpdateGrid();
}



void CPrototypeWizardACVDlg::OnOK() 
{
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
	m_PrototypeACVGrid.SetFixedCols(2);
	m_OpenStat = 0;
*/
	Apply();		

	CDialog::OnOK();
}




void CPrototypeWizardACVDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_ISModifiable = 0;
	m_PrototypeACVGrid.SetFixedCols(2);
	m_OpenStat = 0;
	
	CDialog::OnCancel();
}


void CPrototypeWizardACVDlg::CleanProtoACVGrid()
{
	int i,j,max;
	
	max = MAX_PROTOTYPES_RECORDS_IN_ACVDB;

	for (i = 1; i < max; i++)
		for (j = 0; j <= ACV_DB_FIELDS_NUM; j++)
			m_PrototypeACVGrid.SetTextMatrix(i,j,"");

}

void CPrototypeWizardACVDlg::OnProtoACVRefresh() 
{
	// TODO: Add your control notification handler code here
	CleanProtoACVGrid();
	FillGenInfoGrid();
	//m_ISModifiable = 0;
	m_PrototypeACVGrid.SetFixedCols(2);

}


void CPrototypeWizardACVDlg::SetTableTitles()
{
		//статьи 4...50: см. Г.Ф.Демешко. Проектирование судов. АСВП:
		// таблицы 1,1a,2,3, Приложения,
		// Важнейшие тактико-технические характеристики АСВП;
		// Сопоставление элементов и характеристик АСВП.
			m_PrototypeACVGrid.SetTextMatrix(0,1,"Name ");
			m_PrototypeACVGrid.SetTextMatrix(0,2,"Project Num ");
			m_PrototypeACVGrid.SetTextMatrix(0,3,"Classification");
			m_PrototypeACVGrid.SetTextMatrix(0,4,"   L");
			m_PrototypeACVGrid.SetTextMatrix(0,5,"Width");
			m_PrototypeACVGrid.SetTextMatrix(0,6,"Full Weight, t");
			m_PrototypeACVGrid.SetTextMatrix(0,7,"AC lengh");
			m_PrototypeACVGrid.SetTextMatrix(0,8,"AC width");
			m_PrototypeACVGrid.SetTextMatrix(0,9,"AC lengh/AC width");
			m_PrototypeACVGrid.SetTextMatrix(0,10,"AC area");
			m_PrototypeACVGrid.SetTextMatrix(0,11,"Skirt Height");
			m_PrototypeACVGrid.SetTextMatrix(0,12,"SkirtElement Height/Skirt Height, SkirtElement reletive Height");
			m_PrototypeACVGrid.SetTextMatrix(0,13,"lп=Lп/(D/p)pow)1/3)");
			m_PrototypeACVGrid.SetTextMatrix(0,14,"AC Perim, m, AC Perimeter");
			m_PrototypeACVGrid.SetTextMatrix(0,15,"AC Pressure, kPa, AC Pressure");
			m_PrototypeACVGrid.SetTextMatrix(0,16,"Fan Performence. m3/с");
			m_PrototypeACVGrid.SetTextMatrix(0,17,"Q, m3/sec, Air flow");
			m_PrototypeACVGrid.SetTextMatrix(0,18,"SkirtHeight/AC width, relative height of Skirt");
			m_PrototypeACVGrid.SetTextMatrix(0,19,"Kp, pressure ratio");
			m_PrototypeACVGrid.SetTextMatrix(0,20,"N, kvt, Full continuous power");
			m_PrototypeACVGrid.SetTextMatrix(0,21,"Number and type of main engines");
			m_PrototypeACVGrid.SetTextMatrix(0,22,"Main engine power, Nmax , Nnomin");
			m_PrototypeACVGrid.SetTextMatrix(0,23,"Propulsors num * type");
			m_PrototypeACVGrid.SetTextMatrix(0,24,"Propulsor Diameter, m");
			m_PrototypeACVGrid.SetTextMatrix(0,25,"Propulsor rotation frequency, 1/min");
			m_PrototypeACVGrid.SetTextMatrix(0,26,"Fans num * type");
			m_PrototypeACVGrid.SetTextMatrix(0,27,"Fan Diameter");
			m_PrototypeACVGrid.SetTextMatrix(0,28,"Fan rotation frequency, 1/min");
			m_PrototypeACVGrid.SetTextMatrix(0,29,"Max speed in calm water");
			m_PrototypeACVGrid.SetTextMatrix(0,30,"FrD max");
			m_PrototypeACVGrid.SetTextMatrix(0,31,"Payload, t");
			m_PrototypeACVGrid.SetTextMatrix(0,32,"Number of passengers / cars");
			m_PrototypeACVGrid.SetTextMatrix(0,33,"Fuel reserves, t");
			m_PrototypeACVGrid.SetTextMatrix(0,34,"N/Dfuel, kvt/t, power-to-weight ratio");
			m_PrototypeACVGrid.SetTextMatrix(0,35,"K=G*v/N, Power ratio");
			m_PrototypeACVGrid.SetTextMatrix(0,36,"Qac/Dac, specific air consumption in AC.");
			m_PrototypeACVGrid.SetTextMatrix(0,37,"Qacs*10^2, air consumption coefficient");
			m_PrototypeACVGrid.SetTextMatrix(0,38,"Qach, air consumption coefficient");
			m_PrototypeACVGrid.SetTextMatrix(0,39,"Relative Height of Skirt, Hskirt/(Dfull/p)^1/3");
			m_PrototypeACVGrid.SetTextMatrix(0,40,"t, calculated clearance under skirt edge, m=0.55");
			m_PrototypeACVGrid.SetTextMatrix(0,41,"(t/Lac)10^3, relative clearance");
			m_PrototypeACVGrid.SetTextMatrix(0,42,"(t?/Sac)10^2, relative clearance");
			m_PrototypeACVGrid.SetTextMatrix(0,43,"Gs*10^2, load coefficient");
			m_PrototypeACVGrid.SetTextMatrix(0,44,"Gl*10^2, load coefficient");
			m_PrototypeACVGrid.SetTextMatrix(0,45,"time=(Sac*Hskirt)/Q, sec, AC fill time");
			m_PrototypeACVGrid.SetTextMatrix(0,46,"Tz=6.28*(Hskirt/g)^0.5, sec, Heave period");
			m_PrototypeACVGrid.SetTextMatrix(0,47,"tac/Tz");
			m_PrototypeACVGrid.SetTextMatrix(0,48,"Demp, t, ACV weight without payload");
			m_PrototypeACVGrid.SetTextMatrix(0,49,"(Dload+Dfuel)/Dfull, utilization ratio.");
			m_PrototypeACVGrid.SetTextMatrix(0,50,"Range, miles");
			m_PrototypeACVGrid.SetTextMatrix(0,51,"ACV Build cost");
			m_PrototypeACVGrid.SetTextMatrix(0,52,"Cost of Hull/Full build cost");
			m_PrototypeACVGrid.SetTextMatrix(0,53,"Cost of Power plant/Full build cost");
			m_PrototypeACVGrid.SetTextMatrix(0,54,"Skirt cost/Full build cost");
			m_PrototypeACVGrid.SetTextMatrix(0,55,"Daily operating cost, under way");
			m_PrototypeACVGrid.SetTextMatrix(0,56,"Daily operating cost, in loading/unloading");
			m_PrototypeACVGrid.SetTextMatrix(0,57,"Daily operating cost, на стоянке без гр.операций");
			m_PrototypeACVGrid.SetTextMatrix(0,58,"Daily operating cost, на отстое");
			m_PrototypeACVGrid.SetTextMatrix(0,59,"Daily operating cost, на ремонте");
			m_PrototypeACVGrid.SetTextMatrix(0,60,"Oil cost/Daily operating cost under way");
}




void CPrototypeWizardACVDlg::FillGenInfoGrid()
{
	CFile f( m_pDBFile, CFile::modeRead );
	f.SeekToBegin();
	CArchive ar(&f, CArchive::load);
	Serialize(ar);
	ar.Close();
	f.Close();
}





void CPrototypeWizardACVDlg::Serialize(CArchive& ar) 
{
	if (ar.IsStoring())
	{	// storing code
		CString	str;
		if (m_OpenStat == 3){ //New Prototype. 
							  //Add latest line from Table to DB.
			int i = m_RecordsNumber;
			for (int j = 1; j<=ACV_DB_FIELDS_NUM; j++){
				str = m_PrototypeACVGrid.GetTextMatrix(i,j);
				str.TrimLeft();
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
				for (int j = 1; j <= ACV_DB_FIELDS_NUM; j++){
					str = m_PrototypeACVGrid.GetTextMatrix(i,j);
					str.TrimLeft();
					ar << str;
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
			m_PrototypeACVGrid.SetTextMatrix(i,0,strRowNum);
			for (int j = 1; j<=ACV_DB_FIELDS_NUM; j++){
				ar >> str;
				m_PrototypeACVGrid.SetTextMatrix(i,j,str);
			}
			ar >> str; //end of line
			i++;
		}//end of while (i <= m_RecordsNumber)

	}//end of else //loading code
}

void CPrototypeWizardACVDlg::RecordsNumIncrement(void){

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

void CPrototypeWizardACVDlg::OnProtoACVNew() 
{
	// TODO: Add your control notification handler code here
	m_OpenStat = 3;
	m_ISModifiable = 1;
	m_PrototypeACVGrid.SetFixedCols(1);
}


void CPrototypeWizardACVDlg::OnProtoACVEdit() 
{
	// TODO: Add your control notification handler code here
	m_OpenStat = 2;
	m_ISModifiable = 1;
	m_PrototypeACVGrid.SetFixedCols(1);
}

void CPrototypeWizardACVDlg::OnProtoACVApply() 
{
	Apply();		
}



void CPrototypeWizardACVDlg::Apply() 
{
	// TODO: Add your control notification handler code here
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
	m_PrototypeACVGrid.SetFixedCols(2);
	m_OpenStat = 0;
	
}
