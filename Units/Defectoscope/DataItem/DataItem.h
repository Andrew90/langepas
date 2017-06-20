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
	int currentTimeOffset;	//счётчик считанных кадров с ацп
	int deadSamplesBegin, deadSamplesEnd;
	double ascan[SENS][App::count_frames];	 //кадры с ацп
	int offsets[1 + App::count_zones];		  //смещение кадров по зонам
	double buffer[SENS][App::count_zones];	 //значение в зоне
	char status[SENS][App::count_zones];	 //статус в зоне
	unsigned time[3000];	//				 // время принятого кадра(для рсчёта смещения по зонам)
	unsigned frame[3000];	//распределение кадров по зонам
};

struct PrimaryThickData
{
	static const int count_sensors = App::count_Thick_sensors;
	int currentOffset;	  //счётчик кадров
	int currentOffsetZones;	 //счётчик собранных зон
	int currentTimeOffset;	//счётчик считанных кадров с ацп
	int deadSamplesBegin, deadSamplesEnd; 
	double ascan[count_sensors][App::count_frames];	 //кадры с ацп
	char ascanStatus[count_sensors][App::count_frames];	 //кадры с ацп
	int offsets[1 + App::count_zones];		  //смещение кадров по зонам
	double bufferMin[count_sensors][App::count_zones];	 //значение в зоне
	double bufferMax[count_sensors][App::count_zones];	 //значение в зоне
	char status[count_sensors][App::count_zones];	 //статус в зоне
	unsigned time[3000];	//				 // время принятого кадра(для рсчёта смещения по зонам)
	unsigned frame[3000];	//распределение кадров по зонам
};

struct SolidGroupData
{
	static const int count_sensors = 2;
	int currentOffset;
	int currentOffsetZones;
	double ascan[count_sensors][App::count_frames];	
};

struct ResultData
{
	int currentOffsetZones;
	char status[App::count_zones];
};

template<class T>struct ItemData;

template<>struct ItemData<Cross>: PrimaryData<App::count_cross_sensors>{};
template<>struct ItemData<Long>: PrimaryData<App::count_long_sensors>{};

template<>struct ItemData<Solid>: SolidGroupData{};

template<>struct ItemData<Thick>: PrimaryThickData{};




