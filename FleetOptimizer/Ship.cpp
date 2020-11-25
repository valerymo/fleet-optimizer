// Ship.cpp: implementation of the CShip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "Ship.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
CShip::CShip(){

}
*/

CShip::CShip(char *name, char *RegClass)
{
    strcpy(TypeName, name);
    strcpy(RegisterClass, RegClass);

	//FILE *stream;   //for debug
    //stream = fopen( "E:\\Users\\mv\\Log\\CShip_const.txt", "w" ); // for debug
    //fprintf (stream,"name: %s, RegClass: %s\n",name,RegClass);
    //fclose(stream);

}

CShip::~CShip()
{
    //delete TypeName;
	//delete RegisterClass;
}

int CShip::GetSpeed()
{
    return Speed;
}


int CShip::GetCoast()
{
    return Coast;
}


int CShip::GetLoad()
{
    return Load;
}


char* CShip::GetTypeName()
{
    return TypeName;
}

char* CShip::GetPrototypeName()
{
    return PrototypeName;
}

char* CShip::GetRegisterClass()
{
	return RegisterClass;
}

int CShip::GetCargoCompatibility(int cargoType){
	return CargoCompatibility[cargoType];
}

int CShip::GetRange()
{
	return Range;
}

float CShip::GetH3()
{
	return h3;
}
