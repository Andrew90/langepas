#pragma once
#include "Compute\ComputeZone.h"
template<class T>class ComputeUnit
{
	static unsigned lastZone;
public:
	void Clear()
	{
		lastZone = 0;
	}
	bool Zones(unsigned currentZone)
	{
		
		if(lastZone != currentZone)
		{
			for(unsigned i = lastZone; i < currentZone; ++i)
			{
				for(unsigned j = 0; j < ItemData<T>::count_sensors; ++j)
				{
					 ComputeZone<T>()(i, j);
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
			ComputeZoneBegin<T>()(j);
		}
	}
	void DeathZonesEnd(unsigned currentZone)
	{
		for(unsigned j = 0; j < ItemData<T>::count_sensors; ++j)
		{
			//ComputeZone<T>()(j);
			ComputeZoneEnd<T>()(currentZone, j);
		}
	}
} ;

template<class T>unsigned ComputeUnit<T>::lastZone = 0;