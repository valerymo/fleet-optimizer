// Scheduler.cpp: implementation of the CScheduler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "Scheduler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CScheduler* CScheduler::_instance = 0;

CScheduler* CScheduler::Instance(){
    if (_instance == 0){
        _instance = new CScheduler;
    }
    return _instance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScheduler::CScheduler()
{
    //MessageBox(NULL,"CScheduler()","CScheduler",MB_OK);
	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	COutputView* pView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	pView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	pView->TextOut("CScheduler()");
}
/*
CScheduler::~CScheduler()
{

}*/
