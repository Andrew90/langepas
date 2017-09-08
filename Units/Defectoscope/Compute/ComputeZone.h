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

template<class List, class T>struct __adjust_list__;
template<class Head, class Tail, class T>struct __adjust_list__<Tlst<Head, Tail>, T>
{
	typedef typename __adjust_list__<Tail, T>::Result Result;
};
template<class Tail, class T, int N>struct __adjust_list__<Tlst<Adjust<T, N>, Tail>, T>
{
	typedef Tlst<Adjust<T, N>, typename __adjust_list__<Tail, T>::Result> Result;
};

template<class T>struct __adjust_list__<NullType, T>
{
	typedef NullType Result;
};

template<class T>struct __get_adjust_data__
{
	typedef typename __adjust_list__<AdjustingMultipliersTable::items_list, T>::Result List;
	int sensor;
	double &adjust;
	__get_adjust_data__(int sensor, double &adjust): sensor(sensor), adjust(adjust){}
};

template<class O, class P>struct __get_adjust__
{
	bool operator()(O &o, P &p)
	{
		if(p.sensor == TL::IndexOf<typename P::List, O>::value)
		{
			p.adjust = o.value;
			return false;
		}
		return true;
	}
};

struct ComputeZoneParamsDefault{};
template<class T>struct ComputeZoneParams;
template<>struct ComputeZoneParams<ComputeZoneParamsDefault>
{
	ThresholdsTable::TItems &thresholds;
	AnalogFilterTable::TItems &flt;
	MedianFiltreTable::TItems &filtreParam;
	AdjustingMultipliersTable::TItems &adjust;
	DeadAreaTable::TItems &dead;
	ComputeZoneParams()
		: thresholds(Singleton<ThresholdsTable>::Instance().items)
		, flt(Singleton<AnalogFilterTable>::Instance().items)
		, filtreParam(Singleton<MedianFiltreTable>::Instance().items)
		, adjust(Singleton<AdjustingMultipliersTable>::Instance().items)
		, dead(Singleton<DeadAreaTable>::Instance().items)
	{}
};

template<class T, class Data = ComputeZoneParamsDefault>struct ComputeZone
{
	ComputeZoneParams<Data> params;
	bool Zone(unsigned zone, unsigned sensor, char *status, double *buffer);
	bool Begin(unsigned sensor, char *status, double *buffer);
	bool End(unsigned zone, unsigned sensor, char *status, double *buffer);
};

