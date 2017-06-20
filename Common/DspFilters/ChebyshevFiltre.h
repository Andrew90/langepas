#pragma once
#include "Filter.h"
#include "DspFilters/ChebyshevII.h"

class ChebyshevFiltre: public Dsp::FilterDesign<Dsp::ChebyshevII::Design::LowPass<10>, 1>
{
	typedef Dsp::ChebyshevII::Design::LowPass<10> type_filtre;
public:
	//f.Setup(4000, 3, 50, 40); частота дискретизации, порядок фильтра, частота среза, затухание
	//Setup(4000, 3, 50, 40);
	void Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb);
	double OneSample(double value);
};