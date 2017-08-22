#pragma once
#include "Compute\ComputeZone.h"
template<class T, class Data>class ComputeUnitX
{
	Data &data;
	unsigned lastZone;
public:
	ComputeUnitX(Data &data): data(data){}
	void Clear()
	{
		lastZone = 1;
	}
	bool Zones(unsigned currentZone)
	{
		
		if(lastZone != currentZone)
		{
			for(unsigned i = lastZone; i < currentZone; ++i)
			{
				for(unsigned j = 0; j < ItemData<T>::count_sensors; ++j)
				{
					 ComputeZone<T>()(i, j, data.status[j], data.buffer[j]);
				}
			}
			lastZone = currentZone;
			return true;
		}
		return false;
	}
	void DeathZonesBegin()
	{
		for(unsigned j = 0; j < ItemData<T>::count_sensors; ++j)
		{
			ComputeZoneBegin<T>()(j, data.status[j], data.buffer[j]);
		}
	}
	void DeathZonesEnd(unsigned currentZone)
	{
		for(unsigned j = 0; j < ItemData<T>::count_sensors; ++j)
		{
			ComputeZoneEnd<T>()(currentZone, j, data.status[j], data.buffer[j]);
		}
	}
} ;

