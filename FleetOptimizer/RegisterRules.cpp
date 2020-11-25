// RegisterRules.cpp: implementation of the CRegisterRules class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "RegisterRules.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CRegisterRules* CRegisterRules::_instance = 0;

CRegisterRules* CRegisterRules::Instance(){
    if (_instance == 0){
        _instance = new CRegisterRules;
    }
    return _instance;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegisterRules::CRegisterRules()
{

}

CRegisterRules::~CRegisterRules()
{

}
