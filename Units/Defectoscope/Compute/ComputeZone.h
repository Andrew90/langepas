#pragma once
#include "App/App.h"
#include "DataItem\DataItem.h"
#include "App\AppBase.h"
#include "MessageText\SelectMessage.h"
//#include "DataItem\ThicknessData.h"
#include "Filtre\MedianFiltre.h"
#include "DspFilters\ChebyshevFiltre.h"

template<class T>struct AnalogFiltre
{
	static const int bufSize = 1024 * 10;
	double buf[bufSize];
	void operator()(double *&start, double *&stop)
	{
		if(Singleton<AnalogFilterTable>::Instance().items.get<CutoffFrequencyOn<T>>().value)
		{
			ChebyshevFiltre dsp;
			dsp.Setup(
				Singleton<L502ParametersTable>::Instance().items.get<ChannelSamplingRate>().value
				, 3
				, Singleton<AnalogFilterTable>::Instance().items.get<CutoffFrequency<T>>().value
				, 40
				);

			double *st = start - App::zone_filter_offset;
			int k = 0;
			for(; st < stop; ++st, ++k)
			{
				buf[k] = dsp.OneSample(*st);
			}
			start = &buf[App::zone_filter_offset];
			stop = &buf[k];
		}
	}
};

template<class T>struct ComputeZone
{
	bool operator()(double(&bufx)[10 * 1024], char(&statusx)[10 * 1024]
	, unsigned &countx, unsigned zone, unsigned sensor, int width)
	{
		ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
		double borderKlass2 = Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T>>().value;
		double borderDefect = Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T>>().value;

		double *startZone = &item.ascan[sensor][item.offsets[zone - 1]];
		double *stopZone = &item.ascan[sensor][item.offsets[zone]];

		int samplesZone = stopZone - startZone;
		if(samplesZone <= 0) return false;

		if(samplesZone > 10 * 1024)//dimention_of(buf))
		{
			zprint(" Allarm memory %d\n", samplesZone);
			//			stopZone =  startZone + dimention_of(buf);	
			return false;
		}
		//#pragma message("обработка зоны (фильтрация, ...) дописать")

		AnalogFiltre<T>()(startZone, stopZone);

		char &statusResult =  item.status[sensor][zone - 1];
		double &valueResult = item.buffer[sensor][zone - 1];

		statusResult = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value;
		valueResult = 0;

		MedianFiltre filtre;
		filtre.Init(width, startZone - width);
		//filtre.InitData(startZone - filtre.width - 1);

		for(double *i = startZone; i < stopZone; ++i)
		{
			int st = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value; 

			double val = filtre(*i);

			if(borderDefect < val)st = TL::IndexOf<ColorTable::items_list, Clr<BorderDefect<T> >>::value;
			else if(borderKlass2 < val)st = TL::IndexOf<ColorTable::items_list, Clr<BorderKlass2<T> >>::value; 

			if(val > valueResult)
			{
				valueResult = val;
				statusResult = st;
			}
		}

		return true;
	}
};

template<class T>struct ComputeZoneBegin
{
	bool operator()(double(&xbuf)[10 * 1024], char(&xstatus)[10 * 1024]
	, unsigned &xcount, unsigned zone, unsigned sensor, int width)
	{
		ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
		double borderKlass2 = Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T>>().value;
		double borderDefect = Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T>>().value;

		double *startZone = &item.ascan[sensor][item.offsets[zone - 1]];
		double *stopZone = &item.ascan[sensor][item.offsets[zone]];
		double *deadZone0 = &item.ascan[sensor][item.deadSamplesBegin];
		//double *deadZone1 = &item.ascan[sensor][item.deadSamplesEnd];

		int samplesZone = stopZone - startZone;
		if(samplesZone <= 0) return false;

		if(samplesZone > 10 * 1024)//dimention_of(buf))
		{
			zprint(" Allarm memory %d\n", samplesZone);
			//stopZone =  startZone + dimention_of(buf);	
			return false;
		}
		int offsDead = deadZone0 - startZone;
		AnalogFiltre<T>()(startZone, stopZone);
		deadZone0 = &startZone[offsDead];
		//unsigned k = 0;
		//memmove(buf, startZone, samplesZone * sizeof(double));

		char &statusResult =  item.status[sensor][zone - 1];
		double &valueResult = item.buffer[sensor][zone - 1];

		statusResult = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value;
		valueResult = 0;

		double *i = startZone;

		MedianFiltre filtre;
		filtre.Init(width, startZone - width);

		if(startZone < deadZone0)
		{
			double *stop = stopZone < deadZone0 ? stopZone: deadZone0;
			for(; i < stop; ++i)
			{
				//status[k] = TL::IndexOf<ColorTable::items_list, Clr<DeathZone>>::value; 
				//buf[k] = 
				//statusResult = TL::IndexOf<ColorTable::items_list, Clr<DeathZone>>::value; 
				filtre(*i);
				//	++k;
			}
			statusResult = TL::IndexOf<ColorTable::items_list, Clr<DeathZone>>::value;
		}

		for(; i < stopZone; ++i)
		{
			int st = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value; 

			double val = filtre(*i);
			//buf[k] = val;

			if(borderDefect < val)st = TL::IndexOf<ColorTable::items_list, Clr<BorderDefect<T> >>::value;
			else if(borderKlass2 < val)st = TL::IndexOf<ColorTable::items_list, Clr<BorderKlass2<T> >>::value; 

			if(val > valueResult)
			{
				valueResult = val;
				statusResult = st;
			}
			//++k;
		}

		//	count = k;

		return true;//k > 0;
	}
};

