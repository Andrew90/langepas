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

	//EX(Run, TRUE)

	EX(ReturnTube, FALSE)
	EX(ExitTube, FALSE)

#undef EX

	HANDLE Ex<ExceptionStop>::handle = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE Ex<ExceptionExit>::handle = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE Ex<ExceptionContinue>::handle = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE Ex<ExceptionRun>::handle = CreateEvent(NULL, TRUE, FALSE, NULL);

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
				AND_BITS(
					Ex<ExceptionRun>
					, Ex<ExceptionReturnTube>
					, Ex<ExceptionExitTube>
					, Ex<ExceptionStop>
				)(); //кнопка начала измерений

				(*ptrProc)(data);
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
