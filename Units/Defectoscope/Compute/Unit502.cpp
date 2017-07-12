#include "stdafx.h"
#include "Unit502.h"
#include "L502\L502.h"
#include "DataItem\DataItem.h"
#include "tools_debug\DebugMess.h"

#include "PerformanceCounter\PerformanceCounter.h"
#include "App/App.h"

namespace Unit502N
{
	L502 l502;
	ItemData<Cross> &crossData = Singleton<ItemData<Cross>>::Instance();
	ItemData<Long> &longData = Singleton<ItemData<Long>>::Instance();
	ItemData<Solid> &solidData = Singleton<ItemData<Solid>>::Instance();
	TimeLir &timeFrames = Singleton<TimeLir>::Instance();
}

void Unit502::Read()
{
	double data[L502::buffer_length];
	unsigned startChannel;
	unsigned count = dimention_of(data);

	if(0 < Unit502N::l502.Read(startChannel, data, count))
	{
		bool crossBool = false;
		bool longBool = false;
		unsigned currentTime = Performance::Counter();
		for(int i = 0; i < (int)count; ++i)
		{
			int sens = startChannel + i;
			sens %= 18;
			if(sens < 11)
			{
				Unit502N::crossData.ascan[sens][Unit502N::crossData.currentOffset] = data[i];
			}
			else if(sens == 11)
			{
				Unit502N::crossData.ascan[sens][Unit502N::crossData.currentOffset] = data[i];
				if(crossOn && Unit502N::crossData.currentOffset < App::count_frames)
				{
					++Unit502N::crossData.currentOffset;
					crossBool = true;
				}
			}
			else if(sens < 15)
			{
				Unit502N::longData.ascan[sens - 12][Unit502N::longData.currentOffset] = data[i];
			}
			else if(sens == 15)
			{
				Unit502N::longData.ascan[sens - 12][Unit502N::longData.currentOffset] = data[i];
				if(longOn && Unit502N::longData.currentOffset < App::count_frames)
				{
					++Unit502N::longData.currentOffset;
					longBool = true;
				}
			}
			else if(sens < 17)
			{
				Unit502N::solidData.ascan[sens - 16][Unit502N::solidData.currentOffset] = data[i];				
			}
			else if(sens == 17)
			{
				Unit502N::solidData.ascan[sens - 16][Unit502N::solidData.currentOffset] = data[i];
				if(solidOn  && Unit502N::solidData.currentOffset < App::count_frames)
				{
						++Unit502N::solidData.currentOffset;
				}
			}
		}

		if(crossBool)
		{
			Unit502N::crossData.time[Unit502N::crossData.currentTimeOffset] = currentTime;
			Unit502N::crossData.frame[Unit502N::crossData.currentTimeOffset] = Unit502N::crossData.currentOffset;
			if(Unit502N::crossData.currentTimeOffset < dimention_of(Unit502N::crossData.time))
			{
				++Unit502N::crossData.currentTimeOffset;
			}
			else
			{
				dprint("err crossData.currentTimeOffset\n");
			}
		}

		if(longBool)
		{
			Unit502N::longData.time[Unit502N::longData.currentTimeOffset] = currentTime;
			Unit502N::longData.frame[Unit502N::longData.currentTimeOffset] = Unit502N::longData.currentOffset;
			if(Unit502N::longData.currentTimeOffset < dimention_of(Unit502N::longData.time))
			{
				++Unit502N::longData.currentTimeOffset;
			}
			else
			{
				dprint("err longData.currentTimeOffset\n");
			}
		}

		Unit502N::timeFrames.time[Unit502N::timeFrames.currentTimeOffset] = currentTime;
		//Unit502N::timeFrames.lir[Unit502N::timeFrames.currentTimeOffset] = Lir::Do();
		if(Unit502N::timeFrames.currentTimeOffset < dimention_of(Unit502N::timeFrames.lir))
		{
			++Unit502N::timeFrames.currentTimeOffset;
		}
		else
		{
			dprint("err timeFrames.currentTimeOffset\n");
		}
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
	//Unit502N::crossData.currentOffset = 0;
	//Unit502N::crossData.currentOffset = 0;
	//
	//Unit502N::longData.currentOffset = 0;
	Unit502N::longData.currentTimeOffset = 0;
	//
	//Unit502N::crossData.currentOffset = 0;
	Unit502N::crossData.currentTimeOffset = 0;

	Unit502N::solidData.currentOffset = 0;
	//
	//
	//Unit502N::timeFrames.currentTimeOffset = 0;
	solidOn = crossOn = longOn = false;
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