template<class T>struct ComputeZoneEnd
{
	bool operator()(double(&xbuf)[10 * 1024], char(&xstatus)[10 * 1024]
	, unsigned &xcount, unsigned zone, unsigned sensor, int width)
	{
		ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
		double borderKlass2 = Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T>>().value;
		double borderDefect = Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T>>().value;

		double *startZone = &item.ascan[sensor][item.offsets[zone - 1]];
		double *stopZone = &item.ascan[sensor][item.offsets[zone]];
		double *deadZone1 = &item.ascan[sensor][item.deadSamplesEnd];

		int samplesZone = stopZone - startZone;
		if(samplesZone <= 0) return false;

		if(samplesZone > 10 * 1024)
		{
			zprint(" Allarm memory %d\n", samplesZone);
			return false;
		}

		int offsZone = deadZone1 - startZone;
		AnalogFiltre<T>()(startZone, stopZone);
		deadZone1 = &startZone[offsZone];

		char &statusResult =  item.status[sensor][zone - 1];
		double &valueResult = item.buffer[sensor][zone - 1];

		statusResult = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value;
		valueResult = 0;

		//filtre.InitData(startZone - filtre.width - 1);
		MedianFiltre filtre;
		filtre.Init(width, startZone - width);

		for(double *i = startZone; i < stopZone && i < deadZone1; ++i)
		{
			int st = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value; 

			double val = filtre(*i);
			//buf[k] = val;

			if(borderDefect < val)st = TL::IndexOf<ColorTable::items_list, Clr<BorderDefect<T> >>::value;
			else if(borderKlass2 < *i)st = TL::IndexOf<ColorTable::items_list, Clr<BorderKlass2<T> >>::value; 

			if(val > valueResult)
			{
				valueResult = val;
				statusResult = st;//status[k];
			}
			//	++k;
		}

		if(deadZone1 < stopZone)
		{
			//bool b = k > 0;
			//if(!b)
			statusResult = TL::IndexOf<ColorTable::items_list, Clr<DeathZone>>::value;
		}
		return true;
	}
};

