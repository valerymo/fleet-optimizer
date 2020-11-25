// ACVDesign.cpp: implementation of the CACVDesign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ACVDesign.h"
#include  <math.h> 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CACVDesign* CACVDesign::_instance = 0;

CACVDesign* CACVDesign::Instance(){
    if (_instance == 0){
        _instance = new CACVDesign;
    }
    return _instance;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CACVDesign::CACVDesign()
{
	m_a1 = (float)1.1;
	m_a2 = (float)1.1;
	m_ro = (float)1.025;
	m_ro_vozd = (float)0.0012; 

	CWinApp* pApp = AfxGetApp();
	m_sLogFNameError = pApp->GetProfileString("FleetOptimizer","LogFileErrors");
	m_sLogFileVehiclesDetails = pApp->GetProfileString("FleetOptimizer","LogFileVehiclesDetails");
	m_nErrorStatus = 0;
	m_bPrintIsOn = 0;

	FILE *stream;
	stream = fopen(m_sLogFileVehiclesDetails, "a");
	CString strMessage = "Full weight\tLoad\tAC Lenth\tAC Width\tAC Area\t";
	fprintf(stream, strMessage);
	strMessage = "AC/Skirt Height\tAC pressure\tAir consumption Qp\tFull Power (longtime)\t";
	fprintf(stream, strMessage);
	strMessage = "Propulsors type\tPropulsors Number\tPropulsors Diameter\tFans type\tFans number\tFans Diameter\n";
	fprintf(stream, strMessage);
	
	fclose(stream);

}

CACVDesign::~CACVDesign()
{

}

int CACVDesign::GetACVMainDetails(ACVPrototypeMainDetails *prototype, ACVMainDetails *project)
{
	int res;

	ResetPrivateData();

	if (prototype->speed == project->speed){
		res = ComputeACVMainDetailsSpeedConst(prototype, project);
	}else{
		res = ComputeACVMainDetailsSpeedVar(prototype, project);
	}
	if (res == -1)
		return -1;
	
	SetACVMainDetails(project);

	if (m_bPrintIsOn)
		printCurrentACVDetails(prototype, project);


	return 1;
}

void CACVDesign::ResetPrivateData()
{
	m_Lp = 0;
	m_Bp = 0;
	m_LpBp = 0;
	m_D = 0;
	m_Dgr = 0;
	m_Dtp = 0;
	m_N = 0;
	m_cPtp = 0; 
	m_cND = 0; 
}

int CACVDesign::SetACVMainDetails(ACVMainDetails *proj)
{
	proj->Lp = m_Lp;
	proj->Bp = m_Bp;
	proj->LpBp = m_LpBp;
	proj->Sp = m_Sp;
	proj->D = m_D;
	proj->Dtp = m_Dtp;
	proj->N = m_N;
	proj->cPtp = m_cPtp;
	proj->cDgrD = (float)proj->Dgr/m_D;
	proj->r = m_r;
	proj->Hgo = m_Hgo;

	
	return 1;
}

int CACVDesign::ComputeACVMainDetailsSpeedConst(ACVPrototypeMainDetails *prototype, ACVMainDetails *project)
{
	float D, Lmin;
	int N;
	float lp_prot;
	float cDtp_D, cDtp_D_tmp, speed;
    char buffer[10];
	char strNum[5];


	FILE *streamError;
    streamError = fopen(m_sLogFNameError, "a");

	m_h_3 = project->h3;
	m_r = project->r;

	cDtp_D = prototype->Dtp / prototype->D;
	D = project->Dgr/(prototype->cDgrD - cDtp_D);

	speed = (float)1.4*project->speed;
	N = (int)(5.042 * D * speed/prototype->cKn);

	if(N < 7000){
		m_cPtp = (float)(0.38 - 0.024*(N/1000 - 2));
	}
	else {
		m_cPtp = (float)(0.25 - 1.43*N/1000000);
	}
	
	float nu = (float)(prototype->Dgr + prototype->Dtp)/prototype->D; 
	int fl_rfound = 0;
	
	while (!fl_rfound){
		m_Dtp = m_a1*m_a2*m_cPtp*N*((float)m_r/speed)/1000;
		m_D = (project->Dgr + m_Dtp)/nu;
		cDtp_D_tmp = m_Dtp/m_D;
		if(cDtp_D_tmp > cDtp_D)
			m_r -= 10;
		else
			fl_rfound = 1;
	}

	m_N = (int)(5.042 * m_D * speed/prototype->cKn);

	lp_prot = (float)(prototype->Lp/pow((prototype->D/m_ro), 0.333f));
	m_Lp = (float)(lp_prot*pow((m_D/m_ro), 0.333f));

	Lmin = (float)(9.7822*pow((m_h_3),1.201f));
	if (m_Lp < Lmin){
		CString strMessage = "The length of the hovercraft is too small for a given area of ​​operation. Vessel: ";
		strMessage += project->name;
		strMessage += ",   Payload: ";
		_itoa(project->Dgr, strNum,10);
		strMessage += strNum;	
		strMessage += ", Lп: ";
		_gcvt ( m_Lp,3, buffer );
		strMessage += buffer;		
		strMessage += ", Lмин: ";
		_gcvt ( Lmin,3, buffer );
		strMessage += buffer;		
		strMessage += ". CACVDesign::ComputeACVMainDetailsSpeedConst()\n";
		fprintf(streamError, strMessage);
		m_nErrorStatus = 1;
		fclose(streamError);
		return -1; // СВП слишком мало для заданного района эксплуатации.
	}

	m_LpBp = prototype->LpBp;
	m_Bp = m_Lp/m_LpBp;

	m_Hgo = prototype->HgoBp*m_Bp;
	if (m_Hgo < m_h_3*1.1) {
		CString strMessage = "Высота ГО СВП недостаточна для заданного района эксплуатации. Судно: ";
		strMessage += project->name;
		strMessage += ",   Грузоподъемность: ";
		_itoa(project->Dgr, strNum,10);
		strMessage += strNum;	
		strMessage += ", Hgo: ";
		_gcvt ( m_Hgo,3, buffer );
		strMessage += buffer;		
		strMessage += ", Высота волны: ";
		_gcvt ( m_h_3,3, buffer );
		strMessage += buffer;		
		strMessage += ". CACVDesign::ComputeACVMainDetailsSpeedConst()\n";
		fprintf(streamError, strMessage);
		m_nErrorStatus = 1;
		fclose(streamError);
		return -1;
	}

	float alfa_p = (float)(1 - 0.1*m_Bp/m_Lp);
	m_Sp = alfa_p*m_Lp*m_Bp;

	ComputeACVLiftSystem(prototype, project);
	ComputeACVPropulsionSystem(prototype, project);

	fclose(streamError);
	return 1;
}

int CACVDesign::ComputeACVMainDetailsSpeedVar(ACVPrototypeMainDetails *prototype, ACVMainDetails *project)
{
	float D, Lmin;
	int N;
	float lp_prot;
	float cDtp_D, cDtp_D_tmp, speed;
    char buffer[10];
	char strNum[5];
	
	FILE *streamError;
    streamError = fopen(m_sLogFNameError, "a");

	m_h_3 = project->h3;
	m_r = project->r;
	m_cND = (float)(1.5 * pow(project->speed, 0.9f));

	cDtp_D = prototype->Dtp/prototype->D;
	D = project->Dgr/(prototype->cDgrD - cDtp_D);
	speed = (float)1.4*project->speed;
	N = (int)(5.042 * D * speed/prototype->cKn);

	if(N < 7000){
		m_cPtp = (float)(0.4 - 0.02*((float)N/1000 - 2));
	}
	else {
		m_cPtp = (float)(0.25 - 1.4*((float)N/1000000));
	}
	
	float nu = (float)(prototype->Dgr + prototype->Dtp)/prototype->D; 
	int fl_rfound = 0;
	
	while (!fl_rfound){
		m_Dtp = m_a1*m_a2*m_cPtp*N*((float)m_r/speed)/1000;
		m_D = (project->Dgr + m_Dtp)/nu;
		cDtp_D_tmp = m_Dtp/m_D;
		if(cDtp_D_tmp > cDtp_D)
			m_r -= 10;
		else
			fl_rfound = 1;
	}
	
	m_N = (int)(m_cND*m_D);
	if (m_N < N)
		m_N = N;

	lp_prot = (float)(prototype->Lp/pow((prototype->D/m_ro), 0.333f));
	m_Lp = (float)(lp_prot*pow((m_D/m_ro), 0.333f));

	Lmin = (float)(9.7822*pow((m_h_3),1.201f));
	if (m_Lp < Lmin){
		CString strMessage = "Длина СВП недостаточна для заданного района эксплуатации. Судно: ";
		strMessage += project->name;
		strMessage += ",   Грузоподъемность: ";
		_itoa(project->Dgr, strNum,10);
		strMessage += strNum;	
		strMessage += ",   Lп: ";
		_gcvt ( m_Lp,3, buffer );
		strMessage += buffer;		
		strMessage += ",   Lмин: ";
		_gcvt ( Lmin,3, buffer );
		strMessage += buffer;		
		strMessage += ".   CACVDesign::ComputeACVMainDetailsSpeedVar()\n";
		fprintf(streamError, strMessage);
		m_nErrorStatus = 1;	

		return -1; // СВП слишком мало для заданного района эксплуатации.
	}

	m_LpBp = prototype->LpBp;
	m_Bp = m_Lp/m_LpBp;

	m_Hgo = prototype->HgoBp*m_Bp;
	if (m_Hgo < m_h_3*1.1){
		CString strMessage = "Высота ГО СВП недостаточна для заданного района эксплуатации.   Судно: ";
		strMessage += project->name;
		strMessage += ",   Грузоподъемность: ";
		_itoa(project->Dgr, strNum,10);
		strMessage += strNum;
		strMessage += ",   Hgo: ";
		_gcvt ( m_Hgo,3, buffer );
		strMessage += buffer;		
		strMessage += ",   Высота волны: ";
		_gcvt ( m_h_3,3, buffer );
		strMessage += buffer;		
		strMessage += ".   CACVDesign::ComputeACVMainDetailsSpeedVar()\n";
		fprintf(streamError, strMessage);
		m_nErrorStatus = 1;
		fclose(streamError);
		return -1;
	}

	float alfa_p = (float)(1 - 0.1/m_LpBp);
	m_Sp = alfa_p*m_Lp*m_Bp;

	ComputeACVLiftSystem(prototype, project);
	ComputeACVPropulsionSystem(prototype, project);

	fclose(streamError);
	return 1;
}



int CACVDesign::GetExistACVMainDetails(ACVPrototypeMainDetails *prototype, ACVMainDetails *project)
{
	FILE *streamError;
    streamError = fopen(m_sLogFNameError, "a");
	
	project->Lp = prototype->Lp;
	project->Bp = prototype->Bp;
	project->LpBp = prototype->LpBp;
	project->Sp = prototype->Sp;
	project->D = prototype->D;
	project->Dtp = prototype->Dtp;
	project->N = prototype->N;
	//project->cPtp = prototype->; ??
	project->cDgrD = prototype->cDgrD;
	project->speed = prototype->speed;
	project->Dgr = prototype->Dgr;
	project->r = prototype->r;
	project->Hgo = prototype->Hgo;
	project->h3 =(float)(project->Hgo/1.15); //(10.4) Г.Ф.Демешко.

	if(project->r <= 0) { 
		if (project->Dtp <= 0){
			CString strMessage = "Масса топлива и дальность хода не заданы для СВП Прототипа: ";
			strMessage += prototype->name;
			AfxMessageBox(strMessage);
			strMessage += ". CACVDesign\n";
			fprintf(streamError, strMessage);
			m_nErrorStatus = 1;
			fclose(streamError);
			return 0;
		}
		else { 
			float cPtp;
			if(project->N < 7000){
				cPtp = (float)(0.4 - 0.03*(project->N/1000 - 2));
			}
			else {
				cPtp = (float)(0.3 - 1.45*project->N/1000000);
			}		
			project->r = (int)((1000*project->Dtp*project->speed)/(m_a1*m_a2*project->N));
		}

	}

	fclose(streamError);
	return 1;
}

CString CACVDesign::GetLogFNameError()
{
	return m_sLogFNameError;
}

int CACVDesign::GetErrorStatus()
{
	return m_nErrorStatus;
}

int CACVDesign::ComputeACVLiftSystem(ACVPrototypeMainDetails *prototype, ACVMainDetails *project)
{
	if (m_Sp == 0)
		return 0;
	
	int result = 1;
	float g = (float)9.81;
	float p = (float)0.98;
	float Gs, Gs1;
	float lp;
	float kH, kH_min;
	CString proto_name;

	m_Pp = g*m_D*p/m_Sp; 
	float alfa_p = m_Sp/(m_Lp*m_Bp);
	lp = (float)(prototype->Lp/pow((prototype->D/m_ro), 0.333f));

	Gs = (float)pow ((sqrt(m_LpBp) /(sqrt(alfa_p) * lp)), 3); // (10.3)
	Gs1 = (float)(m_Pp/(m_ro*g*sqrt(m_Sp)));			// (1.13)

	if((Gs<0.01)||(Gs>0.03)||(Gs1<0.01)||(Gs1>0.03)){
		Gs = (float)0.02;
		result = 2;
	}

	m_Qp = (float)(0.7*m_Sp*sqrt(m_Hgo));   
	m_Qs = (float)(m_Qp/(m_Sp*sqrt(2*m_Pp/m_ro_vozd))); 
	if ((m_Qs<0.01)||(m_Qs>0.025))
		result = 3;

	m_Tz = (float)(2*3.14*sqrt(m_Hgo/g));	//(4.15)

	if (m_Qp > 0) {
		m_Tau = m_Sp*m_Hgo/m_Qp;
	} else 
		result = 4;

	m_Npk = (int)(3*m_Pp*m_Qp);
	float NpkN = (float)m_Npk/m_N;
	if (NpkN<0.35){ 
		NpkN = (float)0.35;
		m_Npk = (int)(m_N*NpkN);
		result = 5;
	}
	else if (NpkN>0.45){
		NpkN = (float)0.45;
		m_Npk = (int)(m_N*NpkN);
		result = 6;
	}

	m_nagn_type = 1;
	proto_name = prototype->name;
	if((proto_name.Find("JEFF") != -1) || (proto_name.Find("VT") != -1))
		m_nagn_num = 8;
	else if (m_Bp > 10)
		m_nagn_num = 4;
	else
		m_nagn_num = 2;

	kH = (float)0.25;
	kH_min = m_Pp/(m_ro_vozd*20250);
	if(kH < kH_min)
		result = 7;
	
	//диаметра рабочего колеса нагнетателя
	m_nagn_d = (float)(sqrt(((4*1.06*m_Qp)/(3.142*m_nagn_num*0.18))*sqrt(m_ro_vozd*kH/(m_Pp*2))));

	
	return result;
}

int CACVDesign::ComputeACVPropulsionSystem(ACVPrototypeMainDetails *prototype, ACVMainDetails *project)
{
	
	int result = 1;
	m_Ndv = m_N-m_Npk;

	float NdvN = (float)m_Ndv/m_N;
	if (NdvN<0.45){ 
		m_Ndv = (int)(0.45*m_N);
		result = 2;
	}
	else if (NdvN>0.6){
		m_Ndv = (int)(0.6*m_N);
		result = 3;
	}

	float Nvv =  m_Ndv*(float)0.97; 
	Nvv = Nvv/2;

	int Kgd = 21;
	float R = m_D/Kgd;
	R = R/2;
	
	float e = (float)1.06;
	float Tst_1, Tst_2; 
	float dvv;
	Tst_1 = R/(1- project->speed/202); 
	Tst_2 = (float)0.069*m_D; 
	Tst_2 = Tst_2/2; 
	float ro_vozd_sqrt = (float)sqrt(m_ro_vozd);
	m_dvv = (float)(31.62*e*pow(Tst_1,1.5f))/(Nvv * ro_vozd_sqrt);

	dvv = (float)(31.62*e*pow(Tst_2,1.5f))/(Nvv * ro_vozd_sqrt);

	if (dvv > m_dvv)
		m_dvv = dvv;
	else 
		result = 4;

	m_dvv = (float)0.59*m_dvv;

	if (m_dvv > 6)
		result = 5;

	if (2*m_dvv > 0.6*m_Bp)
		result = 5;

	return result;
}

int CACVDesign::printCurrentACVDetails(ACVPrototypeMainDetails *prototype, ACVMainDetails *project)
{
	
 	FILE *stream;  
	stream = fopen(m_sLogFileVehiclesDetails, "a");

	CString sNagnType;
	if (m_nagn_type = 1)
		sNagnType = "ЦН";
	else 
		sNagnType = "Осевой";

	fprintf(stream, "%.0f\t%d\t%.0f\t%.0f\t%.0f\t",m_D,project->Dgr,project->Lp,project->Bp,project->Sp);
	fprintf(stream, "%.3f\t%.3f\t%.3f\t%d\t",project->Hgo,m_Pp, m_Qp, m_N); 
	fprintf(stream, "%s\t%s\t%.3f\t%s\t%d\t%.3f\n", "ВВН","2", m_dvv, sNagnType.GetString(), m_nagn_num,m_nagn_d);
	fclose (stream);
	return 1;
}
