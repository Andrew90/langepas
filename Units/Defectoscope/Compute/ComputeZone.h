#pragma once
#include "App/App.h"
#include "DataItem\DataItem.h"
#include "App\AppBase.h"
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

		if(samplesZone > 10 * 1024)
		{
			zprint(" Allarm memory %d\n", samplesZone);
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

		int samplesZone = stopZone - startZone;
		if(samplesZone <= 0) return false;

		if(samplesZone > 10 * 1024)
		{
			zprint(" Allarm memory %d\n", samplesZone);
			return false;
		}
		int offsDead = deadZone0 - startZone;
		AnalogFiltre<T>()(startZone, stopZone);
		deadZone0 = &startZone[offsDead];

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
				filtre(*i);
			}
			statusResult = TL::IndexOf<ColorTable::items_list, Clr<DeathZone>>::value;
		}

		for(; i < stopZone; ++i)
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

		MedianFiltre filtre;
		filtre.Init(width, startZone - width);

		for(double *i = startZone; i < stopZone && i < deadZone1; ++i)
		{
			int st = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value; 

			double val = filtre(*i);

			if(borderDefect < val)st = TL::IndexOf<ColorTable::items_list, Clr<BorderDefect<T> >>::value;
			else if(borderKlass2 < *i)st = TL::IndexOf<ColorTable::items_list, Clr<BorderKlass2<T> >>::value; 

			if(val > valueResult)
			{
				valueResult = val;
				statusResult = st;
			}
		}

		if(deadZone1 < stopZone)
		{
			statusResult = TL::IndexOf<ColorTable::items_list, Clr<DeathZone>>::value;
		}
		return true;
	}
};

