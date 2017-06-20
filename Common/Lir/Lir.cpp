#include "stdafx.h"
#include "App/Config.h"
#include "Lir.h"
#include <MMSystem.h>
#include "App/AppBase.h"
#include "inc/bdaqctrl.h"
#include "tools_debug/DebugMess.h"
#include "PerformanceCounter\PerformanceCounter.h"
using namespace Automation::BDaq;
#if 0
Lir::Lir()
	: zone0(Singleton<LirParamTable>::Instance().items.get<TickPerZoneLir0>().value)
	, zone1(Singleton<LirParamTable>::Instance().items.get<TickPerZoneLir1>().value)
{}

#define cnt0 ((UdCounterCtrl *)cnt0_)
#define cnt1 ((UdCounterCtrl *)cnt1_)

bool Lir::Init(int id)
{
#ifndef DEBUG_ITEMS
	cnt0_  = (void *)AdxUdCounterCtrlCreate();
	cnt1_  = (void *)AdxUdCounterCtrlCreate();

	DeviceInformation devInfo(id);
	ErrorCode ret = cnt0->setSelectedDevice(devInfo);
	if(Success == ret)ret = cnt1->setSelectedDevice(devInfo);
	if(Success == ret)ret = cnt0->setChannel(0);
	if(Success == ret)ret = cnt1->setChannel(1);
	if(Success == ret)ret = cnt0->setCountingType(PulseDirection);
	if(Success == ret)ret = cnt1->setCountingType(PulseDirection);
	if(Success == ret)ret= cnt0->setEnabled(true);
	if(Success == ret)ret= cnt1->setEnabled(true);
	return Success == ret;
#else
	return true;
#endif
}
void Lir::Clear()   
{
	cnt0->setInitialValue(0);
	cnt1->setInitialValue(0);
	ptrTick = &Lir::Tick0;
	lenZone = zone0;
}

unsigned Lir::Tick0()
{
	tmpTick = cnt1->getValue();
	return tick = cnt0->getValue();	
}
unsigned Lir::Tick1()
{
	return tick + cnt1->getValue() - tmpTick;
}

void Lir::Do()
{
	(this->*ptrTick)();
}

void Lir::SetTick1()
{
	lenZone = zone1;
	ptrTick = &Lir::Tick1;
}

#undef cnt0
#undef cnt1
#endif
namespace Lir
{
	const double &tickPerZone0 = Singleton<LirParamTable>::Instance().items.get<TickPerZoneLir0>().value;
	const double &tickPerZone1 = Singleton<LirParamTable>::Instance().items.get<TickPerZoneLir1>().value;
	UdCounterCtrl *cnt0, *cnt1;

	double tmpTick, tick;
	unsigned(*tickPtr)();

	unsigned Tick0()
	{
		tmpTick = (double)App::zone_length * cnt1->getValue() / tickPerZone1;
		return unsigned(tick = (double)App::zone_length * cnt0->getValue() / tickPerZone0);	
	};

	unsigned Tick1()
	{
		return unsigned(tick + (double)App::zone_length * cnt1->getValue() / tickPerZone1 - tmpTick);
	};

	bool Init()
	{
		int id = Singleton<LirParamTable>::Instance().items.get<DescriptorLir>().value;
#ifndef DEBUG_ITEMS
		cnt0  = AdxUdCounterCtrlCreate();
		cnt1  = AdxUdCounterCtrlCreate();

		DeviceInformation devInfo(id);
		ErrorCode ret = cnt0->setSelectedDevice(devInfo);
		if(Success == ret)ret = cnt1->setSelectedDevice(devInfo);
		if(Success == ret)ret = cnt0->setChannel(0);
		if(Success == ret)ret = cnt1->setChannel(1);
		if(Success == ret)ret = cnt0->setCountingType(PulseDirection);
		if(Success == ret)ret = cnt1->setCountingType(PulseDirection);
		if(Success == ret)ret= cnt0->setEnabled(true);
		if(Success == ret)ret= cnt1->setEnabled(true);
		return Success == ret;
#else
		return true;
#endif
	}
#ifdef DEBUG_ITEMS
	unsigned x = 0;
	unsigned Do()
	{
		return unsigned(double(1.0 * (Performance::Counter() - x) / 5));
	}

	void Clear()
	{
		x = Performance::Counter();
	}

	void ChangeLir()
	{
	}
#else
	unsigned Do()
	{
		return (*tickPtr)();
	}

	void Clear()
	{
		cnt0->setInitialValue(0);
		cnt1->setInitialValue(0);
		tickPtr = Tick0;
	}

	void ChangeLir()
	{
		tickPtr = Tick1;
	}
#endif
}