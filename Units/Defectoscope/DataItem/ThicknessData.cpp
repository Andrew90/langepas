#include "stdafx.h"
#include "ThicknessData.h"
#include "Lan/Lan.h"
#include <math.h>
#include "App/config.h"
#include "tools_debug/DebugMess.h"
#include "DataItem/DataItem.h"


char ThickData::buffer[ThickData::buffSize];
//----------------------------------------------------------------------------------------------
ThickData::ThickData()
	: nominalThickness(Singleton<ThresholdsTable>::Instance().items.get<BorderNominal<Thick>>().value)
	, bufSize(Lan::bufSize)
	, packetSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, numberPackets(Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value)
{ }
//---------------------------------------------------------------------------------------------
namespace
{
   template<class O, class P>struct __set_coeff__;
   template<int N, class P>struct __set_coeff__<CoefficientA<N>, P>
   {
	   void operator()(CoefficientA<N> &o, P &p)
	   {
		   p->coefficientA[N] = o.value;
	   }
   };
   template<int N, class P>struct __set_coeff__<CoefficientB<N>, P>
   {
	   void operator()(CoefficientB<N> &o, P &p)
	   {
		   p->coefficientB[N] = o.value;
	   }
   };

   template<int N, class P>struct __set_coeff__<MinEnergy<N>, P>
   {
	   void operator()(MinEnergy<N> &o, P &p)
	   {
		   p->minEnergy[N] = o.value;
	   }
   };
   template<int N, class P>struct __set_coeff__<MaxEnergy<N>, P>
   {
	   void operator()(MaxEnergy<N> &o, P &p)
	   {
		   p->maxEnergy[N] = o.value;
	   }
   };
   template<int N, class P>struct __set_coeff__<Peak<N>, P>
   {
	   void operator()(Peak<N> &o, P &p)
	   {
		   p->peak[N] = o.value;
	   }
   };

