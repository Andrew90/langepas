#pragma once

#define __double_1 " %.1f" 
#define __double_2 " %.2f"
#define __int " %d"
#define __void ""
#define __bool " %s"
#define __const_char_ptr " %s"
#define _cat(a, b) a##b

#define MESS(name, tpe, txt, bc, tc) template<>struct Row<name>\
{\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe);}\
};
#define MESSX(name, tpe, txt, bc, tc) template<>struct Row<name>\
{\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return txt;}\
};
#define MESS1(name, tpe, txt, bc, tc) template<>struct Row<name>\
{\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe##_1);}\
};
#define MESS2(name, tpe, txt, bc, tc) template<>struct Row<name>\
{\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe##_2);}\
};

namespace LogMess
{
	enum ID
	{
		
//		StartSycle
//		, StopSycle
//		, OverheadTemperature
		 ProgramOpen
//
		, TimeoutPipe
//		, AlarmSensorReadError
//		, AlarmUSPC
//		, AlarmRestartServiceError
//		, AutomatMode
//		, SingleMode
//
		, ProgramClosed
		, ErrStop
//		, AlarmExitTubeInUnit
//		, AlarmExitDirectionChainOn
//		, AlarmExitInverterFaulty
//
		, AlarmExitRunBitIn			 
		, AlarmExitControlCircuitBitIn 
//		, AlarmExitWorkBitIn			 
//		, AlarmExitSycleBitIn		  
//
//		, InfoOnWorkBitIn 
//		, InfoOffWorkBitIn
//		, InfoDataReceive
//		, InfoOnResultBitOut
//		, InfoReadyBitOut	            
		, InfoOnSycleBitIn  
//		, InfoRotationalSpeedSensorsStart
//		, InfoOnRunBitIn                 
//		, InfoControlBitOut              
//		, InfoOnSQ1BitIn	                
//		, InfoPowerScannerBitOut         
//		, InfoOnSQ3BitIn	                
//		, InfoOffSQ1BitIn                
//		, InfoOffPowerScannerBitOut         
//		, InfoOffSQ3BitIn 
		, InfoUserStop
//		, InfoDataCollectionComplete
//		, InfoDataCollection
//		, InfoBase
//
//		, SQ1BitIn           
//		, SQ3BitIn           
//		, StateBBitIn        
//		, StateABitIn        
//		, RunBitIn           
//		, ControlCircuitBitIn
//		, WorkBitIn          
//		, SycleBitIn         
//		, BaseBitIn 
//		, PowerInverterBitOut
//		, SpeedRLBitOut      
//		, SpeedRMBitOut      
//		, SpeedRHBitOut      
//		, STFBitOut          
//		, PowerScannerBitOut
//		, ReadyBitOut        
//		, ControlBitOut      
//		, ResultBitOut   
//		, WaitControlCircuitBitIn
//		, PowerBMOn
//		, WaitCycle
//		, WaitReady
//		, WaitControl
//
//		, AlarmControlCircuts
//		, AlarmCycle
//		, AlarmBase
//
//		//, ContineCycleOk
//		//, ContineCycleBrak
//
//		, CycleOk  
//		, CycleBrak
//
//		, AlarmNoDataCollection
//		, InfoDataSaved
//		, AlarmBufferOverflow
//		, InfoDataSaving
//
//		, OniCross
//		, OniLong 
//		, OniThick
//		, OniSolid
//
//		, OffiCross
//		, OffiLong 
//		, OffiThick
//		, OffiSolid
//
//		, TubeOnPosition
		, ModulesNotInOperationCross
		, ModulesNotInOperationLong
		, ModulesNotInOperationThick
		, TransverseModuleClampsDidNotWork 
		, PipeReturnAllowed
//		
		, iSQ1prMess	
		, iSQ2prMess	
		, iSQ1poMess	
		, iSQ2poMess	
		, iSQ1tMess
		, iSQ2tMess
//
		, iPCH_B_OFF
		, iPCH_RUN_OFF
//
		, ReturnPipeCompleted
		, PipeReturnTimeExceeded
		, ReturnPipe
//
		, TubeInCrossModule
		, TubeInLongModule
		, TubeInThickModule
		, TubeInSolid
//
		, ExitTube
//
		, NoPipesOnLine
		, RotationTube
//
//		//, PCH_BandPCH_RUN
		, CheckStatusFrequencyConverter
//		, TransportUnlocked
//
		, ThicknessGaugeTookOffSignal
		, CompletionChangeInPositionModule
		, PastureCompleted
//
		, TemperatureCoilSolenoidExceeded0
		, TemperatureCoilSolenoidExceeded1
//
		, AnalogBoardFailure 
		, ModulesInNon_OperatingPosition
		, SettingOperatingPositionControl
//
		, time_overflow
		, error_crc    
		, error_count  
		, transferControlParametersThicknessGauge
		//, repeatTransferControlParametersThicknessGauge
		, waitingThicknessGauge
		, waitingThicknessResult
//		
		, iWork_pnevmoWait
//
		, SOP_MODE
		, TUBE_MODE
		, RequestPipeNumber
		, WAITING_LONGITUDINAL_MODULE
		, WAITING_PERFORMANCE_THICKNESS_CONTROL_MODULE
		, ThicknessModuleNotReady
		, PIPE_CONTROL_IMPLEMENTED
//
		, iRPpr    
		, iOPpr     
		, iRPpo    
		, iOPpo    
		, iRPt     
		, iOPt  

