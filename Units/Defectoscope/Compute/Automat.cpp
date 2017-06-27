#include "stdafx.h"
#include "Automat.h"
#include "Automat.hpp"
#include "Compute\AppKeyHandler.h"
#include "tools_debug\DebugMess.h"
#include "Log\LogMessageToTopLabel.h"
#include "Log/LogBuffer.h"
#include "Compute/Unit502.h"
#include "Compute/Compute.h"
#include "PerformanceCounter\PerformanceCounter.h"
#include "Windows\MainWindow.h"
#include "Dates\ComputeSolid.h"
#include "Dialogs/PacketDlg.h"

namespace AutomatN
{
#define EX(n, init_state)template<>struct Ex<Exception##n>{static HANDLE handle;};\
	HANDLE Ex<Exception##n>::handle = CreateEvent(NULL, init_state, FALSE, NULL);

	EX(Exit, TRUE)
	EX(Stop, FALSE)
	EX(Continue, TRUE)
	EX(Run, TRUE)
	//EX(Alarm, FALSE)

#undef EX

//	unsigned &Off<iСontrolСircuits>::bit = Singleton<InputBitTable>::Instance().items.get<iСontrolСircuits>().value;
//	unsigned &Off<iCycle>::bit =	Singleton<InputBitTable>::Instance().items.get<iCycle>().value;

	HANDLE hThread;
//----------------------------------------------------------------------
	//Unit502 &unit502 = Singleton<Unit502>::Instance();
	template<class T>struct collections_data_unit502
	{
		void operator()(){}
	};
	//template<>struct collections_data_unit502<On<iCross0>>
	//{
	//	void operator()()
	//	{
	//		unit502.crossOn = true;
	//	}
	//};
	//template<>struct collections_data_unit502<On<iLong0>>
	//{
	//	void operator()()
	//	{
	//		unit502.longOn = true;
	//	}
	//};
	//template<>struct collections_data_unit502<On<iSolid>>
	//{
	//	void operator()()
	//	{
	//		unit502.solidOn = true;
	//	}
	//};
	//
	//template<>struct collections_data_unit502<Off<iCross1>>
	//{
	//	void operator()()
	//	{
	//		unit502.crossOn = false;
	//	}
	//};
	//template<>struct collections_data_unit502<Off<iLong1>>
	//{
	//	void operator()()
	//	{
	//		unit502.longOn = false;
	//	}
	//};
	//template<>struct collections_data_unit502<Off<iSolid>>
	//{
	//	void operator()()
	//	{
	//		unit502.solidOn = false;
	//	}
	//};

	template<class T>struct UnitBit;
	template<class T>struct UnitBit<On<T>>
	{
		typedef T type;
		unsigned &time;
		UnitBit()
			: time(Singleton<Time<On<T>>>::Instance().value)
		{}
		void operator()()
		{
			if(0 == time)
			{
				collections_data_unit502<On<T>>()();
				time = Performance::Counter();
			//	zprint("\n");
			}
		}
	};
	
	template<class T>struct __mess__{void operator()(){}};
	//template<>struct __mess__<iCross1>
	//{
	//	void operator()()
	//	{
	//	   Log::Mess<LogMess::OffiCross>();
	//	}
	//};
	//template<>struct __mess__<iLong1>
	//{
	//	void operator()()
	//	{
	//	   Log::Mess<LogMess::OffiLong>();
	//	}
	//};
	//template<>struct __mess__<iThick1>
	//{
	//	void operator()()
	//	{
	//	   Log::Mess<LogMess::OffiThick>();
	//	}
	//};
	template<class T>struct UnitBit<Off<T>>		  
	{
		typedef T type;
		unsigned &time;
		unsigned bit;
		UnitBit()
			: time(Singleton<Time<Off<T>>>::Instance().value)
		{}
		void operator()(unsigned in)
		{
			if(0 == (in & bit) && 0 == time && 0 != unit_bit_on.get<UnitBit<On<T>>>().time)
			{
				collections_data_unit502<Off<T>>()();
				time = Performance::Counter();
				__mess__<T>()();
			}
		}
	};

	//template<>struct UnitBit<Off<iSolid>>
	//{
	//	typedef iSolid type;
	//	unsigned &time;
	//	UnitBit()
	//		: time(Singleton<Time<Off<iSolid>>>::Instance().value)
	//	{}
	//	void operator()()
	//	{
	//		if(0 == time)
	//		{
	//			collections_data_unit502<Off<iSolid>>()();
	//			time = Performance::Counter();
	//		}
	//	}
	//};