   template<int N, class P>struct __set_coeff__<LeftBorderACFCutOff<N>, P>
   {
	   void operator()(LeftBorderACFCutOff<N> &o, P &p)
	   {
		   p->acfBorderLeft[N] = o.value;
	   }
   };
   template<int N, class P>struct __set_coeff__<RightBorderACFCutOff<N>, P>
   {
	   void operator()(RightBorderACFCutOff<N> &o, P &p)
	   {
		   p->acfBorderRight[N] = o.value;
	   }
   };
}
void ThickData::Clear()
{
	current__ = 0;
	startOffset = 0;
	Singleton<ItemData<Thick>>::Instance().currentOffset = 0;
	InitParam();
}
//---------------------------------------------------------------------------------------------
void ThickData::InitParam()
{
	TL::foreach<CoefficientParametersTable::items_list, __set_coeff__>()(Singleton<CoefficientParametersTable>::Instance().items, this);
	TL::foreach<SignalParametersTable::items_list, __set_coeff__>()(Singleton<SignalParametersTable>::Instance().items, this);
	TL::foreach<BorderACFCutOffTable::items_list, __set_coeff__>()(Singleton<BorderACFCutOffTable>::Instance().items, this);

	double minT = Singleton<MinMaxThresholdsTable>::Instance().items.get<MinimumThicknessPipeWall>().value;
	double maxT = Singleton<MinMaxThresholdsTable>::Instance().items.get<MaximumThicknessPipeWall>().value;

	for(int i = 0; i < App::count_Thick_sensors; ++i)
	{
		 minOffset[i] = int(double(minT - coefficientB[i])/ coefficientA[i]);
		 maxOffset[i] = int(double(maxT - coefficientB[i])/ coefficientA[i]);
	}

	fft.Init(packetSize);
}
//---------------------------------------------------------------------------------------------
char *ThickData::CurrentData()
{
	return &buffer[current__ * bufSize];
}
//---------------------------------------------------------------------------------------------
bool ThickData::IncrementOffset()
{
	if(((2 + current__) * bufSize) < buffSize)
	{
		++current__;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------
char *ThickData::SensorData(int sensor, int offset)
{
	return &buffer[((offset) * App::count_Thick_sensors + sensor) * packetSize];
}
//-------------------------------------------------------------------------------------
int ThickData::GetCurrentOffset() const
{
	return current__ * numberPackets;
}
//-------------------------------------------------------------------------------------
void ThickData::SetCurrentOffset(int d)
{
	int bufSize = numberPackets * packetSize * App::count_Thick_sensors;
	current__ = d / bufSize;
}
//--------------------------------------------------------------------------------------
int ThickData::Filling()
{
  return current__ * numberPackets * packetSize * App::count_Thick_sensors;
}
//----------------------------------------------------------------------------------------
void ThickData::Init()
{
	//fft.Init(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value);
	Clear();
}
//----------------------------------------------------------------------------------------
void ThickData::Compute()
{
	double (&ascan)[App::count_Thick_sensors][App::count_frames] = Singleton<ItemData<Thick>>::Instance().ascan;
	char (&ascanStatus)[App::count_Thick_sensors][App::count_frames] = Singleton<ItemData<Thick>>::Instance().ascanStatus;
	int current = current__;
	int start = current * numberPackets;
	for(int i = startOffset; i < start; ++i)
	{
		int offs = i * App::count_Thick_sensors * packetSize;
		CalculationOneFrame(0, &buffer[offs], ascan[0][i], ascanStatus[0][i]);
		offs += packetSize;
		CalculationOneFrame(1, &buffer[offs], ascan[1][i], ascanStatus[1][i]);
		offs += packetSize;
		CalculationOneFrame(2, &buffer[offs], ascan[2][i], ascanStatus[2][i]);
		++Singleton<ItemData<Thick>>::Instance().currentOffset;
	}
	startOffset = start;
}
//-------------------------------------------------------------------------------------------
void ThickData::CalculationOneFrame(int sensorIndex, char *sensorData, double &result, char &status)
{
	status = StatusId<Clr<Undefined>>();
	result = nominalThickness;
	double data[1024] = {};
	double t = 0;
	int j = 0;
	for(; j < packetSize; ++j)
	{
		data[j] = sensorData[j];		
	}
///------------------------------------------
	fft.Direct(data);
	fft.Spectrum(data);	
//--------------------------------------отсечение в частотной области
	ZeroMemory(data, sizeof(double) * acfBorderLeft[sensorIndex]);
	ZeroMemory(&data[acfBorderRight[sensorIndex]], sizeof(double) * (fft.bufferSize - acfBorderRight[sensorIndex]));
//---------------------------------------------------------------------------------------
	fft.Direct(data);
	fft.Spectrum(data);
//------------------------------------------------------------------------------
	if(data[0] < minEnergy[sensorIndex]) 
	{
		status = StatusId<Clr<LessMinimumEnergy>>();//отбраковка минимальная энергия
		return;
	}

	if(data[0] > maxEnergy[sensorIndex]) 
	{
		status = StatusId<Clr<GreaterMaximumEnergy>>();//отбраковка максимальная энергия
		return;
	}
//-----------------------------------поиск пика	
	int maxOffs = maxOffset[sensorIndex];
	int z = minOffset[sensorIndex];

	double minVal = data[z];
	double maxVal = minVal;
	double val = 0;
	int offsMin = z;
	int offsMax = z;
	int offsVal = z;

	for(; z < maxOffs; ++z)
	{
		if(minVal > data[z]) 
		{
			minVal = data[z];
			offsMin = z;
			if(offsMin > offsMax) 
			{
				minVal = maxVal = data[z];
				continue;
			}
		}
		if(maxVal < data[z])
		{
			maxVal = data[z];
			offsMax = z;
		}
		double t = maxVal - minVal;
		if(val < t) 
		{
			val = t;
			offsVal = z;
		}
	}
#pragma message("Отладить fft")
	if(val < data[0] * peak[sensorIndex])
	{
		 status = StatusId<Clr<ValueLessThreshold>>();//TL::IndexOf<message_list, Mess<ValueLessThreshold>>::value;//меньше допустимого порога
		 return;
	}
	result = offsVal;
	result *= coefficientA[sensorIndex];
	result += coefficientB[sensorIndex];

	status = StatusId<Clr<Nominal>>();
}
