#pragma once
#include "App\App.h"
template<class T>struct Singleton;
class SolidData
{
	friend Singleton<SolidData>;
	SolidData();
	SolidData &operator=(const SolidData &);
public:
	static const int MAX_ZONES_COUNT = App::count_frames; 
	int &start;
	int &stop; 
	double (&referenceNoFiltre)[MAX_ZONES_COUNT];
	double (&signalNoFiltre)[MAX_ZONES_COUNT];
	double reference[MAX_ZONES_COUNT];
	double signal[MAX_ZONES_COUNT];
public:	
	//void Clear();
	//void SetData(double *data, int count);
};

