#include "stdafx.h"
#include "Compute\Compute.h"
#include <Windows.h>
#include "Automat.h"
#include "App\App.h"
#include "App\AppBase.h"
#include "DataItem\DataItem.h"
#include "Compute\Automat.hpp"
#include "tools_debug\DebugMess.h"
#include "Windows\MainWindow.h"
#include "window_tool\Emptywindow.h"
#include "Compute\ComputeZone.h"
#include "DataItem\ThicknessData.h"
#include "window_tool\AnimationControl.h"
#include "Filtre\MedianFiltre.h"

namespace Compute
{
	using namespace AutomatN;
	HANDLE hThread, hEvent, hExit, hFrameReady;

	template<class T, int N>struct TimeX;

#define TIMEX(name, num) template<>struct TimeX<name, num>{typedef i##name##num Result;};
	TIMEX(Cross, 0)
	TIMEX(Cross, 1)
	TIMEX(Long, 0)
	TIMEX(Long, 1)
	TIMEX(Thick, 0)
	TIMEX(Thick, 1)
#undef TIMEX

	struct __data__
	{
		int currentOffsetZones;
		int countUnits;
		int stopUnits;
		__data__()
			: currentOffsetZones(0)
		//	, countUnits(App::count_Thick_sensors)
		{}
	};

	template<class T>struct StopPoint
	{
		void operator()()
		{
		}
	};

	template<>struct StopPoint<Thick>
	{
		void operator()()
		{
			dprint(".");
		}
	};

	void ComputeDeadZones();

