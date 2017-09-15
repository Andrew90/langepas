#pragma once
#include "App/App.h"
#include "App/AppBase.h"
#include "tools_debug\DebugMess.h"
#include "templates/typelist.hpp"
#include "DataItem/DataItem.h"
#include "Filtre\MedianFiltre.h"
#include "MessageText\ListMess.hpp"

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
	char status[DataVieverConst::buf_size];
	int count;
	int zone;
	int &medianFiltreWidth;
	bool &medianFiltreOn;
	double &brackThreshold;
	double &klass2Threshold;
	DefectData(int &, bool &, double &, double &);
	 void Set(int start, int stop, double *ascan, const int, const int, int widthFiltre, bool filtreOn, int cutoffFrequencys, double adjustMul);
};

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
	void Do(int zone, int channel, double adjustMul)
	{
		ItemData<T> &d = Singleton<ItemData<T> >::Instance();

		int widthFiltre = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T>>().value;
		if(0 != widthFiltre)
		{
			widthFiltre = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T>>().value;
		}

		int offs0 = d.offsets[zone];
		int offs1 = d.offsets[1 + zone];

		Set(offs0, offs1, d.ascan[channel], STATUS_ID(BorderKlass2<T>), STATUS_ID(BorderDefect<T>), widthFiltre
			, Singleton<AnalogFilterTable>::Instance().items.get<CutoffFrequencyOn<T>>().value
			, Singleton<AnalogFilterTable>::Instance().items.get<CutoffFrequency<T>>().value
			, adjustMul
			);
	}
};

//template<>struct DataViewer<Cross>: DefectData 
//{	
//	typedef Cross T;
//	DataViewer()
//		: DefectData(
//		   Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T> >().value
//		   , Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T> >().value
//		   , Singleton<ThresholdsTable>::Instance().items.get<BorderDefect<T> >().value
//		   , Singleton<ThresholdsTable>::Instance().items.get<BorderKlass2<T> >().value
//		)
//	{}
//	void Do(int zone, int channel, double adjustMul)
//	{
//		if(channel > Singleton<ParametersTable>::Instance().items.get<CrossCountSensors>().value - 1)
//		{
//			count = 0;
//			return;
//		}
//		
//		ItemData<T> &d = Singleton<ItemData<T> >::Instance();
//
//		int widthFiltre = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreOn<T>>().value;
//		if(0 != widthFiltre)
//		{
//			widthFiltre = Singleton<MedianFiltreTable>::Instance().items.get<MedianFiltreWidth<T>>().value;
//		}
//
//		int offs0 = d.offsets[zone];
//		int offs1 = d.offsets[1 + zone];
//
//		Set(offs0, offs1, d.ascan[channel], STATUS_ID(BorderKlass2<T>), STATUS_ID(BorderDefect<T>), widthFiltre
//			, Singleton<AnalogFilterTable>::Instance().items.get<CutoffFrequencyOn<T>>().value
//			, Singleton<AnalogFilterTable>::Instance().items.get<CutoffFrequency<T>>().value
//			, adjustMul
//			);
//	}
//};

struct ThickDataViewer
{
	double data[DataVieverConst::buf_size];
	char *firstScan[DataVieverConst::buf_size];
	char status[DataVieverConst::buf_size];
	int count;
	int zone;
	int &medianFiltreWidth;
	bool &medianFiltreOn;
	double &klass2Border;
	double &klass3Border;
	double &defectBorder;
	ThickDataViewer(int &, bool &, double &, double &, double &);
	 void Set(int start, int stop, int channel, double *ascan, char *scanStatus, const int, const int, int);
};
template<>struct DataViewer<Thick>: ThickDataViewer
{
	typedef Thick T;
	DataViewer();
	void Do(int zone, int channel);
};