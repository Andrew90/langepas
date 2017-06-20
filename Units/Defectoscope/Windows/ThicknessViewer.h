#pragma once

#include "CommonViewer.h"
#include "Graphics/Chart.hpp"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/GridChart.h"
#include "DataItem/DataItem.h"
#include "Graphics/BarSeries.h"

class ThickViewer: public CommonViewer
{
public:
	typedef CommonViewer Parent;
	typedef ChartDraw<Chart, TL::MkTlst<
		NoOffsetLeftAxes
		, BottomAxesGrid
		, BarSeriesDouble
		, FixedGrid		
	>::Result> TChart;
	TChart tchart;

	ItemData<Thick> &viewerData;

	ThickViewer();

	bool GetColorBar(int zone, double &data_, unsigned &color, double &data_1, unsigned &color1);
	bool Draw(TMouseMove &l, VGraphics &g);

	void operator()(TRButtonDown &);
	void operator()(TSize &);
};

