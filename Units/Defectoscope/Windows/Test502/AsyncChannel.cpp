#include "stdafx.h"
#include "Test502Window.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/App.h"
#include "Compute\Unit502.h"

namespace
{
template<class T>struct Tst: T{};
typedef Tst<Offset<MagneticField, 0>> Tst_Offset_MagneticField_0;
PARAM_TITLE(Tst_Offset_MagneticField_0, L"Датчик наличия магнитного поля");
typedef Tst<Offset<Temperature, 0>> Tst_Offset_Temperature_0;
PARAM_TITLE(Tst_Offset_Temperature_0, L"Датчик температуры 1")
	typedef Tst<Offset<Temperature, 1>> Tst_Offset_Temperature_1;
PARAM_TITLE(Tst_Offset_Temperature_1, L"Датчик температуры 2")
//
	struct TstTable
	{
		typedef TL::MkTlst<
			Tst<Offset<MagneticField, 0>>
			, Tst<Offset<Temperature, 0>  >
			, Tst<Offset<Temperature, 1>  >
			>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
	};
//
	struct TstCloseBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Закрыть";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			EndDialog(h, TRUE);
		}
	};
//
	template<class T, int edit_width = 70>struct TstEdit
	{
		HWND Init(HWND h, int &width, int &dy, T &t)
		{
			HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L""
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP | ES_READONLY
				, 10, dy, edit_width, 25, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			CreateWindow(L"static", ParamTitle<T>()()
				, WS_VISIBLE | WS_CHILD
				, edit_width + 20, dy + 3, dlg_width, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			dy += 25;
			return hWnd;
		}
	};
//
	template<class T>struct DlgSubItems<Tst<T>, int>: TstEdit<Tst<T>>{};

	template<class O, class P>struct __init_async__;
	template<template<class, int>class W, class T, int N, class P>struct __init_async__<Tst<W<T, N>>, P>
	{
		typedef Tst<W<T, N>> O;
		void operator()(P &p)
		{
			static const int i = TL::IndexOf<TstTable::items_list, O>::value;
			p.ach[i] = p.ch.get<Offset<T, N>>().value;
			p.amode[i] = p.mode.get<Mode502<T, N>>().value;
			p.arange[i] = p.range.get<Range<T, N>>().value;
		}
	};

	template<class O, class P>struct __send__
	{
		void operator()(O &o, P &p)
		{
			static const int i = TL::IndexOf<TDlg::list, O>::value;
			SetWindowText(o.hWnd, Wchar_from<double, 1>(p.value[i])());
		}
	};

	struct TDlg: TemplDialog<NullType, TstTable, DlgItem, TL::MkTlst<TstCloseBtn>::Result>
	{
		static const int count = TL::Length<TstTable::items_list>::value;
		HANDLE hTimer;
		L502OffsetsTable::TItems &ch;
		L502ModeTable::TItems &mode;
		L502RangeTable::TItems &range;
		unsigned ach[count];
		int amode[count];
		int arange[count];
		double value[count];
		TDlg(TstTable &t)
			: TemplDialog(t)
			, ch(Singleton<L502OffsetsTable>::Instance().items)
			, mode(Singleton<L502ModeTable>::Instance().items)
			, range(Singleton<L502RangeTable>::Instance().items)
		{
			TL::foreach<TstTable::items_list, __init_async__>()(*this);
			CreateTimerQueueTimer(&hTimer, NULL, (WAITORTIMERCALLBACK)WaitOrTimerCallback, this, 100, 500, WT_EXECUTEINTIMERTHREAD);
		}
		~TDlg()
		{
			DeleteTimerQueueTimer(NULL, hTimer, NULL);
		}

		static VOID CALLBACK WaitOrTimerCallback(TDlg *dlg, BOOLEAN TimerOrWaitFired)
		{
			unit502.ReadAsyncChannels(count, dlg->ach, dlg->amode, dlg->arange, dlg->value);
			TL::foreach<TDlg::list, __send__>()(dlg->items, *dlg);
		}
	};
	
}

void AsyncChannels::Do(HWND h)
{
	TDlg(TstTable()).Do(h, L"Дополнительные каналы 502 платы");
}
