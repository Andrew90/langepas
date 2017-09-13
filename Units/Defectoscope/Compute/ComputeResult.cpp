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
	int minimumLengthPipe = 1;
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

	resultData.resultCommon = ResultNorma;
	
	//if(tubeLength < Singleton<MinimumLengthPipeTable>::Instance().items.get<MinimumLengthPipe>().value)
	//{
	//	resultData.resultCommon = ResultBrak;
	//}
	//else
	{
		int start = resultData.cutZone0;
		int stop = 1 + resultData.cutZone1;
		if(0 == resultData.cutZone0 && 0 == resultData.cutZone1)
		{
			start = 0;
			stop = resultData.currentOffsetZones;
		}
		else if(0 == resultData.cutZone0)
		{
		   start = 0;
		}
		else if(0 == resultData.cutZone1)
		{
			stop = resultData.currentOffsetZones - 1;
		}

		for(int i = start; i < stop; ++i)
		{
			unsigned res = IsKlass((unsigned)resultData.status[i]);
			if(res == ResultKlass3) 
			{
				resultData.resultCommon = res;
				break;
			}
			if(res == ResultKlass2) 
			{
				resultData.resultCommon = res;
			}
		}

		int tubeLength =  stop - start;
		if(tubeLength < Singleton<MinimumLengthPipeTable>::Instance().items.get<MinimumLengthPipe>().value)
		{
			resultData.resultCommon = ResultBrak;
			resultData.cutZone0 = 0;
		    resultData.cutZone1 = 0;
		}
	}
}


void ComputeResult()
{
	bool isLong = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long>>().value;
	bool isTick = Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thick>>().value;
	int countCrossSensors = Singleton<ParametersTable>::Instance().items.get<CrossCountSensors>().value;
	unsigned st[1 + App::count_cross_sensors + App::count_long_sensors + App::count_Thick_sensors];
	ItemData<Cross> &crossData = Singleton<ItemData<Cross>>::Instance();
	ItemData<Long> &longData = Singleton<ItemData<Long>>::Instance();
	ItemData<Thick> &thickData = Singleton<ItemData<Thick>>::Instance();

	ResultData &resultData = Singleton<ResultData>::Instance();

    SubLir &lir = Singleton<SubLir>::Instance();

	//Расчёт мёртвой зоны конец
	Module<Cross> &moduleCross = lir.moduleItems.get<Module<Cross>>();
	
	int len = moduleCross.zonesOffs;
	
	Module<Long> &moduleLong = lir.moduleItems.get<Module<Long>>();
	
	
	////longData.currentOffsetZones = lir.moduleItems.get<Module<Long>>().zonesOffs;
	///crossData.currentOffsetZones = lir.moduleItems.get<Module<Cross>>().zonesOffs;

	dprint("long zones %d\n", lir.moduleItems.get<Module<Long>>().zonesOffs);
	dprint("Cross zones %d\n", lir.moduleItems.get<Module<Cross>>().zonesOffs);

	
#if 0
	crossData.currentOffsetZones = len;

	longData.currentOffsetZones = isLong ? len : 0;
	thickData.currentOffsetZones = isTick ? len : 0;
#else
	if(!isLong)longData.currentOffsetZones = 0;
	if(!isTick)thickData.currentOffsetZones = 0;
#endif
	

	ComputeUnitX<Long, ItemData<Long>> longX(Singleton<ItemData<Long>>::Instance());
	if(isLong)
	{
		
		longX.lastZone = 0;
	//	longData.currentOffsetZones += 5;
		longX.Zones(longData.currentOffsetZones );
		//moduleLong.Stop();
		longX.DeathZonesEnd(longData.currentOffsetZones);
		longX.DeathZonesBegin();
	}

	ComputeUnitX<Cross, ItemData<Cross>> crossX(Singleton<ItemData<Cross>>::Instance()); 
	//crossData.currentOffsetZones += 5;
	crossX.lastZone = 0;
	crossX.Zones(crossData.currentOffsetZones);	
	//moduleCross.Stop();
	crossX.DeathZonesEnd(crossData.currentOffsetZones);
	crossX.DeathZonesBegin();

	len = crossData.currentOffsetZones; 
	if(isLong)if(crossData.currentOffsetZones < len) len = crossData.currentOffsetZones;
	if(isTick) if(thickData.currentOffsetZones < len) len = thickData.currentOffsetZones;
	resultData.currentOffsetZones = len;
	
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
				st[last++] = (i < longData.currentOffsetZones) ? longData.status[j][i]: StatusId<Clr<Undefined>>();
			}
		}

		st[last] = -1;

		resultData.status[i] = ResultMessageId(st);
	}

	double res = 0;
	wchar_t *group = NULL;
	unsigned color = 0;

	CuttingZones(); /// зоны реза

	ComputeSolid::Recalculation(res, group, color);
	resultData.solidGroup = *group;
	wchar_t txt[1024];
	txt[0] = 0;
	wchar_t *s = txt;
	if(0 != *group)wsprintf(s, L"<ff>Группа прочности<%x>%s ", color & 0xFFFFFF, group);
	if(0 != resultData.resultCommon)
	{
		if(withCutting)
		{
			if(0 != resultData.cutZone0)
			{
				s += wcslen(s);
				wsprintf(s, L"<ff>\"Зона реза 1\"<ff0000>%d ", resultData.cutZone0); 
			}
			if(0 != resultData.cutZone1)
			{
				s += wcslen(s);
				wsprintf(s, L"<ff>\"Зона реза 2\"<ff0000>%d ", resultData.cutZone1); 
			}
		}
		else
		{
			if(0 != resultData.cutZone0 || 0 != resultData.cutZone1)
			{
				resultData.resultCommon = 0;
				resultData.cutZone0 = 0;
				resultData.cutZone1 = 0;
			}
		}
	}
	s += wcslen(s);
	switch(resultData.resultCommon)
	{
	case 0:	wsprintf(s, L"<ff0000> Брак "); break;
	case 1: wsprintf(s, L"<ff00> Норма ");break;
	case 2: wsprintf(s, L"<ffff00> Класс 2 ");break;
	case 3: wsprintf(s, L"<ff00ff> Класс 3 ");break;
	}
	app.mainWindow.topLabelViewer.SetMessage(txt);
}

void Recalculation()
{
	SubLir &lir = Singleton<SubLir>::Instance();

	ComputeUnitX<Cross, ItemData<Cross>> crossX(Singleton<ItemData<Cross>>::Instance());
	crossX.Clear();
	crossX.Zones(lir.moduleItems.get<Module<Cross>>().zonesOffs);
	//crossX.DeathZonesBegin();

	ComputeUnitX<Long, ItemData<Long>> longX(Singleton<ItemData<Long>>::Instance());
	longX.Clear();
	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long>>().value)
	{
		longX.Zones(lir.moduleItems.get<Module<Long>>().zonesOffs);
		//longX.DeathZonesBegin();
	}

	ComputeResult();

	RepaintWindow(app.mainWindow.hWnd);
}

bool withCutting = true;






