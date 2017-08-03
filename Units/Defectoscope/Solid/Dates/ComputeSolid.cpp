#include "stdafx.h"
#include "Dates\ComputeSolid.h"
#include "SolidData.h"
#include "templates\templates.hpp"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "App\App.h"
#include "ColorPanel\ColorPanel.h"
#include "MainWindow\PointsWindow.h"
#include "App\AppBase.h"
#include "DspFilters\ChebyshevFiltre.h"
//#include "Dates\SolidData.h"
#include "SolidGroupAlgoritm\SolidBase.h"
#include "MainWindow/ChartLines.h"

namespace ComputeSolid
{
	SolidData &solidData = Singleton<SolidData>::Instance();
	ComputeSolidGroup &computeSolidGroup = Singleton<ComputeSolidGroup>::Instance();

	void Filtre()
	{
		//todo написать фильтр
		AnalogFilterTable::TItems &flt = Singleton<AnalogFilterTable>::Instance().items;

		if(flt.get<CutoffFrequencyOn<Voltage>>().value)
		{
			{
				ChebyshevFiltre dsp;
				dsp.Setup(
					Singleton<SolidParametersTable>::Instance().items.get<FrequencySamples>().value / 2
					, 3
					, flt.get<CutoffFrequency<Voltage>>().value
					, 40
					);	
				double (&s)[SolidData::MAX_ZONES_COUNT] = solidData.signal;
				double (&noFiltre)[SolidData::MAX_ZONES_COUNT]	 = solidData.signalNoFiltre;
				for(int i = solidData.start; i < solidData.stop; ++i)
				{
					s[i] = dsp.OneSample(noFiltre[i]);
				}
			}
			{
				ChebyshevFiltre dsp;
				dsp.Setup(
					Singleton<SolidParametersTable>::Instance().items.get<FrequencySignal>().value / 2
					, 3
					, flt.get<CutoffFrequency<Voltage>>().value
					, 40
					);	
				double (&s)[SolidData::MAX_ZONES_COUNT] = solidData.reference;
				double (&noFiltre)[SolidData::MAX_ZONES_COUNT]	 = solidData.referenceNoFiltre;
				for(int i = solidData.start; i < solidData.stop; ++i)
				{
					s[i] = dsp.OneSample(noFiltre[i]);
				}
			}
		}
		else
		{
			memmove(solidData.signal, solidData.signalNoFiltre, sizeof(solidData.signal));
			memmove(solidData.reference, solidData.referenceNoFiltre, sizeof(solidData.reference));
		}

		//if(flt.get<CutoffFrequencyOn<ReferenceSignal>>().value)
		//{
		//	ChebyshevFiltre dsp;
		//	dsp.Setup(
		//		Singleton<SolidParametersTable>::Instance().items.get<FrequencySignal>().value / 2
		//		, 3
		//		, flt.get<CutoffFrequency<ReferenceSignal>>().value
		//		, 40
		//		);	
		//	double (&s)[SolidData::MAX_ZONES_COUNT] = solidData.reference;
		//	double (&noFiltre)[SolidData::MAX_ZONES_COUNT]	 = solidData.referenceNoFiltre;
		//	for(int i = 0; i < solidData.currentOffset; ++i)
		//	{
		//		s[i] = dsp.OneSample(noFiltre[i]);
		//	}
		//}
		//else
		//{
		//	memmove(solidData.reference, solidData.referenceNoFiltre, sizeof(double) * solidData.currentOffset);
		//}

	}

	void Recalculation()
	{
//		solidData.start = int(0.1 * solidData.currentOffset);
//		solidData.stop = solidData.currentOffset - solidData.start;

		double result = 0;
		wchar_t *groupName = L"";
		unsigned color = 0;

		Filtre();

		computeSolidGroup.Frames(
			solidData.signal
			, solidData.reference
			, solidData.start
			, solidData.stop
			, result
			, groupName
			, color
			);
		wchar_t buf[1024];

		wsprintf(buf, L"<ff00>%s<ff>√руппа прочности <%6x>%s <ff>коррел€ци€ <ffffff>%s"
			, computeSolidGroup.currentFile.c_str()
			, color
			, groupName
			, Wchar_from<double>(result)()
			);

		computeSolidGroup.currentGroupName = groupName;
		//int count = solidData.stop - solidData.start;
		//Singleton<L502Signal>::Instance().Set(&solidData.signal[solidData.start], count);
		//Singleton<L502Reference>::Instance().Set(&solidData.reference[solidData.start], count);

		//todo разобратьс€
		//App::PrintTopLabel(buf);
		//App::UpdateMainWindow();

		ColorPanel::SetText(groupName, color);
		PointsWindow::IsOpenUpdate();
	}

	void Recalculation(double &result, wchar_t *&groupName, unsigned &color)
	{
		//solidData.start = int(0.1 * solidData.currentOffset);
		//solidData.stop = solidData.currentOffset - solidData.start;

		result = 0;
		groupName = L"";
		color = 0;

		Filtre();

		computeSolidGroup.Frames(
			solidData.signal
			, solidData.reference
			, solidData.start
			, solidData.stop
			, result
			, groupName
			, color
			);
	}
}
