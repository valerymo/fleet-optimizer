// Line.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "Line.h"
#include "Cargo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLine::CLine(int PNBnum, CCargo::CargoType cargo)
{
   next = NULL;
   count = 0;
   PNB_num = PNBnum;
   cargoType = cargo;
}

CLine::CLine(int PNBnum, int cargo)
{
	next = NULL;
	count = 0;
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
}




CLine::~CLine()
{

}

//void CLine::SetLine(int PNBid, CCargo::CargoType cargo){
//   PNB_id = PNBid;
//   cargoType = cargo;
//}

int CLine::GetCargoType()
{
    return cargoType;
}

int CLine::GetPNBnum()
{
    return PNB_num;
}

CLine* CLine::GetNextLine()
{
	return next;
}

void CLine::SetCount(int LineCountInRouterL)
{
	count = LineCountInRouterL;	
}

int CLine::GetCount()
{
	return count;
}

int CLine::ReplacePNB(int PNBnum)
{
	PNB_num = PNBnum;
	return 1;
}
