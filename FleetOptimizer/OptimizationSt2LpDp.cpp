// OptimizationSt2LpDp.cpp: implementation of the COptimizationSt2LpDp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FleetOptimizer.h"
#include "OptimizationSt2LpDp.h"
#include "BasePlanSt2LpDp.h"
#include "Cargo.h"
#include <math.h>
//#include "InitialBasePlan.h"
#include "OutputView.h"
#include "MarineTechEconomicEng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


COptimizationSt2LpDp* COptimizationSt2LpDp::_instance = 0;

COptimizationSt2LpDp* COptimizationSt2LpDp::Instance(){
    if (_instance == 0){
        _instance = new COptimizationSt2LpDp;
    }
    return _instance;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptimizationSt2LpDp::COptimizationSt2LpDp()
{
    m_RoutersList = 0;
	m_DynamicRoutersList = 0;
	m_DynamicRoutersListPrev = 0;
	//MessageBox(NULL,"COptimizationSt2LpDp()","COptimizationSt2LpDp",MB_OK);
	
	POSITION pos = ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetFirstViewPosition();
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	m_pOutView = (COutputView*) ((CFleetOptimizerApp*)AfxGetApp())->GetCurrFleetOptimizerDoc()->GetNextView(pos);
	//m_pOutView->TextOut("COptimizationSt2LpDp()");
	m_bStepDone = 0;

}

COptimizationSt2LpDp::~COptimizationSt2LpDp()
{

}




/***********************************************************************\
	COptimizationSt2LpDp::SimplexMethod()
	ВТОРОЙ этап задачи оптимизации.
	Процедура линейного программирования.
	Используется модифицированный симплекс-метод /с применением аппарата
	обратных матриц/. Процедура использует оценки векторов /маршрутов/,
	найденных с помощью процедуры динамического программирования(RouteCreate1).
	Используются двойственные оценки линий перевозок, полученные на первом
	этапе решения задачи, а также типы и количества  судов на линиях
	/дробные части/.
	Производится пересчет целевой функции, двойственных оценок,
	обратной матрицы провозоспособностей судов на линиях, количества судов.
	На каждом шаге метода производится замена одного из векторов маршрутом,

/***********************************************************************/
int COptimizationSt2LpDp::SimplexMethod()
{
	CPort *pBasePort;
	int cargo_type, cargo_typeIn, cargo_typeOut, cargoRemain, cargoDone, cargo_typeOnLine, //cargo_typeOut1, 
		PNB_num, PNB_numIn, PNB_numOut, result, countIn, //PNB_numOut1,
		VehicleNumIn, VehicleNumOut, VehicleNum, VehicleInLoad,
		PNBRequiredCargo, LineCount, LineCountIn, LineCountOut, nLCountRemain, 
		VehicleLoadIn, VehicleLoadOut;

	FILE *stream;
	CLine *pLine, *pLineIn;
	CVehicle *pVehicleIn;
	int vehicleCapabilityIn, vehicleCapabilityOut, 
		vehicleCapabilityInOnLine, vehicleCapability;
	double g; //коэффициенты разложения вектора А по базисным векторам
	double dVehicleNum, VectorsMark;
	CRouterL *pRouterPrev;
	CString sCargoType = "";
	
	//MessageBox(NULL,"COptimizationSt2LpDp::SimplexMethod()","SimplexMethod",MB_OK);
	m_pOutView->TextOut(" ");
	m_pOutView->TextOut("2nd stage of Optimization");
	m_pOutView->TextOut("Modified Simplex Method with vectormarks (Routes) build using dynamic programming");
	m_pOutView->TextOut("Routes - list of lines");

	//stream = fopen( "E:\\Users\\mv\\Log\\OptimizationProgressStep2.txt", "w" );
	CString LogFile = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogDir");
	LogFile += "OptimizationProgressStep2.txt";
    stream = fopen(LogFile, "w" );

	result = CBasePlanSt2LpDp::Instance()->CreateBasePlanInit();

	if (result == 0){
		MessageBox(NULL,"List of Routes is empty\nMissing Initial Base Plan","2nd stage of Optimization: SimplexMethod",MB_ICONEXCLAMATION);
		m_pOutView->TextOut("2nd stage of Optimization, SimplexMethod: List of Routes is empty. Missing Initial Base Plan)");
        return 0;        
    }

	if (COptimizationSt1LP::Instance()->IsStepDone() == 0)
		ResetExistVehicleNum();

	GoalFunctionCompute();
	fprintf (stream,"Objective function: %d\n\n", this->GoalFunction);

	BOOL bIncludeLandStructCalc = FALSE;

	//CRouter *pRouterTmp = CBasePlanSt1LP::Instance()->GetRoutersListHead();
	//CRouter *&pRouterOut = *&pRouterTmp;
	CRouterL *pRouterTmp = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();
	CRouterL *&pRouterOut = *&pRouterTmp;
	pBasePort =		pRouterOut->GetBasePort();
	PNB_num = pRouterOut->GetLine()->GetPNBnum();
	cargo_type =	pRouterOut->GetCargoType();
	//pLine	=		pRouterOut->GetLine();
	CRouterL *pRouterIn;
	int fl_PNB;
	//CRouterL *pRouterIn = new CRouterL(pBasePort, PNB_num, cargo_type);

	CRouterL *pRouterTest = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();

	long lInvestmentYearLimit = CFleetTechEconomicEng::Instance()->GetInvestmentYearLimit();
	if (lInvestmentYearLimit > 0){
		long Investment = CFleetTechEconomicEng::Instance()->InvestmentYearCompute(pRouterTest);
		CFleetTechEconomicEng::Instance()->SetInvestmentYear(Investment);
	}


	int optimumFlag = 0;
	int step_SM = 0;
	while(optimumFlag != 1)
	{
		step_SM++;
		if (step_SM > 20){
			MessageBox(NULL,"2-этап оптимизации: SimplexMethod()","Остановка на шаге",MB_OK);
			m_pOutView->TextOut("Остановка на шаге.  2-этап оптимизации: SimplexMethod()");
			break;
		}
		pRouterIn = new CRouterL(pBasePort, PNB_num, cargo_type);
		result = VectorsMarksCompute(pRouterOut, pRouterIn, VectorsMark);

		if (result == 1){
			fprintf (stream,"*******************************************************\n");
			fprintf (stream,"Судно вводимое в базис: %s\n",pRouterIn->GetVehicle()->GetTypeName());
			fprintf (stream,"Грузоподъемность Судна: %d т\n",pRouterIn->GetVehicle()->GetLoad());
			fprintf (stream,"Количество судов: %d\n", pRouterIn->GetVehicleNumber());
			fprintf (stream,"Количество судов нецелое: %f\n", pRouterIn->GetDVehicleNumber());
			fprintf (stream,"Z оценка маршрута: %f\n", VectorsMark);
			pLine = pRouterIn->GetLine();
			while(pLine){
				PNB_num = pLine->GetPNBnum();
				fprintf (stream,"ПНБ: %s\n",pRouterIn->GetBasePort()->GetPNB(PNB_num).name);
				cargo_typeOnLine = pLine->GetCargoType();
				switch(cargo_typeOnLine){
				 case 1:
					 sCargoType = "уголь";
					 break;
				 case 2:
					 sCargoType = "генеральный";
					 break;
				 case 3:
					 sCargoType = "ГСМ";
					 break;
				 case 4:
					 sCargoType = "овоши-фрукты";
					 break;
				}
 				//fprintf (stream,"\tгруз: %d\n", pLine->GetCargoType());
				fprintf (stream,"\tгруз: %s\n", sCargoType.GetString());
				fprintf (stream,"\tКоличество рейсов: %d\n",pLine->GetCount());
				pLine = pLine->GetNextLine();
			}
			//fprintf (stream,"*******************************************************\n");
		}


		if (result == 0){
			fclose(stream);
			return 1;
		}
		if (result == 10) {
			optimumFlag = 1;
			break;
		}

	/******************************************************\
		Нахождение нового допустимого базисного множества
	/******************************************************/
    /*	Маршрут характеризуется вектором А: 
		А=(a1,a2,...,am), где
			ai=ki*Pi;
				ki- кратность использования линии i в маршруте H;
				Pi- грузоподьемность судна /на линии i/.
		Определяются коэффициенты разложения вектора А по базисным векторам:
				q[N']=A/as[N',M].
		Определяется интенсивность с которой А входит в базис:
				U=min(Xi/qi, qi>0),
				i -	номер удаляемого из базиса вектора для замены его
					вектором A. 
	/*****************************************************/
	CRouterL *pRouter = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();
	int NumberOfRouter = 0; //номер текущего базисного вектора (маршрута)
	int NumberOfRouterOut;   //номер выводимого из базиса вектора
	double Umin =1000000000.0; //
	double U_OutCondition;//u:=X[k]/q[k];
	int flG = 0;
	int flVehicleExistNumRemainTmp = 0;
	
	pVehicleIn = pRouterIn->GetVehicle();
	VehicleInLoad = pVehicleIn->GetLoad();

	while(pRouter){  // поиск номера выводимого из базиса вектора (маршрута)
		//Проверка на тип груза: д.б. одинаковый груз
		int flCargoCompartible = 0; //flag
		cargo_typeOut = pRouter->GetLine()->GetCargoType();
		PNB_num = pRouter->GetLine()->GetPNBnum();
		pBasePort =	pRouter->GetBasePort();
		pLineIn = pRouterIn->GetLine();
		VehicleNum = pRouter->GetVehicleNumber();
		if (pVehicleIn->IsExist()){
			pVehicleIn->UpdateExistNumRemainIncrem(VehicleNum); 
			// временная корректировка для провепки с использованием функции VehicleRouterPLineValidation()
			// после проверки - значение возвращается (см. ниже)
			flVehicleExistNumRemainTmp = 1;
		}
		while(pLineIn){
			PNB_numIn = pLineIn->GetPNBnum();
	        if(CFleetTechEconomicEng::Instance()->VehicleRouterPLineValidation(pBasePort,PNB_num,cargo_typeOut,pVehicleIn)==1){
			//cargo_typeIn = pLineIn->GetCargoType();
			//if (cargo_typeIn == cargo_typeOut)//
				flCargoCompartible = 1;
				break;
			}
			pLineIn = pLineIn->GetNextLine();
		}
		if (flVehicleExistNumRemainTmp){
			pVehicleIn->UpdateExistNumRemainDecrem(VehicleNum);
			flVehicleExistNumRemainTmp = 0;
		}
		if (!flCargoCompartible){
			NumberOfRouter++;
			pRouter = pRouter->GetNextRouter();
			continue;
		}

		//END: Проверка на тип груза  

		//Проверка существующего судна (обязательное включение в план)
		if (pRouter->GetVehicle()->IsExistForce() == 1){
			NumberOfRouter++;
			pRouter = pRouter->GetNextRouter();
			continue;	
		}
	
		VehicleNumIn = pRouterIn->GetVehicleNumber();
		VehicleNumOut = pRouter->GetVehicleNumber();

		PNB_numOut = pRouter->GetLine()->GetPNBnum();

		//vehicleCapabilityOut = ((double)pRouter->GetBasePort()->GetPNBRequiredCargo(PNB_numOut,cargo_typeOut))/VehicleNumOut;
		vehicleCapabilityOut = VehicleSCapabilityOnRouterCompute(pRouter);
		//Вычисление vehicleCapabilityIn
		fl_PNB = 0;
		vehicleCapabilityIn = 0;
		pLineIn = pRouterIn->GetLine();
		while(pLineIn){
			LineCount = pLineIn->GetCount();
			if (LineCount == 0){
				pLineIn = pLineIn->GetNextLine();
				continue;
			}
			cargo_typeIn = pLineIn->GetCargoType();
			if (cargo_typeIn != cargo_typeOut){
				pLineIn = pLineIn->GetNextLine();
				continue;
			}
			PNB_numIn = pLineIn->GetPNBnum();
			if (PNB_numIn == PNB_numOut)
				fl_PNB = 1;
			//vehicleCapabilityIn += ((double)pRouterIn->GetBasePort()->GetPNBRequiredCargo(PNB_numIn,cargo_typeIn))/VehicleNumIn;
			//vehicleCapabilityIn += ((double)pRouterIn->GetBasePort()->GetPNBRequiredCargoRemain(PNB_numIn,cargo_typeIn))/VehicleNumIn;
			//vehicleCapabilityIn += ((double)pRouterIn->GetBasePort()->GetPNBRequiredCargo(PNB_numOut,cargo_typeIn))/VehicleNumIn;
			vehicleCapabilityInOnLine = VehicleInLoad * LineCount;
			vehicleCapabilityIn += vehicleCapabilityInOnLine;

			pLineIn = pLineIn->GetNextLine();
		}
		if (fl_PNB == 0)
			vehicleCapabilityIn = 0;
		//END: Вычисление vehicleCapabilityIn


		g = (double)vehicleCapabilityIn/vehicleCapabilityOut; //g - коэффициенты разложения
		if (g > 0){ 
			flG = 1;
			U_OutCondition = (double)VehicleNumIn/g;  //u:=X[k]/q[k];
			if (U_OutCondition < Umin){
				Umin = U_OutCondition; // интенсивность Umin, с которой вектор А входит
				NumberOfRouterOut = NumberOfRouter;  //номер удаляемого из базиса вектора.
				//cargo_typeOut1 = cargo_typeOut;
				//PNB_numOut1 = PNB_numOut;
			}
		} //if (g > 0)

		NumberOfRouter++;
		pRouter = pRouter->GetNextRouter();

	} //END: while(pRouter) поиск номера выводимого из базиса вектора (маршрута)


	/*	Замена маршрутов базисного плана:
			a)(RouterOut<->RouterIn)
			b) проверка/корректировка других маршрутов базисного плана (см. ниже).
		Вычисление новых значений количества судов на маршрутах.
		RouterIn входит без изменений: 
				x'= u* = Umin
		В остальных маршрутах базисного плана количество судов может измениться:
				x' = x-u*g
		Здесь u*g показывает, что часть работы на маршруте (линии) 
		будет выполнять новое судно (маршрута RouterIn), при условии,
		что вводимый и рассматриваемый маршрут имеют общую линию,
	*/
	pRouter = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();
	pRouterPrev = NULL;
	int count = 0;
	while(pRouter){ //Замена и корректировка маршрутов базисного плана
		PNB_num = pRouter->GetLine()->GetPNBnum();
		cargo_type = pRouter->GetCargoType();
		VehicleNum = pRouter->GetVehicleNumber(); 
		//vehicleCapability = ((double)pRouter->GetBasePort()->GetPNBRequiredCargo(PNB_num,cargo_type))/VehicleNum;
		vehicleCapability = VehicleSCapabilityOnRouterCompute(pRouter);
		if (count == NumberOfRouterOut){//RouterOut найден. (RouterOut<->RouterIn)
			//pRouterTmp = pRouter;
			pRouterIn->next = pRouter->next;
			if (pRouterPrev == NULL)
				CBasePlanSt2LpDp::Instance()->RoutersList = pRouterIn;
			else
				pRouterPrev->next = pRouterIn;
			pRouter = pRouterIn;
			//delete pRouterTmp;
			//Корректировка требуемых грузов:
			pLineIn = pRouterIn->GetLine();
			while(pLineIn){
				cargo_typeIn = pLineIn->GetCargoType();
				PNB_numIn = pLineIn->GetPNBnum();
				countIn = pLineIn->GetCount();
				cargoDone =  (pVehicleIn->GetLoad())* countIn;
				PNBRequiredCargo = pRouterIn->GetBasePort()->GetPNBRequiredCargo(PNB_numIn,cargo_typeIn);
				if (PNBRequiredCargo < cargoDone)
					cargoRemain = 0;
				else
					cargoRemain = PNBRequiredCargo - cargoDone;
				pRouterIn->GetBasePort()->SetPNBRequiredCargoRemain0(PNB_numIn,cargo_typeIn, cargoRemain);					
				pLineIn = pLineIn->GetNextLine();
			}
		} 
		else if (pRouter->GetLine()->next == 0) // не изменять вновь-введенные маршруты - наборы-линий
		{
			pLineIn = pRouterIn->GetLine();
			double dVehicleNumMinTmp = 1000;
			while(pLineIn){
				cargo_typeIn = pLineIn->GetCargoType();
				PNB_numIn = pLineIn->GetPNBnum();
				if ((cargo_type == cargo_typeIn)&&(PNB_num ==PNB_numIn)) {
					// вводимый и рассматриваемый маршрут имеют общую линию
					VehicleLoadOut = pRouter->GetVehicle()->GetLoad();
					VehicleLoadIn = pRouterIn->GetVehicle()->GetLoad();
					LineCountOut = pRouter->GetLine()->GetCount();
					LineCountIn = pLineIn->GetCount();
					vehicleCapabilityOut = VehicleLoadOut * LineCountOut;
					vehicleCapabilityIn = VehicleLoadIn * LineCountIn;
					//vehicleCapabilityIn = ((double)pRouterIn->GetBasePort()->GetPNBRequiredCargo(PNB_numIn,cargo_typeIn))/VehicleNumIn;
					g = (double)vehicleCapabilityIn/vehicleCapabilityOut;
					//dVehicleNum = VehicleNum - Umin * g;
					dVehicleNum = VehicleNum - VehicleNumIn * g;
					if (dVehicleNum < 0){
						dVehicleNum = 0; 
						//pRouter->dVehicleNumber = 0;
						//pRouter->vehicleNumber = 0;
					}else{
						if (pRouter->GetLine()->GetNextLine() == 0){ // на маршруте одна линия
							//28.11.04 //if (pRouter->GetLine()->GetCount() > 1){ // число повторений линии > 1
								// если число повторений линии > 1, уменьшаем количество рейсов (повторяемость линии)
								//nLCountNew = ceil (pRouter->dVehicleNumber/dVehicleNum);
								nLCountRemain = (int)ceil((float)(vehicleCapabilityOut-vehicleCapabilityIn)/VehicleLoadOut);
								if (nLCountRemain < 0) ////// 22.10.04
									nLCountRemain = 0; 
								pRouter->GetLine()->SetCount(nLCountRemain);
							/*}else {
								if (dVehicleNum < dVehicleNumMinTmp){
									dVehicleNumMinTmp = dVehicleNum;
									//pRouter->dVehicleNumber = dVehicleNum;
									//pRouter->vehicleNumber = (int)ceil(dVehicleNum);	
								}
							}*/
						}
					}
					pRouter->dVehicleNumber = dVehicleNum;
					pRouter->vehicleNumber = (int)ceil(dVehicleNum);
				}
				pLineIn = pLineIn->GetNextLine();
			}//end: while(pLineIn)
			// Корректировка количества рейсов:
			// Если (на маршруте одна линия) и (число повторений линии > 1)
			// то можно попытаться откорректировать количество рейсов:
			// уменьшить количество рейсов за счет увеличения количества судов.
			int use_this1 = 0;
			if ((pRouter->GetLine()->GetNextLine() == 0) && (use_this1 == 1)) { //одна линия на маршруте
				int nLCount = pRouter->GetLine()->GetCount();
				if ((nLCount > 1)&& (dVehicleNum > 0.3)){
					int nVNumNew = (int)ceil(dVehicleNum);
					double dVNumNew = nVNumNew;
					double LCountNew = dVehicleNum*nLCount/dVNumNew;
					if (LCountNew > 1){
						int nLCountNew = (int)ceil(LCountNew);
						pRouter->dVehicleNumber = dVNumNew;
						pRouter->vehicleNumber = (int)ceil(dVehicleNum);
						pRouter->GetLine()->SetCount(nLCountNew);
					}
				}
			}
		}
		count++;
		pRouterPrev = pRouter;
		pRouter = pRouter->GetNextRouter();
	} //END: while(pRouter) //Замена маршрутов базисного плана

	bIncludeLandStructCalc = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","IncludeLandStructCalc",0);	
	if ((lInvestmentYearLimit > 0) && (bIncludeLandStructCalc))
		CMarineTechEconomicEng::Instance()->UpdateLandStructVirtualDataSt2();
	
	GoalFunctionCompute();
	fprintf (stream,"Objective function: %d\n\n", this->GoalFunction);

	}//	while(optimumFlag != 1)	

	cleanDummyRouters();

	//CBasePlanSt2LpDp::Instance()->print();
	this->print();
	fclose(stream);
	//MessageBox(NULL,"Выполнено","2-этап оптимизации: SimplexMethod()",MB_OK);
	m_pOutView->TextOut("Completed - 2nd stage of Optimization: SimplexMethod, Routes - list of lines");
	m_bStepDone = 1;
	return 1;
}


int COptimizationSt2LpDp::VectorsMarksCompute(CRouterL *&pRouterOut, CRouterL *&pRouterIn, double &VectorsMarkBest)
{

	double DualMark;		// Y - Двойственная оценка
	double VectorsMark;// VectorsMarkBest;	// Z - Оценка векторов
	//double vehicleCapability;
	CPort *pBasePort;
	CLine *pLine;// *pLineIn;
	CVehicle *pVehicle;
	int cargo_type, /*cargo_typeIn,*/ PNB_num, /*PNB_numIn,*/ flagRouterFound;
	int vehicleIncomeR,vehicleIncome, VehicleNum;
	double	dVehicleNum;
    int BuildCoastYearPay;// OperationalExpensesYear;
	int /*cargo_for_navigation,*/ cargoRemain, vehicleCapability;
	int res1;
//	int count, countIn;
	int nVehicleExist, nVehicleExistForce, flagGetVehicle, AdvantPrice, 
		nVehicleBestExist, nVehicleBestExistForce, VehicleBestAdvantPrice, nVehicleExistNum;

	flagRouterFound = 0;
	VectorsMark = 0;
	nVehicleBestExist = 0;
	nVehicleBestExistForce = 0;

	
	CRouterL *pRouter = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();
	
	//FILE *stream;
    //stream = fopen( "E:\\Users\\mv\\Log\\OptimizationSt2LpDp.txt", "a" );

    if (pRouter == NULL){
        //fprintf (stream,"Список маршрутов пуст\n");
		//fclose(stream);
        return 0;
    }
	
	pBasePort =		pRouter->GetBasePort();
	PNB_num = pRouter->GetLine()->GetPNBnum();
	cargo_type =	pRouter->GetCargoType();
	pLine	=		pRouter->GetLine();
	//CRouterL *pRouterL = new CRouterL(pBasePort, PNB_num, cargo_type);
	CRouterL *pRouterL;

	VectorsMarkBest = -10000000;

	long lInvestmentYearLimit = CFleetTechEconomicEng::Instance()->GetInvestmentYearLimit();
	long lInvestmentYear = CFleetTechEconomicEng::Instance()->GetInvestmentYear();
	int  BuildCoastYearVehicleRout, vehicleNumberR;
	CVehicle* pVehicleR;
	int InvestDelta = 0;
	int InvestDeltaBest = 0;

	//Капиталовложения в береговые сооружения
	BOOL bIncludeLandStructCalc = FALSE;
	int LandInvestDelta = 0;
	int LandInvestDeltaBest = 0;
	bIncludeLandStructCalc = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","IncludeLandStructCalc",0);	

    while(pRouter){

		vehicleIncomeR = pRouter->GetVehicleIncome();
		pBasePort =		pRouter->GetBasePort();
		PNB_num =		pRouter->GetLine()->GetPNBnum();
		cargo_type =	pRouter->GetCargoType();
		pVehicleR =	pRouter->GetVehicle();
		vehicleNumberR = pRouter->GetVehicleNumber();


		BuildCoastYearVehicleRout = CFleetTechEconomicEng::Instance()->VehicleBuildCoastYearPay(pVehicleR);


		cargoRemain = pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type);
		if (cargoRemain == 0){
				pRouter = pRouter->GetNextRouter();
				continue;
		}
		
		//pRouterL->SetCargoType(cargo_type);
		//pRouterL->GetLine()->ReplacePNB(PNB_num);
		pRouterL = new CRouterL(pBasePort, PNB_num, cargo_type);


        pVehicle = CVehiclesList::Instance()->GetVehicleListHead();
        //vehicleIncome = CFleetTechEconomicEng::Instance()->VehicleIncomeOnLineForYear(pBasePort,PNB_num,cargo_type,pVehicle,VehicleNum,dVehicleNum, BuildCoastYearPay, OperationalExpensesYear, cargo_for_navigation);
		DualMark = (double)(-vehicleIncomeR)/ (pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type));

        while(pVehicle){
			AdvantPrice = 0;
			if (!(pVehicle->GetCargoCompatibility(cargo_type))){
				pVehicle = pVehicle->GetNextVehicle();
				continue;
			}
			pRouterL->SetVehicle(pVehicle);
		
			m_DynamicRoutersList = 0;
			m_DynamicRoutersListPrev = 0;

			res1 = RouterCreate(pRouterL);// Построение маршрута судна как набора линий (Динамическое программирование).
            if (res1 == 0){; 
				//MessageBox(NULL,"Dynamic routers creation problem!","VectorsMarksCompute",MB_OK);
				//return 0;
				pVehicle = pVehicle->GetNextVehicle();
				continue;
			}
			//Вычисление оценки маршрута pRouterL {begin}
			//Z = <A,Y>-C,
			vehicleCapability = 0;
			//CalculateVehicleNumberOnRouter(VehicleNum, dVehicleNum);
			pLine = pRouterL->GetLine();

/*			VectorsMark = 0;
			while(pLine){
				PNB_num = pLine->GetPNBnum();
				cargo_type = pLine->GetCargoType();
		        vehicleIncome = CFleetTechEconomicEng::Instance()->VehicleIncomeOnLineForYear(pBasePort,PNB_num,cargo_type,pVehicle,VehicleNum,dVehicleNum, BuildCoastYearPay, OperationalExpensesYear, cargo_for_navigation);
				vehicleCapability = ((double)pRouterL->GetBasePort()->GetPNBRequiredCargo(PNB_num,cargo_type))/VehicleNum;
				DualMark = (double)(-vehicleIncome)/ (pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type));
				VectorsMark += (vehicleCapability * DualMark) + vehicleIncomeR;
				pLine = pLine->GetNextLine();
			}
			//VectorsMark += (vehicleIncome); // д.б. +, минус только для проверки
*/			//Вычисление оценки маршрута pRouterL {end}
				
			vehicleIncome = CFleetTechEconomicEng::Instance()->VehicleIncomeOnRouterLForYear(pRouterL,VehicleNum, dVehicleNum, vehicleCapability );
			//vehicleCapability = ((double)pRouterL->GetBasePort()->GetPNBRequiredCargo(PNB_num,cargo_type))/VehicleNum;
			//LATEST: vehicleCapability = ((double)pRouterL->GetBasePort()->GetPNBRequiredCargo(PNB_num,cargo_type))/VehicleNum;
			//!!DualMark = (double)(-vehicleIncome)/ (pBasePort->GetPNBRequiredCargo(PNB_num,cargo_type));
			//!!VectorsMark = vehicleCapability * DualMark + vehicleIncomeR;


			//Проверка возиожности введения (замены) судна 
			//с точки зрения капиталовложений:
			if(lInvestmentYearLimit > 0){// lInvestmentYearLimit если == 0, то нет ограничений
				//Land:
				if (bIncludeLandStructCalc){
					LandInvestDelta = CMarineTechEconomicEng::Instance()->GetLandStructExpenciesYearDeltaSt2(pRouter, pVehicle, VehicleNum);
				}
				//end of Land

				//Если стоимость судов претендентов больще стоимости
				//судов на маршруте, то требуется проверка
				BuildCoastYearPay = CFleetTechEconomicEng::Instance()->VehicleBuildCoastYearPay(pVehicle);
				InvestDelta = (BuildCoastYearPay*VehicleNum - BuildCoastYearVehicleRout*vehicleNumberR);
				if (InvestDelta > 0){
					// Проверка возможности капиталовложений:
					if((lInvestmentYear+InvestDelta+LandInvestDelta) > lInvestmentYearLimit){
						pVehicle = pVehicle->GetNextVehicle();               
						continue;
					}
				}
			}
			//			
			


			flagGetVehicle = 0;
			nVehicleExist = pVehicle->IsExist();
			nVehicleExistForce = pVehicle->IsExistForce();
			if (nVehicleExist){
				if (pVehicle->IsExistDesign() == 0){
					nVehicleExistNum = pVehicle->GetExistNumRemain();
					if (dVehicleNum > nVehicleExistNum){
						pVehicle = pVehicle->GetNextVehicle();
						continue;				
					}
				}
				AdvantPrice = pVehicle->GetExistGenAdvantPrice();
			}
				
			VectorsMark = vehicleCapability * DualMark + vehicleIncome; //vehicleCapability - для всех судов линии

			if (VectorsMark >0){
				if (flagRouterFound == 0) {
					flagRouterFound = 1;
				}
/*
				if (VectorsMark > VectorsMarkBest){
					VectorsMarkBest = VectorsMark;
					
					pRouterIn  = pRouterL;
					pRouterIn->vehicleNumber = VehicleNum;
					pRouterIn->dVehicleNumber = dVehicleNum;
					pRouterIn->vehicleIncome = vehicleIncome;

					//pRouterOut = pRouter;

				}
*/
				if (VectorsMark > VectorsMarkBest){
					if (nVehicleExist){//Ship exists
						if (!nVehicleBestExistForce || nVehicleExistForce)
							flagGetVehicle = 1;
					}
					else {//Ship doesn't exist
						if (nVehicleBestExist == 0){ //previouse(best) Routers' vehicle - doesn't exist
							flagGetVehicle = 1;					
						} 
						else { //previouse(best) Router's vehicle - exists
							//If Current Router/Vehicle MuchBetter ?..:
							if(!nVehicleBestExistForce || VehicleBestAdvantPrice != -1){
								if (VectorsMark > (VehicleBestAdvantPrice + VectorsMarkBest))
									flagGetVehicle = 1;
							}
						}//end: previouse(best) Router's vehicle - exists
					}//end: Ship doesn't exist
				}//end: if (VectorsMark > VectorsMarkBest)
				else { // VectorsMark <= VectorsMarkBest
					if (nVehicleExist && (!nVehicleBestExistForce)) { //Ship Exist; у него есть преимущество ...
						if (nVehicleExistForce || (AdvantPrice == -1) // существующее судно имеет неоспоримое преимущество
							|| ((VectorsMark + VehicleBestAdvantPrice) > VectorsMarkBest)){
									flagGetVehicle = 1;  
						}
					}
				}
				if (flagGetVehicle == 1){

					if (nVehicleBestExist && (pRouterIn->GetVehicle()->IsExistDesign() == 0)) { //выходящее судно - существующее
						pRouterIn->GetVehicle()->UpdateExistNumRemainIncrem(VehicleNum);
					}
					if (nVehicleExist && (pVehicle->IsExistDesign() == 0)){
						pVehicle->UpdateExistNumRemainDecrem(VehicleNum);
					} 
				
					VectorsMarkBest = VectorsMark;
					//pRouterIn  = pRouterL; //!!!! error
					//pRouterIn->SetVehicle(pVehicle);
					//pRouterIn->vehicleNumber = VehicleNum;
					//pRouterIn->dVehicleNumber = dVehicleNum;
					//pRouterIn->vehicleIncome = vehicleIncome;

					pRouterL->vehicleNumber = VehicleNum;
					pRouterL->dVehicleNumber = dVehicleNum;
					pRouterL->vehicleIncome = vehicleIncome;
					
					pRouterIn->CopyDataFromRouter(pRouterL);
					pRouterIn->next = 0;

					nVehicleBestExist = nVehicleExist;
					nVehicleBestExistForce = nVehicleExistForce;
					VehicleBestAdvantPrice = AdvantPrice;
					LandInvestDeltaBest = LandInvestDelta;
					InvestDeltaBest = InvestDelta;
				}

			}
			pVehicle = pVehicle->GetNextVehicle();  
		}
		pRouter = pRouter->GetNextRouter();

    }

	pRouterIn->GetBasePort()->RestoreCargoRequerements();

	if (flagRouterFound){
		//MessageBox(NULL,"Z > 0 found","VectorsMarksCompute",MB_OK);
		m_pOutView->TextOut("2nd stage of Optimization - Route with better mark found, continue ...");
		//fprintf (stream,"\n\n*******************************************************\n");
		//fprintf (stream,"Судно вводимое в базис: %s\n",pRouterIn->GetVehicle()->GetTypeName());
		//fprintf (stream,"Грузоподъемность Судна: %d\n",pRouterIn->GetVehicle()->GetLoad());
		//fprintf (stream,"Количество судов: %d\n", pRouterIn->GetVehicleNumber());
		//fprintf (stream,"Количество судов нецелое: %f\n", pRouterIn->GetDVehicleNumber());
		//fprintf (stream,"Z mark: %f\n", VectorsMarkBest);
		//fprintf (stream,"Список линий (пунктов) маршрута: \n");
		pLine = pRouterIn->GetLine();
		while(pLine){
			PNB_num = pLine->GetPNBnum();
			//fprintf (stream,"ПНБ: %s\n",pRouterIn->GetBasePort()->GetPNB(PNB_num).name);
			//fprintf (stream,"груз: %d\n", pLine->GetCargoType());
			pLine = pLine->GetNextLine();
		}
		GoalFunctionCompute();
		//fprintf (stream,"GoalFunction: %d\n\n\n", this->GoalFunction);

		if (lInvestmentYearLimit > 0){
			lInvestmentYear += InvestDeltaBest;
			if (bIncludeLandStructCalc)
				lInvestmentYear += LandInvestDeltaBest;

			CFleetTechEconomicEng::Instance()->SetInvestmentYear(lInvestmentYear);
		}

		//fclose(stream);
		return 1;

	} else {
		//MessageBox(NULL,"Z оценка маршрута > 0 НЕ НАЙДЕНА","2-этап оптимизации: VectorsMarksCompute()",MB_OK);
		//fprintf (stream,"\n\n*******************************************************\n");
		//fprintf (stream,"Z > 0 NOT FOUND\n");
		//fclose(stream);
		return 10;
	}

}