template<>struct ComputeZone<Thick>
{
	bool operator()(double(&xbuf)[10 * 1024], char(&xstatus)[10 * 1024]
	, unsigned &xcount, unsigned zone, unsigned sensor, int width)//MedianFiltre &filtre)
	{
#if 0
		typedef Thick T;
		ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
		double nominal = Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<T>>().value;
		double borderDefect = nominal
			- Singleton<ThresholdsTable>::Instance().items.get<BorderLower<T>>().value;
		double borderAbove = nominal
			+ Singleton<ThresholdsTable>::Instance().items.get<BorderAbove<T>>().value;

		double *startZone  = &item.ascan[sensor][item.offsets[zone - 1]];
		double *stopZone   = &item.ascan[sensor][item.offsets[zone]];
		char   *scanStatus = &item.ascanStatus[sensor][item.offsets[zone - 1]];

		int samplesZone = stopZone - startZone;

		if(samplesZone > 1024 * 10)//dimention_of(buf))
		{
			zprint(" Allarm memory %d\n", samplesZone);
			//stopZone =  startZone + dimention_of(buf);	
			return false;
		}
#pragma message("обработка зоны (фильтрация, ...) дописать")		

		//memmove(buf, startZone, samplesZone * sizeof(double));

		char &statusResult = item.status   [sensor][zone - 1];
		double &valueMin   = item.bufferMin[sensor][zone - 1];
		double &valueMax   = item.bufferMax[sensor][zone - 1];

		statusResult = StatusId<Clr<Undefined>>(); 
		valueMin = 99999;
		valueMax = 0;

		MedianFiltre filtre;

		filtre.Init(width, startZone - width);
		for(int i = 0; i < width; ++i)
		{
			filtre.status[i] = scanStatus[-width + i];
		}
		int k = 0;

		for(double *i = startZone; i < stopZone; ++i, ++scanStatus, ++k)
		{
			if(StatusId<Clr<Nominal>>() == *scanStatus)
			{
				//	status[k] = StatusId<Clr<Nominal>>();

				//	double val = filtre(*i);
				int offs = filtre.Add(*i, *scanStatus);
				double val = filtre.buf[offs];
				//status[k] = *(char *)filtre.data[offs];
				if(filtre.status[offs] == StatusId<Clr<Nominal>>())				
				{
					//if(status[k] == StatusId<Clr<Nominal>>())
					//{
					int st = StatusId<Clr<Nominal>>();
					if(val > valueMax && val != nominal)
					{
						valueMax = val;
						if(borderDefect > val)st = StatusId<Clr<BorderLower<T>>>();
						else if(borderAbove < val)st = StatusId<Clr<BorderAbove<T>>>(); 
					}
					else if(val < valueMin)
					{
						valueMin = val;
						if(borderDefect > val)st = StatusId<Clr<BorderLower<T>>>();
						else if(borderAbove < val)st = StatusId<Clr<BorderAbove<T>>>();
					}
					//else
					//{
					//	status[k] = StatusId<Clr<Nominal>>();
					//}
					//}
					//else
					//{
					//	status[k] =  StatusId<Clr<Undefined>>();
					//}

					int x[] = {
						statusResult
						, st
						, -1
					};
					statusResult = ResultMessageId(x);
				}
				//else
				//{
				//	status[k] = StatusId<Clr<Undefined>>();
				//}
			}
			//else
			//{
			//	status[k] = StatusId<Clr<Undefined>>();
			//}
		}
		//count = k;
		if(valueMin == 99999) valueMin = nominal;
		if(valueMax == 0) valueMax = nominal;
#endif
		return true;//k > 0;
	}
};

template<>struct ComputeZoneBegin<Thick>
{
	bool operator()(double(&xbuf)[10 * 1024], char(&xstatus)[10 * 1024]
	, unsigned &xcount, unsigned zone, unsigned sensor, int width)//MedianFiltre &filtre)
	{
#if 0
		typedef Thick T;
		ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
		double nominal = Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<T>>().value;
		double borderDefect = nominal
			- Singleton<ThresholdsTable>::Instance().items.get<BorderLower<T>>().value;
		double borderAbove = nominal
			+ Singleton<ThresholdsTable>::Instance().items.get<BorderAbove<T>>().value;

		double *startZone  = &item.ascan[sensor][item.offsets[zone - 1]];
		double *stopZone   = &item.ascan[sensor][item.offsets[zone]];
		char   *scanStatus = &item.ascanStatus[sensor][item.offsets[zone - 1]];

		int samplesZone = stopZone - startZone;

		if(samplesZone > 1024 * 10)//dimention_of(buf))
		{
			zprint(" Allarm memory %d\n", samplesZone);
			//stopZone =  startZone + dimention_of(buf);	
			return false;
		}
#pragma message("обработка зоны (фильтрация, ...) дописать")
		//unsigned k = 0;
		//memmove(buf, startZone, samplesZone * sizeof(double));

		char &statusResult = item.status   [sensor][zone - 1];
		double &valueMin   = item.bufferMin[sensor][zone - 1];
		double &valueMax   = item.bufferMax[sensor][zone - 1];

		double *deadZone0 = &item.ascan[sensor][item.deadSamplesBegin];

		statusResult = TL::IndexOf<ColorTable::items_list, Clr<Undefined>>::value; 
		valueMin = 99999;
		valueMax = 0;

		MedianFiltre filtre;
		filtre.Init(width, startZone - width);

		double *i = startZone;

		if(startZone < deadZone0)
		{
			double *stop = stopZone < deadZone0 ? stopZone: deadZone0;
			for(; i < stop; ++i)
			{
				//status[k] = TL::IndexOf<ColorTable::items_list, Clr<DeathZone>>::value; 
				//buf[k] = 
				filtre(*i);
				//++k;
			}
			statusResult = TL::IndexOf<ColorTable::items_list, Clr<DeathZone>>::value;
		}

		for(; i < stopZone; ++i, ++scanStatus)
		{
			if(StatusId<Clr<Nominal>>() == *scanStatus)
			{
				int st = StatusId<Clr<Nominal>>();

				double val = filtre(*i);
				//buf[k] = val;

				if(val > valueMax)
				{
					valueMax = val;
					if(borderDefect > val)st = StatusId<Clr<BorderLower<T>>>();
					else if(borderAbove < val)st = StatusId<Clr<BorderAbove<T>>>(); 
				}
				else if(val < valueMin)
				{
					valueMin = val;
					if(borderDefect > val)st = StatusId<Clr<BorderLower<T>>>();
					else if(borderAbove < val)st = StatusId<Clr<BorderAbove<T>>>();
				}

				int x[] = {
					statusResult
					, st
					, -1
				};
				statusResult = ResultMessageId(x);
			}
			//else
			//{
			//	status[k] = *scanStatus;
			//}
		}
		//count = k;
		if(valueMin == 99999) valueMin = nominal;
		if(valueMax == 0) valueMax = nominal;
#endif
		return true;
	}
};

