#pragma once

#include "CommonViewer.h"
#include "Graphics/Chart.hpp"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/GridChart.h"
#include "DataItem/DataItem.h"

class LongViewer: public CommonViewer
{
public:
	typedef CommonViewer Parent;
	typedef ChartDraw<Chart, TL::MkTlst<
		FixedLeftAxes
		, BottomAxesGrid
		, FixedGridSeries
		, FixedGrid		
	>::Result> TChart;
	TChart tchart;

	ItemData<Long> &viewerData;

	LongViewer();

	bool GetColorBar(unsigned sensor, int zone, double &data, unsigned &color);
	bool Draw(TMouseMove &l, VGraphics &g);

	void operator()(TRButtonDown &l);
};