int COptimizationSt2LpDp::RouterCreate(CRouterL *&pRouter)
{

	//FILE *stream;
	//stream = fopen( "E:\\Users\\mv\\Log\\OptimSt2_RCreate.txt", "w" );


	//CVehicle *pVehicleIn = pRouterL->GetVehicle();
	//CLine *pLineIn = pRouterL->GetLine();
	//int cargo_type = pLineIn->GetCargoType();
	//if (!(pVehicleIn->GetCargoCompatibility(cargo_type)))
	//	return 0;
	


	//CPort *pBasePort = pRouterL->GetBasePort();
	
	pRouter->GetBasePort()->RestoreCargoRequerements();

	m_DynamicRoutersList = 0;
	m_DynamicRoutersListPrev = 0;


	//int flRouterExist = 1;
	int flBestRouterFound = 0;
	int res;


	if (CreateDynamicRoutersFirst(pRouter)==0)
		return 0;
	
	//Выяснение количества линий в Районе {begin}
	int number_OfLines = pRouter->GetBasePort()->GetLinesNumber();
	//Выяснение количества линий в Районе {END}
	int step = 1; //шаг добавления звена к маршруту

  	while(1){
		if (step > number_OfLines)
			break;	
		res = CreateDynamicRoutersNext();
		if	(res == 1){ //Переходим к следующему шагу построения маршрутов.
			UpdateDynamicRoutersListPrev();
			step++;
		}
		else if (res == 10)
			break;
		else {//(res == 0)
			MessageBox(NULL,"Ощибка построения динамических маршрутов!","2-этап оптимизации: CreateDynamicRoutersNext()",MB_OK);
			m_pOutView->TextOut("Ощибка построения динамических маршрутов! 2-этап оптимизации: CreateDynamicRoutersNext()");
			return 0;
		}

	}

	if (FindDynamicRouterBest()==0){
		MessageBox(NULL,"Ощибка построения динамических маршрутов!","2-этап оптимизации: FindDynamicRouterBest()",MB_OK);
		m_pOutView->TextOut("Ощибка построения динамических маршрутов!.  2-этап оптимизации: FindDynamicRouterBest()");
		return 0;
	}

	pRouter = m_RouterBestOnStep;
	if (pRouter == 0)
		return 0;
	
	normRouter(pRouter);

	return 1;
}

