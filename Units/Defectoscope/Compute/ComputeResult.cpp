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

	//������ ������ ���� �����
	Module<Cross> &moduleCross = lir.moduleItems.get<Module<Cross>>();
	
	int len = moduleCross.zonesOffs;
	
	Module<Long> &moduleLong = lir.moduleItems.get<Module<Long>>();
	if(isLong)
	{
		if(moduleLong.zonesOffs < len) len = moduleLong.zonesOffs;
	}

	longData.currentOffsetZones = lir.moduleItems.get<Module<Long>>().zonesOffs;
	crossData.currentOffsetZones = lir.moduleItems.get<Module<Cross>>().zonesOffs;

	if(isTick) if(thickData.currentOffsetZones < len) len = thickData.currentOffsetZones;
#if 0
	crossData.currentOffsetZones = len;

	longData.currentOffsetZones = isLong ? len : 0;
	thickData.currentOffsetZones = isTick ? len : 0;
#else
	if(!isLong)longData.currentOffsetZones = 0;
	if(!isTick)thickData.currentOffsetZones = 0;
#endif
	resultData.currentOffsetZones = len;

	ComputeUnitX<Long, ItemData<Long>> longX(Singleton<ItemData<Long>>::Instance());
	if(isLong)
	{
		longX.lastZone = 0;
		longX.Zones(len );
		moduleLong.Stop();
		longX.DeathZonesEnd(len);
	}

	ComputeUnitX<Cross, ItemData<Cross>> crossX(Singleton<ItemData<Cross>>::Instance()); 
	crossX.lastZone = 0;
	crossX.Zones(len);	
	moduleCross.Stop();
	crossX.DeathZonesEnd(len);
	
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

	CuttingZones(); /// ���� ����

	ComputeSolid::Recalculation(res, group, color);
	wchar_t txt[1024];
	txt[0] = 0;
	wchar_t *s = txt;
	if(0 != *group)wsprintf(s, L"<ff>\"������ ���������\"<%x>%s ", color & 0xFFFFFF, group);
	if(0 != resultData.resultCommon)
	{
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
	}
	s += wcslen(s);
	switch(resultData.resultCommon)
	{
	case 0:	wsprintf(s, L"<ff0000> ���� "); break;
	case 1: wsprintf(s, L"<ff00> ����� ");break;
	case 2: wsprintf(s, L"<ffff00> ����� 2 ");break;
	case 3: wsprintf(s, L"<ff00ff> ����� 3 ");break;
	}
	app.mainWindow.topLabelViewer.SetMessage(txt);
}

void Recalculation()
{
	SubLir &lir = Singleton<SubLir>::Instance();

	ComputeUnitX<Cross, ItemData<Cross>> crossX(Singleton<ItemData<Cross>>::Instance());
	crossX.Clear();
	crossX.Zones(lir.moduleItems.get<Module<Cross>>().zonesOffs);
	crossX.DeathZonesBegin();

	ComputeUnitX<Long, ItemData<Long>> longX(Singleton<ItemData<Long>>::Instance());
	longX.Clear();
	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long>>().value)
	{
		longX.Zones(lir.moduleItems.get<Module<Long>>().zonesOffs);
		longX.DeathZonesBegin();
	}

	ComputeResult();

	RepaintWindow(app.mainWindow.hWnd);
}

void ToZones()
{
	SubLir &lir = Singleton<SubLir>::Instance();

	SQ<on<Cross,1>> &sqOnCross1 = lir.sqItems.get<SQ<on<Cross,1>>>();
	SQ<on<Cross,2>> &sqOnCross2 = lir.sqItems.get<SQ<on<Cross,2>>>();
					   
	SQ<on<Magn, 1>> &sqOnMagn1 = lir.sqItems.get<SQ<on<Magn,1>>>();
	SQ<on<Magn, 2>> &sqOnMagn2 = lir.sqItems.get<SQ<on<Magn,2>>>();

	SQ<off<Cross,1>> &sqOffCross1 = lir.sqItems.get<SQ<off<Cross,1>>>();
	SQ<off<Cross,2>> &sqOffCross2 = lir.sqItems.get<SQ<off<Cross,2>>>();
			  
	SQ<off<Magn, 1>> &sqOffMagn1 = lir.sqItems.get<SQ<off<Magn,1>>>();
	SQ<off<Magn, 2>> &sqOffMagn2 = lir.sqItems.get<SQ<off<Magn,2>>>();


	
}




