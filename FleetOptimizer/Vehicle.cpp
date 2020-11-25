// Vehicle.cpp: implementation of the CVehicle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "Vehicle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVehicle::CVehicle() : next(0)
{
	SetExistZero();
}

CVehicle::~CVehicle()
{

}



CVehicle * CVehicle::GetNextVehicle()
{
    return next;
}

int CVehicle::IsExist()
{
	return Exist;
}

int CVehicle::IsExistOperation()
{
	return ExistOperation;
}
int CVehicle::IsExistBuild()
{
	return ExistBuild;
}
int CVehicle::IsExistDesign()
{
	return ExistDesign;
}

int CVehicle::GetExistNum()
{
	return ExistNum;
}

int CVehicle::GetExistAdvantPrice()
{
	if(ExistAdvantPrice == 0)
		return -1;
	return ExistAdvantPrice;
}

int CVehicle::GetExistPrice()
{
	return ExistPrice;
}

int CVehicle::GetExistBuildNum()
{
	return ExistBuildNum;
}

int CVehicle::GetExistBuildAdvantPrice()
{
	return ExistBuildAdvantPrice;
}

int CVehicle::GetExistBuildPrice()
{
	return ExistBuildPrice;
}

int CVehicle::GetExistDesignAdvantPrice()
{
	return ExistDesignAdvantPrice;
}

int CVehicle::GetExistDesignPrice()
{
	return ExistDesignPrice;
}

void CVehicle::SetExistZero()
{
	Exist = 0;
	ExistForce = 0;
	ExistOperation = 0;
	ExistBuild = 0;
	ExistDesign = 0;
	ExistNum = 0;
	ExistAdvantPrice = 0;
	ExistPrice = 0;
	ExistBuildNum = 0;
	ExistBuildAdvantPrice = 0;
	ExistBuildPrice = 0;
	ExistDesignAdvantPrice = 0;
	ExistDesignPrice = 0;
	ExistNumRemain = 0;
	ExistBuildNumRemain = 0;

}

int CVehicle::GetExistGenAdvantPrice()
{
	int AdvPrice;

	if (Exist == 0)
		return 0;

	if (ExistDesign == 1)
		AdvPrice = ExistDesignAdvantPrice;
	else if ((ExistOperation == 1) && (ExistBuild == 1))
		AdvPrice = ((ExistNum * ExistAdvantPrice) + (ExistBuildNum * ExistBuildAdvantPrice))/(ExistNum + ExistBuildNum);
	else if ((ExistOperation == 1) && (ExistBuild == 0)){
			AdvPrice = ExistAdvantPrice;
			if(AdvPrice == 0)
				AdvPrice = -1;
	}
	else if	((ExistOperation == 0) && (ExistBuild == 1))
		AdvPrice = ExistBuildAdvantPrice;
	
	return AdvPrice;

}	

int	CVehicle::GetExistNumRemain(){
	return ExistNumRemain;
}

int	CVehicle::GetExistBuildNumRemain(){
	return ExistBuildNumRemain;
}


int CVehicle::UpdateExistNumRemainIncrem(int  num){

	if (Exist == 0)
		return 0;
	
	int delta;

	if ((ExistOperation == 1) && (ExistBuild == 1)){
		if ((ExistBuildNum == ExistBuildNumRemain) && (ExistNum > ExistNumRemain)){
			delta =  ExistNum - ExistNumRemain;	
			if (delta >= num)
				ExistNumRemain += num; // В первую очередь возвращаем  суда в постройкею
			else 
				ExistNumRemain = ExistNum;	
		}
		else if ((ExistBuildNum > ExistBuildNumRemain) && (ExistNum == ExistNumRemain)){
			delta =  ExistBuildNum - ExistBuildNumRemain;	
			if (delta >= num)
				ExistBuildNumRemain += num; // В первую очередь возвращаем  суда в постройкею
			else 
				ExistBuildNumRemain = ExistNum;	
		}
		else if  ((ExistBuildNum > ExistBuildNumRemain) && (ExistNum > ExistNumRemain)){
			// В первую очередь возвращаем  суда в постройке.
			delta =  ExistBuildNum - ExistBuildNumRemain;
			if (delta >= num)
				ExistBuildNumRemain += num; 
			else {
				ExistBuildNumRemain = ExistBuildNum;
				ExistBuildNum += (num - delta);
			}
		}
		else {}
	}
	else if ((ExistOperation == 1) && (ExistBuild == 0)){
		delta =  ExistNum - ExistNumRemain;	
		if (delta >= num)
			ExistNumRemain += num;
		else 
			ExistNumRemain = ExistNum;	
	}
	else if ((ExistOperation == 0) && (ExistBuild == 1)){
		delta =  ExistBuildNum - ExistBuildNumRemain;	
		if (delta >= num)
			ExistBuildNumRemain += num;
		else 
			ExistBuildNumRemain = ExistNum;	
	}
	return 1;

}



int CVehicle::UpdateExistNumRemainDecrem(int  num){

	if (Exist == 0)
		return 0;
	
//	int delta;

	if ((ExistOperation == 1) && (ExistBuild == 1)){
		if ((ExistBuildNum == ExistBuildNumRemain) && (ExistNum > ExistNumRemain)){
			if (ExistNumRemain >= num)
				ExistNumRemain -= num; // В первую очередь возвращаем  суда в постройкею
			else 
				ExistNumRemain = 0;	
		}
		else if ((ExistBuildNum > ExistBuildNumRemain) && (ExistNum == ExistNumRemain)){
			if (ExistBuildNumRemain >= num)
				ExistBuildNumRemain -= num; // В первую очередь возвращаем  суда в постройкею
			else 
				ExistBuildNumRemain = 0;	
		}
		else if  ((ExistBuildNum > ExistBuildNumRemain) && (ExistNum > ExistNumRemain)){
			// В первую очередь используем  суда в эксплуатации.
			if (ExistNumRemain >= num)
				ExistNumRemain -= num; 
			else {
				ExistBuildNumRemain -= (num - ExistNumRemain);
				ExistNumRemain = 0;
			}
		}
	}
	else if ((ExistOperation == 1) && (ExistBuild == 0)){
		if (ExistNumRemain >= num)
			ExistNumRemain -= num;
		else 
			ExistNumRemain = 0;	
	}
	else if ((ExistOperation == 0) && (ExistBuild == 1)){
		if (ExistBuildNumRemain >= num)
			ExistBuildNumRemain -= num;
		else 
			ExistBuildNumRemain = 0;	
	}

	return 1;

}


int CVehicle::IsExistForce()
{
	return ExistForce;
}

int CVehicle::GetTariffForProtoScaleGenVF()
{
	return TariffGenVF;
}

int CVehicle::GetTariffForProtoScaleOil()
{
	return TariffOil;
}

int CVehicle::GetTariffForProtoScaleCoal()
{
	return TariffCoal;
}

void CVehicle::UpdateExistNumRemainReset()
{
	ExistNumRemain = ExistNum;
}
