// PortList.cpp: implementation of the CPortList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "PortList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPortList* CPortList::_instance = 0;

CPortList* CPortList::Instance(){
    if (_instance == 0){
        _instance = new CPortList;
    }
    return _instance;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPortList::CPortList()
{
	PortList = 0;
	//MessageBox(NULL,"CPortList()","CPortList",MB_OK);
	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	COutputView* pView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	pView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	pView->TextOut("CPortList()");
}

CPortList::~CPortList()
{

}
//////////
void CPortList::append(CPort *pPort)
{
    if(CPortList::PortList == 0)
        PortList = pPort;
    else
        at_end->next = pPort;
            
    at_end = pPort;
}

int CPortList::is_empty()
{
    return PortList == 0 ? 1 : 0;
}

CPort * CPortList::GetPortListHead()
{
    return PortList;
}
