#pragma once
#include "App/App.h"
#include "App/AppBase.h"
#include "tools_debug\DebugMess.h"
#include "templates/typelist.hpp"
#include "DataItem/DataItem.h"
#include "Filtre\MedianFiltre.h"

struct DataVieverConst
{
	static const int buf_size = 2 * 2048;
};

struct DefectData
{
private:
	double xdata[App::zone_filter_offset + DataVieverConst::buf_size];
public:
	double (&data)[DataVieverConst::buf_size];
	//double *scan[DataVieverConst::buf_size];
	char status[DataVieverConst::buf_size];
	int count;
	int zone;
	int &medianFiltreWidth;
	bool &medianFiltreOn;
	double &brackThreshold;
	double &klass2Threshold;
	//MedianFiltre filtre;
	DefectData(int &, bool &, double &, double &);
	//void Set(int zone, int start, int stop, int channel, int offs, int maxOffs, double *s
	//	, void (*)(int , double , int , double (&)[App::count_zones], double (&)[App::count_zones], char &)
	//	//,  DefectItem &
	//	);
	 void Set(int start, int stop, double *ascan, const int, const int, int widthFiltre, bool filtreOn, int cutoffFrequencys);
};

//class OffsetChannel
//{
//	struct __data__
//	{
//		int channel, offset;
//	};
//	template<class O, class P>struct __offs__
//	{
//		bool operator()(O *o, P *p)
//		{
//			if(O::ID == p->channel)
//			{
//				p->offset = o->value;
//				return false;
//			}
//			return true;
//		}
//	};
//public:
//	int operator()(int channel)
//	{
//		__data__ data = {channel, 0};
//		TL::find<OffsetsTable::items_list, __offs__>()(&Singleton<OffsetsTable>::Instance().items, &data);
//		return data.offset;
//	}
//};

template<class T>struct DataViewer: DefectData
{	
	DataViewer()
		: DefectData(
		   Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T> >().value
		   , Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T> >().value
		   , Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T> >().value
		)
	{}
	void Do(int zone, int channel)
	{
		ItemData<T> &d = Singleton<ItemData<T> >::Instance();
		int stop = d.offsets[1 + zone];
		if(stop >  d.currentOffset) stop =  d.currentOffset;

		int widthFiltre = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T>>().value;
		if(0 != widthFiltre)
		{
			widthFiltre = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T>>().value;
		}

		Set(d.offsets[zone], stop, d.ascan[channel], StatusId<Clr<BorderKlass2<T>>>(), StatusId<Clr<BorderDefect<T>>>(), widthFiltre
			, Singleton<AnalogFilterTable>::Instance().items.get<CutoffFrequencyOn<T>>().value
			, Singleton<AnalogFilterTable>::Instance().items.get<CutoffFrequency<T>>().value
			);
	}
};

struct ThickDataViewer
{
	double data[DataVieverConst::buf_size];
	char *firstScan[DataVieverConst::buf_size];
	char status[DataVieverConst::buf_size];
	int count;
	int zone;
	int &medianFiltreWidth;
	bool &medianFiltreOn;
	double &aboveBorder  ;
	double &lowerBorder  ;
	double &nominalBorder;
	ThickDataViewer(int &, bool &, double &, double &, double &);
	 void Set(int start, int stop, int channel, double *ascan, char *scanStatus, const int, const int, int);
};
template<>struct DataViewer<Thick>: ThickDataViewer
{
	typedef Thick T;
	DataViewer();
	void Do(int zone, int channel);
};