void COptimizationSt2LpDp::appendDynRList(CRouterL *pRouter)
{
    if(m_DynamicRoutersList == 0)
        m_DynamicRoutersList = pRouter;
    else
        m_at_endDynRList->next = pRouter;
            
    m_at_endDynRList = pRouter;
}

void COptimizationSt2LpDp::appendDynRListPrev(CRouterL* pRouter)
{
    if(m_DynamicRoutersListPrev == 0)
        m_DynamicRoutersListPrev = pRouter;
    else
        m_at_endDynRListPrev->next = pRouter;
            
    m_at_endDynRListPrev = pRouter;

}

/*
int COptimizationSt2LpDp::CreateDuplicateDynRList()
{
// фунекция предназначена для создания временного плана, используемого для
// построения маршрута.  Временный план создается многократно, и должен быть удален после построения маршпута.

	CPort *pBasePort;		//базовый пункт маршрута Базового Плана
	CVehicle *pVehicleBP;	//судно маршрута Базового Плана

	int PNB_num, cargo_type;


	CRouter *pBasePlan = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();
    if (pBasePlan == NULL){
			//fprintf (stream,"BasePlan пуст\n");
        	return 0;
    }
    else{
        while(pBasePlan){	
			pBasePort =		pBasePlan->GetBasePort();
			PNB_num =		pBasePlan->GetLine()->GetPNBnum();
			cargo_type =	pBasePlan->GetCargoType();
			pVehicleBP =	pBasePlan->GetVehicle();
			
			switch(cargo_type){
			 case 1:
				 cargoType = CCargo::Coal;
				 break;
			 case 2:
				 cargoType = CCargo::General;
				 break;
			 case 3:
				 cargoType = CCargo::Oil;
				 break;
			 case 4:
				 cargoType = CCargo::Vegetables_fruits;
				 break;
			 }

			CRouterL *pRouter = new CRouterL(pBasePort, PNB_num, cargo_type);

			pRouter->SetVehicle(pVehicleBP);
			pRouter->vehicleIncome = pBasePlan->GetVehicleIncome();
			pRouter->vehicleNumber = pBasePlan->GetVehicleNumber();
			pRouter->dVehicleNumber = pBasePlan->GetDVehicleNumber();


			_instance->appendDynRList(pRouter);

            pBasePlan = pBasePlan->GetNextRouter();  
        }
    }


	return 0;
}
*/