template<>struct ComputeZoneEnd<Thick>
{
	bool operator()(double(&xbuf)[10 * 1024], char(&xstatus)[10 * 1024]
	, unsigned &xcount, unsigned zone, unsigned sensor, int width)//MedianFiltre &filtre)
	{
#if 0
		typedef Thick T;
		ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
		double nominal = Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<T>>().value;
		double borderDefect = nominal
			- Singleton<ThresholdsTable>::Instance().items.get<BorderLower<T>>().value;
		double borderAbove = nominal
			+ Singleton<ThresholdsTable>::Instance().items.get<BorderAbove<T>>().value;

		double *startZone  = &item.ascan[sensor][item.offsets[zone - 1]];
		double *stopZone   = &item.ascan[sensor][item.offsets[zone]];
		char   *scanStatus = &item.ascanStatus[sensor][item.offsets[zone - 1]];

		int samplesZone = stopZone - startZone;

		if(samplesZone > 1024 * 100)//dimention_of(buf))
		{
			zprint(" Allarm memory %d\n", samplesZone);
			//stopZone =  startZone + dimention_of(buf);	
			return false;
		}
#pragma message("обработка зоны (фильтрация, ...) дописать")
		unsigned k = 0;
		//memmove(buf, startZone, samplesZone * sizeof(double));

		char &statusResult = item.status   [sensor][zone - 1];
		double &valueMin   = item.bufferMin[sensor][zone - 1];
		double &valueMax   = item.bufferMax[sensor][zone - 1];

		double *deadZone1 = &item.ascan[sensor][item.deadSamplesEnd];

		statusResult = TL::IndexOf<ColorTable::items_list, Clr<Undefined>>::value; 
		valueMin = 99999;
		valueMax = 0;

		MedianFiltre filtre;
		filtre.Init(width, startZone - width);

		for(double *i = startZone; i < stopZone && i < deadZone1; ++i, ++scanStatus, ++k)
		{
			if(StatusId<Clr<Nominal>>() == *scanStatus)
			{
				int st = StatusId<Clr<Nominal>>();

				double val = filtre(*i);
				//buf[k] = val;

				if(val > valueMax)
				{
					valueMax = val;
					if(borderDefect > val)st = StatusId<Clr<BorderLower<T>>>();
					else if(borderAbove < val)st = StatusId<Clr<BorderAbove<T>>>(); 
				}
				else if(val < valueMin)
				{
					valueMin = val;
					if(borderDefect > val)st = StatusId<Clr<BorderLower<T>>>();
					else if(borderAbove < val)st = StatusId<Clr<BorderAbove<T>>>();
				}

				int x[] = {
					statusResult
					, st
					, -1
				};
				statusResult = ResultMessageId(x);
			}
			//else
			//{
			//	status[k] = *scanStatus;
			//}
		}

		if(deadZone1 < stopZone)
		{
			//double *i = startZone < deadZone1 ? deadZone1: startZone;
			//for(; i < stopZone; ++i)
			//{
			//	status[k] = TL::IndexOf<ColorTable::items_list, Clr<DeathZone>>::value; 
			//	buf[k] = filtre(*i);
			//	++k;
			//}
			statusResult = TL::IndexOf<ColorTable::items_list, Clr<DeathZone>>::value;
		}

		//count = k;
		if(valueMin == 99999) valueMin = nominal;
		if(valueMax == 0) valueMax = nominal;
#endif
		return true;//k > 0;
	}
};