	template<class T>struct Unit
	{
		unsigned &start0, &start1, &stop0, &stop1;
		unsigned start, stop;
		void (Unit::*ptrProc)(__data__ &);
		ItemData<T> &item;
		TimeLir &timeLir;
		int index;
		int lir;
		int lirIndex;
		int startZone;
		int widthFiltre;
		//MedianFiltre filtre;
		Unit()
			: start0(Singleton<Time<On<TimeX<T,0>::Result>>>::Instance().value)
			, start1(Singleton<Time<On<TimeX<T,1>::Result>>>::Instance().value)
			, stop0(Singleton<Time<Off<TimeX<T,0>::Result>>>::Instance().value)
			, stop1(Singleton<Time<Off<TimeX<T,1>::Result>>>::Instance().value)
			, ptrProc(&Unit::Start)
			, item(Singleton<ItemData<T>>::Instance())
			, timeLir(Singleton<TimeLir>::Instance())
		{
		}
#undef TIMEX

		struct common_data
		{

		};

		inline double Ratio(unsigned a, unsigned b, unsigned c)
		{
			return double(b - a)/(c - a);
		}

		void Init()
		{
			  ZeroMemory(item.frame, sizeof(item.frame));
			  ZeroMemory(item.time, sizeof(item.time));
			  start1 = start0 = 0;
		}

		void Start(__data__ &)
		{		
		   if(0 != start1)
		   {		
			   zprint("~~~~~~~~~\n");
			   start = start0 + (start1 - start0) / 2;
			   int i = 0;
			   for(; 0 != item.time[i] && item.time[i] < start; ++i);
			   if(0 == item.time[i]) return;
			   double ratio = Ratio(item.time[i - 1], start, item.time[i]);
			   item.offsets[0] = item.frame[i - 1] + int(ratio *(item.frame[i] - item.frame[i - 1]));
			   index = i;
			   item.currentOffsetZones = 0;

			   i = 0;
			   for(;timeLir.time[i] < start; ++i);
			   ratio = Ratio(timeLir.time[i - 1], start, timeLir.time[i]);
			   lir = timeLir.lir[i - 1] + int(ratio *(timeLir.lir[i] - timeLir.lir[i - 1]));
			   zprint("  first offset %d  \n", lir);
			   lirIndex = i;
			   ptrProc = &Unit::Next;
			   ZeroMemory(item.status, sizeof(item.status));
			   widthFiltre = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T>>().value;
			   if(0 != widthFiltre)
			   {
				  widthFiltre = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T>>().value;
			   }
			   //filtre.InitWidth(widthFiltre);
		   }
		}

		void Next(__data__ &d)
		{
			for(int i = lirIndex; i < timeLir.currentTimeOffset; ++i)
			{
				int t = timeLir.lir[i] - lir;
				if(t > App::zone_length)
				{
					t = lir - timeLir.lir[i - 1] + App::zone_length;
					double ratio = Ratio(timeLir.lir[i - 1], timeLir.lir[i - 1] + t, timeLir.lir[i - 0]);
					unsigned time = timeLir.time[i - 1] + int(ratio *(timeLir.time[i - 0] - timeLir.time[i - 1])); 

					StopPoint<T>()();

					int j = index;
					for(; 0 != item.time[j] && item.time[j] < time; ++j);
					if(0 == item.time[j])goto BREAK;

					ratio = Ratio(item.time[j - 1], time, item.time[j]);
					if(ratio > 1.0)
					{
						dprint ("err ratio\n", ratio);
						ratio = 1.0;
					}

					item.offsets[1 + item.currentOffsetZones] = item.frame[j - 1] + int(ratio *(item.frame[j] - item.frame[j - 1]));					
					bool ok = true;
					if(item.currentOffsetZones > 0)
					{
						double buf[10 * 1024];
						char status[10 * 1024];
						unsigned countSamplesInZone = 0;

						for(unsigned sensor = 0; sensor < item.count_sensors; ++sensor)
						{
							if(!ComputeZone<T>()(buf, status, countSamplesInZone, item.currentOffsetZones, sensor, widthFiltre))
							{
								ok = false;
								break;
							}
						}
					}
					
					if(ok)++item.currentOffsetZones;
					lir += App::zone_length;
					++lirIndex;
				}
			}
BREAK:
			if(0 != stop1)
			{
				stop = stop0 + (stop1 - stop0) / 2;
				for(int i = item.currentTimeOffset - 1; i > 0; --i)
				{
					if(item.time[i] < stop)
					{
						int frameOffset = item.frame[i];
						for(int j = item.currentOffsetZones - 1; j > 0; --j)
						{
							if(frameOffset > item.offsets[j])
							{
								item.currentOffsetZones = 1 + j;

								double buf[10 * 1024];
								char status[10 * 1024];
								unsigned countSamplesInZone = 0;
								for(unsigned sensor = 0; sensor < item.count_sensors; ++sensor)
								{
									ComputeZone<T>()(
										buf
										, status
										, countSamplesInZone
										, item.currentOffsetZones
										, sensor
										, widthFiltre
										);
								}

								zprint(" @@@@ %d  %d\n", d.currentOffsetZones, item.currentOffsetZones);

								if(0 != d.currentOffsetZones)
								{
									if( d.currentOffsetZones > item.currentOffsetZones)  d.currentOffsetZones = item.currentOffsetZones;
								}
								else
								{
									d.currentOffsetZones = item.currentOffsetZones;
								}
								
								ptrProc = &Unit::Stop;
								if(++d.stopUnits == d.countUnits)ComputeDeadZones();
								return;
							}
						}
					}
				}
			}
		}
		void Stop(__data__ &)
		{
		}
		void DeathZones(__data__ &d)
		{
			DeadAreaMM0<T>::type_value deadAreaMM0 = Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM0<T>>().value;
			DeadAreaMM1<T>::type_value deadAreaMM1 = Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM1<T>>().value;

			item.currentOffsetZones = d.currentOffsetZones;

			int zones  = deadAreaMM0 / App::zone_length;
			int remain = deadAreaMM0 % App::zone_length;			
			int samplesZone = item.offsets[zones] - item.offsets[zones - 1];
			samplesZone = int(double(remain * samplesZone) / App::zone_length);
			samplesZone += item.offsets[zones];
			item.deadSamplesBegin = samplesZone;
//--------------------------------------------------------------------
			zones  = deadAreaMM1 / App::zone_length;
			remain = deadAreaMM1 % App::zone_length;
			int t = d.currentOffsetZones - zones;// + 1;
			samplesZone = item.offsets[t] - item.offsets[t - 1];
			samplesZone = samplesZone - int(double(remain * samplesZone) / App::zone_length);
			item.deadSamplesEnd =  item.offsets[zones] - samplesZone;
//--------------------------------------------------------------------
			double buf[10 * 1024];
			char status[10 * 1024];
			unsigned countSamplesInZone = 0;

			for(int zone = 1; zone < 2 + zones; ++zone)
			{
				for(unsigned sensor = 0; sensor < item.count_sensors; ++sensor)
				{
					ComputeZoneBegin<T>()(
						buf
						, status
						, countSamplesInZone
						, zone
						, sensor
						, widthFiltre
						);
				}
			}

			for(int zone = t; zone <= d.currentOffsetZones; ++zone)
			{
				for(unsigned sensor = 0; sensor < item.count_sensors; ++sensor)
				{
					ComputeZoneEnd<T>()(
						buf
						, status
						, countSamplesInZone
						, zone
						, sensor
						, widthFiltre
						);
				}
			}
		}
	};

