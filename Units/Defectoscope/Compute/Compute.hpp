#pragma once
#include "Compute\ComputeZone.h"
template<class T, class Data>class ComputeUnitX
{
	Data &data;
public:
	unsigned lastZone;
private:
	ComputeZone<T> zone;
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
					 zone.Zone(i, j, data.status[j], data.buffer[j]);
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
			zone.Begin(j, data.status[j], data.buffer[j]);
		}
	}
	void DeathZonesEnd(unsigned currentZone)
	{
		for(unsigned j = 0; j < ItemData<T>::count_sensors; ++j)
		{
			zone.End(currentZone, j, data.status[j], data.buffer[j]);
		}
	}
} ;

template<class T, class Data, class Default = ComputeZoneParamsDefault>struct ComputeUnitSensor
{
	Data &data;
	int sensor;
	ComputeZone<T, Default> zone;
public:
	ComputeUnitSensor(Data &data, int sensor): data(data), sensor(sensor){}
	void operator()()
	{
		for(int i = 0; i < data.currentOffsetZones; ++i)
		{
			 zone.Zone(i, sensor, data.status[sensor], data.buffer[sensor]);
		}
		zone.Begin(sensor, data.status[sensor], data.buffer[sensor]);
		zone.End(data.currentOffsetZones, sensor, data.status[sensor], data.buffer[sensor]);
	}
} ;

