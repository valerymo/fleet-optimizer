// Port.h: interface for the CPort class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_PORT_H__E0E36FBF_ADB8_4773_A667_F24B2F86EA6B__INCLUDED_)
#define AFX_PORT_H__E0E36FBF_ADB8_4773_A667_F24B2F86EA6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Cargo.h"
#include "Vehicle.h"
#define MAXPNBNUMBER 30

class CPort : public CObject
   //Basovi punkt
{
friend class CPortFactory;  
friend class CPortList;
friend class COptimizationSt2LpDp; //::SimplexMethod();
friend class CGPoint;
friend class COptimizationSt3Dp; //::UpdatePNBsCargoRequirments(CRouterP *);
friend class CMarineTechEconomicEng;
public:
	CPort * GetNextPort();
	char * GetPortName();
	CPort();
	virtual ~CPort();
/*    typedef struct cargoes {
        int oil; //neft
        int coal; //ugol
        int general; //generalni
        int vegetables_fruits; //ovoschi i frukti  
    }; */
	DECLARE_SERIAL (CPort);
private:
	int id;
    char name[20];
    CCargo available_cargoes,required_cargoes;

    int DistanceMatrix[MAXPNBNUMBER][MAXPNBNUMBER];
	int ship_loading_productivity[5];
	int acv_loading_productivity[5];

/*	typedef struct Loading_Productivity{
        int coal_loading; 
        int general_loading; 
        int oil_loading; 
        int vegetables_fruits_loading;
	};
*/
	struct structLandStructDataPP {
		BOOL bACVCourtAvailable;
		BOOL bShipCourtAvailable;
		BOOL bACVCourtUse;
		BOOL bShipCourtUse;
		int nShipCourtInvestRequere;
		int nACVCourtInvestRequere;
	}; 

    struct PNB_data {
        char name[20];
        int id;
        struct {
            int coal; //ugol
            int general; //generalni
            int oil; //neft
            int vegetables_fruits; //ovoschi i frukti  
            }required_cargoes,required_cargoes_0;

		//Loading_Productivity acv_loading_productivity;
		//Loading_Productivity ship_loading_productivity;
		int acv_loading_productivity[5];
		int ship_loading_productivity[5];

		structLandStructDataPP LandStructDataPP, LandStructDataPPVirt, LandStructDataPPTmp;

    };

    PNB_data PNBList[MAXPNBNUMBER];

	struct LandStructDataBase {
		int nACVSpace;
		int nACVSpaceElemPrice;
		int nMoorLength;
		int nMoorElemPrice;
		struct {
			int nCapability;
			int nVhehicleLoad;
			int a;	
			int b;
					//a,b - coefficients for: C = a*n + b
					//a - expencies dependent to number of vehicles;
					//b - expencies independent to number of vehicles.\n
					//a,b - for vehicles with load: ACV - 50 t, Ship - 300 t; 
					//it will be recalculated for rated ship.
		}RepairDetailsShip, RepairDetailsACV;

	};
	LandStructDataBase m_LandStructData, m_LandStructDataVirt, m_LandStructDataTmp; //действительные и динамические расчетные данные



private:
	void CleanLandDataUpdateStructure();
	int SetPNBRequiredCargoRemain0(int i, int cargo_type, int cargo);
	void RestoreCargoRequerements(void);
	int SetPNBRequiredCargoRemain(int PNB_num, int cargo_type, int cargo);
	char Region[10];
    CPort* next;
	int LinesNumber; //количество линий в районе
public:
	float GetPortShipLoadingTime(CVehicle* pVehicle, int cargo_type);
	float GetPortShipLoadingTime(CVehicle* pVehicle, int cargo_type, int load);
	int GetPortID();
	int GetPNBShipLoadingProductivity(int PNB_num, CVehicle *pVehicle, int cargo_type);
	float GetPNBShipLoadingTime (int PNB_num, CVehicle* pVehicle, int cargo_type);
	float GetPNBShipLoadingTime (int PNB_num, CVehicle* pVehicle, int cargo_type, int load);
//	virtual void Serialize(CArchive& ar);
	int is_CargoRequired();
	int GetPNBRequiredCargoFirst(int i, int cargo_type);
	int GetLinesNumber(); //количество линий в районе
	int GetDistancePNBPNB(int, int);
	char* GetPortRegion();
	int GetDistancePortPNB(int);
	int GetPNBRequiredCargo (int PNB_num, int cargo_type);
	void GetAvailableCargoesAll (CCargo *);
    int GetAvailableCargo(int cargo_type);
    PNB_data CPort::GetPNB(int);

};



#endif // !defined(AFX_PORT_H__E0E36FBF_ADB8_4773_A667_F24B2F86EA6B__INCLUDED_)

