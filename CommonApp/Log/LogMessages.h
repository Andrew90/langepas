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
	///\brief ������������� ����� ����������
	///\param ����� �� ������������ ID(��. ����)
	///\param ��� ��������������� ���������
	///\param ���������
	///\param ���� ����
	///\param ���� ������
	MESS(StartSycle			          , void  , "���� �����"                     , green, black)
    MESS(StopSycle			          , void  , "���� ����"				        , blue , white)
	MESS1(OverheadTemperature         , double, "����������� ���������"	        , red  , yellow)
	MESS(ProgramOpen		          , void  , "��������� �������"              , black, white)
							          
	MESS(TimeoutPipe		          , void  , "���������� ����� ��������", red  , yellow)
	//MESS(AlarmUSPC                    , void  , "������!!! ����� ��������������� �������� �� ��������", red, yellow)
	MESSX(AlarmUSPC                    , void  , "������!!! ����� ��������������� ��������", red, yellow)
	MESSX(AlarmSensorReadError         , void  , "������!!! ������ ������ � ����� ��������������� ��������", red, yellow)

	MESS(AlarmRestartServiceError     , void  , "������!!! ������ \"USPS7100WinService\" �� ���������������", red, yellow)
							          
	MESS(AutomatMode		          , void  , "����� \"��������������\""		, blue , white)
	MESS(SingleMode			          , void  , "����� \"��������� ���������\""	, blue , white)
							          
	MESS(ProgramClosed		          , void  , "��������� �������"	, red  , yellow)
							          
	MESS(ErrStop                      , void  , "���������� ����� ���������", red, yellow)

	MESS(AlarmExitTubeInUnit          , void, "����� � ������, ��������� �����", red  , yellow)
	MESS(AlarmExitDirectionChainOn    , void, "�������� ���� ����������", red  , yellow)
	MESS(AlarmExitInverterFaulty      , void, "��������� ��������������� �� �������", red  , yellow)

	MESS(AlarmExitRunBitIn			  , void, "������ ���������� ���������������\"", red  , yellow)
	MESS(AlarmExitControlCircuitBitIn , void, "��� ������� \"���� ����������\""	   , red  , yellow)
	MESS(AlarmExitWorkBitIn			  , void, "��� ������� \"������\""			   , red  , yellow)
	MESS(AlarmExitSycleBitIn		  , void, "��� ������� \"����\""			   , red  , yellow)

	MESS(AlarmNoDataCollection        , void, "��������!!! ��� ����� ������", red, yellow)
	MESS(AlarmBufferOverflow, void, "��������!!! ����� ����������", red, yellow)

	MESS(WaitControlCircuitBitIn      , void, "�������� ������� \"���� ����������\"", blue, white)

	MESS(PowerBMOn, void, "�������� \"������� ��\"", blue, white)
	MESS(WaitCycle, void, "�������� �������� ������� \"����\" � \"����������\"", blue, white)
	MESS(WaitReady, void, "�������� �������� ������� \"����������\"", blue, white)
	MESS(WaitControl, void, "�������� �������� ������� \"��������\"", blue, white) 

	MESS(AlarmControlCircuts, void, "������!!! ���� ������ \"���� ����������\"", red, yellow)
	MESS(AlarmBase,           void, "������!!! ��� ������� \"����\"", red, yellow)
	MESS(AlarmCycle			, void, "������!!! ���� ������ \"����\"", red, yellow)

	//MESSX(ContineCycleOk, double, "��������� �������� \"�����\". ����� ����� %.1f �. ��� ����������� ������� ������ \"������\"", blue, white)
	//MESSX(ContineCycleBrak, double, "��������� �������� \"����\". ����� ����� %.1f �. ��� ����������� ������� ������ \"������\"", red, yellow)
		
	MESSX(CycleOk  , double, "��������� �������� \"�����\". ����� �����  %.2f �.", blue, white)
	MESSX(CycleBrak, double, "��������� �������� \"����\". ����� �����  %.2f �.", red, yellow)


	MESS(InfoOnWorkBitIn                , void, "�������� ������� \"������\""					  , blue , white)
	MESS(InfoOffWorkBitIn				, void, "�������� ������ ������� \"������\""			  , green, blue)
	MESS(InfoDataReceive				, void, "������ � ������������ �������"                   , blue, yellow)
	MESS(InfoReadyBitOut	            , void, "��������� ������ \"����������\""				  , blue , white)
	MESS(InfoOnResultBitOut             , void, "��������� ������ \"���������\""                 , blue , white)
	MESS(InfoOnSycleBitIn               , void, "�������� ������� \"����\""					  , blue , white)
	MESS(InfoRotationalSpeedSensorsStart, void, "��������� �������� ������ �������� ������������", blue , white)
	MESS(InfoOnRunBitIn                 , void, "�������� ��������� ������ �������� ������������", blue , white)
	MESS(InfoControlBitOut              , void, "��������� ������ \"��������\""				  , blue , white)
	MESS(InfoOnSQ1BitIn	                , void, "�������� ����� �� ������� SQ1"				  , blue , white)
	MESS(InfoPowerScannerBitOut         , void, "��������� ������������ ����������"			  , blue , white)
	MESS(InfoOnSQ3BitIn	                , void, "�������� ����� �� ������� SQ3"				  , blue , white)
	MESS(InfoOffSQ1BitIn                , void, "�������� ������ ����� � ������� SQ1"			  , blue , white)
	MESS(InfoOffPowerScannerBitOut      , void, "���������� ������������ ����������"		      , blue , white)
	MESS(InfoOffSQ3BitIn                , void, "�������� ������ ����� � ������� SQ3"			  , blue , white)
	MESS(InfoUserStop					, void, "�������� ����� �� �����"			  , red  , yellow)

	MESS(InfoDataCollectionComplete		, void, "���� ������ ��������"                           , green, white);
	MESS(InfoDataCollection     		, void, "���� ������"                           , blue, white);
	MESS(InfoBase, void, "�������� ������ \"����\"", blue, white);

	MESS(InfoDataSaved, void, "������ ���������", green, white);
	MESS(InfoDataSaving, void, "������ �����������", red, blue);

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

	MESS(OniCross , void, "�������� ����� � ���������� ������"				, blue , white)
	MESS(OniLong  , void, "�������� ����� � ���������� ������"				, blue , white)
	MESS(OniThick , void, "�������� ����� � ������ �����������"			, blue , white)
	MESS(OniSolid , void, "�������� ����� � ������ ������ ���������"			, blue , white)

	MESS(OffiCross , void, "����� ����� �� ����������� ������"				, blue , white)
	MESS(OffiLong  , void, "����� ����� �� ����������� ������"				, blue , white)
	MESS(OffiThick , void, "����� ����� �� ������ �����������"			, blue , white)
	MESS(OffiSolid , void, "����� ����� �� ������ ������ ���������"			, blue , white)
	
	MESS(TubeOnPosition , void, "�������� ����� �� �������"			, blue , white)

	MESS(ModulesNotInOperation, void, "������ �� � ��������� ������������", red  , yellow) 
	MESS(TransverseModuleClampsDidNotWork, void, "�� ��������� ������ ����������� ������", red, yellow)
	MESS(PipeReturnAllowed, void , "������ � ��. ������� ����� ��������", blue , white)

	MESS(iSQ1prMess, void, "������ ������� ����� �� ����� � ���������� ������", blue , white)
	MESS(iSQ2prMess, void, "������ ������� ����� �� ������ �� ����������� �������", blue , white)
	MESS(iSQ1poMess, void, "������ ������� ����� �� ����� � ���������� ������", blue , white)
	MESS(iSQ2poMess, void, "������ ������� ����� �� ������ �� ����������� �������", blue , white)
	MESS(iSQ1tMess , void, "������ ������� ����� �� ����� � ������ �������", blue , white)
	MESS(iSQ2tMess , void, "������ ������� ����� �� ������ �� ������ �������", blue , white)

	MESS(iPCH_B_OFF	 , void, "������ \"������ ��������� ���������� ���������������(�)\" ��������", red, yellow)
	MESS(iPCH_RUN_OFF, void, "������ \"������ ��������� ���������� ���������������(Run)\" ��������", red, yellow)

	MESS(ReturnPipeCompleted, void, "������� ����� ��������", blue , white) 
	MESS(PipeReturnTimeExceeded, void, "����� �������� ����� ���������", red, yellow) 
	MESS(ReturnPipe, void, "������� �����", blue , white)

	MESS(ExitTube, void, "����� �����", blue, white)

	MESS(TubeInCrossModule, void, "����� � ���������� ������", red, yellow)
	MESS(TubeInLongModule , void, "����� � ���������� ������", red, yellow)
	MESS(TubeInThickModule, void, "����� � ������ �����������", red, yellow)
	MESS(TubeInSolid	  , void, "����� � ������ ���������������", red, yellow)

	MESS(NoPipesOnLine, void, "����� �� ����� ���. ����� �� ���������", blue , white)

	MESS(RotationTube, void, "�������� �������� �� �������", red, yellow);

	MESS(CheckStatusFrequencyConverter, void, "�������� ��������� ���������� ���������������", blue , white)

	//MESS(PCH_BandPCH_RUN, void , "XXXXX", red, yellow)

	MESS(TransportUnlocked, void, "��������� �������������. ��������� �����", blue, white)
	MESS(ThicknessGaugeTookOffSignal, void, "T��������� ���� ������", red, yellow)
	MESS(CompletionChangeInPositionModule, void, "���������� �� ��������� ��������� ������-���� ������", red, yellow)

	MESS(PastureCompleted, void, "����� ��������", blue, white)

	MESS1(TemperatureCoilSolenoidExceeded0, double, "��������� ����������� ������� ��������� 1", red, yellow)
	MESS1(TemperatureCoilSolenoidExceeded1, double, "��������� ����������� ������� ��������� 2", red, yellow)

	MESS(AnalogBoardFailure, void, "������ ���������� �����", red, yellow)

	//MESS(AnalogBoardFailure, void, "������ ���������� �����", red, yellow)
	MESS(ModulesInNon_OperatingPosition, void, "������ � ��������� ���������", red, yellow)

	MESS(time_overflow, void, "������ ���-�����: ��������� ����� ��������", red, yellow)
	MESS(error_crc    , void, "������ ���-�����: ������ CRC", red, yellow)
	MESS(error_count  , void, "������ ���-�����: ������� 0 ����", red, yellow)

	MESS(iWork_pnevmoWait, void, "�������� ���������� ����������� ������������������", blue , white)

	MESS(SOP_MODE, void, "����� \"���\" �������� �����", blue , white) 
	MESS(TUBE_MODE, void, "����� \"�����\" �������� �����", blue, white)
	MESS(RequestPipeNumber, void, "������ ������ �����", blue, white)
	MESS(WAITING_LONGITUDINAL_MODULE, void, "�������� ���������� ����������� ������", blue, white)
	MESS(WAITING_PERFORMANCE_THICKNESS_CONTROL_MODULE, void, "�������� ���������� ������ �������� �������", blue, white)
	MESS(ThicknessModuleNotReady, void, "������ ������� �� �����", red, yellow)
	MESS(PIPE_CONTROL_IMPLEMENTED, void, "����������� �������� �����", blue, white)

	MESS(iRPpr    	   , bool, "���������� ������ � ������� ���������", blue, white)
	MESS(iOPpr     	   , bool, "���������� ������ � ��������� ������������", blue, white)
	MESS(iRPpo    	   , bool, "���������� ������ � ������� ���������", blue, white)
	MESS(iOPpo    	   , bool, "���������� ������ � ��������� ������������", blue, white)
	MESS(iRPt     	   , bool, "������ ������� � ������� ���������", blue, white)
	MESS(iOPt  		   , bool, "������ ������� � ��������� ������������", blue, white)
	MESS(iZU           , bool, "������� ��������� ����� ����������", blue, white)
	MESS(iPCH_B        , bool, "������ ��������� ���������� ���������������(�)", blue, white)
	MESS(iPCH_RUN      , bool, "������ ��������� ���������� ���������������(Run)", blue, white)
	MESS(iPCH_A        , bool, "������ ��������� ���������� ���������������(A)", blue, white)
	MESS(iWork_pnevmo  , bool, "������� �������� ������ ����������� ������������������", blue, white)
	MESS(iRevers_pnevmo, bool, "������� ������ ������ ����������� ������������������", blue, white)
	MESS(iError_pnevmo , bool, "������� ������ ����������� ������������������", blue, white)

	MESS(WaitLongOn		, void, "�������� ����� � ���������� ������", blue, white)
	MESS(WaitThickOn	, void, "�������� ����� � ������ �����������", blue, white)
	MESS(WaitMagneticOn	, void, "�������� ����� � ������ ���������������", blue, white)
	MESS(WaitLongOff	, void, "�������� ������ ����� �� ����������� ������", blue, white)
	MESS(WaitCrossOff	, void, "�������� ������ ����� �� ����������� ������", blue, white)
	MESS(WaitThickOff	, void, "�������� ������ ����� �� ������ �����������", blue, white)
	MESS(WaitMagneticOff, void, "�������� ������ ����� �� ������ ���������������", blue, white)

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


