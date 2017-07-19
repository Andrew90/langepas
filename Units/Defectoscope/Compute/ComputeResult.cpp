#include "stdafx.h"
#include "ComputeResult.h"
#include "App/App.h"
#include "App/AppBase.h"
#include "DataItem\DataItem.h"
#include "MessageText\StatusMessages.h"

namespace
{
	struct CutingZone{int cut0, cut1;};
}
void CuttingZones()
{
	int minimumLengthPipe = Singleton<MinimumLengthPipeTable>::Instance().items.get<MinimumLengthPipe>().value;
	ResultData &resultData = Singleton<ResultData>::Instance();
	char (&status)[App::count_zones] = resultData.status;
	CutingZone cutingZone[10] = {};
	int current = 0;
	int currentStop = 0;
	bool start = true;
	for(int j = 0; j <  resultData.currentOffsetZones; ++j)
	{
		if(IsDefect(status[j]))
		{
			if((j - 1) - currentStop > minimumLengthPipe)
			{
				cutingZone[current].cut1 = j - 1;
				++current;

			}
			for(;j <  resultData.currentOffsetZones; ++j)
			{
				if(!IsDefect(status[j]))
				{
					currentStop = j;
					cutingZone[current].cut0 = j;
					break;
				}
			}
		}
		else
		{
			cutingZone[current].cut1 = j;
		}
	}
	int len = cutingZone[0].cut1 - cutingZone[0].cut0;
	int offs = 0;
	for(int i = 1; i <= current; ++i)
	{
		int t = cutingZone[i].cut1 - cutingZone[i].cut0;
		if(t > len)
		{
			len = t;
			offs = i;
		}
	}

	resultData.cutZone0 = cutingZone[offs].cut0;
	resultData.cutZone1 = cutingZone[offs].cut1;
}


void ComputeResult()
{
	bool isLong = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thick>>().value;
	bool isTick = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thick>>().value;
	int countCrossSensors = Singleton<ParametersTable>::Instance().items.get<CrossCountSensors>().value;
	unsigned st[1 + App::count_cross_sensors + App::count_long_sensors + App::count_Thick_sensors];
	ItemData<Cross> &crossData = Singleton<ItemData<Cross>>::Instance();
	ItemData<Long> &longData = Singleton<ItemData<Long>>::Instance();
	ItemData<Thick> &thickData = Singleton<ItemData<Thick>>::Instance();

	ResultData &resultData = Singleton<ResultData>::Instance();

	for(int i = 0; i < App::count_zones; ++i)
	{
		int last = 0;
		for(int j = 0; j < countCrossSensors; ++j)
		{
			st[last++] = (i < crossData.currentOffsetZones) ? crossData.status[j][i]: StatusId<Clr<Undefined>>();
		}

		if(isTick)
		{
			st[last++] = (i < thickData.currentOffsetZones) ? thickData.status[i]: StatusId<Clr<Undefined>>();
		}

		if(isLong)
		{
			for(int j = 0; j < App::count_long_sensors; ++j)
			{
				st[last++] = (i < thickData.currentOffsetZones) ? longData.status[j][i]: StatusId<Clr<Undefined>>();
			}
		}

		st[last] = -1;

		resultData.status[i] = ResultMessageId(st);
	}

	int len = crossData.currentOffsetZones;
	if(isTick) if(len < thickData.currentOffsetZones) len = thickData.currentOffsetZones;
	if(isLong) if(len < longData.currentOffsetZones) len = longData.currentOffsetZones;
	resultData.currentOffsetZones = len;
}

