#include "stdafx.h"
#include "ControlMode.h"
#include "tools_debug\DebugMess.h"
#include "ControlTubeSubModules.h"
#include "App\AppBase.h"
#include "Compute\Automat.hpp"
#include "Log\LogBuffer.h"
#include "ASU\Asu.h"
#include "PerformanceCounter\PerformanceCounter.h"
#include "lir\SubLir.h"

namespace Mode
{
	using namespace AutomatN;
	template<class O, class P>struct __set_msk__
	{
		void operator()(O &o, P &p)
		{
			p |= o.value;
		}
	};

	template<class T>struct msg;

#define MSG(n)template<>struct msg<n>{static const LogMess::ID value = LogMess::n;};

	MSG(iRPpr    	   )
	MSG(iOPpr     	   )
	MSG(iRPpo    	   )
	MSG(iOPpo    	   )
	MSG(iRPt     	   )
	MSG(iOPt  		   )
	MSG(iZU            )
	MSG(iPCH_B         )
	MSG(iPCH_RUN       )
	MSG(iPCH_A         )
	MSG(iWork_pnevmo   )
	MSG(iRevers_pnevmo )
	MSG(iError_pnevmo  )

#undef MSG
	struct __send_mess_data__
	{
		unsigned xor, val;
		__send_mess_data__(unsigned xor, unsigned val): xor(xor), val(val){}
	};
	template<class O, class P>struct __send_mess__
	{
		void operator()(O &o, P &p)
		{
			if(0 != (o.value & p.xor))
			{
				Log::Mess<msg<O>::value>(0 != (o.value & p.val));
			}
		}
	};

	struct AllarmBits
	{
		typedef TL::MkTlst<	  
			iRPpr    
			, iOPpr     
			, iRPpo    
			, iOPpo    
			, iRPt     
			, iOPt     
		>::Result list1;
		typedef TL::MkTlst<	  
			iZU           
			, iPCH_B        
			, iPCH_RUN      
			, iPCH_A        
			//, iCycle        
			//, iReadyR1      
			//, iDone         
			//, iReserve0     
			//, iEtalon       
			, iWork_pnevmo  
			, iRevers_pnevmo
			, iError_pnevmo 
			//, iReserve1     
			//, iReserve2     
			//, iSQ1DM        
			//, iSQ2DM         
		>::Result list2;
		static unsigned bits1;
		static unsigned bits2;
		static unsigned msk1;
		static unsigned msk2;
		static void Do(unsigned inp1, unsigned inp2)
		{
			if(bits1 != (inp1 & msk1) || bits2 != (inp2 & msk2))
			{
				TL::foreach<list1, __send_mess__>()(Singleton<InputBit1Table>::Instance().items, __send_mess_data__(inp1 ^ bits1, inp1));
				TL::foreach<list2, __send_mess__>()(Singleton<InputBit2Table>::Instance().items, __send_mess_data__(inp2 ^ bits2, inp2));
				throw ExceptionAlarm();
			}
		}
	};
	unsigned AllarmBits::bits1;
	unsigned AllarmBits::bits2;
	unsigned AllarmBits::msk1;
	unsigned AllarmBits::msk2;

	struct Collection
	{
		static void Do(unsigned, unsigned)
		{
			//	TL::foreach<unit_bit_off_list, __test_bit_off__>()(unit_bit_off, bits);
			static unsigned counter = 0;
			if((++counter % 20) == 0) 
				unit502.Read();   //вызываться будет через ~100 м.сек.
		}
	};
#define ZZZ(sb, n, c) lir.sqItems.get<SQ<sb<n, c>>>().Do();
 /*

	AND_BITS(
				On<iSQ1t>
				, Proc<AllarmBits>
				, Proc<Collection>
				, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				)(60000); 
			ZZZ(on, Thick, 1)
			AND_BITS(
				On<iSQ2t>
				, Proc<AllarmBits>	
				, Proc<Collection>
				, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				)(60000); 
			ZZZ(on, Thick, 2)
   */

#define WAIT(inp, on, sen, num) AND_BITS(inp, Proc<AllarmBits>, Proc<Collection>, Ex<ExceptionStop>)(60000);\
	lir.sqItems.get<SQ<on<sen, num>>>().Do();

