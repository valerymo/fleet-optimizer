// GPoint.h: interface for the CGPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPOINT_H__B16C2F2D_81B5_42BB_B929_B176A66BA4D6__INCLUDED_)
#define AFX_GPOINT_H__B16C2F2D_81B5_42BB_B929_B176A66BA4D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Port.h"

class CGPoint  
{
public:
	CGPoint* GetNextGPoint();
	int GetCargoType();
	int GetPNBnum();
	CGPoint(int PNBnum, int cargo);
	virtual ~CGPoint();

private:
	CGPoint *next;
    CCargo::CargoType cargoType;
	int PNB_num;
	//CPort::PNB_data PNB;
};

#endif // !defined(AFX_GPOINT_H__B16C2F2D_81B5_42BB_B929_B176A66BA4D6__INCLUDED_)
