#pragma once
#include "window_tool\message.h"
#include "MainWindow\PointsViewer.h"

class PointsWindow
{
public: 
	HWND hWnd;
	PointsViewer pointsViewer;
	LRESULT operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TCommand &);
	LRESULT operator()(TNotify &);
	void operator()(TGetMinMaxInfo &);
	void operator()(TMouseWell &);
	static void Show();
	static void IsOpenUpdate();
};