		, iZU           
		, iPCH_B        
		, iPCH_RUN      
		, iPCH_A        
		, iWork_pnevmo  
		, iRevers_pnevmo
		, iError_pnevmo 
		, iWork_pnevmAlarm
//
		, WaitLongOn
		, WaitThickOn
		, WaitMagneticOn
		, WaitLongOff
		, WaitCrossOff
		, WaitThickOff
		, WaitMagneticOff
		, waitingPipeEntranceRollerTable
		, interruptView
		, transferResultControlAutomatedControlSystem
		, contineRun
		, storedDataBase
		, NoLongDriveReady
		, demagnetizationNotCorrect
		, demagnetizationTesting
		, unit502SetupParams
		, emergencyExit
		, tubeExit
//		    
		, max_row
	};

	//typedef const char * const_char_ptr;

	static const int green = 0xff00;
	static const int blue  = 0xff0000;
	static const int black = 0x0;
	static const int white = 0xffffff;
	static const int red = 0xff;
	static const int yellow = 0xffff;

	template<int >struct Row;
	///\brief сопоставление числу сообщениея
	///\param число из перечисления ID(см. выше)
	///\param тип дополнительного параметра
	///\param сообщение
	///\param цвет фона
	///\param цвет шрифта
	MESS(ProgramOpen		          , void  , "Программа открыта"              , black, white)
//							          
	MESS(TimeoutPipe		          , void  , "Превышенно время ожидания", red  , yellow)
						          
	MESS(ProgramClosed		          , void  , "Программа закрыта"	, red  , yellow)
//							          
	MESS(ErrStop                      , void  , "Прерывание цикла измерения", red, yellow)

	MESS(AlarmExitRunBitIn			  , void, "Ошибка частотного преобразователя\"", red  , yellow)
	MESS(AlarmExitControlCircuitBitIn , void, "Нет сигнала \"Цепи управления\""	   , red  , yellow)

	MESS(InfoOnSycleBitIn               , void, "Ожидание сигнала \"Цикл\""					  , blue , white)

	MESS(InfoUserStop					, void, "."			  , red  , yellow)

