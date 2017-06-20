#pragma once
#include "App/App.h"

template<class T>struct ComputeSignal
{
	void operator()(T &owner, int zone, int sensor, int offsetInZone)
	{
		zprint(" zone %d sensor %d offsetInZone %d\n", zone, sensor, offsetInZone);
	}
};

class SignalViewer;
class ACFViewer;
class FFTViewer;

template<>struct ComputeSignal<SignalViewer>
{
	void operator()(SignalViewer &owner, int zone, int sensor, int offsetInZone);
};

template<>struct ComputeSignal<ACFViewer>
{
	void operator()(ACFViewer &owner, int zone, int sensor, int offsetInZone);
};

template<>struct ComputeSignal<FFTViewer>
{
	void operator()(FFTViewer &owner, int zone, int sensor, int offsetInZone);
};

