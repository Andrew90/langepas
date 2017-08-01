#pragma once
#include "App/App.h"
#include "DataItem\DataItem.h"
#include "App\AppBase.h"
#include "Filtre\MedianFiltre.h"
#include "DspFilters\ChebyshevFiltre.h"
#include "MessageText\StatusMessages.h"
#include "MessageText\ListMess.hpp"

static const int AnalogFiltre_bufSize = 10 * 1024;

struct AnalogFiltre
{
	double buf[AnalogFiltre_bufSize];
	void operator()(double *&start, double *&stop, bool CutoffFrequencyOn, int CutoffFrequency)
	{
		if(CutoffFrequencyOn)
		{
			ChebyshevFiltre dsp;
			dsp.Setup(
				Singleton<L502ParametersTable>::Instance().items.get<ChannelSamplingRate>().value
				, 3
				, CutoffFrequency
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
	bool operator()(unsigned zone, unsigned sensor)
	{
		ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
		double borderKlass2 = Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T>>().value;
		double borderDefect = Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T>>().value;

		double *startZone = &item.ascan[sensor][item.offsets[zone - 1]];
		double *stopZone = &item.ascan[sensor][item.offsets[zone]];

		int samplesZone = stopZone - startZone;
		if(samplesZone <= 0) return false;

		if(samplesZone > AnalogFiltre_bufSize)
		{
			zprint(" Allarm memory %d\n", samplesZone);
			return false;
		}

		AnalogFilterTable::TItems &flt = Singleton<AnalogFilterTable>::Instance().items;
		AnalogFiltre()(startZone, stopZone, flt.get<CutoffFrequencyOn<T>>().value, flt.get<CutoffFrequency<T>>().value);

		char &statusResult =  item.status[sensor][zone - 1];
		double &valueResult = item.buffer[sensor][zone - 1];

		statusResult =  STATUS_ID(Nominal);
		valueResult = 0;

		MedianFiltre filtre;
		MedianFiltreTable::TItems &filtreParam = Singleton<MedianFiltreTable>::Instance().items;

		bool filtreOn = filtreParam.get<MedianFiltreOn<T>>().value;
		if(filtreOn)
		{
			int width = filtreParam.get<MedianFiltreWidth<T>>().value;
			filtre.Init(width, startZone - width);
		}

		for(double *i = startZone; i < stopZone; ++i)
		{
			int st = TL::IndexOf<status_list, Nominal>::value; 

			double val = filtreOn ? filtre(*i) : *i;

			if(borderDefect < val)st =  STATUS_ID(BorderDefect<T>);
			else if(borderKlass2 < val)st = STATUS_ID(BorderKlass2<T>); 

			if(val > valueResult)
			{
				valueResult = val;
				statusResult = st;
			}
		}
		return true;
	}
};

template<class T>struct __stop__
{
	void operator()(){}
};

template<>struct __stop__ <Cross>
{
	void operator()()
	{
		dprint("__stop__\n");
	}
};

template<class T>struct ComputeZoneBegin
{
	bool operator()(unsigned sensor)
	{
		ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
		double borderKlass2 = Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T>>().value;
		double borderDefect = Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T>>().value;

		__stop__<T>()();

		int dead = Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM0<T>>().value;

		int full = dead / App::zone_length;
		int remains = dead % App::zone_length;

		int offs = full;
		if(remains > 0) --offs;

		double d = double(remains) / App::zone_length;

		double *startZone = &item.ascan[sensor][item.offsets[full]] + int(d * ( &item.ascan[sensor][item.offsets[1 + full]] -  &item.ascan[sensor][item.offsets[full]]));
		double *stopZone = &item.ascan[sensor][item.offsets[1 + full]];


		AnalogFilterTable::TItems &flt = Singleton<AnalogFilterTable>::Instance().items;
		AnalogFiltre()(startZone, stopZone, flt.get<CutoffFrequencyOn<T>>().value, flt.get<CutoffFrequency<T>>().value);

		MedianFiltre filtre;
		MedianFiltreTable::TItems &filtreParam = Singleton<MedianFiltreTable>::Instance().items;
		bool filtreOn = filtreParam.get<MedianFiltreOn<T>>().value;
		if(filtreOn)
		{
			int width = filtreParam.get<MedianFiltreWidth<T>>().value;
			filtre.Init(width, startZone - width);
		}

		if(filtreOn)
		{
			for(double *i = startZone; i < stopZone; ++i)
			{
				filtre(*i);
			}
		}

		char &statusResult =  item.status[sensor][full];
		double &valueResult = item.buffer[sensor][full];	

		statusResult = STATUS_ID(Nominal);
		valueResult = 0;

		for(double *i = startZone; i < stopZone; ++i)
		{
			int st = STATUS_ID(Nominal); 

			double val = filtreOn ? filtre(*i) : *i;

			if(borderDefect < val)st = STATUS_ID(BorderDefect<T>);
			else if(borderKlass2 < val)st = STATUS_ID(BorderKlass2<T>);

			if(val > valueResult)
			{
				valueResult = val;
				statusResult = st;
			}
		}

		for(int i = 0; i < full; ++i)
		{
			item.status[sensor][i] = STATUS_ID(DeathZone);
		}

		return true;
	}
};

template<class T>struct ComputeZoneEnd
{
	bool operator()(unsigned zone, unsigned sensor)
	{
		--zone;
		ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
		double borderKlass2 = Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T>>().value;
		double borderDefect = Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T>>().value;

		int dead = Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM1<T>>().value;

		int full = dead / App::zone_length;
		int remains = dead % App::zone_length;

		int offs = zone - full;
		if(remains > 0)
		{
			--offs;

			double d = 1.0 - double(remains) / App::zone_length;

			double *startZone = &item.ascan[sensor][item.offsets[offs]];
			double *stopZone = startZone + int(d *(&item.ascan[sensor][item.offsets[offs + 1]] - &item.ascan[sensor][item.offsets[offs]]));

			AnalogFilterTable::TItems &flt = Singleton<AnalogFilterTable>::Instance().items;
			AnalogFiltre()(startZone, stopZone, flt.get<CutoffFrequencyOn<T>>().value, flt.get<CutoffFrequency<T>>().value);

			char &statusResult =  item.status[sensor][offs];
			double &valueResult = item.buffer[sensor][offs];

			statusResult = STATUS_ID(Nominal);
			valueResult = 0;

			MedianFiltre filtre;
			MedianFiltreTable::TItems &filtreParam = Singleton<MedianFiltreTable>::Instance().items;
			bool filtreOn = filtreParam.get<MedianFiltreOn<T>>().value;
			if(filtreOn)
			{
				int width = filtreParam.get<MedianFiltreWidth<T>>().value;
				filtre.Init(width, startZone - width);
			}

			for(double *i = startZone; i < stopZone; ++i)
			{
				int st = STATUS_ID(Nominal); 

				double val = filtreOn ? filtre(*i): *i;

				if(borderDefect < val)st = STATUS_ID(BorderDefect<T>);
				else if(borderKlass2 < *i)st = STATUS_ID(BorderKlass2<T>);

				if(val > valueResult)
				{
					valueResult = val;
					statusResult = st;
				}
			}
			++offs;
		}

		for(int i = offs; i < (int)zone; ++i)
		{
			item.status[sensor][i] = STATUS_ID(DeathZone);
		}
		return true;
	}
};



