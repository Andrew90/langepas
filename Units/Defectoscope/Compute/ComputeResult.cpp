#include "stdafx.h"
#include "ComputeResult.h"
#include "App/App.h"
#include "App/AppBase.h"
#include "DataItem\DataItem.h"
#include "MessageText\StatusMessages.h"

namespace
{
	struct CutingZone{int cut0, cut1;};
void CuttingZones()
{
	int minimumLengthPipe = Singleton<MinimumLengthPipeTable>::Instance().items.get<MinimumLengthPipe>().value;
	ResultData &resultData = Singleton<ResultData>::Instance();
	char (&status)[App::count_zones] = resultData.status;
	CutingZone cutingZone[10] = {};
	int current = 0;
	for(int i =0; i < dimention_of(cutingZone); ++i)
	{
		for(int j = 0; j <  dimention_of(status); ++j)
		{
		}
	}
}
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

