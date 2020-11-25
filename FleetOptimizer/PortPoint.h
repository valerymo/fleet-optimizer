// PortPoint.h: interface for the CPortPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORTPOINT_H__D3F394FE_854A_4A30_8839_8C7CE22A60A2__INCLUDED_)
#define AFX_PORTPOINT_H__D3F394FE_854A_4A30_8839_8C7CE22A60A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPortPoint  
{
friend class CRouterP; 
public:
	int GetCargoLoadInShipPlan(void);
	void SetCargoLoadInShipPlan(int cargoLoad);
	CPortPoint(int PNBnum, int cargo);
	virtual ~CPortPoint();

	CPortPoint* GetNextPPoint();
	int GetCargoType();
	int GetPNBnum();

private:
	CPortPoint *next;
    CCargo::CargoType cargoType;
	int cargoLoadInShipPlan;
	int PNB_num;
};

#endif // !defined(AFX_PORTPOINT_H__D3F394FE_854A_4A30_8839_8C7CE22A60A2__INCLUDED_)
