// ShipDesign.h: interface for the CShipDesign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHIPDESIGN_H__6AFC2F6F_C899_4611_97BA_2E42C5C28E41__INCLUDED_)
#define AFX_SHIPDESIGN_H__6AFC2F6F_C899_4611_97BA_2E42C5C28E41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _ShipMainDetails{

	CString name;
	float L;
	float B;
	float T;
	float H;
	int D;
	int DW;
	int Pgr;
	
	float delta;

	int N;
	float speed; 
	float mu;

} ShipMainDetails;

//// Прототипы /////////////////////////
typedef struct _ShipPrototypeMainDetails{

	CString name;	
	CString projectNumber;	
	CString registerClass;	
	float L;
	float Lpp;
	float B;	
	float H;	
	float T;	
	int D;	
	int DW;	
	int Pgr;
	float speed;	
	float speed_bal;
	int dist;
	int Wkip;
	int Wnas;
	int luk_num;
	CString load_stub1;	
	int stab1;
	CString load_stub2;	
	int stab2;
	int priceBuild;
	float priceK1;
	float priceK2;
	int exp1;
	int exp2;
	int exp3;
	int exp4;
	int exp5;
	float priceK3;
	int p01;
} ShipPrototypeMainDetails;

/////////////////////


class AFX_EXT_CLASS CShipDesign : public CObject  
{
friend class CShipFactory;  
public:
	int CheckShipCargoCapacity (ShipPrototypeMainDetails *prototype, ShipMainDetails *project);
	int GetShipMainDetails(ShipPrototypeMainDetails* prototype_details, ShipMainDetails* project_details);
	int GetExistShipMainDetails(ShipPrototypeMainDetails* prototype_details, ShipMainDetails* project_details);
	CShipDesign();
	virtual ~CShipDesign();
    static CShipDesign* Instance();

private:
	int GetComputMethod_delta(ShipPrototypeMainDetails *prototype);
	int GetComputMethod_l(ShipPrototypeMainDetails *prototype);
	int ComputeShipMainDetailsSpeedVar(ShipPrototypeMainDetails *prototype, ShipMainDetails *project);
	int ComputeShipMainDetailsSpeedConst(ShipPrototypeMainDetails *prototype, ShipMainDetails *project);
	void ResetPrivateData(void);
	int SetShipMainDetails(ShipMainDetails*);
	float m_L;
	float m_B;
	float m_T;
	float m_H;
	float m_LB;
	float m_BT;
	float m_HT;
	float m_LH;
	float m_D;
	float m_V; 
	int m_DW;
	int m_Pgr;
	//**********

	float m_g; 
	
	float m_delta; 
	float m_ro; 
	float m_k;


	int m_N;
	float m_Fr;
	float m_speed; 


	static CShipDesign* _instance;

};



#endif // !defined(AFX_SHIPDESIGN_H__6AFC2F6F_C899_4611_97BA_2E42C5C28E41__INCLUDED_)