	MESS(ModulesNotInOperationCross, void, "Поперечный модуль не в положении обслуживания", red  , yellow) 
	MESS(ModulesNotInOperationLong, void, "Продольный модуль не в положении обслуживания", red  , yellow) 
	MESS(ModulesNotInOperationThick, void, "Модуль толщины не в положении обслуживания", red  , yellow) 
	MESS(TransverseModuleClampsDidNotWork, void, "Не сработали зажимы поперечного модуля", red, yellow)
	MESS(PipeReturnAllowed, void , "МОДУЛИ в ОП. Возврат трубы разрещен", blue , white)
//
	MESS(iSQ1prMess, void, "Датчик наличия трубы на входе в продольный модуль", blue , white)
	MESS(iSQ2prMess, void, "Датчик наличия трубы на выходе из продольного модулья", blue , white)
	MESS(iSQ1poMess, void, "Датчик наличия трубы на входе в поперечный модуль", blue , white)
	MESS(iSQ2poMess, void, "Датчик наличия трубы на выходе из поперечного модулья", blue , white)
	MESS(iSQ1tMess , void, "Датчик наличия трубы на входе в модуль толщины", blue , white)
	MESS(iSQ2tMess , void, "Датчик наличия трубы на выходе из модуля толщины", blue , white)
//
	MESS(iPCH_B_OFF	 , void, "Авария \"Сигнал состояния частотного преобразователя(В)\" отключен", red, yellow)
	MESS(iPCH_RUN_OFF, void, "Авария \"Сигнал состояния частотного преобразователя(Run)\" отключен", red, yellow)
//
	MESS(NoLongDriveReady, void, "Авария \"Нет готовности привода продольного модуля\"", red, yellow)
//
	MESS(ReturnPipeCompleted, void, "Возврат трубы завершен", blue , white) 
	MESS(PipeReturnTimeExceeded, void, "Время возврата трубы превышено", red, yellow) 
	MESS(ReturnPipe, void, "Возврат трубы", blue , white)
//
	MESS(ExitTube, void, "Выгон трубы", blue, white)
//
	MESS(TubeInCrossModule, void, "Труба в поперечном модуле", red, yellow)
	MESS(TubeInLongModule , void, "Труба в продольном модуле", red, yellow)
	MESS(TubeInThickModule, void, "Труба в модуле толщиномера", red, yellow)
	MESS(TubeInSolid	  , void, "Труба в модуле размагничивания", red, yellow)
//
	MESS(NoPipesOnLine, void, "ТРУБЫ НА ЛИНИИ НЕТ. ВЫГОН НЕ ТРЕБУЕТСЯ", blue , white)
//
	MESS(RotationTube, void, "ВКЛЮЧИТЕ ВРАЩЕНИЕ СУ ТОЛЩИНЫ", red, yellow);
//
	MESS(CheckStatusFrequencyConverter, void, "Проверка состояния частотного преобразователя", blue , white)
//
	MESS(ThicknessGaugeTookOffSignal, void, "Tолщиномер снял сигнал", red, yellow)
	MESS(CompletionChangeInPositionModule, void, "Завершение по изменению положения какого-либо модуля", red, yellow)
//
	MESS(PastureCompleted, void, "Выгон завершен", blue, white)
//
	MESS1(TemperatureCoilSolenoidExceeded0, double, "Превышена температура обмотки соленоида 1", red, yellow)
	MESS1(TemperatureCoilSolenoidExceeded1, double, "Превышена температура обмотки соленоида 2", red, yellow)
//
	MESS(AnalogBoardFailure, void, "Авария платы 502", red, yellow)
//
	MESS(SettingOperatingPositionControl, void , "Установка рабочего положения модулей контроля", blue, white)
	MESS(ModulesInNon_OperatingPosition, void, "Модули в нерабочем положении", red, yellow)
//
	MESS(time_overflow, void, "Ошибка КОМ-порта: превышено время ожидания", red, yellow)
	MESS(error_crc    , void, "Ошибка КОМ-порта: ошибка CRC", red, yellow)
	MESS(error_count  , void, "Ошибка КОМ-порта: принято 0 байт", red, yellow)
//
	MESS(iWork_pnevmoWait, void, "Ожидание готовности контроллера пневмооборудования", blue , white)
	MESS(iWork_pnevmAlarm, void, "Авария контроллера пневмооборудования", red, yellow)
//
	MESS(SOP_MODE, void, "Режим \"СОП\" ожидание трубы", blue , white) 
	MESS(TUBE_MODE, void, "Режим \"ТРУБА\" ожидание трубы", blue, white)
	MESS(RequestPipeNumber, void, "Запрос номера трубы", blue, white)
	MESS(WAITING_LONGITUDINAL_MODULE, void, "ОЖИДАНИЕ ГОТОВНОСТИ ПРОДОЛЬНОГО МОДУЛЯ", blue, white)
	MESS(WAITING_PERFORMANCE_THICKNESS_CONTROL_MODULE, void, "ОЖИДАНИЕ ГОТОВНОСТИ МОДУЛЯ КОНТРОЛЯ ТОЛЩИНЫ", blue, white)
	MESS(ThicknessModuleNotReady, void, "Модуль толщины не готов", red, yellow)
	MESS(PIPE_CONTROL_IMPLEMENTED, void, "ВЫПОЛНЯЕТСЯ КОНТРОЛЬ ТРУБЫ", blue, white)
//
	MESS(iRPpr    	   , bool, "Продольный модуль в рабочем положении", blue, white)
	MESS(iOPpr     	   , bool, "Продольный модуль в положении обслуживания", blue, white)
	MESS(iRPpo    	   , bool, "Поперечный модуль в рабочем положении", blue, white)
	MESS(iOPpo    	   , bool, "Поперечный модуль в положении обслуживания", blue, white)
	MESS(iRPt     	   , bool, "Модуль толщины в рабочем положении", blue, white)
	MESS(iOPt  		   , bool, "Модуль толщины в положении обслуживания", blue, white)
	MESS(iZU           , bool, "Признак включения цепей управления", blue, white)
	MESS(iPCH_B        , bool, "Сигнал состояния частотного преобразователя(В)", blue, white)
	MESS(iPCH_RUN      , bool, "Сигнал состояния частотного преобразователя(Run)", blue, white)
	MESS(iPCH_A        , bool, "Сигнал состояния частотного преобразователя(A)", blue, white)
	MESS(iWork_pnevmo  , bool, "Признак рабочего режима контроллера пневмооборудования", blue, white)
	MESS(iRevers_pnevmo, bool, "Признак режима реверс контроллера пневмооборудования", blue, white)
	MESS(iError_pnevmo , bool, "Признак аварии контроллера пневмооборудования", blue, white)
//
	MESS(WaitLongOn		, void, "Ожидание трубы в продольном модуле", blue, white)
	MESS(WaitThickOn	, void, "Ожидание трубы в модуле толщиномера", blue, white)
	MESS(WaitMagneticOn	, void, "Ожидание трубы в модуле размагничивания", blue, white)
	MESS(WaitLongOff	, void, "Ожидание съезда трубы из продольного модуля", blue, white)
	MESS(WaitCrossOff	, void, "Ожидание съезда трубы из поперечного модуля", blue, white)
	MESS(WaitThickOff	, void, "Ожидание съезда трубы из модуля толщиномера", blue, white)
	MESS(WaitMagneticOff, void, "Ожидание съезда трубы из модуля размагничивания", blue, white)
//
	MESS(transferControlParametersThicknessGauge, void, "Передача параметров контроля в толщиномер", blue, white)
	//MESS(repeatTransferControlParametersThicknessGauge, void, "Повторить передачу параметров контроля в толщиномер",red, yellow)
	MESS(waitingThicknessGauge, void, "Oжидание готовности толщиномера", blue, white)
	MESS(waitingPipeEntranceRollerTable, void, "Ожидание трубы на входном рольганге", blue, white)
	MESS(waitingThicknessResult, void, "Oжидание результатов измерений толщиномера", blue, white)
//
	MESS(interruptView, void, "Прерывание на просмотр", blue, white)
//
	MESS(transferResultControlAutomatedControlSystem, void, "передача результата контроля в АСУ", blue, white)
	MESS(contineRun, void, "\"F4 ЦИКЛ\"далее \"F11 Продол...\"повторить запрос к АСУ \"Esc Стоп\"выход из цикла", red, white)
	MESS(storedDataBase, void, "Сохранение в базе данных", blue, white)

	MESS(demagnetizationNotCorrect, void, "Авария. Размагничивание неисправно", red, yellow)
	MESS(demagnetizationTesting, void, "Тестирование размагничивания", blue, white)

	MESS(unit502SetupParams, void, "Плата L502 не инициализируется", red, yellow)

    MESS(emergencyExit, void, "Выход по аварии", red, yellow)

	MESS(tubeExit, void, "Труба вышла из установки", blue, white)

	class FactoryMessages
	{
		struct Inner;
		Inner &inner;
		FactoryMessages();
	public:
		void StartTime();
		bool Color(int i, unsigned &backColor, unsigned &textColor);
		bool Text(int i, char *buf, double val);
		static FactoryMessages &Instance();
	};
}
#undef MESS
#undef MESS1
#undef MESS2
#undef __double_1 
#undef __double_2 
#undef __int 
#undef __void 
#undef _cat


