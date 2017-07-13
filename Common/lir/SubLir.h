#pragma once
#include "PerformanceCounter\PerformanceCounter.h"
#include "templates/typelist.hpp"
#include "Base\TablesDefine.h"
#include "App/App.h"
#include "App/AppBase.h"
#include "DataItem\DataItem.h"

template<class, int>class on;
template<class, int>class off;

class SubLir;

template<class T>class SQ
{
	SubLir &lir;
public:
	unsigned time;
	double perSamples;
	const int &offs;	
	SQ(SubLir &lir);
	void Do();
};

template<class T>class Module
{
	SubLir &lir;
public:
	unsigned time;
	int framesOffs;
	int zonesOffs;
	int zonesStop;
	double rem;
	unsigned (&zones)[1 + App::count_zones];
	Module(SubLir &lir);
	void Start();
};
#pragma warning(disable: 4355)
template<class T>struct __sq_do_data__
{
	SubLir &lir;
	SQ<T> &sq;
	__sq_do_data__(SubLir &lir): lir(lir), sq(lir.sqItems.get<SQ<T>>()){}
};
template<class O, class P>struct __sq_do__
{
	void operator()(P &p)
	{
		Module<O> &module = p.lir.moduleItems.get<Module<O>>();
		Zones::Do(p.lir, p.sq.time, p.sq.perSamples, module.zonesOffs, module.zones, module.framesOffs, module.rem);
	}
};

template<class O, class P, int N>struct __sq_do__<off<O, N>, P>
{
	void operator()(P &p)
	{
		Module<O> &module = p.lir.moduleItems.get<Module<O>>();
		unsigned t = p.lir.sqItems.get<SQ<off<O, 1>>>().time;
		unsigned time = t + (p.sq.time - t) / 2;
		Zones::Do(p.lir, time, p.sq.perSamples, module.zonesOffs, module.zones, module.framesOffs, module.rem);
		module.zonesStop =  module.zonesOffs;
	}
};


template<>struct TL::foreach<NullType, __sq_do__>
{
	template<class P>void operator()(P &p)
	{			
	}			
};

template<class T>struct __zones_do__
{
	typedef TL::MkTlst<Cross, Long>::Result Result;
};
template<>struct __zones_do__<on<Cross,1>>
{
	typedef NullType Result;
};
template<>struct __zones_do__<on<Cross,2>>
{
	typedef TL::MkTlst<Cross>::Result Result;
};
template<int N>struct __zones_do__<on<Thick,N>>
{
	typedef TL::MkTlst<Cross>::Result Result;
};
template<>struct __zones_do__<on<Long,1>>
{
	typedef TL::MkTlst<Cross>::Result Result;
};
template<int N>struct __zones_do__<off<Magn,N>>
{
	typedef NullType Result;
};

template<>struct __zones_do__<off<Cross,2>>
{
	typedef TL::MkTlst<off<Cross, 2>>::Result Result;
};
template<>struct __zones_do__<off<Long,2>>
{
	typedef TL::MkTlst<off<Long, 2>>::Result Result;
};

template<class O, class P>struct __init_modules__
{
	void operator()(O &o)
	{
		o.framesOffs = o.zonesOffs = 0;
	}
};

class SubLir
{
public:
	typedef TL::MkTlst<
		SQ<on<Cross,1>>
		, SQ<on<Cross,2>>
		, SQ<on<Thick, 1>>
		, SQ<on<Thick, 2>>
		, SQ<on<Long, 1>>
		, SQ<on<Long, 2>>
		, SQ<on<Magn, 1>>
		, SQ<on<Magn, 2>>

		, SQ<off<Cross,1>>
		, SQ<off<Cross,2>>
		, SQ<off<Thick, 1>>
		, SQ<off<Thick, 2>>
		, SQ<off<Long, 1>>
		, SQ<off<Long, 2>>
		, SQ<off<Magn, 1>>
		, SQ<off<Magn, 2>>
		>::Result sq_list;
	TL::Factory<sq_list> sqItems;
	static const int buf_size = TL::Length<sq_list>::value - 2;
	int timeIndex;
	int offset[buf_size];
	double tmpPerSamples;
	typedef TL::MkTlst<Module<Cross>, Module<Long>>::Result module_list;
	TL::Factory<module_list> moduleItems;

	unsigned tick[3000];
	unsigned samples[3000];
	unsigned lastTime;
	int lastOffs;
	int index;
	int indexLength;
	unsigned startTime;
	int currentSamples;
	SubLir()
		: sqItems(*this)
		, moduleItems(*this)
	{ }
	void Start()
	{
		timeIndex = 0;
		indexLength = 0;
		index = 0;
		currentSamples = 0;
		TL::foreach<module_list, __init_modules__>()(moduleItems);
	}
	void Do()
	{
		__sq_do_data__<on<Magn,2>>data(*this);
		data.sq.time = tick[index];
		TL::foreach<__zones_do__<__zones_do__<on<Magn,2>>>::Result, __sq_do__>()(data);
	}
};

