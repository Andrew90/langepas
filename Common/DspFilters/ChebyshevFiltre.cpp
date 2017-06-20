#include "stdafx.h"
#include "ChebyshevFiltre.h"

void ChebyshevFiltre::Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb)
{
	Dsp::Params params;
	params[0] = sample_rate;
	params[1] = order;
	params[2] = cutoffFrequency;
	params[3] = stopBandDb;
	setParams (params);
}
double ChebyshevFiltre::OneSample(double value)
{
	return m_state.m_state[0].process(value, FilterDesignBase<type_filtre>::m_design);
}
