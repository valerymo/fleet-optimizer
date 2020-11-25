// PortDistMatrixPPage.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "PortDistMatrixPPage.h"
#include "PortDetailsPSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortDistMatrixPPage property page

IMPLEMENT_DYNCREATE(CPortDistMatrixPPage, CPropertyPage)

CPortDistMatrixPPage::CPortDistMatrixPPage() : CPropertyPage(CPortDistMatrixPPage::IDD)
{
	//{{AFX_DATA_INIT(CPortDistMatrixPPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ISModifiable = FALSE;
	m_ISLoadDetails = FALSE;
	//m_pPortDistMatrixFile = "ports_dist.fl";
	CString	str = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","DbPortsDistMatrix");
	m_pPortDistMatrixFile = _strdup(str.GetBuffer(20));

	m_bInitialized = FALSE;
}

CPortDistMatrixPPage::~CPortDistMatrixPPage()
{
}

void CPortDistMatrixPPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortDistMatrixPPage)
	DDX_Control(pDX, IDC_MSFLEXGRID_DISTMATRIX, m_PortDistMatrixGrid);
	//}}AFX_DATA_MAP

	/* if (m_ISLoadDetails == TRUE){
		CString strMatrixValue = "DUDU P";
		for (int i = 1; i <= 3; i++)
			for (int j = 1; j <= 3; j++)
				m_PortDistMatrixGrid.SetTextMatrix(i,j,strMatrixValue);
		m_ISLoadDetails = FALSE;
	} */

	if(!m_bInitialized){
		FillDistMatrix();
		m_bInitialized = TRUE;
	}
    /*
	if(!m_bInitialized){

		char strNum[5];
		for (int i = 1; i < 100; i++){
			_itoa(i, strNum, 10);
			m_PortDistMatrixGrid.SetColWidth(i, 400);
			m_PortDistMatrixGrid.SetTextMatrix(i,0,strNum);
			m_PortDistMatrixGrid.SetTextMatrix(0,i,strNum);
		}

		CFile f;
		CFileException e;
		f.Open( m_pPortDistMatrixFile, CFile::modeRead, &e);
		CArchive arPortDist(&f, CArchive::load);
		m_PortsNumber = ((CPortDetailsPSheet*)m_pParentPtr)->GetRecordsNumber(); //stab
		CString strMatrixValue;
		for (i = 1; i <= m_PortsNumber; i++)
			for (int j = 1; j <= i; j++){
				strMatrixValue = "";
				arPortDist >> strMatrixValue;
				m_PortDistMatrixGrid.SetTextMatrix(i,j,strMatrixValue);
			}
		arPortDist.Close();
		f.Close();

		m_bInitialized = TRUE;
	}
*/
}


