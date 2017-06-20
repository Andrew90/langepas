#pragma once
#include "SignalViewer.h"
#include "templates/typelist.hpp"
#include "Common.h"
#include "Windows/TopLabelViewer.h"
#include "App/AppBase.h"

class SignalWindow
{
public:
	HWND hWnd;
	int zone, sensor, offsetInZone;
	int leftAcfCutOff, rightAcfCutOff;
	TL::Factory<BorderACFCutOffTable::items_list> bordersCutOff;
	typedef TL::MkTlst<
		SignalViewer
		, ACFViewer
		, FFTViewer
	>::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	TopLabelViewer topLabelViewer;
	SignalWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	LRESULT operator()(TCreate &);
	void operator()(TMouseWell &);
	void operator()(TDestroy &);
	void operator()(TClose &);
	static wchar_t *Title();

	bool DrawCursor(TMouseMove &, VGraphics &);	

	static void Open(int zone, int sensor, int offsetInZone);
	static void ChangeOffset(int zone, int offsetInZone);
};