/*
int COptimizationSt2LpDp::RemoveDynamicRoutersList(CRouterL *)
{
	return 1;
}
*/



int COptimizationSt2LpDp::CreateDynamicRoutersFirst(CRouterL * pRouterL)
{
	CPort	*pBasePortIn, *pBasePort;
	CVehicle *pVehicleIn;
	int cargo_type, cargo_typeIn, PNB_num, PNBRequiredCargo;
	int LineCount, LineCountMax, VehicleLoad;
	int routers_count = 0;
	int vehicleOperationDaysInYearAll, daysRemain, RouterTime; //DaysInRouter
	//int VehicleNumber;
	//double dVehicleNumber;
	float dLineCount;


	CRouterL *pRouter = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();
    if (pRouter == NULL){
        return 0;
    }


	pVehicleIn = pRouterL->GetVehicle();
	VehicleLoad = pVehicleIn->GetLoad();

	pBasePortIn = pRouterL->GetBasePort();
	cargo_typeIn = pRouterL->GetCargoType();

	vehicleOperationDaysInYearAll = CFleetTechEconomicEng::Instance()->GetVehicleOperationsDaysInYear(pVehicleIn->GetRegisterClass(),pBasePortIn->GetPortRegion());
	pRouterL->SetVehicleOperationsDaysRemain(vehicleOperationDaysInYearAll);

    while(pRouter){
		pBasePort = pRouter->GetBasePort();
		if (pBasePort != pBasePortIn){
			pRouter = pRouter->GetNextRouter();
			continue;
		}
		cargo_type =	pRouter->GetCargoType();
		if(cargo_type != cargo_typeIn){
			pRouter = pRouter->GetNextRouter();
			continue;
		}
		PNB_num = pRouter->GetLine()->GetPNBnum();

        if(CFleetTechEconomicEng::Instance()->VehicleRouterPLineValidation(pBasePortIn,PNB_num,cargo_type,pVehicleIn)==0){
			pRouter = pRouter->GetNextRouter();
			continue;
		}

		//Вычисление числа повторений линии в марщруте 
		//Line counter <begin>
		PNBRequiredCargo = pRouter->GetBasePort()->GetPNBRequiredCargo(PNB_num,cargo_type);
		if (PNBRequiredCargo == 0){
			pRouter = pRouter->GetNextRouter();
			continue;	
		}
		LineCount = (int)floor(double(PNBRequiredCargo)/VehicleLoad);
		if(LineCount == 0){
			pRouter = pRouter->GetNextRouter();
			continue;	
		}

		dLineCount = (float)PNBRequiredCargo/VehicleLoad;
		LineCount = (int)ceil(dLineCount);

		if ((LineCount - dLineCount) > 0.5)
			LineCount = (int)floor(dLineCount);



		LineCountMax = CFleetTechEconomicEng::Instance()->VehicleRouterNumberForNavigation(pBasePortIn,PNB_num,cargo_type,pVehicleIn);
		if (LineCount > LineCountMax)
			LineCount = LineCountMax;

/*		if (LineCount == LineCountMax)
			dVehicleNumber = 1.000;
		else 
			dVehicleNumber = ((double)LineCount)/LineCountMax;
		if (dVehicleNumber < 0.5){
			pRouter = pRouter->GetNextRouter();
			continue;	
		}
		VehicleNumber = ceil(dVehicleNumber);
*/

		//Line counter <end>		

		CRouterL *pDynRouter = new CRouterL(pBasePortIn, PNB_num, cargo_type);
		pDynRouter->SetVehicle(pVehicleIn);
		//pDynRouter->vehicleNumber = VehicleNumber;
		//pDynRouter->dVehicleNumber = dVehicleNumber; 
		pDynRouter->GetLine()->SetCount(LineCount);
		RouterTime = pDynRouter->GetRouterTime();
		daysRemain = vehicleOperationDaysInYearAll - RouterTime;
		pDynRouter->SetVehicleOperationsDaysRemain(daysRemain);

		appendDynRList(pDynRouter);
		if(m_DynamicRoutersList == 0) //test tmp
			   return 0;
		pRouter = pRouter->GetNextRouter();
		routers_count++;
	}	

	if (routers_count == 0)
		return 0; // не построено ни одного маршрута
	


	return 1;
}


