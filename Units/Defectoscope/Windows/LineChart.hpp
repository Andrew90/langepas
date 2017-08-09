#pragma once
#include "App/AppBase.h"
#include "ScanWindow.h"
#include "window_tool/ItemIni.h"
#include "templates/templates.hpp"
#include "window_tool\MenuAPI.h"
#include "DataItem/DataViewer.h"
#include "MessageText\StatusMessages.h"

namespace
{
	template<class T>struct Title;
	template<>struct Title<Cross>{wchar_t *operator()(){return L"����������";}};
	template<>struct Title<Long>{wchar_t *operator()(){return L"����������";}};
	template<>struct Title<Thick>{wchar_t *operator()(){return L"�������";}};
};

template<class T>struct DefVal
{
	double operator()(double d, int){return d;};
};

template<class T>struct SignalWrapper
{
	void operator()(int, int){}
};

template<>struct SignalWrapper<Thick>
{
	void operator()(int zone , int offset){/*SignalWindow::ChangeOffset(zone, offset);*/}
};

template<class T, int N>struct Line: LineTresholdsViewer<typename TL::SelectT<ThresholdsTable::items_list, typename T::sub_type>::Result>
{
	typedef LineTresholdsViewer<typename TL::SelectT<ThresholdsTable::items_list, typename T::sub_type>::Result> Parent;
	T *owner;		
	DataViewer<typename T::sub_type> dataViewer;
	Line()
	{
		((Parent::TChart *)chart)->items.get<BarSeries>().SetColorBarHandler(this, &Line::GetColorBar);
		cursor->SetMouseMoveHandler(this, &Line::CursorDraw);
	}	
	bool GetColorBar(int offs, double &data, unsigned &color)
	{
		if(NULL != dataViewer.data && offs < dataViewer.count)
		{
			data = DefVal<typename T::sub_type>()(dataViewer.data[offs], offs);
			color = StatusColor()(dataViewer.status[offs]);
			return true;
		}
		return false;
	}

	template<class T, int N>struct Test
	{
		template<class Z>int operator()(Z &t, int)
		{
			return 0;
		}
	};
	
	bool CursorDraw(TMouseMove &l, VGraphics &g)	  
	{	
		if(owner->drawZones)
		{
			double dX = (chart->rect.right - chart->rect.left - chart->offsetAxesLeft - chart->offsetAxesRight)
				/(chart->maxAxesX - chart->minAxesX);
			storedMouseMove.x = (WORD)(chart->rect.left + chart->offsetAxesLeft + dX * offsetX + dX / 2);

			double valY = ((Parent::TChart *)chart)->items.get<BarSeries>().ValueY(offsetX);
			unsigned color;
			char status = dataViewer.status[offsetX];
			bool b;
			wchar_t s[256];
			StatusText()(dataViewer.status[offsetX], color, b, s);
			bool no = TL::IndexOf<ColorTable::items_list, Clr<Undefined>>::value == status
				;

			if(!no)
			{
				wsprintf(label.buffer, L"<ff>���� <ff0000>%d <ff>������ <ff0000>%d <ff>�������� %d  �������� %s   %s    <ff00>%d   %d  %d"
					, 1 + owner->lastZone, 1 + N, 1 + offsetX, Wchar_from<double, 1>(valY)(), s, dataViewer.status[offsetX]
					, Test<T, 0>()(dataViewer, offsetX)
					, Test<T, 1>()(dataViewer, offsetX)
					);
			}
			else
			{
				wsprintf(label.buffer, L"<ff>���� <ff0000>%d <ff>������ <ff0000>%d <ff>�������� %d   %s    <ff00>%d "
					, 1 + owner->lastZone, 1 + N, 1 + offsetX, s, dataViewer.status[offsetX]);
			}
		}
		else
		{
			label.buffer[0] = 0;
		}
		label.Draw(g());
		SignalWrapper<typename T::sub_type>()(owner->lastZone, offsetX);
		return true;
	}

	void MenuItemScan()
	{
		//if(owner->viewer.viewerData.currentOffsetZones > owner->lastZone) SignalWindow::Open(owner->lastZone, N, offsetX);
	}
	/*

	*/
	void MenuZoneDisable()
	{
		owner->adjustItem.get<AdjustingMultipliers<Line<T, N>>>().Show();
	}

	void operator()(TRButtonDown &l);	
};

template<class T, void(T::*)()>struct MenuNum{};

template<class T, void(T::*X)()>struct TopMenu<MenuNum<T, X> >
{
	typedef NullType list;	
};

template<class T>struct NameMenu<TopMenu<MenuNum<T, &T::MenuItemScan> > >{wchar_t *operator()(HWND){return L"�������� �������";}};
template<class T>struct NameMenu<TopMenu<MenuNum<T, &T::MenuZoneDisable> > >{wchar_t *operator()(HWND){return L"�������������� ���������";}};

template<class T, void(T::*X)()>struct Event<TopMenu<MenuNum<T, X> >> 	
{										
	static void Do(HWND h)				
	{									
		(((T *)GetWindowLongPtr(h, GWLP_USERDATA))->*X)();
	}									
};

template<class T, int N>struct __line_sub_menu__
{
	void operator()(HWND h)
	{
		typedef TL::MkTlst<	
			 TopMenu<MenuNum<Line<T, N>, &Line<T, N>::MenuZoneDisable> >
		>::Result items_list;
		PopupMenu<items_list>::Do(h, h);
	}
};

template<class T, int N>void Line<T, N>::operator()(TRButtonDown &l)
{
	__line_sub_menu__<T, N>()(l.hwnd);
}

namespace
{
	template<class O, class P>struct __scan__
	{
		bool operator()(O *o, P *p)
		{
			return true;
		}
	};

	struct __scan_data__
	{
		int channel;
		int zone;
		int offs;
		double *scan;
	};

	template<template<class, int>class L, class T, int N, class P>struct __scan__<L<T, N>, P>
	{
		typedef typename L<T, N> O;
		bool operator()(O *o, P *p)
		{
			if(N == p->channel)
			{
				o->dataViewer.Do(p->zone, p->channel);
				p->scan = o->dataViewer.scan[p->offs];
				typedef typename O::Parent::Parent TWhell;

				o->offsetX = p->offs;
				TMouseWell w = {o->hWnd, WM_MOUSEWHEEL};
				(*(TWhell *)o)(w);
				return false;
			}
			return true;
		}
	};

	template<class T>struct __for_label__
	{
		wchar_t *operator()(double *d, ScanWindow &s)
		{
			return s.label.buffer;
		}
	};

	
	template<class T>struct __gates__
	{
		void operator()(ScanWindow &s, double *d)
		{
			
		}
	};	
#undef USPC
}

