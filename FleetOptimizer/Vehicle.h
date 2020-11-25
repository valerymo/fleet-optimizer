// Vehicle.h: interface for the CVehicle class.
//
//////////////////////////////////////////////////////////////////////
#include "Cargo.h"

#if !defined(AFX_VEHICLE_H__FC9446B5_E917_4856_A0FF_DC8154A4F7C4__INCLUDED_)
#define AFX_VEHICLE_H__FC9446B5_E917_4856_A0FF_DC8154A4F7C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVehicle //: public CObject  
{
friend class CShipFactory;
friend class CVehiclesList; 
friend class CInitialBasePlan; 
public:
	int GetTariffForProtoScaleCoal();
	int GetTariffForProtoScaleOil();
	int GetTariffForProtoScaleGenVF();
	int GetExistGenAdvantPrice(void);
	virtual int GetCargoCompatibility(int cargo_type)= 0;
	CVehicle * GetNextVehicle();
	virtual char* GetTypeName()= 0;
	virtual char* GetPrototypeName()= 0;
	virtual char* GetRegisterClass()= 0;
	virtual int GetLoad()= 0;
	virtual int GetCoast()= 0;
	virtual int GetSpeed() = 0;
	virtual int GetRange()= 0;
	virtual float GetH3()= 0;

	CVehicle();
	virtual ~CVehicle();

// existance ship functions:  begin
	int IsExist(void);
	int IsExistForce(void);
	int IsExistOperation(void);
	int IsExistBuild(void);
	int IsExistDesign(void);
	int	GetExistNum(void);
	int	GetExistNumRemain(void);
	int	GetExistAdvantPrice(void);
	int	GetExistPrice(void);
	int	GetExistBuildNum(void);
	int	GetExistBuildNumRemain(void);
	int	GetExistBuildAdvantPrice(void);
	int	GetExistBuildPrice(void);
	int	GetExistDesignAdvantPrice(void);
	int	GetExistDesignPrice(void);
	int	UpdateExistNumRemainIncrem(int);
	int	UpdateExistNumRemainDecrem(int);
	void UpdateExistNumRemainReset(void);


// existance ship functions:  end

private:
	void SetExistZero(void);
    CVehicle *next;
protected:
    char TypeName[20];
    char PrototypeName[20];
    char RegisterClass[10];
	int Load;
	int Coast;
	int Speed;
	int CargoCompatibility[5];
	int test;

	int Range; //added 07.10.03
	float h3;	//added 07.10.03


	// existing ship details begin
	int	Exist;
	int	ExistForce;
	int	ExistOperation;
	int	ExistBuild;
	int	ExistDesign;
	int	ExistNum;
	int ExistNumRemain;
	int	ExistAdvantPrice;
	int	ExistPrice;
	int	ExistBuildNum;
	int	ExistBuildNumRemain;
	int	ExistBuildAdvantPrice;
	int	ExistBuildPrice;
	int	ExistDesignAdvantPrice;
	int	ExistDesignPrice;
	// existing ship details end

	//tariffs for scale of prototype
	int TariffGenVF;
	int TariffOil;
	int TariffCoal;

};

#endif // !defined(AFX_VEHICLE_H__FC9446B5_E917_4856_A0FF_DC8154A4F7C4__INCLUDED_)
