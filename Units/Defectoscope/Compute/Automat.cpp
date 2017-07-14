#include "stdafx.h"
#include "Automat.h"
#include "Automat.hpp"
#include "Compute\AppKeyHandler.h"
#include "tools_debug\DebugMess.h"
#include "Log\LogMessageToTopLabel.h"
#include "Log/LogBuffer.h"
#include "Compute/Unit502.h"
//#include "Compute/Compute.h"
#include "PerformanceCounter\PerformanceCounter.h"
#include "Windows\MainWindow.h"
#include "Dates\ComputeSolid.h"
#include "Dialogs/PacketDlg.h"
#include "ControlMode.h"
#include "Compute\ControlTubeSubModules.h"

namespace AutomatN
{
	void(*ptrProc)(Mode::Data &) = NULL;
#define EX(n, init_state)template<>struct Ex<Exception##n>{static HANDLE handle;};\
	HANDLE Ex<Exception##n>::handle = CreateEvent(NULL, init_state, FALSE, NULL);

	EX(Run, TRUE)

	EX(ReturnTube, FALSE)
	EX(ExitTube, FALSE)

#undef EX

	HANDLE Ex<ExceptionStop>::handle = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE Ex<ExceptionExit>::handle = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE Ex<ExceptionContinue>::handle = CreateEvent(NULL, FALSE, FALSE, NULL);

	HANDLE hThread;
//----------------------------------------------------------------------
	template<class T>struct collections_data_unit502
	{
		void operator()(){}
	};

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

#define UNIT_BIT_ON(n) unit_bit_on.get<UnitBit<On<n>>>()() 

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
	template<class O, class P>struct __test_bit_off__
	{
		void operator()(O &o, P &p)
		{
			o(p);
		}
	};	
	//struct ComputeData
	//{
	//	static void Do(unsigned bits)
	//	{
	//		static unsigned time = 0;
	//		unsigned t = GetTickCount();
	//		if(t - time > 1000)
	//		{
	//			time = t;
	//			Compute::Do();
	//		}
	//	}
	//};
//----------------------------------------------------------------------
	ExceptionAlarm::ExceptionAlarm()
	{
		Log::TData *d;
		Log::TailMessage(d);
		LogMess::FactoryMessages &f = LogMess::FactoryMessages::Instance();
		char c[128];
		f.Text(d->id, c, d->value);
		MessageBoxA(app.mainWindow.hWnd, c, "Сообщение!!!", MB_ICONINFORMATION);
	}
	//----------------------------------------------------------------------------
	static DWORD WINAPI Do(LPVOID)
	{		
		device1730_1.Write(0);
		device1730_2.Write(0);
		AppKeyHandler::Init();
		LogMessageToTopLabel logMessageToTopLabel;
		Log::Mess<LogMess::ProgramOpen>(0);

		Mode::Data data;
		data.packet = true;
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
#else
				AND_BITS(
					Ex<ExceptionRun>
					, Ex<ExceptionReturnTube>
					, Ex<ExceptionExitTube>
					, Ex<ExceptionStop>
				)(); //кнопка начала измерений
				(*ptrProc)(data);
#endif
				
			}
			catch(ExceptionStop)
			{
				ResetEvent(Ex<ExceptionRun>::handle);
				Log::Mess<LogMess::InfoUserStop>();	
				device1730_1.Write(0);
				device1730_2.Write(0);
				DisableDemagnetization();
				AppKeyHandler::Stop();
				data.packet = true;
			}
			catch(ExceptionTimeOut)
			{
				ResetEvent(Ex<ExceptionRun>::handle);
				Log::Mess<LogMess::TimeoutPipe>();	
				device1730_1.Write(0);
				device1730_2.Write(0);
				DisableDemagnetization();
			//	AppKeyHandler::Stop();
				data.packet = true;
			}
			catch(ExceptionAlarm)
			{
				ResetEvent(Ex<ExceptionRun>::handle);
				device1730_1.Write(0);
				device1730_2.Write(0);
				DisableDemagnetization();
			//	AppKeyHandler::Stop();
				data.packet = true;
			}
			catch(ExceptionExit)
			{			
				Log::Mess<LogMess::ProgramClosed>();	
				device1730_1.Write(0);
				device1730_2.Write(0);
				DisableDemagnetization();
				return 0;
			}
		}
		device1730_1.Write(0);
		device1730_2.Write(0);
		DisableDemagnetization();
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
	AppKeyHandler::Run();
	 SetEvent(AutomatN::Ex<AutomatN::ExceptionRun>::handle);	
}

void Automat::Continue()
{
	AppKeyHandler::Run();
	SetEvent(AutomatN::Ex<AutomatN::ExceptionContinue>::handle);	
}
void Automat::Exit()
{
	 SetEvent(AutomatN::Ex<AutomatN::ExceptionExit>::handle);
	 WaitForSingleObject(AutomatN::hThread, 50000);
	 dprint("Exit from automat loop\n");
}

void Automat::ReturnTube()
{
	AppKeyHandler::ReturnTubeMode();
	SetEvent(AutomatN::Ex<AutomatN::ExceptionReturnTube>::handle);
}


void Automat::ExitTube()
{
	AppKeyHandler::ExitTubeMode();
	SetEvent(AutomatN::Ex<AutomatN::ExceptionExitTube>::handle);
}
