#pragma once
#include "PerformanceCounter\PerformanceCounter.h"
#include <math.h>
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
public:
	SubLir &lir;
public:
	int framesOffs;
	int zonesOffs;
	double offset;
	unsigned startLen;
	unsigned stopLen;
	unsigned (&zones)[1 + App::count_zones];
	Module(SubLir &lir);
	void Start();
	void Stop();
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
		unsigned t = Performance::Counter();
		Zones::Do(p.lir, t, p.sq.perSamples, module.zonesOffs, module.zones, module.framesOffs//, module.rem
			, module.startLen
			, module.offset
			);
	}
};

template<class O, class P, int N>struct __sq_do__<off<O, N>, P>
{
	void operator()(P &p)
	{
		Module<O> &module = p.lir.moduleItems.get<Module<O>>();
		unsigned t = p.lir.sqItems.get<SQ<off<O, 1>>>().time;
		unsigned time = t + (p.sq.time - t) / 2;
		Zones::Do(p.lir, time, p.sq.perSamples, module.zonesOffs, module.zones, module.framesOffs//, module.rem
			, module.startLen
			, module.offset
			);
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
		o.framesOffs = o.zonesOffs = o.startLen = 0;
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
	//int offset[buf_size];
	double tmpPerSamples;
	typedef TL::MkTlst<Module<Cross>, Module<Long>>::Result module_list;
	TL::Factory<module_list> moduleItems;

	double samplesLenMax;

	unsigned tick[3000];
	unsigned samples[3000];
	unsigned samplesLen[3000];
	unsigned lastTime;
	int lastOffs;
	int index;
	unsigned startTime;
	int currentSamples;
	SubLir()
		: sqItems(*this)
		, moduleItems(*this)
	{ }
	void Start()
	{
		timeIndex = 0;
		tmpPerSamples = 0;
		lastTime = 0;
		lastOffs = 0;
		index = 0;
		startTime = 0;
		currentSamples = 0;
		samplesLenMax = 0;
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
	static void Do(SubLir &lir, unsigned sq_time
		, double sq_perSamples
		, int  &module_zonesOffs, unsigned *module_zones
		, int &module_framesOffs_
		, unsigned startLen
		, double &offset
		)
	{	
		if(0 == module_framesOffs_) return;
		unsigned *samples = lir.samples;
		unsigned *tick = lir.tick;
		unsigned *samplesLen = lir.samplesLen;

		int index = lir.index;
		int i = module_framesOffs_;
		double offs = samples[i - 1];
		for(; i < index; ++i)
		{
			if(tick[i] > sq_time)
			{
				if(i != module_framesOffs_)module_framesOffs_ = i - 1;
				return;
			}

			int t = lir.samplesLen[i] - startLen;
			int j = t / App::zone_length;
			if(j > module_zonesOffs)
			{
				double d = double(samples[i] - samples[i - 1]) / (samplesLen[i] -  samplesLen[i - 1]); 
				for(int i = module_zonesOffs; i < j; ++i)
				{
					offset += d * App::zone_length;
					module_zones[i] = int(offset);
					++module_zonesOffs;
				}
			}
		}
		module_framesOffs_ = i;
	}
};

template<class T>struct __sq__;
template<class X, int N, template<class, int>class W>struct __sq__<W<X, N>>
{
	typedef SQ<W<X, N>> O;
	void operator()(SubLir &lir)
	{
		O &sq = lir.sqItems.get<O>();
		SQ<W<Cross, 1>> &sqC = lir.sqItems.get<SQ<W<Cross, 1>>>();
		sq.time = Performance::Counter();
		double t = sq.perSamples = double(sq.offs - sqC.offs) / (sq.time - sqC.time);

		int stop = lir.index - 1;
		for(int i = lir.timeIndex; i < stop; ++i)
		{
			lir.samplesLenMax += t * (lir.tick[1 + i] - lir.tick[i]);
			lir.samplesLen[i] = (unsigned)lir.samplesLenMax;
		}

		lir.timeIndex = stop;

		lir.lastTime = sq.time;
		lir.lastOffs = sq.offs;
		lir.tmpPerSamples = t;
		zprint(" tmpPerSamples %f offs %d\n", lir.tmpPerSamples, sq.offs);
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
		lir.lastOffs = sq.offs;
		lir.startTime = sq.time;
		sq.perSamples = lir.tmpPerSamples;
		zprint(" tmpPerSamples %f\n", lir.tmpPerSamples);
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
		lir.lastOffs = sq.offs;
		sq.perSamples = lir.tmpPerSamples;
		zprint(" tmpPerSamples %f\n", lir.tmpPerSamples);
	}
};

template<template<class, int>class W>struct __sq__<W<Magn, 2>>
{
	void operator()(SubLir &lir)
	{	
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
		//посмотреть чтобы было смещение для гр пр
		Singleton<ItemData<Solid>>::Instance().start = lir.samples[lir.index + 1] + 5000;	
	}
};

template<>struct __start__<off<Magn, 1>>
{
	void operator()(SubLir &lir)
	{
		Singleton<ItemData<Solid>>::Instance().stop = lir.samples[lir.index - 1];		
	}
};

template<>struct __start__<off<Magn, 2>>
{
	void operator()(SubLir &lir)
	{

	}
};


template<>struct __start__<on<Long, 2>>
{
	void operator()(SubLir &lir)
	{
		lir.moduleItems.get<Module<Long>>().Start();
	}
};

template<>struct __start__<on<Cross, 2>>
{
	void operator()(SubLir &lir)
	{
		lir.moduleItems.get<Module<Cross>>().Start();
	}
};

template<>struct __start__<off<Cross, 2>>
{
	void operator()(SubLir &lir)
	{
		lir.moduleItems.get<Module<Cross>>().Stop();
	}
};

template<>struct __start__<off<Long, 2>>
{
	void operator()(SubLir &lir)
	{
		lir.moduleItems.get<Module<Long>>().Stop();
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
/// начало измерений модуля(смещение центра)
template<class T>void Module<T>::Start()
{
	zprint(" <<>>>><>> module start\n");
	SQ<on<T,1>> &sq1 = lir.sqItems.get<SQ<on<T,1>>>();
	SQ<on<T,2>> &sq2 = lir.sqItems.get<SQ<on<T,2>>>();
	unsigned offs = sq1.time + (sq2.time - sq1.time) / 2;

	unsigned *tick = lir.tick;
	unsigned *samples = lir.samples;
	unsigned *samplesLen = lir.samplesLen;
	int index = lir.index;
#if 1
	for(int i = 0; i < index; ++i)  /// смещение 1 зоны в отчётах
	{
		if(tick[i] > offs)
		{
			double d = double(tick[i] - offs) / (tick[i] - tick[i - 1]);
			zones[0] = samples[i - 1] + unsigned(d * (samples[i] - samples[i - 1]));
			offset = zones[0];
			zonesOffs = 1;
			framesOffs = i;
			startLen = 	samplesLen[i - 1] + unsigned(d * (samplesLen[i] - samplesLen[i - 1]));
			break;
		}
	}
#else
	offset = zones[0] = samples[0];
	zonesOffs = 1;
	framesOffs = 1;
#endif
}

template<class T>struct __module_stop__;
template<class T>struct __module_stop__<Module<T>>
{
	void operator()(Module<T> &p)
	{
		SQ<off<T,1>> &sq1 = p.lir.sqItems.get<SQ<off<T,1>>>();
		SQ<off<T,2>> &sq2 = p.lir.sqItems.get<SQ<off<T,2>>>();
		unsigned offs = sq1.time + (sq2.time - sq1.time) / 2;
		unsigned *tick = p.lir.tick;
		unsigned *samples = p.lir.samples;
		int index = p.lir.index - 1;
		for(int i = index; i > 0; --i)
		{
			if(tick[i] < offs)
			{
				double d = 1.0 - double(offs - tick[i]) / (tick[i + 1] - tick[i]);
				unsigned offs1 = samples[i] + unsigned(d * (samples[i + 1] - samples[i]));
				unsigned dZone = p.zones[1] - p.zones[0];
				for(int i = 0; i < 62; ++i)
				{
					if(0 == p.zones[i])
					{
						p.zones[i] = p.zones[i - 1] + dZone;
					}
					if(offs1 < p.zones[i])
					{
						for(int k = i + 1, j = 0; j < 4 && k < 62; ++k, ++j)
						{
							p.zones[k] = p.zones[k - 1] + dZone;
						}
						p.zonesOffs = 4 + i;
						ItemData<T> &module = Singleton<ItemData<T>>::Instance();
						module.currentOffsetZones = p.zonesOffs; 
						return;
					}
				}
			}
		}
	}
};

template<>struct __module_stop__<Module<Long>>
{
	void operator()(Module<Long> &p)
	{
		typedef Long T;
		unsigned dZone = p.zones[1] - p.zones[0];

		for(int i = 0; i < 62; ++i)
		{
			if(0 == p.zones[i])
			{
				p.zones[i] = p.zones[i - 1] + dZone;
			}
		}
		p.zonesOffs = Singleton<ItemData<Cross>>::Instance().currentOffsetZones;
		Singleton<ItemData<T>>::Instance().currentOffsetZones = p.zonesOffs;
	}
};

template<class T>void Module<T>::Stop()
{
	__module_stop__<Module<T>>()(*this);
}



