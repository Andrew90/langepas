#pragma once 
#include "window_tool\message.h"
#include "AddThresholdsWindow\SolidGroupGridWindow.h"
#include "window_tool\Grid.h"

class AddThresholdWindow
{
public:
	HWND hWnd;
	SolidGroup::GridWindow gridViewer;
	GridNotify grid;
	LRESULT operator()(TCreate &);

	void operator()(TSize &);
	void operator()(TGetMinMaxInfo &);
	void operator()(TClose &);

	LRESULT operator()(TNotify &);
	void operator()(TCommand &);

	static void Show();
	static void Update();

	void ChangeStandard(bool);
};