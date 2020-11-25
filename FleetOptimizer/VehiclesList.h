// VehiclesList.h: interface for the CVehiclesList class.
//
//////////////////////////////////////////////////////////////////////
#include "Vehicle.h"
#include "Ship.h"

#if !defined(AFX_VEHICLESLIST_H__917000F4_FF8A_4BDA_9EC8_B97B0F21649B__INCLUDED_)
#define AFX_VEHICLESLIST_H__917000F4_FF8A_4BDA_9EC8_B97B0F21649B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVehiclesList  
{
friend class CShipFactory;  
public:
	CVehicle * GetVehicleListHead();
	void print(void);
	static CVehiclesList* Instance();
    void append (CVehicle*);
protected:    
    CVehiclesList();
	virtual ~CVehiclesList();
private:
    static CVehiclesList * _instance;
    CVehicle *VehiclesList;
   	CVehicle *at_end;


};

#endif // !defined(AFX_VEHICLESLIST_H__917000F4_FF8A_4BDA_9EC8_B97B0F21649B__INCLUDED_)