	void ControlTube(Data &)
	{
		SubLir &lir = Singleton<SubLir>::Instance();
		//lir.
		//TODO Проверка температуры обмоток соленоида
		TestCoilTemperature(); 
		//TODO Проверка модуля размагничивания
		CheckDemagnetizeModule();
		//TODO Установка рабочего положения модулей контроля
		SettingWorkingPositionControlModules();
		//TODO Если толщиномер используется передать параметры
		TransferParametersThicknessModule();
		//TODO Подготовка частотного преобразователя продольного модуля
		FrequencyInverterPreparation();
		///TODO Установка режима работы контроллера пневмооборудования
		SettingOperatingModeAirConditioningController();

		OUT_BITS(On<oCooling>);   /// включение вентилятора охлаждения

		Log::Mess<LogMess::InfoOnSycleBitIn>();
		AND_BITS(
			On<iCycle>	  /// \brief ожидание цикла
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(); 

		Sleep(1000);

		bool sop = TEST_IN_BITS(On<iEtalon>); /// если есть сигнал  то режим работы - проверка сопа

		if(sop)   /// вывод сообщения режима работы
		{
			Log::Mess<LogMess::SOP_MODE>();
		}
		else
		{
			Log::Mess<LogMess::TUBE_MODE>();
		}

		if(!sop) OUT_BITS(On<oReloc1>, On<oReloc2>); ///разрешение перекладки в транспортную систему

		AND_BITS(
			On<iReadyR1>	  /// \brief ожидание цикла
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(); 

		OUT_BITS(Off<oReloc1>, Off<oReloc2>, Off<oDefect>);

		CleaningScreen();	///Очистка экрана

		OnTheJobTable::TItems &job = Singleton<OnTheJobTable>::Instance().items;

		char numberTube[9] = "00000000";
		if(job.get<OnTheJob<ACS>>().value)	 /// если работа с АСУ
		{
			///TODO Запрос номера трубы, если "00000000" то повтор
			RequestPipeNumber(numberTube);
		}

		if(job.get<OnTheJob<Thick>>().value)	 /// если работа с толщиномером
		{
			OUT_BITS(On<oT_Cycle>);
		}
		if(job.get<OnTheJob<Long>>().value)	 /// если работа с продольным
		{
			Log::Mess<LogMess::WAITING_LONGITUDINAL_MODULE>();
			OUT_BITS(Off<oPowerSU>);
			FrequencyInverterRun();
			Sleep(4000);
			if(TEST_IN_BITS(Off<iPCH_B>))
			{
				Log::Mess<LogMess::iPCH_B_OFF>();
				throw ExceptionAlarm();
			}
		}
		if(job.get<OnTheJob<Thick>>().value)
		{
			Log::Mess<LogMess::WAITING_PERFORMANCE_THICKNESS_CONTROL_MODULE>();
			if(!TEST_IN_BITS(On<iReadyT>, On<iControlT>, Off<iResultT>))
			{
				Log::Mess<LogMess::ThicknessModuleNotReady>();
				throw ExceptionAlarm();
			}
		}

		OUT_BITS(On<oMagnet>, On<oRP>);
		Log::Mess<LogMess::PIPE_CONTROL_IMPLEMENTED>();

		EnableDemagnetization();

		///отслеживаемые биты для аварийного завершения программы
		AllarmBits::msk1 = 0;
		TL::foreach<AllarmBits::list1, __set_msk__>()(Singleton<InputBit1Table>::Instance().items, AllarmBits::msk1);
		AllarmBits::bits1 = device1730_1.Read() & AllarmBits::msk1;

		AllarmBits::msk2 = 0;		
		TL::foreach<AllarmBits::list2, __set_msk__>()(Singleton<InputBit2Table>::Instance().items, AllarmBits::msk2);
		AllarmBits::bits2 = device1730_2.Read() & AllarmBits::msk2;

		AND_BITS(
			On<iSQ1po>
			, Proc<AllarmBits>	 
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(60000); 
		unit502.Start();
		GUARD{unit502.Stop();};
		ZZZ(on, Cross, 1)

		//AND_BITS(
		//	On<iSQ2po>
		//	, Proc<AllarmBits>	 
		//	, Proc<Collection>
		//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
		//	)(60000); 
		//ZZZ(on, Cross, 2)
		WAIT(On<iSQ2po>, on, Cross, 2)
		if(job.get<OnTheJob<Thick>>().value)
		{
			Log::Mess<LogMess::WaitThickOn>();
			//AND_BITS(
			//	On<iSQ1t>
			//	, Proc<AllarmBits>
			//	, Proc<Collection>
			//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			//	)(60000); 
			//ZZZ(on, Thick, 1)
			//AND_BITS(
			//	On<iSQ2t>
			//	, Proc<AllarmBits>	
			//	, Proc<Collection>
			//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			//	)(60000); 
			//ZZZ(on, Thick, 2)
			WAIT(On<iSQ1t>, on, Thick, 1)
			WAIT(On<iSQ2t>, on, Thick, 2)
		}
		if(job.get<OnTheJob<Long>>().value)
		{
			Log::Mess<LogMess::WaitLongOn>();
			//AND_BITS(
			//	On<iSQ1pr>
			//	, Proc<AllarmBits>
			//	, Proc<Collection>
			//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			//	)(60000); 
			//prOnSQ1 = Performance::Counter();
			//AND_BITS(
			//	On<iSQ2pr>
			//	, Proc<AllarmBits>	
			//	, Proc<Collection>
			//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			//	)(60000); 
			//prOnSQ2 = Performance::Counter();
			WAIT(On<iSQ1pr>, on, Long, 1)
			WAIT(On<iSQ2pr>, on, Long, 2)
		}
		Log::Mess<LogMess::WaitMagneticOn>();
		//AND_BITS(
		//	On<iSQ1DM>
		//	, Proc<AllarmBits>
		//	, Proc<Collection>
		//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
		//	)(60000); 
		//mOnSQ1 = Performance::Counter();
		//AND_BITS(
		//	On<iSQ2DM>
		//	, Proc<AllarmBits>
		//	, Proc<Collection>
		//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
		//	)(60000); 
		//OUT_BITS(On<oT_Base>);
		//mOnSQ2 = Performance::Counter();
		WAIT(On<iSQ1DM>, on, Magn, 1)
		WAIT(On<iSQ2DM>, on, Magn, 2)
		OUT_BITS(On<oT_Base>);
//.................................................................
		Log::Mess<LogMess::WaitCrossOff>();
		//AND_BITS(
		//	Off<iSQ1po>
		//	, Proc<AllarmBits>	 
		//	, Proc<Collection>
		//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
		//	)(60000); 
		//poOffSQ1 = Performance::Counter();
		//AND_BITS(
		//	Off<iSQ2po>
		//	, Proc<AllarmBits>	 
		//	, Proc<Collection>
		//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
		//	)(60000); 
		//poOffSQ2 = Performance::Counter();
		WAIT(Off<iSQ1po>, off, Cross, 1)
		WAIT(Off<iSQ2po>, off, Cross, 2)
		if(job.get<OnTheJob<Thick>>().value)
		{
			Log::Mess<LogMess::WaitThickOff>();
			//AND_BITS(
			//	Off<iSQ1t>
			//	, Proc<AllarmBits>
			//	, Proc<Collection>
			//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			//	)(60000); 
			//tOffSQ1 = Performance::Counter();
			//AND_BITS(
			//	Off<iSQ2t>
			//	, Proc<AllarmBits>	
			//	, Proc<Collection>
			//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			//	)(60000); 
			//tOffSQ2 = Performance::Counter();
			WAIT(Off<iSQ1t>, off, Thick, 1)
			WAIT(Off<iSQ2t>, off, Thick, 2)

		}
		if(job.get<OnTheJob<Long>>().value)
		{
			Log::Mess<LogMess::WaitLongOff>();
			//AND_BITS(
			//	Off<iSQ1pr>
			//	, Proc<AllarmBits>
			//	, Proc<Collection>
			//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			//	)(60000); 
			//prOffSQ1 = Performance::Counter();
			//AND_BITS(
			//	Off<iSQ2pr>
			//	, Proc<AllarmBits>	
			//	, Proc<Collection>
			//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			//	)(60000);
			//prOffSQ2 = Performance::Counter();
			WAIT(Off<iSQ1pr>, off, Long, 1)
			WAIT(Off<iSQ2pr>, off, Long, 2)
		}
		Log::Mess<LogMess::WaitMagneticOff>();
		//AND_BITS(
		//	Off<iSQ1DM>
		//	, Proc<AllarmBits>
		//	, Proc<Collection>
		//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
		//	)(60000); 
		//mOffSQ1 = Performance::Counter();
		//AND_BITS(
		//	Off<iSQ2DM>
		//	, Proc<AllarmBits>
		//	, Proc<Collection>
		//	, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
		//	)(60000);
		//mOffSQ2 = Performance::Counter();
		WAIT(Off<iSQ1DM>, off, Magn, 1)
		WAIT(Off<iSQ2DM>, off, Magn, 2)
		unit502.Stop();
//---------------------------------------------------------------

	}
}
#undef ZZZ
#undef WAIT