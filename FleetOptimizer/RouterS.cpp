// RouterS.cpp: implementation of the CRouterS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "RouterS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRouterS::CRouterS(CPort *pBasePort,int PNB_num, int cargo_type)
{
     Line = 0;
     next = 0;
     basePort = pBasePort;
     switch(cargo_type){
     case 1:
         cargoType = CCargo::Coal;
         break;
     case 2:
         cargoType = CCargo::General;
         break;
     case 3:
         cargoType = CCargo::Oil;
         break;
     case 4:
         cargoType = CCargo::Vegetables_fruits;
         break;
     }
    
     //CLine *pLine = new CLine(pBasePort->GetPNB(PNB_num).id,cargoType );
     CLine *pLine = new CLine(PNB_num,cargoType );
     Line = pLine;
     //int id = pBasePort->GetPNB(PNB_num).id;
     //Line->SetLine(id, cargoType);
}

CRouterS::~CRouterS()
{

}

void CRouterS::GetRouter()
{

}

CLine * CRouterS::GetLine()
{
    return Line;
}

void CRouterS::appendLine(CLine *pLine)
{
	Line = pLine;
}