	//typedef TL::MkTlst<
	//	UnitBit<Off<iCross0>>
	//	, UnitBit<Off<iCross1>>
	//	, UnitBit<Off<iLong0 >>
	//	, UnitBit<Off<iLong1 >>
	//	, UnitBit<Off<iThick0>>
	//	, UnitBit<Off<iThick1>>
	//>::Result unit_bit_off_list;
	//
	//typedef TL::MkTlst<
	//	  UnitBit<On<iCross0>>
	//	, UnitBit<On<iCross1>>
	//	, UnitBit<On<iLong0 >>
	//	, UnitBit<On<iLong1 >>
	//	, UnitBit<On<iThick0>>
	//	, UnitBit<On<iThick1>>
	//	, UnitBit<On<iSolid>>
	//	, UnitBit<Off<iSolid>>
	//>::Result unit_bit_on_list;

#define UNIT_BIT_ON(n) unit_bit_on.get<UnitBit<On<n>>>()() 

//	TL::Factory<unit_bit_off_list>  unit_bit_off;
//	TL::Factory<unit_bit_on_list>  unit_bit_on;
	template<class O, class P>struct unit_bit_off_init
	{
		void operator()(O &o, P &p)
		{
			o.bit = p.get<typename O::type>().value;
			o.time = 0;
		}
	};
	template<class O, class P>struct unit_bit_on_init
	{
		void operator()(O &o, P &p)
		{
			o.time = 0;
		}
	};
//	void UnitBitInit()
//	{
//		TL::foreach<unit_bit_off_list, unit_bit_off_init>()(
//			unit_bit_off
//			, Singleton<InputBitTable>::Instance().items
//			);
//		TL::foreach<unit_bit_on_list, unit_bit_on_init>()(
//			unit_bit_on
//			, Singleton<InputBitTable>::Instance().items
//			);
//	};
	template<class O, class P>struct __test_bit_off__
	{
		void operator()(O &o, P &p)
		{
			o(p);
		}
	};
	struct Collection
	{
		static void Do(unsigned bits)
		{
		//	TL::foreach<unit_bit_off_list, __test_bit_off__>()(unit_bit_off, bits);
		//	static unsigned counter = 0;
		//	if((++counter % 20) == 0) 
		//		unit502.Read();   //вызываться будет через ~100 м.сек.
		}
	};

	struct ComputeData
	{
		static void Do(unsigned bits)
		{
			static unsigned time = 0;
			unsigned t = GetTickCount();
			if(t - time > 1000)
			{
				time = t;
				Compute::Do();
			}
		}
	};

	//unsigned &iСontrolСircuitsBit = Singleton<InputBitTable>::Instance().items.get<iСontrolСircuits>().value;

