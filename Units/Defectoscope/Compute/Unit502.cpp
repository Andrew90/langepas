#include "stdafx.h"
#include "Unit502.h"
#include "L502\L502.h"
#include "DataItem\DataItem.h"
#include "tools_debug\DebugMess.h"

#include "PerformanceCounter\PerformanceCounter.h"
#include "App/App.h"
#include "lir\SubLir.h"
#include "App\SyncChannel.hpp"

namespace Unit502N
{
	L502 l502;
	ItemData<Cross> &crossData = Singleton<ItemData<Cross>>::Instance();
	ItemData<Long> &longData = Singleton<ItemData<Long>>::Instance();
	ItemData<Solid> &solidData = Singleton<ItemData<Solid>>::Instance();
	TimeLir &timeFrames = Singleton<TimeLir>::Instance();
	SubLir &lir = Singleton<SubLir>::Instance();
	typedef SYNC(L502RangeTable::items_list) range_list;

	static const int length = TL::Length<range_list>::value;

	double *arr[length];
	double kor[length];
	double data[L502::buffer_length] = {};
	struct InitArr
	{
		void operator()()
		{
			int k = 0;
			for(int i = 0; i < dimention_of(longData.ascan); ++i)
			{
				arr[k++] = longData.ascan[i];
			}
			for(int i = 0; i < dimention_of(crossData.ascan); ++i)
			{
				arr[k++] = crossData.ascan[i];
			}
			for(int i = dimention_of(solidData.ascan) - 1; i >= 0; i--)
			{
				arr[k++] = solidData.ascan[i];
			}
		}
	};

}
void Unit502::Read()
{	
	unsigned startChannel;
	unsigned count = dimention_of(Unit502N::data);

	if(Unit502N::l502.Read(startChannel, Unit502N::data, count))
	{
		int offs = Unit502N::lir.currentSamples;
		for(int i = 0; i < (int)count; ++i)
		{
			int k = offs / Unit502N::length;
			if(k < App::count_frames)
			{
				int sens = (startChannel + i) % Unit502N::length;
				Unit502N::arr[sens][k] = Unit502N::data[i] * Unit502N::kor[sens];
				++offs;
			}
		}
		if(dimention_of(Unit502N::lir.samples) > Unit502N::lir.index)
		{
			Unit502N::lir.samples[Unit502N::lir.index] = offs / Unit502N::length;
			Unit502N::lir.tick[Unit502N::lir.index] = Performance::Counter();
			//if(Unit502N::lir.index > 0)
			//{
			//   Unit502N::lir.samplesLenMax += Unit502N::lir.tmpPerSamples * (Unit502N::lir.tick[Unit502N::lir.index] - Unit502N::lir.tick[Unit502N::lir.index - 1]);
			//   Unit502N::lir.samplesLen[Unit502N::lir.index] = (unsigned)Unit502N::lir.samplesLenMax;
			//}
			++Unit502N::lir.index;
		}
		Unit502N::lir.currentSamples = offs;
	}
}

bool Unit502::Init()
{
	start = false;
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
namespace Unit502N
{
	const double k[] = {10.0, 5.0, 2.0, 1.0, 0.5, 0.2};
	template<class O, class P>struct __koef__
	{
		void operator()(O &o)
		{
			kor[TL::IndexOf<range_list, O>::value] = 100.0 / k[o.value];
		}
	};
}

int Unit502::Start()
{
	Unit502N::InitArr()();
	Unit502N::lir.currentSamples = 0;
	Unit502N::lir.index = 0;
	Unit502N::lir.tmpPerSamples = 0;
	TL::foreach<Unit502N::range_list, Unit502N::__koef__>()(Singleton<L502RangeTable>::Instance().items);
	start = true;
	return Unit502N::l502.Start();
}

int Unit502::Stop()
{
	if(start)
	{				
		start = false;
		return Unit502N::l502.Stop();
	}
	else
	{
		return -1;
	}
}

bool Unit502::ReadAsync(unsigned ch, int mode, int range, double &value)
{
	return Unit502N::l502.ReadAsync(ch, mode, range, value);
}
bool Unit502::ReadAsyncChannels(int count, unsigned *ch, int *mode, int *range, double *value)
{
	return Unit502N::l502.ReadAsyncChannels(count, ch, mode, range, value);
}

bool Unit502::BitOut(unsigned ch, bool value)
{
	return Unit502N::l502.BitOut(ch, value);
}

bool Unit502::BitIn(unsigned &value)
{
	return Unit502N::l502.BitIn(value);
}


