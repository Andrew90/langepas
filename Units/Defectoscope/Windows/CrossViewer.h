#pragma once

#include "CommonViewer.h"
#include "Graphics/Chart.hpp"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/GridChart.h"
#include "DataItem/DataItem.h"

class CrossViewer: public CommonViewer
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

	//ItemData<Cross> &viewerData;
	ViewerData<Cross, Parent> viewerData;

	CrossViewer();

	bool GetColorBar(unsigned sensor, int zone, double &data, unsigned &color);
	bool Draw(TMouseMove &l, VGraphics &g);

	void operator()(TRButtonDown &l);
};

