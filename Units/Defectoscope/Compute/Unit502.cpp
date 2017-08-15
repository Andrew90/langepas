#include "stdafx.h"
#include "Unit502.h"
#include "L502\L502.h"
#include "DataItem\DataItem.h"
#include "tools_debug\DebugMess.h"

#include "PerformanceCounter\PerformanceCounter.h"
#include "App/App.h"
#include "lir\SubLir.h"

namespace Unit502N
{
	L502 l502;
	ItemData<Cross> &crossData = Singleton<ItemData<Cross>>::Instance();
	ItemData<Long> &longData = Singleton<ItemData<Long>>::Instance();
	ItemData<Solid> &solidData = Singleton<ItemData<Solid>>::Instance();
	TimeLir &timeFrames = Singleton<TimeLir>::Instance();
	SubLir &lir = Singleton<SubLir>::Instance();

	static const int length = App::count_cross_sensors + App::count_long_sensors + SolidGroupData::count_sensors;

	double *arr[length];

	struct InitArr
	{
		InitArr()
		{
			int k = 0;
			for(int i = 0; i < dimention_of(crossData.ascan); ++i)
			{
				arr[k++] = crossData.ascan[i];
			}
			for(int i = 0; i < dimention_of(longData.ascan); ++i)
			{
				arr[k++] = longData.ascan[i];
			}
			for(int i = 0; i < dimention_of(solidData.ascan); ++i)
			{
				arr[k++] = solidData.ascan[i];
			}
		}
	} initArr;

}
const double koef = 100.0 / 0x7fff; //16 - разрядный АЦП - приводим к 100 %
void Unit502::Read()
{
	double data[L502::buffer_length];
	unsigned startChannel;
	unsigned count = dimention_of(data);

	if(Unit502N::l502.Read(startChannel, data, count))
	{
		int offs = Unit502N::lir.currentSamples;
		for(int i = 0; i < (int)count; ++i)
		{
			int k = offs / Unit502N::length;
			if(k < App::count_frames)
			{
				int sens = startChannel + i;
				Unit502N::arr[sens % Unit502N::length][k] = data[i] * koef;;
				++offs;
			}
		}
		if(dimention_of(Unit502N::lir.samples) > Unit502N::lir.index)
		{
			Unit502N::lir.samples[Unit502N::lir.index] = offs / Unit502N::length;
			Unit502N::lir.tick[Unit502N::lir.index] = Performance::Counter();
			if(Unit502N::lir.index > 0)
			{
			   Unit502N::lir.samplesLenMax += Unit502N::lir.tmpPerSamples * (Unit502N::lir.tick[Unit502N::lir.index] - Unit502N::lir.tick[Unit502N::lir.index - 1]);
			   Unit502N::lir.samplesLen[Unit502N::lir.index] = (unsigned)Unit502N::lir.samplesLenMax;
			}
			else
			{
				Unit502N::lir.samplesLenMax = 100000;
				Unit502N::lir.samplesLen[Unit502N::lir.index] = 0;
			}
			++Unit502N::lir.index;
		}
		Unit502N::lir.currentSamples = offs;
	}
}

bool Unit502::Init()
{
	return Unit502N::l502.Init();
}

void Unit502::Destroy()
{
	Unit502N::l502.Destroy();
}

bool Unit502::SetupParams()
{
	return Unit502N::l502.SetupParams();
}

int Unit502::Start()
{
	Unit502N::lir.currentSamples = 0;
	Unit502N::lir.index = 0;
	Unit502N::lir.tmpPerSamples = 0;
	return Unit502N::l502.Start();
}

int Unit502::Stop()
{
	return Unit502N::l502.Stop();
}

bool Unit502::ReadAsync(unsigned ch, int range, double &value)
{
	return Unit502N::l502.ReadAsync(ch, range, value);
}

bool Unit502::BitOut(unsigned ch, bool value)
{
	return Unit502N::l502.BitOut(ch, value);
}


