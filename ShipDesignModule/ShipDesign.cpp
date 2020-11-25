// ShipDesign.cpp: implementation of the CShipDesign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShipDesign.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CShipDesign* CShipDesign::_instance = 0;

CShipDesign* CShipDesign::Instance(){
    if (_instance == 0){
        _instance = new CShipDesign;
    }
    return _instance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShipDesign::CShipDesign()
{
	m_ro = (float)1.025;	// плотность воды
	m_k = (float)1.01;		// коэффициент выступающих частей
}

CShipDesign::~CShipDesign()
{

}

int CShipDesign::GetShipMainDetails(ShipPrototypeMainDetails *prototype, ShipMainDetails *project)
{
	ResetPrivateData();

	if (prototype->speed == project->speed){
		ComputeShipMainDetailsSpeedConst(prototype, project);
	}else{
		ComputeShipMainDetailsSpeedVar(prototype, project);
	}
	
	SetShipMainDetails(project);

	return 1;
}

int CShipDesign::SetShipMainDetails(ShipMainDetails * proj)
{
	proj->L = m_L;
	proj->B = m_B;
	proj->T = m_T;
	proj->H = m_H;
	proj->D = (int)m_D;
	proj->delta = m_delta;
	
	return 1;
}


void CShipDesign::ResetPrivateData()
{
	m_L = 0;
	m_B = 0;
	m_T = 0;
	m_H = 0;
	m_LB = 0;
	m_BT = 0;
	m_HT = 0;
	m_LH = 0;
	m_D = 0;
	m_V = 0;
	m_DW = 0;
	m_Pgr = 0;

	float m_delta = 0; 
	int m_N  = 0;
	float m_Fr = 0;
	float m_speed = 0;
}

int CShipDesign::ComputeShipMainDetailsSpeedConst(ShipPrototypeMainDetails *prototype, ShipMainDetails *project)
{
	int V_prot;
	float l_prot;
	float cPgrD_prot;

	V_prot = (int)(prototype->D/m_ro);
	l_prot = (float)(prototype->L/pow(V_prot, 0.33f));
	cPgrD_prot = (float)prototype->Pgr/prototype->D;
	
	m_LB = (float)prototype->L/prototype->B;
	m_BT = (float)prototype->B/prototype->T;
	m_HT = (float)prototype->H/prototype->T;

	m_Pgr = project->Pgr;
	m_D = m_Pgr/cPgrD_prot;
	m_V = m_D/m_ro;
	m_L = (float)(l_prot*pow(m_V, 0.33f));
	m_B = m_L/m_LB;
	m_T = m_B/m_BT;
	m_H = m_T * m_HT;

	int resCheckCapacity = 1;
	resCheckCapacity = CheckShipCargoCapacity(prototype, project);

	
	return 1;
}

int CShipDesign::ComputeShipMainDetailsSpeedVar(ShipPrototypeMainDetails *prototype, ShipMainDetails *project)
{

	float P04_proto, P04_proto_changed, Pgr_proto_changed;
	float cPgrD_prot_changed;

	P04_proto = float (0.05*prototype->D);
	P04_proto_changed = float(P04_proto*pow(project->speed,3.0f)/pow(prototype->speed,3.0f));

	Pgr_proto_changed = prototype->Pgr - (P04_proto_changed - P04_proto);
	cPgrD_prot_changed = Pgr_proto_changed/prototype->D;

	float l_proj;
	int l_method;
	l_method = GetComputMethod_l(prototype);
	
	switch(l_method){
	case 1:
		l_proj = (float)(4.47 + 0.06*project->speed);
		break;
	case 2:
		l_proj = (float)(1.36*sqrt(project->speed));
		break;
	}

	m_Pgr = project->Pgr;
	m_D = m_Pgr/cPgrD_prot_changed;
	m_V = m_D/m_ro;
	m_L = (float)(l_proj*pow(m_V, 0.33f));
	m_BT = (float)prototype->B/prototype->T;
	m_HT = (float)prototype->H/prototype->T;

	m_Fr = (float)(project->speed/sqrt(9.81*m_L));
	int delta_method = GetComputMethod_delta(prototype);
	switch(delta_method){
	case 1:
		m_delta = float(0.91 - 1.1*m_Fr);
		break;
	case 2:
		m_delta = float(0.395/pow(m_Fr,0.33f));
		break;
	case 3:
		m_delta = float(0.99 - 1.2*m_Fr);
		break;
	case 4:
		m_delta = float(0.445/pow(m_Fr,0.33f));
		break;
	}

	m_B = (float)sqrt(m_V*m_BT/(m_delta*m_L));	
	m_T = m_D/(m_ro*m_k*m_delta*m_L*m_B);
	m_H = m_T * m_HT;

	int resCheckCapacity = 1;
	resCheckCapacity = CheckShipCargoCapacity(prototype, project);
	
	return 1;
}

int CShipDesign::GetComputMethod_l(ShipPrototypeMainDetails *prototype)
{
	int l_method;  
	float V_prot, l_tmp, diff_l, diff_l_tmp, l_prot;

	V_prot = prototype->D/m_ro;
	l_prot = (float)(prototype->L/pow(V_prot, 0.33f));

	l_method = 1;
	l_tmp = (float)(4.47 + 0.06*prototype->speed);
	diff_l = (float)fabs(l_prot - l_tmp);
	
	l_tmp = (float)(1.36*sqrt(prototype->speed));
	diff_l_tmp = (float)fabs(l_prot - l_tmp);
	if (diff_l_tmp < diff_l){
		diff_l = diff_l_tmp;
		l_method = 2;
	}

	return l_method;
}

int CShipDesign::GetComputMethod_delta(ShipPrototypeMainDetails *prototype)
{
	float Fr, delta_tmp, diff, diff_tmp, delta, L, B, T;
	int method;

	L = prototype->L;
	B = prototype->B;
	T = prototype->T;

	Fr = float(prototype->speed/sqrt(9.81*L)); 
	delta = prototype->D/(m_ro*L*B*T);

	method = 1;  
	delta_tmp = float(0.91 - 1.1*Fr);
	diff = (float)fabs(delta - delta_tmp);

	delta_tmp = float(0.395/pow(Fr,0.33f));
	diff_tmp = (float)fabs(delta - delta_tmp);
	if (diff_tmp < diff){
		diff = diff_tmp;
		method = 2; 
	}

	delta_tmp = float(0.99 - 1.2*Fr);
	diff_tmp = (float)fabs(delta - delta_tmp);
	if (diff_tmp < diff){
		diff = diff_tmp;
		method = 3; 
	}


	delta_tmp = float(0.445/pow(Fr,0.33f));
	diff_tmp = (float)fabs(delta - delta_tmp);
	if (diff_tmp < diff){
		diff = diff_tmp;
		method = 4; 
	}

	return method;
}

int CShipDesign::GetExistShipMainDetails(ShipPrototypeMainDetails *prototype, ShipMainDetails *project)
{
	project->L = prototype->L;
	project->B = prototype->B;
	project->T = prototype->T;
	project->D = prototype->D;
	//proj->delta = ; ???
	project->speed = prototype->speed;
	project->Pgr = prototype->Pgr;
	
	return 1;

}

int CShipDesign::CheckShipCargoCapacity(ShipPrototypeMainDetails *prototype, ShipMainDetails *project)
{
	float Wgr; 
	float V, alfa, HT;
	int result;
	float dHT = 0;
	
	if ((prototype->Wkip>0) && (prototype->Wkip<3))
		project->mu = (float)prototype->Wkip/prototype->Pgr;

	if ((project->mu < 0) || (project->mu > 3))
		project->mu = 1.5;

	Wgr = project->mu * m_Pgr;
	alfa = (float)(0.98*pow(m_delta,0.5f));
	
	V = Wgr/(1+((alfa/m_delta)*((m_H/m_T)-1)));

	if (V <= m_V)
		result = 1;
	else 
		result = 0;

	if (result==0) {
		alfa = (float)(alfa+0.06);
		V = Wgr/(1+((alfa/m_delta)*((m_H/m_T)-1)));	
		if (V <= m_V){
			HT = 1 + ((m_delta/alfa)*((Wgr/V)-1));
			dHT = HT - m_HT;
		}
	}

	return result;
}
