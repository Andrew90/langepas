#pragma once
#include "App/App.h"
#include "DataItem\DataItem.h"
#include "App\AppBase.h"
#include "Filtre\MedianFiltre.h"
#include "DspFilters\ChebyshevFiltre.h"

static const int AnalogFiltre_bufSize = 10 * 1024;

//template<class T>
struct AnalogFiltre
{
	double buf[AnalogFiltre_bufSize];
	void operator()(double *&start, double *&stop, bool CutoffFrequencyOn, int CutoffFrequency)
	{
		//if(Singleton<AnalogFilterTable>::Instance().items.get<CutoffFrequencyOn<T>>().value)
		if(CutoffFrequencyOn)
		{
			ChebyshevFiltre dsp;
			dsp.Setup(
				Singleton<L502ParametersTable>::Instance().items.get<ChannelSamplingRate>().value
				, 3
				//, Singleton<AnalogFilterTable>::Instance().items.get<CutoffFrequency<T>>().value
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
	bool operator()(//double(&bufx)[AnalogFiltre_bufSize], char(&statusx)[AnalogFiltre_bufSize]
	 //unsigned &countx, 
	 unsigned zone, unsigned sensor)//, int width)
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

		statusResult = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value;
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
			int st = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value; 

			double val = filtreOn ? filtre(*i) : *i;

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
	//bool operator()(double(&xbuf)[AnalogFiltre_bufSize], char(&xstatus)[AnalogFiltre_bufSize]
	//, unsigned &xcount, unsigned zone, unsigned sensor, int width)
	bool operator()(unsigned sensor)
	{
		ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
		double borderKlass2 = Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T>>().value;
		double borderDefect = Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T>>().value;

		//double *startZone = &item.ascan[sensor][item.offsets[zone - 1]];
		//double *stopZone = &item.ascan[sensor][item.offsets[zone]];
		//double *deadZone0 = &item.ascan[sensor][item.deadSamplesBegin];
		double *startZone = &item.ascan[sensor][item.offsets[0]];
		double *stopZone = &item.ascan[sensor][item.offsets[1]];
		double *deadZone0 = &item.ascan[sensor][item.deadSamplesBegin];

		int samplesZone = stopZone - startZone;
		if(samplesZone <= 0) return false;

		if(samplesZone > AnalogFiltre_bufSize)
		{
			zprint(" Allarm memory %d\n", samplesZone);
			return false;
		}

		int offsDead = deadZone0 - startZone;
		AnalogFilterTable::TItems &flt = Singleton<AnalogFilterTable>::Instance().items;
		AnalogFiltre()(startZone, stopZone, flt.get<CutoffFrequencyOn<T>>().value, flt.get<CutoffFrequency<T>>().value);

		deadZone0 = &startZone[offsDead];

		//char &statusResult =  item.status[sensor][zone - 1];
		//double &valueResult = item.buffer[sensor][zone - 1];
		char &statusResult =  item.status[sensor][0];
		double &valueResult = item.buffer[sensor][0];

		statusResult = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value;
		valueResult = 0;

		double *i = startZone;

		MedianFiltre filtre;
		//filtre.Init(width, startZone - width);
		MedianFiltreTable::TItems &filtreParam = Singleton<MedianFiltreTable>::Instance().items;
		bool filtreOn = filtreParam.get<MedianFiltreOn<T>>().value;
		if(filtreOn)
		{
			int width = filtreParam.get<MedianFiltreWidth<T>>().value;
			filtre.Init(width, startZone - width);
		}

		if(startZone < deadZone0)
		{
			double *stop = stopZone < deadZone0 ? stopZone: deadZone0;
			if(filtreOn)
			{
				for(; i < stop; ++i)
				{
					filtre(*i);
				}
			}
			statusResult = TL::IndexOf<ColorTable::items_list, Clr<DeathZone>>::value;
		}

		for(; i < stopZone; ++i)
		{
			int st = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value; 

			double val = filtreOn ? filtre(*i) : *i;

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
	bool operator()(//double(&xbuf)[AnalogFiltre_bufSize], char(&xstatus)[AnalogFiltre_bufSize]
//	, unsigned &xcount, 
		unsigned zone, unsigned sensor)//, int width)
	{
		ItemData<T>	&item = Singleton<ItemData<T>>::Instance();
		double borderKlass2 = Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T>>().value;
		double borderDefect = Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T>>().value;

		double *startZone = &item.ascan[sensor][item.offsets[zone - 1]];
		double *stopZone = &item.ascan[sensor][item.offsets[zone]];
		double *deadZone1 = &item.ascan[sensor][item.deadSamplesEnd];

		int samplesZone = stopZone - startZone;
		if(samplesZone <= 0) return false;

		if(samplesZone > AnalogFiltre_bufSize)
		{
			zprint(" Allarm memory %d\n", samplesZone);
			return false;
		}

		int offsZone = deadZone1 - startZone;
		//AnalogFiltre<T>()(startZone, stopZone);
		AnalogFilterTable::TItems &flt = Singleton<AnalogFilterTable>::Instance().items;
		AnalogFiltre()(startZone, stopZone, flt.get<CutoffFrequencyOn<T>>().value, flt.get<CutoffFrequency<T>>().value);
		deadZone1 = &startZone[offsZone];

		char &statusResult =  item.status[sensor][zone - 1];
		double &valueResult = item.buffer[sensor][zone - 1];

		statusResult = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value;
		valueResult = 0;

		MedianFiltre filtre;
		//filtre.Init(width, startZone - width);
		MedianFiltreTable::TItems &filtreParam = Singleton<MedianFiltreTable>::Instance().items;
		bool filtreOn = filtreParam.get<MedianFiltreOn<T>>().value;
		if(filtreOn)
		{
			int width = filtreParam.get<MedianFiltreWidth<T>>().value;
			filtre.Init(width, startZone - width);
		}

		for(double *i = startZone; i < stopZone && i < deadZone1; ++i)
		{
			int st = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value; 

			double val = filtreOn ? filtre(*i): *i;

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



