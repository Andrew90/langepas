#include "stdafx.h"
#include "SignalViewer.h"

#include "DataItem/ConstData.h"
#include "MessageText/SelectMessage.h"
#include "templates/templates.hpp"
#include "App/AppBase.h"
#include "window_tool\MenuAPI.h"
#include "DataItem/ThicknessData.h"
//#include "Windows\SignalWindow.h"
#include "Lan\LanParameters.h"
#include "templates\templates.hpp"

XViewer::XViewer()
	: CommonViewer(&tchart, 1)
    , tchart(backScreen)
{
	chart = &tchart;
	cursor.horizontalLine = false;
}

SignalViewer::SignalViewer(SignalWindow &o)
	: owner(o)
{
}

ACFViewer::ACFViewer(SignalWindow &o)
	: CommonViewer(&tchart, 1)
    , tchart(backScreen)
	, owner(o)
	, ptrProc(&Parent::operator())
{
	chart = &tchart;
	cursor.horizontalLine = false;
	cursor.SetMouseMoveHandler(this, &ACFViewer::Draw);	
}

bool ACFViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = currentX;
	wsprintf(label.buffer, L"<ff>x <ff0000>%d  <ff>y <ff0000>%s"
		, x
		, Wchar_from<double>(scan[x])()
		);
	label.Draw(g());
	return true;
}

namespace
{
	struct LeftCutOff  {};
	struct RightCutOff {};
	struct Cursor      {};

	template<>struct TopMenu<LeftCutOff> {typedef NullType list;};
	template<>struct TopMenu<RightCutOff>{typedef NullType list;};
	template<>struct TopMenu<Cursor>     {typedef NullType list;};

	MENU_TEXT(L"Левое отсечение" , TopMenu<LeftCutOff> )
	MENU_TEXT(L"Правое отсечение", TopMenu<RightCutOff>)
	MENU_TEXT(L"Курсор"          , TopMenu<Cursor>     )

	template<>struct Event<TopMenu<LeftCutOff>>
	{
		static void Do(HWND h)
		{
			zprint("\n");
			ACFViewer *e = (ACFViewer *)GetWindowLongPtr(h, GWLP_USERDATA);
			e->ptrProc = &ACFViewer::LeftCutOff;
		}
	};
	template<>struct Event<TopMenu<RightCutOff>>
	{
		static void Do(HWND h)
		{
			zprint("\n");
			ACFViewer *e = (ACFViewer *)GetWindowLongPtr(h, GWLP_USERDATA);
			e->ptrProc = &ACFViewer::RightCutOff;
		}
	};
	template<>struct Event<TopMenu<Cursor>>
	{
		static void Do(HWND h)
		{
			zprint("\n");
			ACFViewer *e = (ACFViewer *)GetWindowLongPtr(h, GWLP_USERDATA);
			e->ptrProc = &ACFViewer::Parent::operator();
		}
	};

		typedef TL::MkTlst<
			TopMenu<LeftCutOff> 
			, TopMenu<RightCutOff>
			, Separator<1>
			, TopMenu<Cursor>     
		>::Result ACFViewerMenu;
}

void ACFViewer::operator()(TRButtonDown &l)
{
	PopupMenu<ACFViewerMenu>::Do(l.hwnd, l.hwnd);
}

void ACFViewer::operator()(TSize &l)
{
	//tchart.items.get<LeftBorder>().value = owner.leftAcfCutOff;
	//tchart.items.get<RightBorder>().value = owner.rightAcfCutOff;
	//
	//(*(Parent *)this)(l);
}

void ACFViewer::operator()(TMouseWell &l)
{
	(this->*this->ptrProc)(l);
}

namespace
{
	template<class List, template<int>class T>struct __select_item__;
	template<class Head, class Tail, template<int>class T>struct __select_item__<Tlst<Head, Tail>, T>
	{
		typedef typename __select_item__<Tail, T>::Result Result;
	};

	template<int N, class Tail, template<int>class T>struct __select_item__<Tlst<T<N>, Tail>, T>
	{
		typedef Tlst<T<N>, typename __select_item__<Tail, T>::Result> Result;
	};

	template<template<int>class T>struct __select_item__<NullType, T>
	{
		typedef NullType Result;
	};

	struct __data_cut__
	{
		int sensor, border;
		__data_cut__(int sensor, int border): sensor(sensor), border(border){}
	};
	template<class O, class P>struct __set_border__ {bool operator()(O &, P &){return true;}};