template<class T, class Data>bool ComputeZone<T, Data>::Zone(unsigned zone, unsigned sensor, char *status, double *buffer)
{
	ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
	double borderKlass2 = params.thresholds.get<BorderKlass2<T>>().value;
	double borderDefect = params.thresholds.get<BorderDefect<T>>().value;

	double *startZone = &item.ascan[sensor][item.offsets[zone - 1]];
	double *stopZone = &item.ascan[sensor][item.offsets[zone]];

	//if(0 == (sensor & 1))
	//{
	//	double offs = 45.0 * (stopZone - startZone) / 200;
	//	startZone -= int(offs);
	//	stopZone -= int(offs);
	//}

	int samplesZone = stopZone - startZone;
	if(samplesZone <= 0) return false;

	if(samplesZone > AnalogFiltre_bufSize)
	{
		return false;
	}

	AnalogFiltre()(startZone, stopZone, params.flt.get<CutoffFrequencyOn<T>>().value, params.flt.get<CutoffFrequency<T>>().value);

	char &statusResult =  status[zone - 1];
	double &valueResult = buffer[zone - 1];

	statusResult =  STATUS_ID(Nominal);
	valueResult = 0;

	MedianFiltre filtre;
	double adjust = 1;
	typedef __get_adjust_data__<T> Sub;
	TL::find<Sub::List, __get_adjust__>()(
		params.adjust
		, Sub(sensor, adjust)
		);

	bool filtreOn = params.filtreParam.get<MedianFiltreOn<T>>().value;
	if(filtreOn)
	{
		int width = params.filtreParam.get<MedianFiltreWidth<T>>().value;
		filtre.AbsInit(width, startZone - width);
		for(int i = 0; i < dimention_of(filtre.buf); ++i)
		{
			filtre.buf[i] *= adjust;
		}
	}

	for(double *i = startZone; i < stopZone; ++i)
	{
		int st = STATUS_ID(Nominal);

		double val = filtreOn ? filtre(*i) : *i;
		val *= adjust;
		if(val < 0) val = -val;

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


//template<class T>struct __stop__
//{
//	void operator()(){}
//};
//
//template<>struct __stop__ <Cross>
//{
//	void operator()()
//	{
//		dprint("__stop__\n");
//	}
//};

template<class T, class Data>bool ComputeZone<T, Data>::Begin(unsigned sensor, char *status, double *buffer)
{
	ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
	double borderKlass2 = params.thresholds.get<BorderKlass2<T>>().value;
	double borderDefect = params.thresholds.get<BorderDefect<T>>().value;

	//__stop__<T>()();

	int dead = params.dead.get<DeadAreaMM0<T>>().value;

	int full = dead / App::zone_length;
	int remains = dead % App::zone_length;

	int offs = full;
	if(remains > 0) --offs;

	double d = double(remains) / App::zone_length;

	double *startZone = &item.ascan[sensor][item.offsets[full]] + int(d * ( &item.ascan[sensor][item.offsets[1 + full]] -  &item.ascan[sensor][item.offsets[full]]));
	double *stopZone = &item.ascan[sensor][item.offsets[1 + full]];

//	item.beginZoneOffs[sensor] = startZone;

	AnalogFiltre()(startZone, stopZone, params.flt.get<CutoffFrequencyOn<T>>().value, params.flt.get<CutoffFrequency<T>>().value);

	MedianFiltre filtre;

	double adjust = 1;
	typedef __get_adjust_data__<T> Sub;
	TL::find<Sub::List, __get_adjust__>()(
		params.adjust
		, Sub(sensor, adjust)
		);

	bool filtreOn = params.filtreParam.get<MedianFiltreOn<T>>().value;
	if(filtreOn)
	{
		int width = params.filtreParam.get<MedianFiltreWidth<T>>().value;
		filtre.AbsInit(width, startZone - width);
		for(int i = 0; i < dimention_of(filtre.buf); ++i)
		{
			filtre.buf[i] *= adjust;
		}
	}

	char &statusResult =  status[full];
	double &valueResult = buffer[full];

	statusResult = STATUS_ID(Nominal);
	valueResult = 0;

	for(double *i = startZone; i < stopZone; ++i)
	{
		int st = STATUS_ID(Nominal); 

		double val = filtreOn ? filtre(*i) : *i;
		val *= adjust;

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

template<class T, class Data>bool ComputeZone<T, Data>::End(unsigned zone, unsigned sensor, char *status, double *buffer)
{
	--zone;
	ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
	double borderKlass2 =  params.thresholds.get<BorderKlass2<T>>().value;
	double borderDefect =  params.thresholds.get<BorderDefect<T>>().value;

	int dead = params.dead.get<DeadAreaMM1<T>>().value;

	int full = dead / App::zone_length;
	int remains = dead % App::zone_length;

	int offs = zone - full;
	if(remains > 0)
	{
		--offs;

		double d = 1.0 - double(remains) / App::zone_length;

		double *startZone = &item.ascan[sensor][item.offsets[offs]];
		double *stopZone = startZone + int(d *(&item.ascan[sensor][item.offsets[offs + 1]] - &item.ascan[sensor][item.offsets[offs]]));

	//	item.endZoneOffs[sensor] = stopZone;

		AnalogFiltre()(startZone, stopZone, params.flt.get<CutoffFrequencyOn<T>>().value, params.flt.get<CutoffFrequency<T>>().value);

		char &statusResult =  status[offs];
		double &valueResult = buffer[offs];

		statusResult = STATUS_ID(Nominal);
		valueResult = 0;

		MedianFiltre filtre;
		bool filtreOn = params.filtreParam.get<MedianFiltreOn<T>>().value;

		double adjust = 1;
		typedef __get_adjust_data__<T> Sub;
		TL::find<Sub::List, __get_adjust__>()(
			params.adjust
			, Sub(sensor, adjust)
			);

		if(filtreOn)
		{
			int width = params.filtreParam.get<MedianFiltreWidth<T>>().value;
			filtre.Init(width, startZone - width);
			for(int i = 0; i < dimention_of(filtre.buf); ++i)
			{
				filtre.buf[i] *= adjust;
			}
		}

		for(double *i = startZone; i < stopZone; ++i)
		{
			int st = STATUS_ID(Nominal); 

			double val = filtreOn ? filtre(*i): *i;
			val *= adjust;

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



