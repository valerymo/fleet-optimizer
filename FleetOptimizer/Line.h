// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////
//#include "Router.h"

#if !defined(AFX_LINE_H__8CF043B1_23BA_4A24_A207_8DAFB5049B4A__INCLUDED_)
#define AFX_LINE_H__8CF043B1_23BA_4A24_A207_8DAFB5049B4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLine
{
friend class CRouter; 
friend class CRouterS; 
friend class CRouterL; 
friend class CRouterP; 
friend class COptimizationSt2LpDp;
public:
	int GetCount();
	void SetCount(int);
	CLine* GetNextLine();
	int GetPNBnum();
	int GetCargoType();
    CLine(int, CCargo::CargoType cargoType);
	CLine::CLine(int PNBnum, int cargo);
	virtual ~CLine();

private: 
	int ReplacePNB(int PNB_num);
    CLine *next;
    int PNB_num;
    int count;
    CCargo::CargoType cargoType;
    //void SetLine(int, CCargo::CargoType cargoType);



};

#endif // !defined(AFX_LINE_H__8CF043B1_23BA_4A24_A207_8DAFB5049B4A__INCLUDED_)
