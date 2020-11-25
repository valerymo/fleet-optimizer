// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "FleetOptimizerView.h"
#include "OutputView.h"

#include "MainFrm.h"
#include <direct.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCENARIO_LOADED, OnUpdateIndicatorScenarioLoaded)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCENARIO_LOADED,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pScenarioFileMF = _strdup("");
	m_bScenLoaded = 0;
	GetSetRegistryProfileData();
	CreateLogDir();
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}


//BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
//	CCreateContext* pContext)
/*{
		return m_wndSplitter.Create(this,
		2, 2,               // TODO: adjust the number of rows, columns
		CSize(10, 10),      // TODO: adjust the minimum pane size
		pContext);


}
*/

BOOL CMainFrame::OnCreateClient( LPCREATESTRUCT /*lpcs*/,
                                 CCreateContext* pContext)
{
    VERIFY(m_wndSplitter.CreateStatic(this, 2, 1));
    VERIFY(m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CFleetOptimizerView),
                                    CSize(150, 150), pContext));
    VERIFY(m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(COutputView),
                                    CSize(150, 150), pContext));
    return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers




void CMainFrame::GetSetRegistryProfileData()
{
	CString str, sLogDir, sAppDir, sUserWorkDir, sPortsDbFile;
	//scenario file, full path
	str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","ScenarioFile");	
	m_pScenarioFileMF = _strdup(str.GetBuffer(20));

	//working directory
	int length = 200;
	GetCurrentDirectory(length,m_cCurrentDir);
	sAppDir = m_cCurrentDir;
	sAppDir += "\\";

	// Проверка ситуации когда FleetOptimizer запускается из программы установки
	WIN32_FIND_DATA FindFileData; 
	sPortsDbFile = sAppDir + "..\\DB\\ports_db.fd";

	if (FindFirstFile(sPortsDbFile, &FindFileData)  == INVALID_HANDLE_VALUE){ 
		//ports_db.fd - не найден. Ситуация возможна в случае, 
		//если FleetOptimizer запускается из программы установки 
		sAppDir = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","InstallDir");
		sAppDir += "\\";
	}
	//

	//user directory
	// get from registry; if not found (working in developmnet environmnet, or running from exporer) -
	sUserWorkDir = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","UserWorkDir");
	if (sUserWorkDir == "")
		sUserWorkDir = sAppDir + "..";

	str = 	((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","AppDir");	
	//if(str == ""){
		strcpy(m_cAppDir, sAppDir);
		strcpy(m_cUserWorkDir, sUserWorkDir);
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","AppDir",m_cAppDir);	
		//Logs:
		sLogDir = sUserWorkDir;
		sLogDir += "\\Log\\";
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","LogDir",sLogDir);	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","LogFileVehiclesList",sLogDir+"VehiclesList.txt");	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","LogFileInitDBMVehiclesAll",sLogDir+"InitDBMVehiclesAll.txt");	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","LogFileErrors",sLogDir+"Errors_log.txt");	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","LogFileInitialBasePlan",sLogDir+"InitialBasePlan.txt");	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","LogFileOptimizStep1Plan",sLogDir+"Step1Plan.txt");	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","LogFileOptimizStep2Plan",sLogDir+"Step2Plan.txt");	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","LogFileOptimizStep3Plan",sLogDir+"Step3Plan.txt");	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","LogFileVehiclesDetails",sLogDir+"VehiclesDetails.xls");	

		//DB files:
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","DbPorts",sAppDir+"..\\DB\\ports_db.fd");	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","DbShips",sAppDir+"..\\DB\\ships_db.fd");	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","DbProtoACV",sAppDir+"..\\DB\\proto_acv_db.fd");	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","DbProtoShip",sAppDir+"..\\DB\\proto_ship_db.fd");	
		((CFleetOptimizerApp*)AfxGetApp())->WriteProfileString("FleetOptimizer","DbPortsDistMatrix",sAppDir+"..\\DB\\ports_dist.fl");	

	//}
	//else
	//	strcpy(m_cAppDir, str);
	
}


void CMainFrame::OnUpdateIndicatorScenarioLoaded(CCmdUI* pCmdUI)
{
	pCmdUI->SetText(_T(""));

	//Compute drawing zone rectangle
	CRect rect;
	m_wndStatusBar.GetItemRect(pCmdUI->m_nIndex,&rect);
	rect.DeflateRect(2,2);

	//Draw current shape in pane
	CClientDC dc(&m_wndStatusBar);

	//Erase destination rectangle
	CBrush brBack(::GetSysColor(COLOR_3DFACE));
	CBrush* pOldBrush = dc.SelectObject(&brBack);

	dc.PatBlt(rect.left,rect.top,rect.Width(),rect.Height(),PATCOPY);

	if (m_bScenLoaded)
	{
		//Draw shape
		CBrush brShape(RGB(0,255,0));//green
		dc.SelectObject(&brShape);	

		//case CIRCLE :
		//dc.Ellipse(&rect);
		dc.FillRect(&rect,&brShape );

	}//end of if (m_bSpyRegisteredForStatusBar == TRUE)
	else
	{
		//Draw shape
		//CBrush brShape(RGB(200,0,0));//red
		CBrush brShape(RGB(255,255,0));//red
		dc.SelectObject(&brShape);	

		//case CIRCLE :
		//dc.Ellipse(&rect);
		dc.FillRect(&rect,&brShape );
	}

	dc.SelectObject(pOldBrush);
	pCmdUI->Enable(TRUE);

}

void CMainFrame::UpdateScenarioFile(CString str)
{
	if (str != "")
		m_bScenLoaded = 1;
	else
		m_bScenLoaded = 0;
}

void CMainFrame::CreateLogDir()
{
	CString sLogDir;
	WIN32_FIND_DATA FindFileData; 

	sLogDir = m_cUserWorkDir;
	sLogDir += "\\Log\\";

	if (FindFirstFile(sLogDir, &FindFileData)  == INVALID_HANDLE_VALUE){ //Log directory doesn't exist
		if( _mkdir(sLogDir) != 0 ){
			TRACE0("Failed to create Log directory\n");
		}
	}
}
