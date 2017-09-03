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
#include "Compute\Compute.hpp"
#include "App/App.h"
#include "Windows\MainWindow.h"
#include "window_tool\Emptywindow.h"
#include "Compute\ComputeResult.h"
#include "Compute\AppKeyHandler.h"

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
			, iWork_pnevmo  
			, iRevers_pnevmo
			, iError_pnevmo 
		>::Result list2;
		static unsigned bits1;
		static unsigned bits2;
		static unsigned msk1;
		static unsigned msk2;
		template<class T>static void Do(T &t)
		{
			if(bits1 != (t.val1 & msk1) || bits2 != (t.val2 & msk2))
			{
				TL::foreach<list1, __send_mess__>()(Singleton<InputBit1Table>::Instance().items, __send_mess_data__(t.val1 ^ bits1, t.val1));
				TL::foreach<list2, __send_mess__>()(Singleton<InputBit2Table>::Instance().items, __send_mess_data__(t.val2 ^ bits2, t.val2));
				
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
		template<class T>static void Do(T &t)
		{
			static unsigned counter = 0;
			if((++counter % 20) == 0)
			{
				unit502.Read();   //вызываться будет через ~100 м.сек.
			}
		}
	};

	//SubLir &lir = Singleton<SubLir>::Instance();
	ComputeUnitX<Cross, ItemData<Cross>>CrossX(Singleton<ItemData<Cross>>::Instance()); 
	ComputeUnitX<Long, ItemData<Long>>LongX(Singleton<ItemData<Long>>::Instance()); 

	template<class T>struct __updata_window__
	{
		void operator()(){}
	};

	struct ComputeZones
	{
		template<class T>static void Do(T &)
		{
			static unsigned counter = 0;

			if((++counter % 20) == 0) 
			{
				SubLir &lir = Singleton<SubLir>::Instance();
				lir.Do();   //вызываться будет через ~100 м.сек.
				if(CrossX.Zones(lir.moduleItems.get<Module<Cross>>().zonesOffs)) __updata_window__<Cross>()();
				if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long>>().value && LongX.Zones(lir.moduleItems.get<Module<Long>>().zonesOffs))__updata_window__<Long>()();
			}
		}
	};

	template<>struct __updata_window__<Cross>
	{
		void operator()()
		{
			SubLir &lir = Singleton<SubLir>::Instance();
			unsigned z = Singleton<ItemData<Cross>>::Instance().currentOffsetZones = lir.moduleItems.get<Module<Cross>>().zonesOffs;
			dprint("cross zones %d\n", z);
			RepaintWindow(app.mainWindow.viewers.get<CrossViewer>().hWnd);
		}
	};
	template<>struct __updata_window__<Long>
	{
		void operator()()
		{
			SubLir &lir = Singleton<SubLir>::Instance();
			unsigned z = Singleton<ItemData<Long>>::Instance().currentOffsetZones = lir.moduleItems.get<Module<Long>>().zonesOffs;
			dprint("long zones %d\n", z);
			RepaintWindow(app.mainWindow.viewers.get<LongViewer>().hWnd);
		}
	};	

	template<class T>struct __compute_unit__
	{
		void operator()(SubLir &lir, ComputeUnitX<T, ItemData<T>> &x)
		{
			if(x.Zones(lir.moduleItems.get<Module<T>>().zonesOffs))
			{
				__updata_window__<T>()();
			}
		}
	};

	template<>struct ComputeUnitX<Thick, int>{};
	template<>struct ComputeUnitX<Magn, int>{};

	template<>struct __compute_unit__<Thick>{void operator()(SubLir &, ComputeUnitX<Thick, int>&){}};
	template<>struct __compute_unit__<Magn>{void operator()(SubLir &, ComputeUnitX<Magn, int> &){}};

	


#define ZZZ(sb, n, c) lir.sqItems.get<SQ<sb<n, c>>>().Do();

#define WAIT(inp, on, sen) AND_BITS(inp, Proc<AllarmBits>, Proc<Collection>, Ex<ExceptionStop>)(60000);\
	lir.sqItems.get<SQ<on<sen, 1>>>().Do();
 
