// Cargo.cpp: implementation of the CCargo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "Cargo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCargo::CCargo()
{
    coal = 0; //ugol
    general = 0; //generalni
    oil = 0; //neft
    vegetables_fruits = 0; //овощи, фрукты
}


CCargo::~CCargo()
{

}