BEGIN_MESSAGE_MAP(CPortDistMatrixPPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPortDistMatrixPPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortDistMatrixPPage message handlers

BEGIN_EVENTSINK_MAP(CPortDistMatrixPPage, CPropertyPage)
    //{{AFX_EVENTSINK_MAP(CPortDistMatrixPPage)
	ON_EVENT(CPortDistMatrixPPage, IDC_MSFLEXGRID_DISTMATRIX, -601 /* DblClick */, OnDblClickDistmatrixGrid, VTS_NONE)
	ON_EVENT(CPortDistMatrixPPage, IDC_MSFLEXGRID_DISTMATRIX, 73 /* Scroll */, OnScrollDistmatrixGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPortDistMatrixPPage::OnDblClickDistmatrixGrid() 
{
	// TODO: Add your control notification handler code here
	if (m_ISModifiable == TRUE){
		short i = 13;
		m_PortDistMatrixGrid.OnKeyPressGrid(&i);// Simulate a return.
	}

}

void CPortDistMatrixPPage::OnScrollDistmatrixGrid() 
{
	// TODO: Add your control notification handler code here
	m_PortDistMatrixGrid.OnUpdateGrid();
	
}
/*
void CPortDistMatrixPPage::Serialize(CArchive& ar) 
{
	m_PortsNumber = ((CPortDetailsPSheet*)m_pParentPtr)->GetRecordsNumber(); //stab

	if (ar.IsStoring())
	{	// storing code m_PortsNumber = 
		//m_PortsNumber = ((CPortDetailsPSheet*)m_pParentPtr)->GetRecordsNumber(); //stab
		int count = 0;
		//ar << m_PortsNumber;
		for (int i = 1; i <= m_PortsNumber; i++)
			for (int j = 1; j <= m_PortsNumber; j++){
				ar << m_PortDistMatrixGrid.GetTextMatrix(i,j);
				count++;
			}
	}
	else
	{	// loading code
		CString strMatrixValue;
		int num;
		//m_PortsNumber = 3; //stab
		//ar >> num;
		for (int i = 1; i <= m_PortsNumber; i++)
			for (int j = 1; j <= m_PortsNumber; j++){
				strMatrixValue = "";
				ar >> strMatrixValue;
				m_PortDistMatrixGrid.SetTextMatrix(i,j,strMatrixValue);
			}
	}
}
*/
void CPortDistMatrixPPage::SetParentPtr(CPropertySheet *p)
{
			m_pParentPtr = (CPortDetailsPSheet*)p;
}

void CPortDistMatrixPPage::SetDistMatrixTitles()
{
	m_PortDistMatrixGrid.SetTextMatrix(1,0,_T("1"));
	m_PortDistMatrixGrid.SetTextMatrix(2,0,_T("2"));
	m_PortDistMatrixGrid.SetTextMatrix(3,0,_T("3"));
	m_PortDistMatrixGrid.SetTextMatrix(4,0,_T("4"));
}


void CPortDistMatrixPPage::Serialize(CArchive& ar) 
{
	CFile f;
	CFileException e;
	CString strTmp;

	m_PortsNumber = ((CPortDetailsPSheet*)m_pParentPtr)->GetRecordsNumber(); //stab
	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileInt("FleetOptimizer","PortsNumberInDB",m_PortsNumber);	

	
	if (ar.IsStoring())
	{	
		//f.Open( m_pPortDistMatrixFile, CFile::modeWrite, &e);
		//CArchive arPortDist(&f, CArchive::store);
		int count = 0;
		//ar << m_PortsNumber;
		for (int i = 1; i <= m_PortsNumber; i++)
			for (int j = 1; j <= i; j++){
				strTmp = m_PortDistMatrixGrid.GetTextMatrix(i,j);
				ar << strTmp;
				//ar << m_PortDistMatrixGrid.GetTextMatrix(i,j);
				count++;
			}
		//arPortDist.Close();
		//f.Close();
	}
	else
	{	// loading code
		CString strMatrixValue;

		f.Open( m_pPortDistMatrixFile, CFile::modeRead, &e);
		CArchive arPortDist(&f, CArchive::load);

		for (int i = 1; i <= m_PortsNumber; i++)
			for (int j = 1; j <= i; j++){
				strMatrixValue = "";
				arPortDist >> strMatrixValue;
				m_PortDistMatrixGrid.SetTextMatrix(i,j,strMatrixValue);
			}
	//	arPortDist.Close();
	}
	
	//f.Close();
	
}

void CPortDistMatrixPPage::FillDistMatrix()
{
		char strNum[5];
		for (int i = 1; i < 100; i++){
			_itoa(i, strNum, 10);
			m_PortDistMatrixGrid.SetColWidth(i, 400);
			m_PortDistMatrixGrid.SetTextMatrix(i,0,strNum);
			m_PortDistMatrixGrid.SetTextMatrix(0,i,strNum);
		}

		CFile f;
		CFileException e;
		f.Open( m_pPortDistMatrixFile, CFile::modeRead, &e);
		f.SeekToBegin();
		CArchive arPortDist(&f, CArchive::load);
		m_PortsNumber = ((CPortDetailsPSheet*)m_pParentPtr)->GetRecordsNumber(); //stab
		CString strMatrixValue;
		int flEx = 0;
		for (int i = 1; i <= m_PortsNumber; i++){
			for (int j = 1; j <= i; j++){
				strMatrixValue = "";
				try{
					arPortDist >> strMatrixValue;
				}
				catch(CArchiveException* pArchEx)
				{
					int cause = pArchEx->m_cause;
					if (cause == 3){
						flEx = 1;
						break;
					}
				}
				m_PortDistMatrixGrid.SetTextMatrix(i,j,strMatrixValue);
			}
			if (flEx)
				break;
		}
		arPortDist.Close();
		f.Close();

}

BOOL CPortDistMatrixPPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_bInitialized = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}