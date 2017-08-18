#include "stdafx.h"
#include "ComputeResult.h"
#include "App/App.h"
#include "App/AppBase.h"
#include "DataItem\DataItem.h"
#include "MessageText\StatusMessages.h"
#include "lir\SubLir.h"
#include "Compute\Compute.hpp"
#include "window_tool\EmptyWindow.h"
#include "Windows\MainWindow.h"
#include "Solid\Dates\ComputeSolid.h"

namespace
{
	struct CutingZone{int cut0, cut1;};
}
void CuttingZones()
{
	int minimumLengthPipe = 1;//Singleton<MinimumLengthPipeTable>::Instance().items.get<MinimumLengthPipe>().value;
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
			int t = (j - 1) - currentStop;
			if(t > minimumLengthPipe)
			{
				cutingZone[current].cut1 = j - 1;
				minimumLengthPipe = t;
				++current;

			}
			for(;j <  resultData.currentOffsetZones; ++j)
			{
				if(!IsDefect(status[j]))
				{
					currentStop = j;
					cutingZone[current].cut0 = 1 + j;
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
	resultData.cutZone1 = 1 + cutingZone[offs].cut1;

	if(1 == resultData.cutZone0)  resultData.cutZone0 = 0;
	if(resultData.currentOffsetZones - 1 <= resultData.cutZone1)  resultData.cutZone1 = 0;
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

    SubLir &lir = Singleton<SubLir>::Instance();

	//������ ������ ���� �����
	Module<Cross> &moduleCross = lir.moduleItems.get<Module<Cross>>();
	moduleCross.Stop();
	
	int len = moduleCross.zonesOffs;
	
	Module<Long> &moduleLong = lir.moduleItems.get<Module<Long>>();
	if(isLong)
	{
		moduleLong.Stop();
		if(moduleLong.zonesOffs < len) len = moduleLong.zonesOffs;
	}

	longData.currentOffsetZones = lir.moduleItems.get<Module<Long>>().zonesOffs;
	crossData.currentOffsetZones = lir.moduleItems.get<Module<Cross>>().zonesOffs;

	if(isTick) if(thickData.currentOffsetZones < len) len = thickData.currentOffsetZones;

	moduleCross.zonesOffs = len;
	moduleLong.zonesOffs = len;
	thickData.currentOffsetZones = len;

	crossData.currentOffsetZones = len;
	thickData.currentOffsetZones = len;
	resultData.currentOffsetZones = len;

	ComputeUnit<Cross>::Zones(len);
	ComputeUnit<Cross>().DeathZonesEnd(len);
	if(isLong)
	{
		ComputeUnit<Long>::Zones(len);
		ComputeUnit<Long>().DeathZonesEnd(len);
	}

	for(int i = 0; i < len; ++i)
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

	double res = 0;
	wchar_t *group = NULL;
	unsigned color = 0;

	CuttingZones(); /// ���� ����

	ComputeSolid::Recalculation(res, group, color);
	wchar_t txt[1024];
	wchar_t *s = txt;
	wsprintf(s, L"<ff>\"������ ���������\"<%x>%s ", color & 0xFFFFFF, group);
	if(0 != resultData.cutZone0)
	{
		s += wcslen(s);
		wsprintf(s, L"<ff>\"���� ���� 1\"<ff0000>%d ", resultData.cutZone0); 
	}
	if(0 != resultData.cutZone1)
	{
		s += wcslen(s);
		wsprintf(s, L"<ff>\"���� ���� 2\"<ff0000>%d ", resultData.cutZone1); 
	}
	app.mainWindow.topLabelViewer.SetMessage(txt);
}

void Recalculation()
{
	SubLir &lir = Singleton<SubLir>::Instance();

	ComputeUnit<Cross>::Clear();
	ComputeUnit<Cross>::Zones(lir.moduleItems.get<Module<Cross>>().zonesOffs);
	ComputeUnit<Cross>::DeathZonesBegin();

	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long>>().value)
	{
		ComputeUnit<Long>::Clear();
		ComputeUnit<Long>::Zones(lir.moduleItems.get<Module<Long>>().zonesOffs);
		ComputeUnit<Long>::DeathZonesBegin();
	}

	ComputeResult();

	RepaintWindow(app.mainWindow.hWnd);
}

