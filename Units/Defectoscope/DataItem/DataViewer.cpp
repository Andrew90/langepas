#include "stdafx.h"
#include "DataItem\DataViewer.h"
#include "templates/typelist.hpp"
#include "Filtre\MedianFiltre.h"
#include "App/AppBase.h"
#include "App/App.h"
//#include "DataItem\ThicknessData.h"
#include "DspFilters\ChebyshevFiltre.h"

namespace
{
	static const int count_buffer = 3000;
	struct PTR
	{
		double *data[count_buffer];
	};
	struct Data
	{
		double data[count_buffer];
	};
}

DefectData::DefectData(int &filterWidth, bool &filterOn, double &brak, double &klass2)
	: medianFiltreWidth(filterWidth)
	, medianFiltreOn(filterOn)
	, brackThreshold(brak) 
	, klass2Threshold(klass2)
	, count(0)
	, zone(0)
	, data((double(&)[DataVieverConst::buf_size])(xdata[App::zone_filter_offset]))
{
}

void DefectData::Set(int start, int stop, double *ascan, const int borderKlass2, const int borderDefect, int widthFiltre, bool filtreOn, int cutoffFrequency, double adjustMul)
{
	int len = stop - start;
	count = len;
	
	MedianFiltre filtre;

	double *startZone = &ascan[start];

	static const int bufSize = 10 * 1024;
	double buf[bufSize];

	if(filtreOn)
	{
		ChebyshevFiltre dsp;
		dsp.Setup(
			Singleton<L502ParametersTable>::Instance().items.get<ChannelSamplingRate>().value
			, 3
			, cutoffFrequency
			, 40
			);	

		if(len > bufSize) len = bufSize;
		startZone = &ascan[start - App::zone_filter_offset];
		for(int i = 0; i < len + App::zone_filter_offset; ++i)
		{
			buf[i] = dsp.OneSample(startZone[i] * adjustMul);
		}
		startZone = &buf[App::zone_filter_offset];		
	}

	filtre.Init(widthFiltre,  startZone - widthFiltre);
	double *s =  startZone;
	for(int i = 0; i < len; ++i)
	{
		data[i] = filtre(s[i]);

		if(data[i] > brackThreshold) status[i] = borderDefect;
		else if(data[i] > klass2Threshold) status[i] = borderKlass2;
    		else status[i] = STATUS_ID(Nominal);
	}
}

ThickDataViewer::ThickDataViewer(int &width, bool &on, double &klass2Border, double &klass3Border, double &defectBorder)
	: medianFiltreWidth(width)
	, medianFiltreOn(on)
	, klass2Border(klass2Border)
	, klass3Border(klass3Border)
	, defectBorder(defectBorder)
{}

void ThickDataViewer::Set(int start, int stop, int channel, double *ascan, char *scanStatus, const int lower, const int above, int widthFiltre)
{
#if 0
	int len = stop - start;
	count = len;
	char *buf = ThickData::buffer;
	double aboveBorder_	= nominalBorder + aboveBorder;
	double lowerBorder_	= nominalBorder - lowerBorder;
	int packetSize = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;

	MedianFiltre filtre;
	filtre.Init(widthFiltre, &ascan[start] - widthFiltre);
	for(int i = 0; i < widthFiltre; ++i)
	{
		filtre.data[i] = &buf[packetSize * ((start - widthFiltre + i) * App::count_Thick_sensors + channel)];
		filtre.status[i] = scanStatus[start - widthFiltre + i];
	}

	double sum = 0;
	int count = 0;

	for(int i = 0; i < len; ++i)
	{
		if(StatusId<Clr<Nominal>>() == scanStatus[start + i])
		{

			int k = filtre.Add(ascan[start + i], scanStatus[start + i], &buf[packetSize * ((start + i) * App::count_Thick_sensors + channel)]);
			data[i] =  filtre.buf[k];
			firstScan[i] = (char *)filtre.data[k];

			if(/*nominalBorder == data[i] || */filtre.status[k] == StatusId<Clr<Nominal>>())
			{
				if(data[i] > aboveBorder_) status[i] = above;
				else if(data[i] < lowerBorder_) status[i] = lower;
				else status[i] = StatusId<Clr<Nominal>>();
				sum += data[i];
				++count;
			}
			else
			{
				 status[i] = filtre.status[k];
			}
		}
		else
		{
			status[i] = scanStatus[start + i];
			firstScan[i] = &buf[packetSize * ((start + i) * App::count_Thick_sensors + channel)]; 
			data[i] = nominalBorder;
		}
	}
	if(count > 0)
	{
		sum /= count;
		for(int i = 0; i < len; ++i)
		{
			if(nominalBorder == data[i]) data[i] = sum;
		}
	}

	zprint(" start %d channel %d\n", start, channel);
#endif
}

DataViewer<Thick>::DataViewer()
	: ThickDataViewer(
	Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T> >().value
	, Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T> >().value
	, Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2  <Thick> >().value
	, Singleton<ThresholdsTable>::Instance().items.get<BorderKlass3  <Thick> >().value
	, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<Thick> >().value
	)
{}

void DataViewer<Thick>::Do(int zone, int channel)
{
	//ItemData<T> &d = Singleton<ItemData<T> >::Instance();
	//int stop = d.offsets[1 + zone];
	//if(stop > d.currentOffset) stop = d.currentOffset;
	//int widthFiltre = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T>>().value;
	//if(0 != widthFiltre)
	//{
	//	widthFiltre = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T>>().value;
	//}
//	Set(d.offsets[zone], stop, channel, d.ascan[channel], d.ascanStatus[channel], StatusId<Clr<BorderLower<T>>>(), StatusId<Clr<BorderAbove<T>>>(), widthFiltre);
}
