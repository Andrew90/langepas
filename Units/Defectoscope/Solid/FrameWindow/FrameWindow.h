#pragma once
#include "window_tool\message.h"
#include "FrameWindow\FrameWindowToolBar.h"
#include "FrameWindow\ThresholdsViewer.h"
#include "FrameWindow\FrameViewer.h"

template<class>struct Singleton;
class App;
class FrameWindow
{
	friend App;
	friend Singleton<FrameWindow>;
	HWND hStatusWindow;
	FrameWindowToolBar toolBar;
public:
	HWND hWnd;
	ThresholdsViewer thresholdsViewer;
	FrameViewer frameViewer;
private:
	FrameWindow(): hWnd(NULL)
	{
	}
	FrameWindow &operator =(const FrameWindow &);
public:
	void IncDecFrame();
public:	
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);

	void operator()(TSize &);
	void operator()(TCommand &);
	
	void operator()(TMouseWell &);

	void operator()(TGetMinMaxInfo &);

	void IncFrame();
	void DecFrame();

	void AddTreshold();

	static void SelectTreshGraph(double *);
	bool MessageText(TMouseMove &, VGraphics &);
	bool MessageTextNOOP(TMouseMove &, VGraphics &);
};

void Title();