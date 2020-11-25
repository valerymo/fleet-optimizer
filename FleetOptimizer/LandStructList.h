// LandStructList.h: interface for the CLandStructList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANDSTRUCTLIST_H__331EFC97_A0D7_4B3A_AB75_214D39B2E4B3__INCLUDED_)
#define AFX_LANDSTRUCTLIST_H__331EFC97_A0D7_4B3A_AB75_214D39B2E4B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LandStructure.h"

class CLandStructList  
{
public:
	void append(CLandStructure*);
	CLandStructList();
	virtual ~CLandStructList();

private:
	CLandStructure* at_end;
	CLandStructure* m_pLandStructList;
};

#endif // !defined(AFX_LANDSTRUCTLIST_H__331EFC97_A0D7_4B3A_AB75_214D39B2E4B3__INCLUDED_)
