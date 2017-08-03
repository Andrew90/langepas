#include "stdafx.h"
#include "SolidData.h"
#include <math.h>
#include "ChartData.h"
#include "DataItem\DataItem.h"

#include "tools_debug/DebugMess.h"

SolidData::SolidData()
	: start(Singleton<ItemData<Solid>>::Instance().start)
	, stop(Singleton<ItemData<Solid>>::Instance().stop)
	, referenceNoFiltre(Singleton<ItemData<Solid>>::Instance().ascan[0])
	, signalNoFiltre(Singleton<ItemData<Solid>>::Instance().ascan[1])
{}

//void SolidData::Clear()
//{
//	currentOffset = 0;
//}
//
//void SolidData::SetData(double *data, int count)
//{
//	if(count <= 0) return;
//	if(currentOffset + count / 2 > MAX_ZONES_COUNT - 10) return;
//	for(int i = 0; i < count; i += 2)
//	{
//		referenceNoFiltre[currentOffset] = reference[currentOffset] = data[i];
//		signalNoFiltre[currentOffset] = signal[currentOffset] = data[i + 1];
//		++currentOffset;
//	}
//}

