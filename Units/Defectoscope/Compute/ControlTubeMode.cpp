#include "stdafx.h"
#include "ControlMode.h"
#include "tools_debug\DebugMess.h"
#include "ControlTubeSubModules.h"
#include "App\AppBase.h"
#include "Compute\Automat.hpp"
#include "Log\LogBuffer.h"
#include "ASU\Asu.h"
#include "PerformanceCounter\PerformanceCounter.h"

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

	void ControlTube(Data &)
	{
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

		CleaningScreen();

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

		unsigned poOnSQ1 = 0, poOnSQ2 = 0, prOnSQ1 = 0, prOnSQ2 = 0, tOnSQ1 = 0, tOnSQ2 = 0, mOnSQ1 = 0, mOnSQ2 = 0 ///время включения поперечного, продольногоб толщ., размаг.
		, poOffSQ1 = 0, poOffSQ2 = 0, prOffSQ1 = 0, prOffSQ2 = 0, tOffSQ1 = 0, tOffSQ2 = 0, mOffSQ1 = 0, mOffSQ2 = 0;///время откл. поперечного, продольногоб толщ., размаг.

		AND_BITS(
			On<iSQ1po>
			, Proc<AllarmBits>	 
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(60000); 
		poOnSQ1 = Performance::Counter();
		unit502.Start();
		GUARD{unit502.Stop();};

		AND_BITS(
			On<iSQ2po>
			, Proc<AllarmBits>	 
			, Proc<Collection>
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(60000); 
		poOnSQ2 = Performance::Counter();	
		if(job.get<OnTheJob<Thick>>().value)
		{
			Log::Mess<LogMess::WaitThickOn>();
			AND_BITS(
				On<iSQ1t>
				, Proc<AllarmBits>
				, Proc<Collection>
				, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				)(60000); 
			tOnSQ1 = Performance::Counter();
			AND_BITS(
				On<iSQ2t>
				, Proc<AllarmBits>	
				, Proc<Collection>
				, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				)(60000); 
			tOnSQ2 = Performance::Counter();
		}
		if(job.get<OnTheJob<Long>>().value)
		{
			Log::Mess<LogMess::WaitLongOn>();
			AND_BITS(
				On<iSQ1pr>
				, Proc<AllarmBits>
				, Proc<Collection>
				, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				)(60000); 
			prOnSQ1 = Performance::Counter();
			AND_BITS(
				On<iSQ2pr>
				, Proc<AllarmBits>	
				, Proc<Collection>
				, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				)(60000); 
			prOnSQ2 = Performance::Counter();
		}
		Log::Mess<LogMess::WaitMagneticOn>();
		AND_BITS(
			On<iSQ1DM>
			, Proc<AllarmBits>
			, Proc<Collection>
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(60000); 
		mOnSQ1 = Performance::Counter();
		AND_BITS(
			On<iSQ2DM>
			, Proc<AllarmBits>
			, Proc<Collection>
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(60000); 
		OUT_BITS(On<oT_Base>);
		mOnSQ2 = Performance::Counter();
//.................................................................
		Log::Mess<LogMess::WaitCrossOff>();
		AND_BITS(
			Off<iSQ1po>
			, Proc<AllarmBits>	 
			, Proc<Collection>
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(60000); 
		poOffSQ1 = Performance::Counter();
		AND_BITS(
			Off<iSQ2po>
			, Proc<AllarmBits>	 
			, Proc<Collection>
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(60000); 
		poOffSQ2 = Performance::Counter();
		if(job.get<OnTheJob<Thick>>().value)
		{
			Log::Mess<LogMess::WaitThickOff>();
			AND_BITS(
				Off<iSQ1t>
				, Proc<AllarmBits>
				, Proc<Collection>
				, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				)(60000); 
			tOffSQ1 = Performance::Counter();
			AND_BITS(
				Off<iSQ2t>
				, Proc<AllarmBits>	
				, Proc<Collection>
				, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				)(60000); 
			tOffSQ2 = Performance::Counter();
		}
		if(job.get<OnTheJob<Long>>().value)
		{
			Log::Mess<LogMess::WaitLongOff>();
			AND_BITS(
				Off<iSQ1pr>
				, Proc<AllarmBits>
				, Proc<Collection>
				, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				)(60000); 
			prOffSQ1 = Performance::Counter();
			AND_BITS(
				Off<iSQ2pr>
				, Proc<AllarmBits>	
				, Proc<Collection>
				, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				)(60000);
			prOffSQ2 = Performance::Counter();
		}
		Log::Mess<LogMess::WaitMagneticOff>();
		AND_BITS(
			Off<iSQ1DM>
			, Proc<AllarmBits>
			, Proc<Collection>
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(60000); 
		mOffSQ1 = Performance::Counter();
		AND_BITS(
			Off<iSQ2DM>
			, Proc<AllarmBits>
			, Proc<Collection>
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(60000);
		mOffSQ2 = Performance::Counter();
		unit502.Stop();
//---------------------------------------------------------------

	}
}