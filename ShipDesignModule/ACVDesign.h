// ACVDesign.h: interface for the CACVDesign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACVDESIGN_H__A6D10F9A_7485_4D47_9E30_84C5BE3C94F6__INCLUDED_)
#define AFX_ACVDESIGN_H__A6D10F9A_7485_4D47_9E30_84C5BE3C94F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _ACVMainDetails{

	CString name;	
	float Lp;		
	float Bp;
	float Sp;
	float D;		
	float Dtp;	
	int Dgr;	
	float speed;
	float LpBp;	
	float Hgo;
	
	int N; 
	float cKn;
	float cPtp;
	float cND;	
	float cDgrD;
				
	int r;		
	float h3;	

} ACVMainDetails;


typedef struct _ACVPrototypeMainDetails {

	float speed;	
	float a1;		
	float a2;		
					
	float h_3;		

	CString name;	
	CString projectNumber;	
	CString registerClass;	
	float L;				
	float B;				
	float D;				
	float Lp;				
	float Bp;				
	float LpBp;			
	float Sp;			
	float Hgo;			
	float HnlHgo;		
	float lp;			
	float Perp;			
	float Pp;			
	int Qn;				
	int Q;				
	float HgoBp;		
	float Kp;			
	int N;				
	CString GD;			
	CString NmaxNnom;	
	CString Dviz;		
	float Ddviz;		
	int n_dviz;			
	CString num_type_nagn;
	float Dnagn;		
	int n_nagn;			
	float speed_lim;	
	float FrD;			
	int Dgr;			
	CString pas_car;	
	float Dtp;			
	float cND;			
	float cKn;			
	float cQp;			
	float cQps;			
	float cQph;			
	float hgo_d;		
	float te;			
	float teLp;			
	float teSp;			
	float Gs;			
	float Gl;			
	float timep;		
	float Tz;			
	float timepTz;		
	float Dpor;			
	float cDgrD;		
	int r;				
	int priceBuild;		
	float priceK1;		
	float priceK2;		
	float priceK21;		
	int exp1;			
	int exp2;			
	int exp3;			
	int exp4;			
	int exp5;			
	float priceK3;		

}ACVPrototypeMainDetails;


class AFX_EXT_CLASS CACVDesign : public CObject  
{
friend class CShipFactory;  
public:
	int GetErrorStatus();
	CString GetLogFNameError();
	int GetACVMainDetails(ACVPrototypeMainDetails *prototype, ACVMainDetails *project);
	int GetExistACVMainDetails(ACVPrototypeMainDetails *prototype, ACVMainDetails *project);
	CACVDesign();
	virtual ~CACVDesign();
    static CACVDesign* Instance();

private:
	int printCurrentACVDetails(ACVPrototypeMainDetails *prototype, ACVMainDetails *project);
	int m_bPrintIsOn;
	int ComputeACVPropulsionSystem(ACVPrototypeMainDetails *prototype, ACVMainDetails *project);
	int ComputeACVLiftSystem(ACVPrototypeMainDetails *prototype, ACVMainDetails *project);
	int ComputeACVMainDetailsSpeedVar(ACVPrototypeMainDetails *prototype, ACVMainDetails *project);
	int ComputeACVMainDetailsSpeedConst(ACVPrototypeMainDetails *prototype, ACVMainDetails *project);
	int SetACVMainDetails(ACVMainDetails * proj);
	void ResetPrivateData();

	int m_nErrorStatus;
	CString m_sLogFNameError;
	CString m_sLogFileVehiclesDetails;

	
	float m_Lp;
	float m_Bp;
	float m_Sp;
	//float m_T;
	float m_Hgo;
	float m_LpBp;
	//float m_BT;
	//float m_LH;
	float m_D;
	int m_Dgr;
	float m_Dtp;
	float m_a1; 
	float m_a2; 
	            
	int m_N;	
	float m_cPtp;
	float m_cND; 
	float m_h_3; 
	int m_r;	
	float m_ro;	
	float m_ro_vozd; 
	
	//=============
	float m_Pp; 
	float m_Qp; 
	float m_Qs; 
	float m_Tz; 
	float m_Tau;

	int m_Npk;  
	int m_Ndv;  
	
	int m_nagn_type; 
	int m_nagn_num; 
	float m_nagn_d; 

	float m_dvv;    
	


	static CACVDesign* _instance;
				
};

#endif // !defined(AFX_ACVDESIGN_H__A6D10F9A_7485_4D47_9E30_84C5BE3C94F6__INCLUDED_)
