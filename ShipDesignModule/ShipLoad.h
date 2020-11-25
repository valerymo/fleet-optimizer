// ShipLoad.h: interface for the CShipLoad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIPLOAD_H__AB52A098_9336_40E7_B3E0_633540A1603E__INCLUDED_)
#define AFX_SHIPLOAD_H__AB52A098_9336_40E7_B3E0_633540A1603E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShipLoad : public CObject  
{
public:
	CShipLoad();
	virtual ~CShipLoad();

};

typedef struct _ShipLoad {
	int DW;
	int Dpoln;
	int Dpor;
	int P01;
	int P02;
	int P03;
	int P04;
	int P05;
	int P06;
	int P07;
	int P08;
	int P09;
	int P010;
	int P011;
	int P012;
	int P013;
	int P014;
	int P015;
	int P016;
	int P017;
	int P018;
} ShipLoad;


#endif // !defined(AFX_SHIPLOAD_H__AB52A098_9336_40E7_B3E0_633540A1603E__INCLUDED_)


