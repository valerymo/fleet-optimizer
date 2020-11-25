// GPoint.cpp: implementation of the CGPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "GPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGPoint::CGPoint(int PNBnum, int cargo)
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
}

CGPoint::~CGPoint()
{

}

int CGPoint::GetPNBnum()
{
	return PNB_num;
}

int CGPoint::GetCargoType()
{
	return cargoType;
}

CGPoint* CGPoint::GetNextGPoint()
{
	return next;
}