#define WAIT_COMPUTE(inp, on, sen) AND_BITS(inp, Proc<AllarmBits>, Proc<Collection>, Ex<ExceptionStop>)(60000);\
	lir.sqItems.get<SQ<on<sen, 2>>>().Do();\
	__compute_unit__<sen>()(lir, sen##X);

	void ControlTube(Data &)
	{
		if(!TestControlCircuit())
		{
			if(WAIT_TIMEOUT != WaitForSingleObject(Ex<ExceptionStop>::handle , INFINITE))
			{
				throw ExceptionStop();//выход  по кнопке стоп
			}
		}
		SubLir &lir = Singleton<SubLir>::Instance();
		ItemData<Long> &dataLong = Singleton<ItemData<Long>>::Instance();
		ItemData<Cross> &dataCross = Singleton<ItemData<Cross>>::Instance();


		Module<Long> &ml = lir.moduleItems.get<Module<Long>>();
		Module<Cross> &cl = lir.moduleItems.get<Module<Cross>>();
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
			OUT_BITS(On<oReloc1>, On<oReloc2>); ///разрешение перекладки в транспортную систему
		}

		Log::Mess<LogMess::waitingPipeEntranceRollerTable>();
		AND_BITS(
			On<iReadyR1>	  /// \brief ожидание цикла
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(); 

		OUT_BITS(Off<oReloc1>, Off<oReloc2>, Off<oDefect>);

		lir.Start();
		
		CrossX.Clear();
		LongX.Clear();
		CleaningScreen();	///Очистка экрана

		OnTheJobTable::TItems &job = Singleton<OnTheJobTable>::Instance().items;

		char numberTube[9] = "00000000";
		if(job.get<OnTheJob<ACS>>().value)	 /// если работа с АСУ
		{
			///TODO Запрос номера трубы, если "00000000" то повтор
			RequestPipeNumber(numberTube);
		}
		OUT_BITS(Off<oReloc1>); /// 6.10
		if(job.get<OnTheJob<Thick>>().value)	 /// если работа с толщиномером
		{
			OUT_BITS(On<oT_Cycle>);	 //6.11
		}
		if(job.get<OnTheJob<Long>>().value)	 /// если работа с продольным
		{
			Log::Mess<LogMess::WAITING_LONGITUDINAL_MODULE>();
			OUT_BITS(On<oPowerSU>);
			FrequencyInverterRun();

			AND_BITS(
				  On<iPCH_B  >
				, Off<iPCH_RUN>
				, Off<iPCH_A  >
				, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				, Proc<ExceptionAl<LogMess::NoLongDriveReady>>	/// если нет готовности - выход
				)(4000);  /// \brief ожидание 4 сек

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
		unsigned msk1 = 0;
		TL::foreach<AllarmBits::list1, __set_msk__>()(Singleton<InputBit1Table>::Instance().items, msk1);
		AllarmBits::bits1 = device1730_1.Read() & msk1;
		AllarmBits::msk1 = msk1;

		unsigned msk2 = 0;		
		TL::foreach<AllarmBits::list2, __set_msk__>()(Singleton<InputBit2Table>::Instance().items, msk2);
		AllarmBits::bits2 = device1730_2.Read() & msk2;
		AllarmBits::msk2 = msk2;
		AND_BITS(
			On<iSQ1po>							  ///ожидание наезда трубы на датчик
			, Proc<AllarmBits>
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(60000); 
		unit502.Start();
		GUARD{unit502.Stop();};	  /// \brief выключает 502 при досрочном выходе из цикла 
		ZZZ(on, Cross, 1)  /// сохранение времени наезда на датчик поперечный 
		WAIT_COMPUTE(On<iSQ2po>, on, Cross)

		ComputeUnitX<Thick, int>ThickX;
		ComputeUnitX<Magn, int>MagnX;

		if(job.get<OnTheJob<Thick>>().value)
		{
			Log::Mess<LogMess::WaitThickOn>();
			WAIT(On<iSQ1t>, on, Thick)			
			WAIT_COMPUTE(On<iSQ2t>, on, Thick)
		}
		if(job.get<OnTheJob<Long>>().value)
		{
			Log::Mess<LogMess::WaitLongOn>();
			WAIT(On<iSQ1pr>, on, Long)
			WAIT_COMPUTE(On<iSQ2pr>, on, Long)	
			FrequencyInverterRunWork(); // включение рабочей скорости вращения
		}
				
		Log::Mess<LogMess::WaitMagneticOn>();
		WAIT(On<iSQ1DM>, on, Magn)
		WAIT_COMPUTE(On<iSQ2DM>, on, Magn)
		OUT_BITS(On<oT_Base>);		
//.................................................................		
		Log::Mess<LogMess::WaitCrossOff>();
		AND_BITS(
			Off<iSQ1po>
			, Proc<AllarmBits>	 	  
			, Proc<Collection>	   
			, Proc<ComputeZones>   
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(60000); 
		ZZZ(off, Cross, 1)
		WAIT_COMPUTE(Off<iSQ2po>, off, Cross)

		///Расчёт мёртвой зоны начало
		CrossX.DeathZonesBegin();
		if(job.get<OnTheJob<Long>>().value)
		{
			LongX.DeathZonesBegin();
		}

		if(job.get<OnTheJob<Thick>>().value)
		{
			Log::Mess<LogMess::WaitThickOff>();
			WAIT(Off<iSQ1t>, off, Thick)
			WAIT_COMPUTE(Off<iSQ2t>, off, Thick)

		}

		if(job.get<OnTheJob<Long>>().value)
		{
			Log::Mess<LogMess::WaitLongOff>();
			WAIT(Off<iSQ1pr>, off, Long)
			WAIT_COMPUTE(Off<iSQ2pr>, off, Long)
		}

		Log::Mess<LogMess::WaitMagneticOff>();
		WAIT(Off<iSQ1DM>, off, Magn)
		Log::Mess<LogMess::tubeExit>();
		WAIT_COMPUTE(Off<iSQ2DM>, off, Magn)
		
		OUT_BITS(Off<oT_Base>, Off<oSTF>, Off<oPowerSU>, Off<oMagnet>, Off<oRP>);
		DisableDemagnetization();
		unit502.Stop();
//---------------------------------------------------------------	
		GetDataFromThicknessModule();

		ComputeResult();
		UpdateScreen();
#ifdef EMUL
		SubLir &Xlir = lir;
		
		dprint("count lond modules zones %d\n", ml.zonesOffs);

		for(int i = 1; i < ml.zonesOffs; ++i)
		{
			dprint("samples in zone %d  %d st %d    %d\n", i, ml.zones[i] - ml.zones[i - 1], dataLong.status[0][i], cl.zones[i] - cl.zones[i - 1]);
		}
		dprint("count lond modules zones %d\n", ml.zonesOffs);
		OUT_BITS(Off<oCooling>);
#endif

		if(job.get<OnTheJob<ViewInterrupt>>().value)
		{
		//	Log::Mess<LogMess::interruptView>();
			AppKeyHandler::Continue();	/// включили кнопку продолжить
			AND_BITS(
				Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				, Ex<ExceptionContinue>	 /// \brief Выход по кнопке продолжить
				)(); 
		}
		
		WorkACS(numberTube); //передача в асу
		StoredData(!sop); // сохранение в базе
//---------------------------------------------------------------
		//TODO Проверка температуры обмоток соленоида
		TestCoilTemperature(); 
	}
//////////////////////////////////////////////test 

void TestRUN___()
	{
		ItemData<Cross> &dataCross = Singleton<ItemData<Cross>>::Instance();
		   
		SubLir &lir = Singleton<SubLir>::Instance();

		Module<Cross> &cl = lir.moduleItems.get<Module<Cross>>();
		
		CrossX.Clear();

		__compute_unit__<Cross>()(lir, CrossX);

		ComputeUnitX<Magn, int>MagnX;

		__compute_unit__<Magn>()(lir, MagnX);
//.................................................................		
		
		ZZZ(off, Cross, 1)
		WAIT_COMPUTE(Off<iSQ2po>, off, Cross)

		///Расчёт мёртвой зоны начало
		CrossX.DeathZonesBegin();
		

		WAIT(Off<iSQ1DM>, off, Magn)
		WAIT_COMPUTE(Off<iSQ2DM>, off, Magn)
		
		

		ComputeResult();
		UpdateScreen();

	}
	}

///////////////////////////////////////////////
#undef ZZZ
#undef WAIT