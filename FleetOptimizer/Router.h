// Router.h: interface for the CRouter class.
//
//////////////////////////////////////////////////////////////////////
#include "Vehicle.h"
#include "Port.h"
#include "Line.h"
#include "OptimizationSt1LP.h"


#if !defined(AFX_ROUTER_H__E5DBC36A_B9C2_43AE_B547_CB9DFEE383E7__INCLUDED_)
#define AFX_ROUTER_H__E5DBC36A_B9C2_43AE_B547_CB9DFEE383E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRouter  
{
friend class CInitialBasePlan;
friend class CBasePlanSt1LP;
friend class CBasePlanSt2LpDp;
friend class CBasePlanSt3Dp;
friend class COptimizationSt1LP;
friend class COptimizationSt2LpDp;
friend class COptimizationSt3Dp;
friend class CMarineTechEconomicEng;
public:
	int GetRouterTime();
	//virtual void appendLine(CLine *pLine) = 0;
	virtual void appendLine(CLine *pLine);
	CRouter* GetNextRouter();
	//int GetVehicleCargoForNavigation(void);
	int GetVehicleNumber();		// get целое количество судов
	double GetDVehicleNumber(); // get нецелое количество судов
	//virtual CLine * GetLine() = 0;
	virtual CLine * GetLine();
	CPort* GetBasePort() const;
	int GetCargoType();
	int GetVehicleIncome();
	virtual void GetRouter();
	CVehicle* GetVehicle() const;
	CRouter();
	virtual ~CRouter();

protected:
    CPort *basePort;    //Базовый пункт маршрута
    CCargo::CargoType cargoType;      //Тип груза на маршруте
    CRouter *next;

  	void SetVehicle(CVehicle *);
	void SetCargoType(int cargo_type);

    CVehicle *vehicle;  //указатель на судно(транспортное средство) маршрута
    int vehicleNumber;  //количество судов на маршруте целое
    double dVehicleNumber;  //количество судов на маршруте нецелое
    int vehicleIncome;  //Чистый доход одного судна на маршруте за год
	int vehicleOperationsDaysRemain;

private:
	CLine* Line;
	void SetVehicleOperationsDaysRemain(int days_remain);
	int GetVehicleOperationsDaysRemain();
    /*CVehicle *vehicle;  //указатель на судно(транспортное средство) маршрута
    int vehicleNumber;  //количество судов на маршруте целое
    double dVehicleNumber;  //количество судов на маршруте нецелое
    int vehicleIncome;  //Чистый доход одного судна на маршруте за год
	int vehicleOperationsDaysRemain;
	*/
	/////////int vehicleCargoForNavigation;  //провозоспособность судна на маршруте.
									//(количество перевезенного судном груза за навигацию)

  	//void SetVehicle(CVehicle *);
	//void SetCargoType(int cargo_type);


};

#endif // !defined(AFX_ROUTER_H__E5DBC36A_B9C2_43AE_B547_CB9DFEE383E7__INCLUDED_)