template<class T>struct SQ_SubType;
template<template<class, int>class W, class T, int NUM>struct SQ_SubType<W<T, NUM>>
{
	typedef T Result;
	static const int value = NUM;
};

template<class T>SQ<T>::SQ(SubLir &lir)
	: lir(lir)
	, offs(Singleton<OffsetSensorsTable>::Instance().items.get<typename OffsSQ<typename SQ_SubType<T>::Result, SQ_SubType<T>::value>>().value)
{}

template<class T>struct Stop{void operator()(){}};

template<template<class, int>class W, int N>struct Stop<W<Cross, N>>
{
	void operator()()
	{
		printf("%s\n", __FUNCTION__);
	}
};

struct Zones
{
	static void Do(SubLir &lir, unsigned sq_time, double sq_perSamples
		, int  &module_zonesOffs, unsigned *module_zones
		, int &module_framesOffs, double &module_rem
		)
	{	
		if(0 == module_framesOffs) return;
		unsigned *samples = lir.samples;
		unsigned *tick = lir.tick;

		int index = lir.index;

		for(; module_framesOffs < index; ++module_framesOffs)
		{
			if(tick[module_framesOffs] > sq_time)
			{
				break;
			}
			if(module_rem > App::zone_length)
			{
				module_rem -= App::zone_length;
				double d = module_rem / App::zone_length;
				module_zones[module_zonesOffs] = samples[module_framesOffs - 1] + unsigned(d * (samples[module_framesOffs] - samples[module_framesOffs - 1]));
				if(module_zonesOffs < 1 + App::count_zones)	++module_zonesOffs;
			}
			module_rem += (tick[module_framesOffs] - tick[module_framesOffs - 1]) * sq_perSamples;
		}
	}
};

template<class T>struct __sq__
{
	typedef SQ<T> O;
	void operator()(SubLir &lir)
	{
		O &sq = lir.sqItems.get<O>();
		sq.time = Performance::Counter();
		lir.offset[lir.timeIndex] = sq.offs - lir.lastOffs;
		lir.tmpPerSamples = sq.perSamples = double(sq.offs - lir.lastOffs) / (sq.time - lir.lastTime);
		printf("tmpPerSamples %f\n", lir.tmpPerSamples);
		++lir.timeIndex;
		lir.lastTime = sq.time;
		lir.lastOffs = sq.offs;
	}
};

template<>struct __sq__<on<Cross, 1>>
{
	typedef SQ<on<Cross, 1>> O;
	void operator()(SubLir &lir)
	{
		O &sq = lir.sqItems.get<O>();
		sq.time = Performance::Counter();
		lir.lastTime = sq.time;
		lir.lastOffs = 0;
		lir.startTime = sq.time;
		sq.perSamples = lir.tmpPerSamples;
	}
};

template<>struct __sq__<off<Cross, 1>>
{
	typedef SQ<off<Cross, 1>> O;
	void operator()(SubLir &lir)
	{	
		O &sq = lir.sqItems.get<O>();
		sq.time = Performance::Counter();
		lir.lastTime = sq.time;
		lir.lastOffs = 0;
		sq.perSamples = lir.tmpPerSamples;
	}
};

template<class T>struct __start__
{
	void operator()(SubLir &lir)
	{
	}
};

template<>struct __start__<on<Magn, 2>>
{
	void operator()(SubLir &lir)
	{
	}
};

template<class T>struct __start__<on<T, 2>>
{
	void operator()(SubLir &lir)
	{
		lir.moduleItems.get<Module<T>>().Start();
	}
};

template<class T>void SQ<T>::Do()
{
	__sq__<T>()(lir);	 //сохранение времени срабатывания датчика
	__start__<T>()(lir);  //смещение начала отчёта данных в модуле
	TL::foreach<__zones_do__<T>::Result, __sq_do__>()(__sq_do_data__<T>(lir));
}

template<class T>Module<T>::Module(SubLir &lir)
	: lir(lir)
	, zones( Singleton<ItemData<T>>::Instance().offsets)
{}

template<class T>void Module<T>::Start()
{
	zonesStop = 0;
	SQ<on<T,1>> &sq1 = lir.sqItems.get<SQ<on<T,1>>>();
	SQ<on<T,2>> &sq2 = lir.sqItems.get<SQ<on<T,2>>>();
	unsigned offs = sq1.time + (sq2.time - sq1.time) / 2;

	unsigned *tick = lir.tick;
	unsigned *samples = lir.samples;
	int index = lir.index;
	int i = 0;
	for(; i < index; ++i)  /// смещение 1 зоны в отчётах
	{
		if(tick[i] > offs)
		{
			double d = double(tick[i] - offs) / (tick[i - 1] - tick[i]);
			zones[0] = samples[i - 1] + unsigned(d * (samples[i] - samples[i - 1]));
			zonesOffs = 1;
			framesOffs = i;
			rem = d * (tick[i] - tick[i - 1]) * sq2.perSamples;
			break;
		}
	}	
}


