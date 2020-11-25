// Ship.h: interface for the CShip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIP_H__9FBBE7E9_5434_4FB3_AFDD_83981311CC50__INCLUDED_)
#define AFX_SHIP_H__9FBBE7E9_5434_4FB3_AFDD_83981311CC50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Vehicle.h"

class CShip : public CVehicle  
{
  friend class CShipFactory;  
public:
	float GetH3();
	int GetRange();
	char* GetTypeName();
	char* GetPrototypeName();
	char* GetRegisterClass();
	int GetLoad();
	int GetCoast();
	int GetSpeed();
	int GetCargoCompatibility(int cargo_type);

	//CShip();
	CShip(char *, char *);
	virtual ~CShip();

private:
   	//char TypeName[20];
	//char RegisterClass[10];
	//int Load;
	//int Coast;
	//int Speed;
	//int CargoCompatibility[4];
	

};

#endif // !defined(AFX_SHIP_H__9FBBE7E9_5434_4FB3_AFDD_83981311CC50__INCLUDED_)
