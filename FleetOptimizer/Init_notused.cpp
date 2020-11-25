			if ((nVechicleExist) && (!pVehicle->IsExistDesign())) {
								// если выбрано существующее судно, и это судно не в стадии проектирования,
								// то нужно проверить, достаточно ли количество судов и, если нет -
								// дополнить линию расчетным судном. На последующих этапах расчетное судно 
								// м.б. заменено существующим.
								// В дальнейшем процедуру можно улучшить так, чтобы уже на даном этапе
								// линия могла дополняться существующими судами (рекурсия).

								nVehicleExistNum = pVehicle->GetExistNum();
								if ((VehicleNum > nVehicleExistNum) && (nVehicleBestExist == 0)){
									CVehicle *pVehicleBest2;
									int VehicleBestIncome2,VehicleBestIncomeAll2, VehicleBestNum2;
									int VehicleNum2 = 0; //количество судов на маршруте (целое)
									double dVehicleNum2, dVehicleNumBest2; //нецелое количество судов на маршруте
									int flagVehicleFound2;
									int vehicleIncome2, vehicleIncomeAll2;
									int BuildCoastYearPay2, OperationalExpensesYear2;
									int cargo_for_navigation2, VehicleCargoForNavigationBest2,
									
									// количество перевезенного груза: cargo_for_navigation
									int cargo_done = cargo_for_navigation;
									// Начать перебор судов сначала
									CVehicle *pVehicle2 = CVehiclesList::Instance()->GetVehicleListHead();
									int itmp2 = 1;
									pVehicleBest2 = 0;
									VehicleBestIncomeAll2 = -1000000;
									int nVehicleBestExist2 = 0;
									flagVehicleFound2 = 0;
									while(pVehicle2){ 
										if (itmp2 == 3){
											if (pVehicle2->IsExist())
												continue;
											//Проверка совместимости судна и линии:
											if(CFleetTechEconomicEng::Instance()->VehicleLineValidation(pBasePort,PNB_num,cargo_type,pVehicle2,cargo_done)==0){
												pVehicle2 = pVehicle2->GetNextVehicle();               
												continue;
											}
                
											flagVehicleFound2 = 1;
											//Определение Чистого Дохода судна на линии за навигацию.
											vehicleIncome2 = CFleetTechEconomicEng::Instance()->VehicleIncomeOnLineForYear(pBasePort,PNB_num,cargo_type,pVehicle2,VehicleNum2,dVehicleNum2,BuildCoastYearPay2, OperationalExpensesYear2, cargo_for_navigation2, cargo_done);
											vehicleIncomeAll2 = vehicleIncome2 * VehicleNum2;

											fprintf (stream1,"PNB: %s,\tГруз: %d,\tСудно: %s,\tГрузоподъемность: %d,\tКоличество: %d,\tСтроительная стоимость: %d,\tГодовой платеж (строит.стоим): %d,\tЭксплуатационные расходы за год: %d,\tIncomeYear: %d\n",pBasePort->GetPNB(PNB_num).name, cargo_type, pVehicle2->GetTypeName(),pVehicle2->GetLoad(),VehicleNum2, pVehicle2->GetCoast(),BuildCoastYearPay2,OperationalExpensesYear2,vehicleIncome2);
											
											if (vehicleIncomeAll2 > VehicleBestIncomeAll2){
 												VehicleBestIncomeAll2 = vehicleIncomeAll2;                       
												VehicleBestIncome2 = vehicleIncome2;
												pVehicleBest2 = pVehicle2;
												VehicleBestNum2 = VehicleNum2;
												dVehicleNumBest2 = dVehicleNum2;
												VehicleCargoForNavigationBest2 = cargo_for_navigation2;
											}
										}
										itmp2++;
										if (itmp2 == 4)
											itmp2 = 1;
										pVehicle2 = pVehicle2->GetNextVehicle();               

									}//Vehicle2 loop