	template<class O, class P>struct __find_zones_min_offset__
	{
		void operator()(O &o, P &p)
		{
			if(o.item.currentOffsetZones < p.currentOffsetZones)
				p.currentOffsetZones = o.item.currentOffsetZones;
			else if(0 == p.currentOffsetZones) p.currentOffsetZones = o.item.currentOffsetZones;
		}
	};

	template<class O, class P>struct __zones__
	{
		void operator()(O &o, P &p)
		{
			int status[2 + o.item.count_sensors];
			for(int i = 0; i < o.item.count_sensors; ++i)
			{
				status[i] = o.item.status[i][p.currentZones];
			}
			status[o.item.count_sensors] = p.statusRezult;
			status[o.item.count_sensors + 1] = -1;

			p.statusRezult = ResultMessageId(status);
		}
	};

	template<class P>struct __zones__<Unit<ResultData>, P>
	{
		typedef Unit<ResultData> O;
		void operator()(O &o, P &p){}		
	};

	template<>struct Unit<ResultData>
	{
		ResultData &item;
		__data__ &data;
		int statusRezult;
		int currentZones;
		int currentOffsetZones;
		void (Unit::*ptrProc)(__data__ &);
		Unit()
			: item(Singleton<ResultData>::Instance())
			, ptrProc(&Unit::Do)
			, data(Singleton<__data__>::Instance())
		{}
		void Do(__data__ &d);
		
		void Stop(__data__ &d){}
	};

	
	
	typedef TL::MkTlst<
		Unit<Cross>, Unit<Long>, 
		Unit<Thick>
		, Unit<ResultData>
	>::Result units_list;
	TL::Factory<units_list> units;


	void Unit<ResultData>::Do(__data__ &d)
	{
		int start = currentOffsetZones - 1;
		if(start < 0) start = 0;
		currentOffsetZones = 0;
		TL::foreach<TL::EraseItem<units_list, Unit<ResultData>>::Result, __find_zones_min_offset__>()(units, *this);
		//--currentOffsetZones;
		for(int i = start; i < currentOffsetZones; ++i)
		{
			currentZones = i;
			statusRezult = 	StatusId<Clr<Undefined>>();
			TL::foreach<TL::EraseItem<units_list, Unit<ResultData>>::Result, __zones__>()(units, *this);
			item.status[i] = statusRezult;			
		}
		item.currentOffsetZones = currentOffsetZones;
	}

	template<class O, class P>struct __do__;
	template<class O, class P>struct __clear__;
	template<class O, class P>struct __dead_zones__;
	void ComputeDeadZones()
	{
		__data__ &data = Singleton<__data__>::Instance();
		TL::foreach<units_list, __dead_zones__>()(units, data);
		TL::foreach<TL::MkTlst<Unit<ResultData>>::Result, __clear__>()(units);	  
		TL::foreach<TL::MkTlst<Unit<ResultData>>::Result, __do__>()(units, data);
	}

	void LongDo()
	{
		//Unit<Long> o(
		//    Singleton<Time<AutomatN::On<iLong0>>>::Instance().value
		//  , Singleton<Time<AutomatN::On<iLong1>>>::Instance().value
		//  , Singleton<Time<AutomatN::Off<iLong0>>>::Instance().value
		//  , Singleton<Time<AutomatN::Off<iLong1>>>::Instance().value
		//  );
		//(o.*o.ptrProc)();
		//(o.*o.ptrProc)();
	}

	template<class O, class P>struct __do__
	{
		void operator()(O &o, P &p)
		{
			(o.*o.ptrProc)(p);
		}
	};

	static DWORD WINAPI __Do__(LPVOID)
	{
		HANDLE h[] = {hExit, hEvent, hFrameReady};
		ThickData &thickData = Singleton<ThickData>::Instance();
		__data__ &data = Singleton<__data__>::Instance();
		while(true)
		{
			switch(WaitForMultipleObjects(dimention_of(h), h, FALSE, INFINITE))
			{
			case 0 + WAIT_OBJECT_0: return 0;
			case 1 + WAIT_OBJECT_0:
				{
					TL::foreach<units_list, __do__>()(units, data);
					RepaintWindow(Singleton<MainWindow>::Instance().hWnd);
				}
				break;
			case 2 + WAIT_OBJECT_0:
				{
					thickData.Compute();
				}
				break;
			}
		}
		return 0;
	}

