#pragma once
#include "PerformanceCounter\PerformanceCounter.h"
#include "templates/typelist.hpp"
#include "Base\TablesDefine.h"

class Cross{};
class Long{};
class Thick{};
class Magn{};

template<class T, int N>struct OffsSQ;

DEFINE_PARAM_WAPPER_NUM(OffsSQ, Cross, 1, int, 0)
	DEFINE_PARAM_WAPPER_NUM(OffsSQ, Cross, 2, int, 600)

	DEFINE_PARAM_WAPPER_NUM(OffsSQ, Thick, 1, int, 800)
	DEFINE_PARAM_WAPPER_NUM(OffsSQ, Thick, 2, int, 1400)

	DEFINE_PARAM_WAPPER_NUM(OffsSQ, Long, 1, int, 1600)
	DEFINE_PARAM_WAPPER_NUM(OffsSQ, Long, 2, int, 2200)

	DEFINE_PARAM_WAPPER_NUM(OffsSQ, Magn, 1, int, 2400)
	DEFINE_PARAM_WAPPER_NUM(OffsSQ, Magn, 2, int, 3000)

struct OffsetSensorsTable
{
	typedef TL::MkTlst<
		OffsSQ<Long, 1>
		, OffsSQ<Long, 2>
		, OffsSQ<Cross, 1>
		, OffsSQ<Cross, 2>
		, OffsSQ  <Thick, 1>
		, OffsSQ  <Thick, 2>
		, OffsSQ<Magn, 1>
		, OffsSQ<Magn, 2>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OffsetSensorsTable";}
};

template<class, int>class On;
template<class, int>class Off;

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
	double rem;
	unsigned zones[61];
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


template<>struct TL::foreach<NullType, __sq_do__>
{
	template<class P>void operator()(P &p)
	{			
	}			
};

template<class T>struct __zones_do__
{
	typedef TL::MkTlst<Cross, Thick, Long>::Result Result;
};
template<>struct __zones_do__<On<Cross,1>>
{
	typedef NullType Result;
};
template<>struct __zones_do__<On<Cross,2>>
{
	typedef TL::MkTlst<Cross>::Result Result;
};
template<>struct __zones_do__<On<Thick,1>>
{
	typedef TL::MkTlst<Cross>::Result Result;
};
template<>struct __zones_do__<On<Thick,2>>
{
	typedef TL::MkTlst<Cross, Thick>::Result Result;
};
template<>struct __zones_do__<On<Long,1>>
{
	typedef TL::MkTlst<Cross, Thick>::Result Result;
};

class SubLir
{
public:
	typedef TL::MkTlst<
		SQ<On<Cross,1>>
		, SQ<On<Cross,2>>
		, SQ<On<Thick, 1>>
		, SQ<On<Thick, 2>>
		, SQ<On<Long, 1>>
		, SQ<On<Long, 2>>
		, SQ<On<Magn, 1>>
		, SQ<On<Magn, 2>>

		, SQ<Off<Cross,1>>
		, SQ<Off<Cross,2>>
		, SQ<Off<Thick, 1>>
		, SQ<Off<Thick, 2>>
		, SQ<Off<Long, 1>>
		, SQ<Off<Long, 2>>
		>::Result sq_list;
	TL::Factory<sq_list> sqItems;
	static const int buf_size = TL::Length<sq_list>::value - 2;
	int timeIndex;
	int offset[buf_size];
	double tmpPerSamples;
	typedef TL::MkTlst<Module<Cross>, Module<Thick>, Module<Long>>::Result module_list;
	TL::Factory<module_list> moduleItems;

	unsigned *tick;
	unsigned *samples;
	unsigned lastTime;
	int lastOffs;
	int &index;
	int indexLength;
	unsigned startTime;
	SubLir(unsigned *tick, unsigned *samples, int &index)
		: sqItems(*this)
		, moduleItems(*this)
		, tick(tick)
		, samples(samples)
		, index(index)
	{ }
	void Start()
	{
		timeIndex = 0;
		indexLength = 0;
	}
	void Do()
	{
		__sq_do_data__<On<Magn,2>>data(*this);
		data.sq.time = tick[index];
		TL::foreach<__zones_do__<__zones_do__<On<Magn,2>>>::Result, __sq_do__>()(data);
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
		unsigned *samples = lir.samples;
		unsigned *tick = lir.tick;

		int index = lir.index;

		for(; module_framesOffs < index; ++module_framesOffs)
		{
			if(tick[module_framesOffs] > sq_time)
			{
				break;
			}
			if(module_rem > 200.0)
			{
				module_rem -= 200.0;
				double d = module_rem / 200.0;
				module_zones[module_zonesOffs] = samples[module_framesOffs - 1] + unsigned(d * (samples[module_framesOffs] - samples[module_framesOffs - 1]));
				if(module_zonesOffs < 60)
				{
					++module_zonesOffs;
					//	printf("%02d ", module_zonesOffs);
				}
				//else
				//{
				//	printf("x");
				//}
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
		++lir.timeIndex;
		lir.lastTime = sq.time;
		lir.lastOffs = sq.offs;
	}
};

template<>struct __sq__<On<Cross, 1>>
{
	typedef SQ<On<Cross, 1>> O;
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

template<>struct __sq__<Off<Cross, 1>>
{
	typedef SQ<Off<Cross, 1>> O;
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

template<>struct __start__<On<Magn, 2>>
{
	void operator()(SubLir &lir)
	{
	}
};

template<class T>struct __start__<On<T, 2>>
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
{}

template<class T>void Module<T>::Start()
{
	SQ<On<T,1>> &sq1 = lir.sqItems.get<SQ<On<T,1>>>();
	SQ<On<T,2>> &sq2 = lir.sqItems.get<SQ<On<T,2>>>();
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


