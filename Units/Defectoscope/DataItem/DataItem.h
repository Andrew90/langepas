#pragma once
#include "App/App.h"


struct TimeLir
{
	LARGE_INTEGER frequency, start;
	int currentTimeOffset;
	unsigned time[App::count_time_count_down];
	unsigned lir[App::count_time_count_down];
	///> сбрасывает в 0 число собранных данных
	//void Clear();
	/////> запоминает текущий тик и смещение лир
	//void Do(unsigned);
	/////> выводит тик с момента Clear в милисек.
	//unsigned CurrentTickCount();
};

template<int SENS>struct PrimaryData
{
	static const int count_sensors = SENS;
	int currentOffset;	  //счётчик кадров
	int currentOffsetZones;	 //счётчик собранных зон
	double ascan[SENS][App::count_frames];	 //кадры с ацп
	unsigned offsets[1 + App::count_zones];		  //смещение кадров по зонам
	double buffer[SENS][App::count_zones];	 //значение в зоне
	char status[SENS][App::count_zones];	 //статус в зоне
};

struct PrimaryThickData
{
	int currentOffsetZones;
	double buffer[App::count_zones];
	char status[App::count_zones];
};

struct SolidGroupData
{
	static const int count_sensors = App::count_solid_sensors;
	int start, stop;
	double ascan[count_sensors][App::count_frames];	
};

struct ResultData
{	
	int resultCommon;
	int cutZone0, cutZone1;
	int currentOffsetZones;
	char status[App::count_zones];
	char solidGroup;
};

template<class T>struct ItemData;

template<>struct ItemData<Cross>: PrimaryData<App::count_cross_sensors>{};
template<>struct ItemData<Long>: PrimaryData<App::count_long_sensors>{};

template<>struct ItemData<Solid>: SolidGroupData{};

template<>struct ItemData<Thick>: PrimaryThickData{};

template<class T, class Z>struct ViewerData
{
	int &currentOffsetZones;
	double buffer[ItemData<T>::count_sensors][App::count_zones];	 //значение в зоне
	char status[ItemData<T>::count_sensors][App::count_zones];	 //статус в зоне
	ViewerData(ItemData<T> &d)
		: currentOffsetZones(d.currentOffsetZones)
	{
		memmove(buffer, d.buffer, sizeof(buffer));
		memmove(status, d.status, sizeof(status));
	}
};
class CommonViewer;
template<class T>struct ViewerData<T, CommonViewer>
{
	int &currentOffsetZones;
	double (&buffer)[ItemData<T>::count_sensors][App::count_zones];	 //значение в зоне
	char (&status)[ItemData<T>::count_sensors][App::count_zones];	 //статус в зоне
	ViewerData(ItemData<T> &d)
		: currentOffsetZones(d.currentOffsetZones)
		, buffer(d.buffer)
		, status(d.status)
	{}
};