int COptimizationSt2LpDp::CreateDynamicRoutersNext()
{
	CRouterL *pRouter;
	CLine *pLine, *pLineBest, *pLineR;
	CPort *pBasePort;
	CVehicle *pVehicle;
	CCargo cargoType;
  	int PNB_num, PNB_numBest, PNB_id, id, fl, num, PNB_numR, cargoDone,
		cargo_type, cargo_typeR, cargo_typeBest, LineCount, LineCountMax, LineCountBest, LineCountR,
		OperationDaysInYear, //OperationDaysInRouterPrev, 
		OperationDaysRemain, OperationDaysRemain_0, OperationDaysRemainBest, DistancePortPNB, //DistancePNBPNB, 
		DistanceBest, DistanceInit,
		PNBRequiredCargo, //cargo_remains, PNBRequiredCargoRemain,
		VehicleLoad, RouterTime, RouterTimePrev;
    float TripTimeDays, TripTimeDaysAll;
	float dLineCount, dVehicleLoading; //dReminder;
	float LoadTimeDays; // = 1; //stab, TBD
	int routers_count = 0;

	if (m_DynamicRoutersList == 0)
	   return 0;

	pRouter = m_DynamicRoutersList;
	pBasePort = pRouter->GetBasePort();
	DistanceInit = 1000000;

	int requered_cargo[MAXPNBNUMBER][5];
	int num_pnb, num_cargo;
	for (num_pnb = 0; num_pnb < MAXPNBNUMBER; num_pnb++){
		id = pBasePort->GetPNB(num_pnb).id;
		if (id == 999)
			continue;
		for (num_cargo = 1; num_cargo <= 4; num_cargo++){
			requered_cargo[num_pnb][num_cargo] = pRouter->GetBasePort()->GetPNBRequiredCargo(num_pnb,num_cargo);
		}
	}

	while(pRouter){
		OperationDaysRemain = pRouter->GetVehicleOperationsDaysRemain();
		if (OperationDaysRemain <= 0){
			pRouter = pRouter->GetNextRouter();
			continue;
		}

		OperationDaysRemain_0 = OperationDaysRemain;

		pVehicle = pRouter->GetVehicle();
		VehicleLoad = pVehicle->GetLoad();
		OperationDaysInYear = CFleetTechEconomicEng::Instance()->GetVehicleOperationsDaysInYear(pVehicle->GetRegisterClass(), pBasePort->GetPortRegion());
		RouterTimePrev = pRouter->GetRouterTime();

	
		//add best line to RouterL <begin>
		//Добавление лучшей линии.
		//Критерий: минимальное расстояние перехода между линиями: 
		//			расстояние между ПНБ, конечными пунктами, двух линий.
		//Проверяются ограничения: 
		//		совместимость линии и заданного судна маршрута;
		//		ограничение эксплуатационного периода;
		pLine = pRouter->GetLine();
		PNB_num = pLine->GetPNBnum();
		PNB_id = pBasePort->GetPNB(PNB_num).id;
		cargo_type = pLine->GetCargoType();


		DistanceBest = DistanceInit;
	
//		pRouter1 = DynamicRoutersListPrev;
//		while(pRouter1)

		fl = 0;
		for (num_cargo = 1; num_cargo <= 4; num_cargo++){
			for (num = 0; num < MAXPNBNUMBER; num++){
				
				id = pBasePort->GetPNB(num).id;
				if (id == 999)
					continue;
				
				// дообслуживание (переход в другой район)не реализовано еще. TBD.
				// рассматриваются только пункты одного района.	
				
				//if (num == PNB_num)   //24.01.04
				//	continue;
				
				PNBRequiredCargo = requered_cargo[num][num_cargo];
				if (PNBRequiredCargo == 0)
					continue;	
				OperationDaysRemain = OperationDaysRemain_0;
				
				//Выяснение количества рассматриваемого груза
				//уже перевезенного на данном маршруте
				pLineR = pRouter->GetLine();
				while(pLineR){
					cargo_typeR = pLineR->GetCargoType();
					PNB_numR = pLineR->GetPNBnum();
					if ((cargo_typeR == num_cargo) && (PNB_numR == num)) {
						LineCountR = pLineR->GetCount();
						cargoDone =  VehicleLoad * LineCountR;
						PNBRequiredCargo -= cargoDone;
						if (PNBRequiredCargo < 0)
							PNBRequiredCargo = 0;
						break;
					}
					pLineR = pLineR->GetNextLine();
				}
				if (PNBRequiredCargo == 0)
					continue;	

				//проверка загрузки судна
				dVehicleLoading = (float)PNBRequiredCargo/VehicleLoad;
				if(dVehicleLoading < 0.9)
					continue;
				
				if(CFleetTechEconomicEng::Instance()->VehicleRouterPLineValidation(pBasePort,num,num_cargo,pVehicle)==0)
					continue;
				
				DistancePortPNB = pBasePort->GetDistancePortPNB(id);
				LoadTimeDays = pBasePort->GetPNBShipLoadingTime(num, pVehicle, num_cargo);
				TripTimeDays = ((2*(float)DistancePortPNB/pVehicle->GetSpeed())/24)+ LoadTimeDays ;
				if (OperationDaysRemain - TripTimeDays < 0)
					continue;
				RouterTime = RouterTimePrev + (int)ceil(TripTimeDays);
				if (RouterTime >= OperationDaysInYear)
					continue;

				LineCountMax = (int)floor(OperationDaysRemain/TripTimeDays);

				dLineCount = (float)PNBRequiredCargo/VehicleLoad;
				LineCount = (int)ceil(dLineCount);
				if ((LineCount - dLineCount) > 0.5)
					LineCount = (int)floor(dLineCount);

				if (LineCount > LineCountMax)
					LineCount = LineCountMax;

				//Уточнение допустимого повторения линий
				//Допускается несоответствие :(RouterTime-OperationDaysInYear)>2
				TripTimeDaysAll = TripTimeDays*LineCount;
				RouterTime = RouterTimePrev + (int)ceil(TripTimeDaysAll);
				if ((LineCount > 1)&&((RouterTime-OperationDaysInYear)>3)){
					int nCount = LineCount;
					int flag1 =1;
					while(flag1 == 1){
						nCount-=1;
						if (nCount == 1)
							flag1 = 0;
						TripTimeDaysAll = TripTimeDays*nCount;
						RouterTime = RouterTimePrev + (int)ceil(TripTimeDaysAll);
						if(RouterTime - OperationDaysInYear < 2)
							flag1 = 0;
					}
					LineCount = nCount;
					OperationDaysRemain -= (int)ceil(TripTimeDaysAll);
				}
					
				
				//DistancePNBPNB = pBasePort->GetDistancePNBPNB(PNB_id, id);
				//if((DistancePNBPNB < DistanceBest)&& (DistancePNBPNB > 0)){
					fl = 1; //valid router
					//DistanceBest = 	DistancePNBPNB;
					LineCountBest = LineCount;
					PNB_numBest = num;
					cargo_typeBest = num_cargo;
					OperationDaysRemainBest = OperationDaysRemain;
				//}	
			}//end: for (num =  ...
		} //end: for (num_cargo = ... 
		
		//add best line to RouterL <end>
		if(fl == 1){ //valid router found
			pLineBest = new CLine(PNB_numBest, cargo_typeBest);
			pLineBest->SetCount(LineCountBest);
			pRouter->appendLine(pLineBest);
			pRouter->SetVehicleOperationsDaysRemain(OperationDaysRemainBest);
			int RouterTime1 = pRouter->GetRouterTime();
			if (RouterTime1 > OperationDaysInYear){
				RouterTime1--; //stab 
			}

			/*
			PNBRequiredCargoRemain = requered_cargo[PNB_numBest][cargo_typeBest] - LineCountBest * VehicleLoad;
			if (PNBRequiredCargoRemain < 0)
				PNBRequiredCargoRemain = 0;
			pBasePort->SetPNBRequiredCargoRemain(PNB_numBest, cargo_typeBest, PNBRequiredCargoRemain);
			*/

			routers_count++;
		}


		pRouter = pRouter->GetNextRouter();
	}

	if (routers_count == 0)
		return 10;  // не построено ни одного маршрута данного шага (звенности), 
					//т.е. лучший маршрут должен быть выбран из маршрутов 
					// предыдущего шага

	return 1;
}
// End of CreateDynamicRoutersNext()



