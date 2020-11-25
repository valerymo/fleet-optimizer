// PortPoint.cpp: implementation of the CPortPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "PortPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPortPoint::CPortPoint(int PNBnum, int cargo)
{
	next = NULL;
	PNB_num = PNBnum;
	switch(cargo){
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
	cargoLoadInShipPlan = 0;
}

CPortPoint::~CPortPoint()
{

}



int CPortPoint::GetPNBnum()
{
	return PNB_num;
}

int CPortPoint::GetCargoType()
{
	return cargoType;
}

CPortPoint* CPortPoint::GetNextPPoint()
{
	return next;
}

void CPortPoint::SetCargoLoadInShipPlan(int cargoLoad)
{
	cargoLoadInShipPlan = cargoLoad;
}

int CPortPoint::GetCargoLoadInShipPlan()
{
	return cargoLoadInShipPlan;
}
