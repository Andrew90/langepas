#pragma once
#include "stdafx.h"
#include "Test502Window.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "App/App.h"
#include "Compute\Unit502.h" 

namespace
{
	 template<class T>struct DlgItemsRadio
	{
		HWND hWnd;
		HWND Init(HWND h, int &width, int &dy, T t)
		{
			hWnd = CreateWindow(L"button", ParamTitle<T>()()
				,  WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON
				, 10, dy, 600, 15, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			dy += 20;
			return hWnd;
		}
	};

	template<class T>struct dc;

	PARAM_TITLE(dc<Inp502<sinhro_s>>, L"Вход синхросигнала 1(SINHRO_S)")
	PARAM_TITLE(dc<Inp502<sinhro_d>>, L"Вход синхросигнала 2(SINHRO_D)")
	PARAM_TITLE(dc<Inp502<error_x> >, L"Вход состояния размагничивания(ERROR)")
	PARAM_TITLE(dc<Out502<start_x> >, L"Включение размагничивания(START)")

	//template<class T>struct DlgSubItems<dc<Inp502<T>>, int>:  DlgSubItems<Inp502<T>, bool>{};
	template<class T>struct DlgSubItems<dc<Inp502<T>>, bool>:  DlgItemsRadio<dc<Inp502<T>>>{};

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

    DEFINE_PARAM_WAPPER2(dc, Inp502, sinhro_s, bool, false)
    DEFINE_PARAM_WAPPER2(dc, Inp502, sinhro_d, bool, false)
    DEFINE_PARAM_WAPPER2(dc, Inp502, error_x , bool, false)
    DEFINE_PARAM_WAPPER2(dc, Out502, start_x , bool, false)

	struct DigitTable
	{
		typedef TL::MkTlst<
			  dc<Inp502<sinhro_s>>
			, dc<Inp502<sinhro_d>>
			, dc<Inp502<error_x> >
			, dc<Out502<start_x> >
		>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
	};

	struct TDlg: TemplDialog<NullType, DigitTable, DlgItem, TL::MkTlst<TstCloseBtn>::Result>
	{
		static const int count = TL::Length<DigitTable::items_list>::value;
		HANDLE hTimer;
		unsigned value;
		L502OffsetsDigitTable::TItems &bits;
		TDlg(DigitTable &t)
			: TemplDialog(t)
			, bits(Singleton<L502OffsetsDigitTable>::Instance().items)
		{
			CreateTimerQueueTimer(&hTimer, NULL, (WAITORTIMERCALLBACK)WaitOrTimerCallback, this, 100, 500, WT_EXECUTEINTIMERTHREAD);
		}
		~TDlg()
		{
			DeleteTimerQueueTimer(NULL, hTimer, NULL);
		}

		template<class O, class P>struct __set__;
		template<class T, class P>struct __set__<DlgItem<dc<Out502<T>>>, P>
		{
			typedef DlgItem<dc<Out502<T>>> O;
			void operator()(O &o, P &p)
			{
				unit502.BitOut(
					p.bits.get<Out502<T>>().value
					, BST_CHECKED == Button_GetCheck(o.hWnd) ? 1: 0
					);
			}
		};
		template<class T, class P>struct __set__<DlgItem<dc<Inp502<T>>>, P>
		{
			typedef DlgItem<dc<Inp502<T>>> O;
			void operator()(O &o, P &p)
			{
				Button_SetCheck(
					o.hWnd
					, 0 != (p.value & (1 << p.bits.get<Inp502<T>>().value)) ? BST_CHECKED: BST_UNCHECKED
					);
			}
		};

		static VOID CALLBACK WaitOrTimerCallback(TDlg *dlg, BOOLEAN TimerOrWaitFired)
		{
			unit502.BitIn(dlg->value);
			TL::foreach<TDlg::list, __set__>()(dlg->items, *dlg);
		}
	};
}

void Digit502Channel::Do(HWND h)
{
	DigitTable table;
	TDlg dlg(table);
	dlg.Do(h, L"Дискретные каналы 502 платы");
	unit502.BitOut(dlg.bits.get<Out502<start_x>>().value, 0);
}