int COptimizationSt2LpDp::FindDynamicRouterBest()
{
// Критерий: 
// Минимальное суммарное расстояние перехода между линиями маршрута.
// При наличии единственного базового пункта (рассмотрении одного района)
// данное требование не играет существенной роли и означает 
// что линии в маршруте сгруппированы по принципу минимума расстояния 
// между ПНБ, конечными пунктами двух смежных линий

	CRouterL *pRouter;
	CLine *pLine;
	CPort *pBasePort;
	CVehicle *pVehicle;
  	int PNB_num, PNB_id, PNB_id1, DistanceBest,Distance;


	pRouter = m_DynamicRoutersList;
	if (pRouter == NULL)
		return 0;

	Distance = 0;
	DistanceBest = 100000;

	CRouterL* pRouter_best = 0;


	while(pRouter){
		pBasePort = pRouter->GetBasePort();
		pVehicle = pRouter->GetVehicle();
		pLine = pRouter->GetLine();
		PNB_num = pLine->GetPNBnum();
		PNB_id = pBasePort->GetPNB(PNB_num).id;
		pLine=pLine->GetNextLine();
		while(pLine){
			PNB_id1 = PNB_id;
			PNB_num = pLine->GetPNBnum();
			PNB_id = pBasePort->GetPNB(PNB_num).id;
			Distance+= pBasePort->GetDistancePNBPNB(PNB_id, PNB_id1);
			pLine=pLine->GetNextLine();
		}
		if ((Distance < DistanceBest)){
			pRouter_best = pRouter;
			DistanceBest = Distance;
		}
		pRouter = pRouter->GetNextRouter();
	}

	if (m_RouterBestOnStepPrev == 0)
		m_RouterBestOnStep = m_RouterBestOnStepPrev = pRouter_best;
	else{
		m_RouterBestOnStepPrev = m_RouterBestOnStep;
		m_RouterBestOnStep = pRouter_best;
	}
	//if (CheckPNBsCargoRequirments(m_RouterBestOnStep) == 0)
	//				MessageBox(NULL,"FindDynamicRouterBest:\nPNB Requered Cargo Problem","STEP3. CheckPNBsCargoRequirments",MB_OK);
	
	if (m_RouterBestOnStep == NULL)
		return 0;

	return 1;
}



