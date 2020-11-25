// ShipList.cpp: implementation of the CShipList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "ShipList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShipList::CShipList()
{

}

CShipList::~CShipList()
{

}

//////////////////////

void CShipList::append(CShip *pShip)
{
    if(CPortList::PortList == 0)
        PortList = pPort;
    else
        at_end->next = pPort;
            
    at_end = pPort;
}