	template<int N, template<int>class T, class P>struct __set_border__<T<N>, P> 
	{
		typedef T<N> O;
		bool operator()(O &o, P &p)
		{
			if(N == p.sensor)
			{
				o.value = p.border;
				return false;
			}
			return true;
		}
	};
}

void ACFViewer::LeftCutOff(TMouseWell &l)
{
	mouseMove = false;

	int offs = l.delta / 120;

	if(0 == l.flags.lButton)
	{
		currentX -= offs;

		if(currentX >= App::count_zones) 	currentX = App::count_zones - 1;
		else  if(currentX < 0) 	currentX = 0;
		chart->CellCoord(storedMouseMove.x, storedMouseMove.y, currentX, currentY);
		cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
	}
	else
	{
//		owner.leftAcfCutOff -= offs;
//
//		if(owner.leftAcfCutOff >= chart->maxAxesX) 	owner.leftAcfCutOff = (int)chart->maxAxesX - 1;
//		else  if(owner.leftAcfCutOff < 0) 	owner.leftAcfCutOff = 0;
//		TL::find<__select_item__<BorderACFCutOffTable::items_list, LeftBorderACFCutOff>::Result, __set_border__>()(owner.bordersCutOff, __data_cut__(owner.sensor, owner.leftAcfCutOff));
//		RepaintWindow(l.hwnd);
	}	    
}

void ACFViewer::RightCutOff(TMouseWell &l)
{
	mouseMove = false;

	int offs = l.delta / 120;

	if(0 == l.flags.lButton)
	{
		currentX -= offs;

		if(currentX >= App::count_zones) 	currentX = App::count_zones - 1;
		else  if(currentX < 0) 	currentX = 0;
		chart->CellCoord(storedMouseMove.x, storedMouseMove.y, currentX, currentY);
		cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
	}
	else
	{
		//owner.rightAcfCutOff -= offs;
		//
		//if(owner.rightAcfCutOff >= chart->maxAxesX) 	owner.rightAcfCutOff = (int)chart->maxAxesX - 1;
		//else  if(owner.rightAcfCutOff < 0) 	owner.rightAcfCutOff = 0;
		//TL::find<__select_item__<BorderACFCutOffTable::items_list, RightBorderACFCutOff>::Result, __set_border__>()(owner.bordersCutOff, __data_cut__(owner.sensor, owner.rightAcfCutOff));
		//RepaintWindow(l.hwnd);
	}	    
}
//--------------------------------------------------------------
FFTViewer::FFTViewer(SignalWindow &o)
	: CommonViewer(&tchart, 1)
    , tchart(backScreen)
	, owner(o)
{
	cursor.SetMouseMoveHandler(this, &FFTViewer::Draw);	
}

namespace
{
	ThickData &thickData = Singleton<ThickData>::Instance();
}

bool FFTViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = currentX;
	double max = 0;
	int offs = 0;
	for(int i = (int)tchart.items.get<LeftBorder>().value; i < (int)tchart.items.get<RightBorder>().value; ++i)
	{
		if(scan[i] > max) 
		{
			max = scan[i];
			offs = i;
		}
	}

	wchar_t s[64] = L"";

	if(scan[0] * thickData.peak[owner.sensor] < max)
	{
		double thick = thickData.coefficientA[owner.sensor] * offs + thickData.coefficientB[owner.sensor];
		wsprintf(s, L"<0xbeafcafe>толщина <ff00>%s", Wchar_from<double, 1>(thick)());
	}

	wsprintf(label.buffer, L"<ff>x <ff0000>%d  <ff>y <ff0000>%s %s"
		, x
		, Wchar_from<double>(scan[x])()
		, s
		);
	label.Draw(g());
	return true;
}

void FFTViewer::operator()(TSize &l)
{
	tchart.items.get<LeftBorder>().value = thickData.minOffset[owner.sensor];
	tchart.items.get<RightBorder>().value = thickData.maxOffset[owner.sensor];
	tchart.items.get<MinEnergy>().value = thickData.minEnergy[owner.sensor];
	tchart.items.get<MaxEnergy>().value = thickData.maxEnergy[owner.sensor];

	

	tchart.items.get<Peak>().value = scan[0] * thickData.peak[owner.sensor];
	(*(Parent *)this)(l);
	zprint(" Peak %f  max %f\n", tchart.items.get<Peak>().value, scan[0]);
}

//void FFTViewer::operator()(TMouseWell &l)
//{
//	(this->*this->ptrProc)(l);
//}

