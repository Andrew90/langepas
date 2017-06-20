#pragma once
#include "ThicknessViewer.h"
#include "Windows/LineViewer.h"
#include "templates/typelist.hpp"
#include "DataItem/ConstData.h"
#include "Windows/LineChart.hpp"
#include "Common.h"

//template<>class NoSubMenu<ThickViewer>: public ThickViewer
//{
//	public:
//	typedef ThickViewer Parent;
//	explicit NoSubMenu(){}
//	void operator()(TRButtonDown &){}
//	void operator()(TMouseWell &);
//};

class ThickWindow
{
public:
	typedef Thick sub_type;
	int lastZone;
public:
	HWND hWnd;
	bool drawZones;
	
	typedef TL::CreateWapperNumList<Line, ThickWindow, 0, App::count_Thick_sensors - 1>::Result line_list;
	typedef TL::AppendList<
		    TL::MkTlst<NoSubMenu<ThickViewer>>::Result
		    , line_list
	    >::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	ThickViewer &viewer;
	TopLabelViewer topLabelViewer;
	ThickWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	LRESULT operator()(TCreate &);
	void operator()(TMouseWell &);
	void operator()(TDestroy &);
	static wchar_t *Title();

	bool DrawCursor(TMouseMove &, VGraphics &);	
};