int COptimizationSt2LpDp::GoalFunctionCompute()
{
    int Function = 0;
	//CRouter *pRouter = RoutersList;
	CRouterL *pRouter = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();

    if (pRouter == NULL){
		MessageBox(NULL,"Ошибка: Список маршрутов - пуст!","2-этап оптимизации: GoalFunctionCompute()",MB_OK);
 		m_pOutView->TextOut("Ошибка: Список маршрутов - пуст! 2-этап оптимизации: GoalFunctionCompute");
		return 0;        
    }
    else{
        while(pRouter){
			//Function += (int)(pRouter->GetVehicleIncome() * pRouter->GetDVehicleNumber());
			Function += (int)pRouter->GetVehicleIncome();
            pRouter = pRouter->next;  
        }
    }

	//Land struct:
	BOOL bIncludeLandStructCalc = FALSE;
	bIncludeLandStructCalc = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileInt("FleetOptimizer","IncludeLandStructCalc",0);	
	if (bIncludeLandStructCalc){
		int LandStructExpenciesYear = CMarineTechEconomicEng::Instance()->GetLandStructExpenciesYearAllSt2();
		Function += LandStructExpenciesYear;
	} 
	// end of Land struct
	
	GoalFunction = Function;

	return Function;
}

int COptimizationSt2LpDp::print()
{
	CLine *pLine;
	int PNBnum, PNB_id, cargo_type,DistancePortPNB, nTripDays, nLineCount,
		vehicleOperationDaysInYearAll/*, nRouterTime1*/;
	float LoadTimeDays, TripTimeDays, RouterTime;
	CVehicle *pVehicle;
	CString sCargoType = "";

	CRouterL *pRouter = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();

    FILE *stream;
    //stream = fopen( "E:\\Users\\mv\\Log\\Step2Plan.txt", "w" );
	CString LogFile = ((CFleetOptimizerApp*)AfxGetApp())->GetProfileString("FleetOptimizer","LogFileOptimizStep2Plan");	
    stream = fopen(LogFile, "w" );

    if (pRouter == NULL){
        fprintf (stream,"Список маршрутов пуст. 2-этап оптимизации. print()\n");
		fclose(stream);
		return 1;
    }

    while(pRouter){
		RouterTime = 0;
		pVehicle = pRouter->GetVehicle();
	    pLine = pRouter->GetLine();
        fprintf (stream,"Базовый порт: %s\n",pRouter->GetBasePort()->GetPortName());
        fprintf (stream,"Судно: %s\n",pRouter->GetVehicle()->GetTypeName());
        fprintf (stream,"Грузоподъемность судна: %d т\n",pRouter->GetVehicle()->GetLoad());
        while(pLine){
			PNBnum = pLine->GetPNBnum();
			PNB_id = pRouter->GetBasePort()->GetPNB(PNBnum).id;
			cargo_type = pLine->GetCargoType();
			nLineCount = pLine->GetCount();
			fprintf (stream,"ПНБ: %s\n",pRouter->GetBasePort()->GetPNB(PNBnum).name);
			switch(cargo_type){
			 case 1:
				 sCargoType = "уголь";
				 break;
			 case 2:
				 sCargoType = "генеральный";
				 break;
			 case 3:
				 sCargoType = "ГСМ";
				 break;
			 case 4:
				 sCargoType = "овоши-фрукты";
				 break;
			}
			//fprintf (stream,"\tгруз: %d\n", cargo_type);
            fprintf (stream,"\tгруз: %s\n", sCargoType.GetString());
			fprintf (stream,"\tколичество рейсов: %d\n",nLineCount);
			//fprintf (stream,"Количество груза необходимое ПНБ: %d\n", pRouter->GetBasePort()->GetPNBRequiredCargoFirst(PNBnum,cargo_type));
			//fprintf (stream,"Количество груза необходимое ПНБ. Остаток: %d\n", pRouter->GetBasePort()->GetPNBRequiredCargo(PNBnum,cargo_type));
			//time begin
			DistancePortPNB = pRouter->GetBasePort()->GetDistancePortPNB(PNB_id);
			LoadTimeDays = pRouter->GetBasePort()->GetPNBShipLoadingTime(PNBnum, pVehicle, cargo_type);
			TripTimeDays = ((2*(float)DistancePortPNB/pVehicle->GetSpeed())/24)+ LoadTimeDays ;
			nTripDays = int(ceil(TripTimeDays));
			//fprintf (stream,"\tПродолжит. рейса (дней): %d\n", nTripDays);
			fprintf (stream,"\tпродолжительность рейса (дней): %f\n", TripTimeDays);
			//time end
			RouterTime += TripTimeDays*nLineCount;
			pLine = pLine->GetNextLine();
		}
        //fprintf (stream,"Провозоспособность судна: %d\n",pRouter->GetVehicleCargoForNavigation());
		vehicleOperationDaysInYearAll = CFleetTechEconomicEng::Instance()->GetVehicleOperationsDaysInYear(pVehicle->GetRegisterClass(),pRouter->GetBasePort()->GetPortRegion());
		fprintf (stream,"Эксплуатационный период (дней): %d\n",vehicleOperationDaysInYearAll);
		fprintf (stream,"Время выполнения маршрута (дней): %d\n",(int)(ceil(RouterTime)));

		//nRouterTime1 = pRouter->GetRouterTime();
		//fprintf (stream,"Время выполнения маршрута1 (дней): %d\n",nRouterTime1);
		
        fprintf (stream,"Количество судов: %f\n", pRouter->GetDVehicleNumber());
        //24.01.04fprintf (stream,"Количество судов целое: %d\n", pRouter->GetVehicleNumber());
        ///////fprintf (stream,"Доход одного судна на маршруте: %d\n\n", pRouter->GetVehicleIncome());
        fprintf (stream,"Доход судов на маршруте: %d\n\n", ((pRouter->GetVehicleIncome())*(pRouter->GetVehicleNumber())));
        fprintf (stream,"*************************************\n\n\n");


        pRouter = pRouter->next;  
    }




	fclose(stream);
	return 1;
}



