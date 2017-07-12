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
		StartSycle
		, StopSycle
		, OverheadTemperature
		, ProgramOpen

		, TimeoutPipe
		, AlarmSensorReadError
		, AlarmUSPC
		, AlarmRestartServiceError
		, AutomatMode
		, SingleMode

		, ProgramClosed
		, ErrStop
		, AlarmExitTubeInUnit
		, AlarmExitDirectionChainOn
		, AlarmExitInverterFaulty

		, AlarmExitRunBitIn			 
		, AlarmExitControlCircuitBitIn 
		, AlarmExitWorkBitIn			 
		, AlarmExitSycleBitIn		  

		, InfoOnWorkBitIn 
		, InfoOffWorkBitIn
		, InfoDataReceive
		, InfoOnResultBitOut
		, InfoReadyBitOut	            
		, InfoOnSycleBitIn  
		, InfoRotationalSpeedSensorsStart
		, InfoOnRunBitIn                 
		, InfoControlBitOut              
		, InfoOnSQ1BitIn	                
		, InfoPowerScannerBitOut         
		, InfoOnSQ3BitIn	                
		, InfoOffSQ1BitIn                
		, InfoOffPowerScannerBitOut         
		, InfoOffSQ3BitIn 
		, InfoUserStop
		, InfoDataCollectionComplete
		, InfoDataCollection
		, InfoBase

		, SQ1BitIn           
		, SQ3BitIn           
		, StateBBitIn        
		, StateABitIn        
		, RunBitIn           
		, ControlCircuitBitIn
		, WorkBitIn          
		, SycleBitIn         
		, BaseBitIn 
		, PowerInverterBitOut
		, SpeedRLBitOut      
		, SpeedRMBitOut      
		, SpeedRHBitOut      
		, STFBitOut          
		, PowerScannerBitOut
		, ReadyBitOut        
		, ControlBitOut      
		, ResultBitOut   
		, WaitControlCircuitBitIn
		, PowerBMOn
		, WaitCycle
		, WaitReady
		, WaitControl

		, AlarmControlCircuts
		, AlarmCycle
		, AlarmBase

		//, ContineCycleOk
		//, ContineCycleBrak

		, CycleOk  
		, CycleBrak

		, AlarmNoDataCollection
		, InfoDataSaved
		, AlarmBufferOverflow
		, InfoDataSaving

		, OniCross
		, OniLong 
		, OniThick
		, OniSolid

		, OffiCross
		, OffiLong 
		, OffiThick
		, OffiSolid

		, TubeOnPosition
		, ModulesNotInOperation
		, TransverseModuleClampsDidNotWork 
		, PipeReturnAllowed
		
		, iSQ1prMess	
		, iSQ2prMess	
		, iSQ1poMess	
		, iSQ2poMess	
		, iSQ1tMess
		, iSQ2tMess

		, iPCH_B_OFF
		, iPCH_RUN_OFF

		, ReturnPipeCompleted
		, PipeReturnTimeExceeded
		, ReturnPipe

		, TubeInCrossModule
		, TubeInLongModule
		, TubeInThickModule
		, TubeInSolid

		, ExitTube

		, NoPipesOnLine
		, RotationTube

		//, PCH_BandPCH_RUN
		, CheckStatusFrequencyConverter
		, TransportUnlocked

		, ThicknessGaugeTookOffSignal
		, CompletionChangeInPositionModule
		, PastureCompleted

		, TemperatureCoilSolenoidExceeded0
		, TemperatureCoilSolenoidExceeded1

		, AnalogBoardFailure 
		, ModulesInNon_OperatingPosition

		, time_overflow
		, error_crc    
		, error_count  
		
		, iWork_pnevmoWait

		, SOP_MODE
		, TUBE_MODE
		, RequestPipeNumber
		, WAITING_LONGITUDINAL_MODULE
		, WAITING_PERFORMANCE_THICKNESS_CONTROL_MODULE
		, ThicknessModuleNotReady
		, PIPE_CONTROL_IMPLEMENTED

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

		, WaitLongOn
		, WaitThickOn
		, WaitMagneticOn
		, WaitLongOff
		, WaitCrossOff
		, WaitThickOff
		, WaitMagneticOff
		    
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
	MESS(StartSycle			          , void  , "Цикл старт"                     , green, black)
    MESS(StopSycle			          , void  , "Цикл стоп"				        , blue , white)
	MESS1(OverheadTemperature         , double, "Температура превышена"	        , red  , yellow)
	MESS(ProgramOpen		          , void  , "Программа открыта"              , black, white)
							          
	MESS(TimeoutPipe		          , void  , "Превышенно время ожидания", red  , yellow)
	//MESS(AlarmUSPC                    , void  , "Авария!!! Плата ультрозвукового контроля не исправна", red, yellow)
	MESSX(AlarmUSPC                    , void  , "Авария!!! Платы ультразвукового контроля", red, yellow)
	MESSX(AlarmSensorReadError         , void  , "Авария!!! Чтения данных с платы ультразвукового контроля", red, yellow)

	MESS(AlarmRestartServiceError     , void  , "Авария!!! Сервис \"USPS7100WinService\" не перезапускается", red, yellow)
							          
	MESS(AutomatMode		          , void  , "Режим \"Автоматический\""		, blue , white)
	MESS(SingleMode			          , void  , "Режим \"Одиночное измерение\""	, blue , white)
							          
	MESS(ProgramClosed		          , void  , "Программа закрыта"	, red  , yellow)
							          
	MESS(ErrStop                      , void  , "Прерывание цикла измерения", red, yellow)

	MESS(AlarmExitTubeInUnit          , void, "Труба в модуле, выполните выгон", red  , yellow)
	MESS(AlarmExitDirectionChainOn    , void, "Включите цепи управления", red  , yellow)
	MESS(AlarmExitInverterFaulty      , void, "Частотный преодразователь не включён", red  , yellow)

	MESS(AlarmExitRunBitIn			  , void, "Ошибка частотного преобразователя\"", red  , yellow)
	MESS(AlarmExitControlCircuitBitIn , void, "Нет сигнала \"Цепи управления\""	   , red  , yellow)
	MESS(AlarmExitWorkBitIn			  , void, "Нет сигнала \"Работа\""			   , red  , yellow)
	MESS(AlarmExitSycleBitIn		  , void, "Нет сигнала \"Цикл\""			   , red  , yellow)

	MESS(AlarmNoDataCollection        , void, "Внимание!!! Нет сбора данных", red, yellow)
	MESS(AlarmBufferOverflow, void, "Внимание!!! Буфер переполнен", red, yellow)

	MESS(WaitControlCircuitBitIn      , void, "Ожидание сигнала \"Цепи управления\"", blue, white)

	MESS(PowerBMOn, void, "Включено \"ПИТАНИЕ БМ\"", blue, white)
	MESS(WaitCycle, void, "Ожидание входного сигнала \"Цикл\" и \"Готовность\"", blue, white)
	MESS(WaitReady, void, "Ожидание входного сигнала \"Готовность\"", blue, white)
	MESS(WaitControl, void, "Ожидание входного сигнала \"Контроль\"", blue, white) 

	MESS(AlarmControlCircuts, void, "Авария!!! Снят сигнал \"Цепи управления\"", red, yellow)
	MESS(AlarmBase,           void, "Авария!!! Нет сигнала \"БАЗА\"", red, yellow)
	MESS(AlarmCycle			, void, "Авария!!! Снят сигнал \"Цикл\"", red, yellow)

	//MESSX(ContineCycleOk, double, "Результат контроля \"ГОДНО\". Длина трубы %.1f м. Для продолжения нажмите кнопку \"Повтор\"", blue, white)
	//MESSX(ContineCycleBrak, double, "Результат контроля \"БРАК\". Длина трубы %.1f м. Для продолжения нажмите кнопку \"Повтор\"", red, yellow)
		
	MESSX(CycleOk  , double, "Результат контроля \"ГОДНО\". Длина трубы  %.2f м.", blue, white)
	MESSX(CycleBrak, double, "Результат контроля \"БРАК\". Длина трубы  %.2f м.", red, yellow)


	MESS(InfoOnWorkBitIn                , void, "Ожидание сигнала \"Работа\""					  , blue , white)
	MESS(InfoOffWorkBitIn				, void, "Ожидание снятия сигнала \"Работа\""			  , green, blue)
	MESS(InfoDataReceive				, void, "Данные с дефектоскопа приняты"                   , blue, yellow)
	MESS(InfoReadyBitOut	            , void, "Выставлен сигнал \"Готовность\""				  , blue , white)
	MESS(InfoOnResultBitOut             , void, "Выставлен сигнал \"Результат\""                 , blue , white)
	MESS(InfoOnSycleBitIn               , void, "Ожидание сигнала \"Цикл\""					  , blue , white)
	MESS(InfoRotationalSpeedSensorsStart, void, "Включение вращения модуля датчиков сканирования", blue , white)
	MESS(InfoOnRunBitIn                 , void, "Ожидание раскрутки модуля датчиков сканирования", blue , white)
	MESS(InfoControlBitOut              , void, "Выставлен сигнал \"Контроль\""				  , blue , white)
	MESS(InfoOnSQ1BitIn	                , void, "Ожидание трубы на датчике SQ1"				  , blue , white)
	MESS(InfoPowerScannerBitOut         , void, "Включение сканирующего устройства"			  , blue , white)
	MESS(InfoOnSQ3BitIn	                , void, "Ожидание трубы на датчике SQ3"				  , blue , white)
	MESS(InfoOffSQ1BitIn                , void, "Ожидание съезда трубы с датчика SQ1"			  , blue , white)
	MESS(InfoOffPowerScannerBitOut      , void, "Отключение сканирующего устройства"		      , blue , white)
	MESS(InfoOffSQ3BitIn                , void, "Ожидание съезда трубы с датчика SQ3"			  , blue , white)
	MESS(InfoUserStop					, void, "Оператор вышел из цикла"			  , red  , yellow)

	MESS(InfoDataCollectionComplete		, void, "Сбор данных закончен"                           , green, white);
	MESS(InfoDataCollection     		, void, "Сбор данных"                           , blue, white);
	MESS(InfoBase, void, "Сработал датчик \"База\"", blue, white);

	MESS(InfoDataSaved, void, "Данные сохранены", green, white);
	MESS(InfoDataSaving, void, "Данные сохраняются", red, blue);

	MESS(SQ1BitIn             , bool, "SQ1BitIn            ", white, black)
	MESS(SQ3BitIn             , bool, "SQ3BitIn            ", white, black)
	MESS(StateBBitIn          , bool, "StateBBitIn         ", white, black)
	MESS(StateABitIn          , bool, "StateABitIn         ", white, black)
	MESS(RunBitIn             , bool, "RunBitIn            ", white, black)
	MESS(ControlCircuitBitIn  , bool, "ControlCircuitBitIn ", white, black)
	MESS(WorkBitIn            , bool, "WorkBitIn           ", white, black)
	MESS(SycleBitIn           , bool, "SycleBitIn          ", white, black)
	MESS(BaseBitIn 			  , bool, "BaseBitIn 		   ", white, black)
	MESS(PowerInverterBitOut  , bool, "PowerInverterBitOut ", white, black)
	MESS(SpeedRLBitOut        , bool, "SpeedRLBitOut       ", white, black)
	MESS(SpeedRMBitOut        , bool, "SpeedRMBitOut       ", white, black)
	MESS(SpeedRHBitOut        , bool, "SpeedRHBitOut       ", white, black)
	MESS(STFBitOut            , bool, "STFBitOut           ", white, black)
	MESS(PowerScannerBitOut	  , bool, "PowerScannerBitOut  ", white, black)
	MESS(ReadyBitOut          , bool, "ReadyBitOut         ", white, black)
	MESS(ControlBitOut        , bool, "ControlBitOut       ", white, black)
	MESS(ResultBitOut   	  , bool, "ResultBitOut   	 "  , white, black)

	MESS(OniCross , void, "Ожидание трубы в поперечном модуле"				, blue , white)
	MESS(OniLong  , void, "Ожидание трубы в продольном модуле"				, blue , white)
	MESS(OniThick , void, "Ожидание трубы в модуле толщиномера"			, blue , white)
	MESS(OniSolid , void, "Ожидание трубы в модуле группы прочности"			, blue , white)

	MESS(OffiCross , void, "Выход трубы из поперечного модуля"				, blue , white)
	MESS(OffiLong  , void, "Выход трубы из продольного модуля"				, blue , white)
	MESS(OffiThick , void, "Выход трубы из модуля толщиномера"			, blue , white)
	MESS(OffiSolid , void, "Выход трубы из модуля группы прочности"			, blue , white)
	
	MESS(TubeOnPosition , void, "Ожидание трубы на позиции"			, blue , white)

	MESS(ModulesNotInOperation, void, "Модули не в положении обслуживания", red  , yellow) 
	MESS(TransverseModuleClampsDidNotWork, void, "Не сработали зажимы поперечного модуля", red, yellow)
	MESS(PipeReturnAllowed, void , "МОДУЛИ в ОП. Возврат трубы разрещен", blue , white)

	MESS(iSQ1prMess, void, "Датчик наличия трубы на входе в продольный модуль", blue , white)
	MESS(iSQ2prMess, void, "Датчик наличия трубы на выходе из продольного модулья", blue , white)
	MESS(iSQ1poMess, void, "Датчик наличия трубы на входе в поперечный модуль", blue , white)
	MESS(iSQ2poMess, void, "Датчик наличия трубы на выходе из поперечного модулья", blue , white)
	MESS(iSQ1tMess , void, "Датчик наличия трубы на входе в модуль толщины", blue , white)
	MESS(iSQ2tMess , void, "Датчик наличия трубы на выходе из модуля толщины", blue , white)

	MESS(iPCH_B_OFF	 , void, "Авария \"Сигнал состояния частотного преобразователя(В)\" отключен", red, yellow)
	MESS(iPCH_RUN_OFF, void, "Авария \"Сигнал состояния частотного преобразователя(Run)\" отключен", red, yellow)

	MESS(ReturnPipeCompleted, void, "Возврат трубы завершен", blue , white) 
	MESS(PipeReturnTimeExceeded, void, "Время возврата трубы превышено", red, yellow) 
	MESS(ReturnPipe, void, "Возврат трубы", blue , white)

	MESS(ExitTube, void, "Выгон трубы", blue, white)

	MESS(TubeInCrossModule, void, "Труба в поперечном модуле", red, yellow)
	MESS(TubeInLongModule , void, "Труба в продольном модуле", red, yellow)
	MESS(TubeInThickModule, void, "Труба в модуле толщиномера", red, yellow)
	MESS(TubeInSolid	  , void, "Труба в модуле размагничивания", red, yellow)

	MESS(NoPipesOnLine, void, "ТРУБЫ НА ЛИНИИ НЕТ. ВЫГОН НЕ ТРЕБУЕТСЯ", blue , white)

	MESS(RotationTube, void, "ВКЛЮЧИТЕ ВРАЩЕНИЕ СУ ТОЛЩИНЫ", red, yellow);

	MESS(CheckStatusFrequencyConverter, void, "Проверка состояния частотного преобразователя", blue , white)

	//MESS(PCH_BandPCH_RUN, void , "XXXXX", red, yellow)

	MESS(TransportUnlocked, void, "ТРАНСПОРТ РАЗБЛОКИРОВАН. ВЫПОЛНИТЕ ВЫГОН", blue, white)
	MESS(ThicknessGaugeTookOffSignal, void, "Tолщиномер снял сигнал", red, yellow)
	MESS(CompletionChangeInPositionModule, void, "Завершение по изменению положения какого-либо модуля", red, yellow)

	MESS(PastureCompleted, void, "Выгон завершен", blue, white)

	MESS1(TemperatureCoilSolenoidExceeded0, double, "Превышена температура обмотки соленоида 1", red, yellow)
	MESS1(TemperatureCoilSolenoidExceeded1, double, "Превышена температура обмотки соленоида 2", red, yellow)

	MESS(AnalogBoardFailure, void, "Авария аналоговой платы", red, yellow)

	//MESS(AnalogBoardFailure, void, "Авария аналоговой платы", red, yellow)
	MESS(ModulesInNon_OperatingPosition, void, "Модули в нерабочем положении", red, yellow)

	MESS(time_overflow, void, "Ошибка КОМ-порта: превышено время ожидания", red, yellow)
	MESS(error_crc    , void, "Ошибка КОМ-порта: ошибка CRC", red, yellow)
	MESS(error_count  , void, "Ошибка КОМ-порта: принято 0 байт", red, yellow)

	MESS(iWork_pnevmoWait, void, "Ожидание готовности контроллера пневмооборудования", blue , white)

	MESS(SOP_MODE, void, "Режим \"СОП\" ожидание трубы", blue , white) 
	MESS(TUBE_MODE, void, "Режим \"ТРУБА\" ожидание трубы", blue, white)
	MESS(RequestPipeNumber, void, "Запрос номера трубы", blue, white)
	MESS(WAITING_LONGITUDINAL_MODULE, void, "ОЖИДАНИЕ ГОТОВНОСТИ ПРОДОЛЬНОГО МОДУЛЯ", blue, white)
	MESS(WAITING_PERFORMANCE_THICKNESS_CONTROL_MODULE, void, "ОЖИДАНИЕ ГОТОВНОСТИ МОДУЛЯ КОНТРОЛЯ ТОЛЩИНЫ", blue, white)
	MESS(ThicknessModuleNotReady, void, "Модуль толщины не готов", red, yellow)
	MESS(PIPE_CONTROL_IMPLEMENTED, void, "ВЫПОЛНЯЕТСЯ КОНТРОЛЬ ТРУБЫ", blue, white)

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

	MESS(WaitLongOn		, void, "Ожидание трубы в продольном модуле", blue, white)
	MESS(WaitThickOn	, void, "Ожидание трубы в модуле толщиномера", blue, white)
	MESS(WaitMagneticOn	, void, "Ожидание трубы в модуле размагничивания", blue, white)
	MESS(WaitLongOff	, void, "Ожидание съезда трубы из продольного модуля", blue, white)
	MESS(WaitCrossOff	, void, "Ожидание съезда трубы из поперечного модуля", blue, white)
	MESS(WaitThickOff	, void, "Ожидание съезда трубы из модуля толщиномера", blue, white)
	MESS(WaitMagneticOff, void, "Ожидание съезда трубы из модуля размагничивания", blue, white)

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


