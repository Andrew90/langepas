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
	SubLir &lir;
public:
	int framesOffs;
	int zonesOffs;
	double offset;
	unsigned startLen;
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
		tmpPerSamples = 100;
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
			// 
			//if(0 == samplesLen[0] && 0.0 != lir.tmpPerSamples)
			//{
			//	int z = index - 1;
			////	if(samplesLen[z] == 100000) return;
			//
			//	
			//	//for(; z > 0; --z)
			//	//{
			//	//	if(samplesLen[z] == 100000) break;
			//	//}
			//
			//	if(index - z < 3) break;
			//
			//	unsigned d = samplesLen[z + 2] - samplesLen[z + 1];
			//
			//	unsigned start = samplesLen[z + 1];
			//
			//	for(int k = z; k >= 0; --k)
			//	{
			//		start -= d;
			//		samplesLen[k] = start;
			//	}
			//
			//	lir.moduleItems.get<Module<Cross>>().Start();
			//}
			int t = lir.samplesLen[i] - startLen;
			int j = t / App::zone_length;
			if(j > module_zonesOffs)
			{
				double d = double(samples[i] - samples[i - 1]) / (samplesLen[i] -  samplesLen[i - 1]); 
				for(; module_zonesOffs < j; ++module_zonesOffs)
				{
					offset += d * App::zone_length;
					module_zones[module_zonesOffs] = (unsigned )offset;
				}
			}
		}
		module_framesOffs_ = i;
	}
};

template<class T>struct __sq__
{
	typedef SQ<T> O;
	void operator()(SubLir &lir)
	{
		O &sq = lir.sqItems.get<O>();
		sq.time = Performance::Counter();
//		lir.offset[lir.timeIndex] = sq.offs - lir.lastOffs;
		lir.tmpPerSamples = sq.perSamples = double(sq.offs - lir.lastOffs) / (sq.time - lir.lastTime);
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

//template<class T>struct __stop_0_
//{
//	void operator()(){};
//};
//
//template<>struct __stop_0_<off<Cross, 2>>
//{
//	void operator()()
//	{
//		zprint("\n");
//	}
//};

template<class T>void SQ<T>::Do()
{
	__sq__<T>()(lir);	 //сохранение времени срабатывания датчика
	__start__<T>()(lir);  //смещение начала отчёта данных в модуле
	//__stop_0_<T>()();
	TL::foreach<__zones_do__<T>::Result, __sq_do__>()(__sq_do_data__<T>(lir));
}

template<class T>Module<T>::Module(SubLir &lir)
	: lir(lir)
	, zones( Singleton<ItemData<T>>::Instance().offsets)
{}
/// начало измерений модуля(смещение центра)
template<class T>void Module<T>::Start()
{
	SQ<on<T,1>> &sq1 = lir.sqItems.get<SQ<on<T,1>>>();
	SQ<on<T,2>> &sq2 = lir.sqItems.get<SQ<on<T,2>>>();
	unsigned offs = sq1.time + (sq2.time - sq1.time) / 2;

	unsigned *tick = lir.tick;
	unsigned *samples = lir.samples;
	unsigned *samplesLen = lir.samplesLen;
	int index = lir.index;

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
}

template<class T>void Module<T>::Stop()
{
	SQ<off<T,1>> &sq1 = lir.sqItems.get<SQ<off<T,1>>>();
	SQ<off<T,2>> &sq2 = lir.sqItems.get<SQ<off<T,2>>>();
	unsigned offs = sq1.time + (sq2.time - sq1.time) / 2;
	//
	unsigned *tick = lir.tick;
	unsigned *samples = lir.samples;
	for(int i = lir.lastOffs - 1; i > 0; --i)
	{
		if(tick[i] < offs)
		{
			double d = 1.0 - double(offs - tick[i]) / (tick[i + 1] - tick[i]);
			unsigned offs = samples[i] + unsigned(d * (samples[i + 1] - samples[i]));
			for(int k = zonesOffs - 1; k > 0; --k)
			{
				if(offs > zones[k])
				{
					zonesOffs = 1 + k;
					zones[1 + k] = offs;
					return;
				}
			}
		}
	}
}