	//struct Alarm
	//{
	//	static void Do(unsigned bits)
	//	{
	//		if(0 == (bits & iСontrolСircuitsBit))
	//		{
	//			Log::Mess<LogMess::AlarmExitControlCircuitBitIn>();
	//			throw ExceptionAlarm();
	//		}
	//	}
	//};
//----------------------------------------------------------------------
	static DWORD WINAPI Do(LPVOID)
	{		
		device1730.Write(0);
		AppKeyHandler::Init();
		LogMessageToTopLabel logMessageToTopLabel;
		Log::Mess<LogMess::ProgramOpen>(0);

		bool &onTheJobCross = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Cross>>().value;
		bool &onTheJobLong = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long>>().value;
		bool &onTheJobThick = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thick>>().value;
		bool packet = true;
		while(true)
		{
			try
			{
#if 0
				App::measurementOfRunning = false;	
				AND_BITS(Ex<ExceptionRun>, Ex<ExceptionStop>)(); //кнопка начала измерений
				if(packet && !PacketDlg::Do())
				{
					ResetEvent(Ex<ExceptionRun>::handle);
					continue;
				}
				packet = false;
				dprint("collection Start\n");
				AppKeyHandler::Run();
				App::measurementOfRunning = true;

				Log::Mess<LogMess::WaitControlCircuitBitIn>();
				AND_BITS(Ex<ExceptionStop>, On<iСontrolСircuits>)(20000);

				Log::Mess<LogMess::TubeOnPosition>();
				AND_BITS(Ex<ExceptionStop>, On<iReady>)(20000);

				Compute::Clear();
				zprint(" iReady\n");

				Log::Mess<LogMess::OniCross>();
				AND_BITS(On<iCross0>, Proc<Alarm>, Ex<ExceptionStop>)(10000);
				zprint(" UnitBitInit\n");
				UnitBitInit();
				Lir::Clear();
			
				unit502.Start(); 
				Lan::Start();
				zprint(" Lan::Start()\n");
				GUARD{
					unit502.Stop();
					Lan::Stop();
					dprint("unit502.Stop();\n");
				};	  //при выходе из итерации цикла по любой причине останавливает плату

				UNIT_BIT_ON(iCross0);
				Singleton<MainWindow>::Instance().ClearCharts();
				AND_BITS(On<iCross1>, Proc<Alarm>, Proc<Collection>, Ex<ExceptionStop>)(10000);
				UNIT_BIT_ON(iCross1);
				dprint("Compute::Do();\n");
				Compute::Do();

				

		//		if(onTheJobLong)
				{
					Log::Mess<LogMess::OniLong>();
					AND_BITS(On<iLong0>, Proc<Alarm>, Proc<Collection>, Ex<ExceptionStop>)(10000);
					UNIT_BIT_ON(iLong0);
					AND_BITS(On<iLong1>, Proc<Alarm>, Proc<Collection>, Ex<ExceptionStop>)(10000);
					UNIT_BIT_ON(iLong1);
					Compute::Do();
				}
				dprint("__1\n");

		//		if(onTheJobThick)
				{
					Log::Mess<LogMess::OniThick>();
					AND_BITS(On<iThick0>, Proc<Alarm>, Proc<Collection>, Ex<ExceptionStop>)(10000);
					UNIT_BIT_ON(iThick0);
					AND_BITS(On<iThick1>, Proc<Alarm>, Proc<Collection>, Ex<ExceptionStop>)(10000);
					UNIT_BIT_ON(iThick1);
					Compute::Do();
				}

				dprint("__2\n");

				ItemData<Long> &longItem = Singleton<ItemData<Long>>::Instance();
				ItemData<Cross> &crossItem = Singleton<ItemData<Cross>>::Instance();

				Log::Mess<LogMess::OniSolid>();
				AND_BITS(On<iSolid>, Proc<Alarm>, Proc<Collection>, Ex<ExceptionStop>)(10000);
				UNIT_BIT_ON(iSolid);

				AND_BITS(Off<iSolid>, Proc<Alarm>, Proc<Collection>, Proc<ComputeData>, Ex<ExceptionStop>)(40000);
				unit_bit_on.get<UnitBit<Off<iSolid>>>()();
				Log::Mess<LogMess::OffiSolid>();
				unit502.Stop();
				Lan::Stop();
				Log::Mess<LogMess::InfoDataCollectionComplete>();
				Compute::Do();

				//расчёт группы прочности
				double result = 0; 
				wchar_t *groupName = L""; 
				unsigned color;
				ComputeSolid::Recalculation(result, groupName, color);
				//расчёт группы прочности конец

				zprint("  collection stop\n");
#endif
				
			}
			catch(ExceptionStop)
			{
				ResetEvent(Ex<ExceptionRun>::handle);
				Log::Mess<LogMess::InfoUserStop>();	
				device1730.Write(0);
				AppKeyHandler::Stop();
				packet = true;
			}
			catch(ExceptionTimeOut)
			{
				ResetEvent(Ex<ExceptionRun>::handle);
				Log::Mess<LogMess::TimeoutPipe>();	
				device1730.Write(0);
				AppKeyHandler::Stop();
				packet = true;
			}
			catch(ExceptionAlarm)
			{
				ResetEvent(Ex<ExceptionRun>::handle);
				device1730.Write(0);
				AppKeyHandler::Stop();
				packet = true;
			}
			catch(ExceptionExit)
			{			
				Log::Mess<LogMess::ProgramClosed>();	
				device1730.Write(0);
				return 0;
			}
		}
		return 0;
	}
}

void Automat::Init()
{
	AutomatN::hThread = CreateThread(NULL, 0, AutomatN::Do, NULL, 0, NULL);
}

void Automat::Stop()
{
	 SetEvent(AutomatN::Ex<AutomatN::ExceptionStop>::handle);
}

void Automat::Start()
{
	 SetEvent(AutomatN::Ex<AutomatN::ExceptionRun>::handle);	
}

void Automat::Continue(){}
void Automat::Exit()
{
	 SetEvent(AutomatN::Ex<AutomatN::ExceptionExit>::handle);
	 WaitForSingleObject(AutomatN::hThread, 5000);
	 dprint("Exit from automat loop\n");
}