	void Init()
	{
		Singleton<ThickData>::Instance().Init();
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		hExit = CreateEvent(NULL, TRUE, FALSE, NULL);
		hFrameReady = CreateEvent(NULL, FALSE, FALSE, L"LanDeviceFrameReady");
		hThread = CreateThread(NULL, 0, __Do__, NULL, 0, NULL);
	}

	void Destroy()
	{
		SetEvent(hExit);
		WaitForSingleObject(hThread, INFINITE);
	}

	void Do()
	{
		SetEvent(hEvent);
	}

	template<class O, class P>struct __clear__;

	void Clear()
	{
		__data__ &data = Singleton<__data__>::Instance();
		data.currentOffsetZones = 0;
		data.countUnits = TL::Length<TL::EraseItem<units_list,  Unit<ResultData>>::Result>::value;
		data.stopUnits = 0;
		TL::foreach<units_list, __clear__>()(units);
	}

	template<class O, class P>struct __start__
	{
		void operator()(O &o)
		{			
			o.ptrProc = &O::Start;

			o.item.currentOffset = 0;
			o.item.currentOffsetZones = 0;
		}
	};

	template<class O, class P>struct __recompute_start__
	{
		void operator()(O &o)
		{			
			o.ptrProc = &O::Start;

			//o.item.currentOffset = 0;
			o.item.currentOffsetZones = 0;
		}
	};

	template<class O, class P>struct __dead_zones__
	{
		void operator()(O &o, P &p)
		{			
			o.DeathZones(p);															
		}
	};

	template<class P>struct __dead_zones__<Unit<ResultData>, P>
	{
		typedef Unit<ResultData> O;
		void operator()(O &o, P &p)
		{			
		}
	};

	template<class O, class P>struct __clear__
	{
		void operator()(O &o)
		{
			o.start0 = 0;
			o.start1 = 0;
			o.stop0	 = 0;
			o.stop1	 = 0;
			o.Init();
			o.ptrProc = &O::Start;
			
			o.item.currentOffset = 0;
			o.item.currentOffsetZones = 0;
			
			o.timeLir.currentTimeOffset = 0;
		}
	};

	template<class P>struct __clear__<Unit<ResultData>, P>
	{
		typedef Unit<ResultData> O;
		void operator()(O &o)
		{			
			o.item.currentOffsetZones = 0;
			o.currentOffsetZones = 0;
			ZeroMemory(o.item.status, sizeof(o.item.status));
		}
	};

	template<class P>struct __start__<Unit<ResultData>, P>
	{
		typedef Unit<ResultData> O;
		void operator()(O &o){}
	};

	template<class P>struct __recompute_start__<Unit<ResultData>, P>
	{
		typedef Unit<ResultData> O;
		void operator()(O &o){}
	};


	static DWORD WINAPI RecomputeThreadProc(_In_ LPVOID lpParameter)
	{
		MainWindow &mainWindow = Singleton<MainWindow>::Instance();

		__data__ &data = Singleton<__data__>::Instance();
		data.currentOffsetZones = 0;
		data.stopUnits = 0;
		ThickData &thickData = Singleton<ThickData>::Instance();
		TL::foreach<units_list, __recompute_start__>()(units);

		thickData.startOffset = 0;
		thickData.InitParam();
		thickData.Compute();

		for(int i = 0; i < 3; ++i)
		{
			TL::foreach<units_list, __do__>()(units, data);
		}

		mainWindow.topLabelViewer.SetMessage(L"<ff00>Перерасчёт окончен");
		RepaintWindow(mainWindow.hWnd);
		dprint(" recompute stop\n");

		AnimationWindow::Destroy();
		return 0;
	}

	void Recompute()
	{
		MainWindow &mainWindow = Singleton<MainWindow>::Instance();
		mainWindow.topLabelViewer.SetMessage(L"<ff0000>Перерасчёт");
		mainWindow.ClearCharts();

		AnimationWindow::Init(mainWindow.hWnd, L"Перерасчёт");
		QueueUserWorkItem(RecomputeThreadProc, NULL, WT_EXECUTEDEFAULT);
	}
}