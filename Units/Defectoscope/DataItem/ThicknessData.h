#pragma once
#include "App/AppBase.h"
#include "App/App.h"
#include "FFT/FFT.h"

class ThickData
{
	double &nominalThickness;
public:
	int current__;
	CFFT fft;
	int acfBorderLeft[App::count_Thick_sensors], acfBorderRight[App::count_Thick_sensors];
	double minEnergy[App::count_Thick_sensors], maxEnergy[App::count_Thick_sensors];
	int minOffset[App::count_Thick_sensors], maxOffset[App::count_Thick_sensors];
	double coefficientA[App::count_Thick_sensors], coefficientB[App::count_Thick_sensors];
	double peak[App::count_Thick_sensors];
public:
	static const int buffSize = 512 * 1024 * 1024;
	static const int sensorBuffSize = buffSize / 474 / App::count_Thick_sensors;
	const size_t &bufSize;
	const int &packetSize;
	static char buffer[buffSize];
	int countZones;
	int startOffset;
	const int &numberPackets;
	unsigned frameStartTime;
	ThickData();
	short *operator[](int );
	void Clear();
	void InitParam();
	char *CurrentData();
	unsigned &GetRealOffsetTime();
	bool IncrementOffset();
	char *SensorData(int sensor, int offset);
	int GetCurrentOffset() const;
	void SetCurrentOffset(int);
	int Filling();//заполнение буфера в short

	void Init();
	void Compute();
	void CalculationOneFrame(int sensorIndex, char *sensorData, double &result, char &status);
};



