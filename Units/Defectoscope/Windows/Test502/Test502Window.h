#pragma once
#include "App/App.h"
#include "SignalViewer.h"
#include "window_tool\message.h"

class Test502Window
{
	HWND hWnd;
	HWND hStatusWindow;
	HWND hToolBar;
	//typedef TL::MkTlst<
	//	SignalViewer
	//>::Result viewers_list;
	void operator()(TSize &);
	LRESULT operator()(TCreate &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	static void Do(HWND);
};