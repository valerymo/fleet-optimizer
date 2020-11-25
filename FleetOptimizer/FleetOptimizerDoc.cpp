// FleetOptimizerDoc.cpp : implementation of the CFleetOptimizerDoc class
//

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "FleetOptimizerDoc.h"
#include "OutputView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerDoc

IMPLEMENT_DYNCREATE(CFleetOptimizerDoc, CDocument)

BEGIN_MESSAGE_MAP(CFleetOptimizerDoc, CDocument)
	//{{AFX_MSG_MAP(CFleetOptimizerDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerDoc construction/destruction

CFleetOptimizerDoc::CFleetOptimizerDoc()
{
	// TODO: add one-time construction code here
}

CFleetOptimizerDoc::~CFleetOptimizerDoc()
{
}

BOOL CFleetOptimizerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerDoc serialization

void CFleetOptimizerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerDoc diagnostics

#ifdef _DEBUG
void CFleetOptimizerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFleetOptimizerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFleetOptimizerDoc commands




CDC* CFleetOptimizerDoc::GetOutputViewDC()
{
	POSITION pos = GetFirstViewPosition();
	COutputView* pView = (COutputView*)GetNextView(pos); //0 view: FleetOptimizerView
	pView = (COutputView*)GetNextView(pos); // 1st view: OutputView

	CDC* pDC = pView->GetDC();
	return pDC;

}

COutputView* CFleetOptimizerDoc::GetOutputView()
{
	POSITION pos = GetFirstViewPosition();
	COutputView* pView = (COutputView*)GetNextView(pos); //0 view: FleetOptimizerView
	pView = (COutputView*)GetNextView(pos); // 1st view: OutputView
	return pView;
}

void CFleetOptimizerDoc::OnFileSave() 
{
	//Save active scenario file name in Registry for next session
	CString sScenarioFile = ((CFleetOptimizerApp*)AfxGetApp())->m_pScenarioFile;
	((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","ScenarioFile", sScenarioFile);	

	
}



void CFleetOptimizerDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	
}