int COptimizationSt2LpDp::UpdateDynamicRoutersListPrev()
{
	int PNB_num, cargo_type;
	CLine* pLine;
	CVehicle* pVehicle;


	RemoveDynamicRoutersListPrev();

	CRouterL *pt = m_DynamicRoutersList;
	CPort *pBasePort = pt->GetBasePort();

	while (pt){
		pVehicle = pt->GetVehicle();
		pLine = pt->GetLine();
		PNB_num = pLine->GetPNBnum();
		cargo_type = pLine->GetCargoType();
		CRouterL *pRouterNew = new CRouterL(pBasePort, PNB_num, cargo_type);
		pLine = pLine->GetNextLine();
		while(pLine){
			PNB_num = pLine->GetPNBnum();
			cargo_type = pLine->GetCargoType();
			CLine *pLineNew = new CLine(PNB_num, cargo_type);
			pRouterNew->appendLine(pLineNew);
			pLine = pLine->GetNextLine();
		}
		pRouterNew->SetVehicle(pVehicle);

		//if (CheckPNBsCargoRequirments(pRouterNew) == 0)
		//		MessageBox(NULL,"UpdateDynamicRoutersListPrev:\nPNB Requered Cargo Problem","STEP3. CheckPNBsCargoRequirments",MB_OK);

		appendDynRListPrev(pRouterNew);
		pt = pt->next;	


	}

	
	return 1;
}

void COptimizationSt2LpDp::RemoveDynamicRoutersListPrev()
{
	CRouterL *pt = m_DynamicRoutersListPrev;
	
	while (pt){
		CRouterL *tmp = pt;
		pt = pt->next;
		delete tmp;
	}
	m_DynamicRoutersListPrev = m_at_endDynRListPrev = 0;
}


void COptimizationSt2LpDp::RemoveDynamicRoutersList()
{
	CRouterL *pt = m_DynamicRoutersList;
	
	while (pt){
		CRouterL *tmp = pt;
		pt = pt->next;
		delete tmp;
	}
	m_DynamicRoutersList = m_at_endDynRList = 0;
}


int COptimizationSt2LpDp::normRouter(CRouterL *&pt)
{
	int LineCount;
	if (pt == 0)
		return 0;

	CPort *pBasePort = pt->GetBasePort();
	CVehicle* pVehicle = pt->GetVehicle();
	int daysRemains = pt->GetVehicleOperationsDaysRemain();
	CLine* pLine = pt->GetLine();
	int PNB_num = pLine->GetPNBnum();
	int cargo_type = pLine->GetCargoType();
	LineCount = pLine->GetCount();
	CRouterL *pRouter = new CRouterL(pBasePort, PNB_num, cargo_type);
	pRouter->SetVehicle(pVehicle);
	pRouter->SetVehicleOperationsDaysRemain(daysRemains);
	pRouter->GetLine()->SetCount(LineCount);
	pLine = pLine->GetNextLine();
	while(pLine){
		PNB_num = pLine->GetPNBnum();
		cargo_type = pLine->GetCargoType();
		LineCount = pLine->GetCount();
		CLine *pLineNew = new CLine(PNB_num, cargo_type);
		pLineNew->SetCount(LineCount);
		pRouter->appendLine(pLineNew);
		pLine = pLine->GetNextLine();
	}

	CRouterL *tmp = pt;
	pt = pRouter;
	delete tmp;

	//if (CheckPNBsCargoRequirments(pt) == 0)
	//				MessageBox(NULL,"normRouter:\nPNB Requered Cargo Problem","STEP3. CheckPNBsCargoRequirments",MB_OK);
	

	return 1;
}



int COptimizationSt2LpDp::VehicleSCapabilityOnRouterCompute(CRouterL *pRouter)
//capability of one vehicle on router
{
	int vehicle_load, LineCount,
		VehicleCapability, VehicleCapabilityOnLine;
	
	CLine* pLine = pRouter->GetLine();
	if (pLine == NULL)
		return 0;
	vehicle_load = pRouter->GetVehicle()->GetLoad();
	VehicleCapability = 0;
	while(pLine){
		LineCount = pLine->GetCount();
			if (LineCount == 0){
				pLine = pLine->GetNextLine();
				continue;
			}
		VehicleCapabilityOnLine = vehicle_load * LineCount;
		VehicleCapability += VehicleCapabilityOnLine;
		pLine = pLine->GetNextLine();
	}
	
	return VehicleCapability;
}

int COptimizationSt2LpDp::cleanDummyRouters()
{
	double dVehicleNum;	
	CRouterL *pRouter, *pRouterPrev, *pRouterTmp;
	int nLineCount, PNBRequiredCargo, PNBRequiredCargo0, cargoRemain, cargoDone, PNB_num, cargo_type;
	int nRouterTime, vehicleOperationDaysInYearAll;
	double VechicleUsageCoef;

	CLine* pLine = NULL;
	CVehicle* pVehicle;

	CRouterL *pRouterHead = CBasePlanSt2LpDp::Instance()->GetRoutersListHead();
	if (pRouterHead == NULL)
		return 0;

	pRouterPrev = pRouter = pRouterHead;
    while(pRouter){
		pVehicle = pRouter->GetVehicle();
		dVehicleNum = pRouter->GetDVehicleNumber();
		nLineCount = pRouter->GetLine()->GetCount();
		nRouterTime = pRouter->GetRouterTime();
		vehicleOperationDaysInYearAll = CFleetTechEconomicEng::Instance()->GetVehicleOperationsDaysInYear(pVehicle->GetRegisterClass(),pRouter->GetBasePort()->GetPortRegion());
		VechicleUsageCoef = (double)nRouterTime/vehicleOperationDaysInYearAll;

		//if ((dVehicleNum <= 0.2) || (pRouter->GetLine()->GetCount() == 0)){
		if ((dVehicleNum <= 0.2) || (nLineCount == 0) || (VechicleUsageCoef < 0.2)){
			if ((nLineCount > 0) && (pVehicle != NULL) && (dVehicleNum != NULL)){
				pLine = pRouter->GetLine();
				while(pLine){
					cargo_type = pLine->GetCargoType();
					PNB_num = pLine->GetPNBnum();
					cargoDone =  (pVehicle->GetLoad())* nLineCount;
					PNBRequiredCargo = pRouter->GetBasePort()->GetPNBRequiredCargo(PNB_num,cargo_type);
					PNBRequiredCargo0 = pRouter->GetBasePort()->GetPNBRequiredCargoFirst(PNB_num,cargo_type);
					cargoRemain = PNBRequiredCargo + cargoDone;
					//if(cargoRemain > PNBRequiredCargo0){
					//	cargoRemain = PNBRequiredCargo0;
					//}
					pRouter->GetBasePort()->SetPNBRequiredCargoRemain0(PNB_num, cargo_type, cargoRemain);
					pRouter->GetBasePort()->SetPNBRequiredCargoRemain(PNB_num, cargo_type, cargoRemain);
					pLine = pLine->GetNextLine();
				}
			}

			if (pRouter == pRouterHead){
				// если первый маршрут - dummy - скопировать в него 
				// содержание следующего маршрута, и опять проверить (continue!),
				// не преводя указатель на следующий маршрут
				pRouter->CopyDataFromRouter(pRouter->next);
				//delete(pRouter->next);
				continue;
			}
			// маршрут - не первый; удалить, и перейти к следующему
			pRouterTmp = pRouter;
			pRouterPrev->next = pRouter->next;
			pRouter = pRouterPrev;
			//delete(pRouterTmp);
		}
		pRouterPrev = pRouter;
        pRouter = pRouter->next;  
	}
	
	return 1;
}

void COptimizationSt2LpDp::ResetExistVehicleNum()
{
	CVehicle* pVehicle;

    pVehicle = CVehiclesList::Instance()->GetVehicleListHead();
    while(pVehicle){   		
		if (pVehicle->IsExistForce()){
			pVehicle->UpdateExistNumRemainReset();
		}
		pVehicle = pVehicle->GetNextVehicle();
	}
}

int COptimizationSt2LpDp::IsStepDone()
{
	return m_bStepDone;
}
