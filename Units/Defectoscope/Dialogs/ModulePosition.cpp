#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "Compute\Automat.hpp"

namespace
{
	using namespace AutomatN;
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
	template<class T>struct Pos{};
	template<class T>struct Stat{};

	DEFINE_PARAM_WAPPER(Stat, iRPpo, bool, false)
	DEFINE_PARAM_WAPPER(Stat, iOPpo, bool, false)
	DEFINE_PARAM_WAPPER(Stat, iRPpr, bool, false)
	DEFINE_PARAM_WAPPER(Stat, iOPpr, bool, false)
	DEFINE_PARAM_WAPPER(Stat, iRPt , bool, false)
	DEFINE_PARAM_WAPPER(Stat, iOPt , bool, false)
	DEFINE_PARAM_WAPPER(Pos, Cross, bool, false)
	DEFINE_PARAM_WAPPER(Pos, Long, bool, false)
	DEFINE_PARAM_WAPPER(Pos, Thick, bool, false)

	DEFINE_PARAM_WAPPER(Stat, iSQ1pr, bool, false)
	DEFINE_PARAM_WAPPER(Stat, iSQ2pr, bool, false)
	DEFINE_PARAM_WAPPER(Stat, iSQ1po, bool, false)
	DEFINE_PARAM_WAPPER(Stat, iSQ2po, bool, false)
	DEFINE_PARAM_WAPPER(Stat, iSQ1t , bool, false)
	DEFINE_PARAM_WAPPER(Stat, iSQ2t , bool, false)

	struct Position
	{
		typedef TL::MkTlst<
			Pos<Cross>
			, Pos<Long>
			, Pos<Thick>

			, Stat<iRPpo>
			, Stat<iOPpo>
			, Stat<iRPpr>
			, Stat<iOPpr>
			, Stat<iRPt >
			, Stat<iOPt >

			, Stat<iSQ1pr>
			, Stat<iSQ2pr>
			, Stat<iSQ1po>
			, Stat<iSQ2po>
			, Stat<iSQ1t >
			, Stat<iSQ2t >
		>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
	};

	PARAM_TITLE(Pos<Cross>, L"Поперечный модуль")
	PARAM_TITLE(Pos<Long>, L"Продольный модуль")
	PARAM_TITLE(Pos<Thick>, L"Модуль толщиномера")

	PARAM_TITLE(Stat<iRPpo>, L"(RPpo)Рабочее положение поперечного модуля")
	PARAM_TITLE(Stat<iOPpo>, L"(OPpo)Положение обслуживания поперечного модуля")
	PARAM_TITLE(Stat<iRPpr>, L"(RPpr)Рабочее положение продольного модуля")
	PARAM_TITLE(Stat<iOPpr>, L"(OPpr)Положение обслуживания продольного модуля")
	PARAM_TITLE(Stat<iRPt >, L"(RPt )Рабочее положение модуля толщинометра")
	PARAM_TITLE(Stat<iOPt >, L"(OPt )Положение обслуживания модуля толщиномера")

	template<>struct DlgSubItems<Stat<iRPpo> , bool>: DlgItemsRadio<Stat<iRPpo>>{};
	template<>struct DlgSubItems<Stat<iOPpo> , bool>: DlgItemsRadio<Stat<iOPpo>>{};
	template<>struct DlgSubItems<Stat<iRPpr> , bool>: DlgItemsRadio<Stat<iRPpr>>{};
	template<>struct DlgSubItems<Stat<iOPpr> , bool>: DlgItemsRadio<Stat<iOPpr>>{};
	template<>struct DlgSubItems<Stat<iRPt > , bool>: DlgItemsRadio<Stat<iRPt >>{};
	template<>struct DlgSubItems<Stat<iOPt > , bool>: DlgItemsRadio<Stat<iOPt >>{};

	PARAM_TITLE(Stat<iSQ1pr>, L"(SQ1pr)Датчик наличия трубы на входе в продольный модуль")
	PARAM_TITLE(Stat<iSQ2pr>, L"(SQ2pr)Датчик наличия трубы на выходе из продольного модуля")
	PARAM_TITLE(Stat<iSQ1po>, L"(SQ1po)Датчик наличия трубы на входе в поперечный модуль")
	PARAM_TITLE(Stat<iSQ2po>, L"(SQ2po)Датчик наличия трубы на выходе из поперечного модуля")
	PARAM_TITLE(Stat<iSQ1t >, L"(SQ1t )Датчик наличия трубы на входе в модуль толщины")
	PARAM_TITLE(Stat<iSQ2t >, L"(SQ2t )Датчик наличия трубы на выходе из модуля толщины")

	template<>struct DlgSubItems<Stat<iSQ1pr> , bool>: DlgItemsRadio<Stat<iSQ1pr>>{};
	template<>struct DlgSubItems<Stat<iSQ2pr> , bool>: DlgItemsRadio<Stat<iSQ2pr>>{};
	template<>struct DlgSubItems<Stat<iSQ1po> , bool>: DlgItemsRadio<Stat<iSQ1po>>{};
	template<>struct DlgSubItems<Stat<iSQ2po> , bool>: DlgItemsRadio<Stat<iSQ2po>>{};
	template<>struct DlgSubItems<Stat<iSQ1t > , bool>: DlgItemsRadio<Stat<iSQ1t >>{};
	template<>struct DlgSubItems<Stat<iSQ2t > , bool>: DlgItemsRadio<Stat<iSQ2t >>{};

	struct OkBtnPos
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			TL::foreach<typename Owner::list, __write_state__>()(owner.items);
			(( DlgW &)owner).currentTime = 1000 + GetTickCount();
		}
	};

	struct CancelBtnPos
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDCANCEL;
		wchar_t *Title(){return L"Закрыть";}
		template<class Owner>void BtnHandler(Owner &o, HWND h)
		{
			EndDialog(h, FALSE);
		}
	};
	class DlgW: public TemplDialog<NullType, Position, DlgItem, TL::MkTlst<OkBtnPos, CancelBtnPos>::Result>
	{
	public:
		unsigned currentTime;
		DlgW(Position &pos): TemplDialog(pos), currentTime(0){} 
	};

	template<class List, template<class>class W>struct __sel__;
	template<class Head, class Tail, template<class>class W>struct __sel__<Tlst<Head, Tail>, W>
	{
		typedef typename __sel__<Tail, W>::Result Result;
	};
	template<class Head, class Tail, template<class>class W>struct __sel__<Tlst<W<Head>, Tail>, W>
	{
		typedef Tlst<W<Head>, typename __sel__<Tail, W>::Result> Result;
	};
	template<template<class>class W>struct __sel__<NullType, W>
	{
		typedef NullType Result;
	};

	template<class O, class P>struct __read_state__
	{
		void operator()(O &o, P &p)
		{
			SendMessage (o.hWnd, BM_SETCHECK
				, (TEST_IN_BITS(On<TL::Inner<TL::Inner<O>::Result>::Result>))
				? BST_CHECKED
				: BST_UNCHECKED
				, 0);

			if(p.currentTime != 0 && p.currentTime > GetTickCount())
			{
				p.currentTime = 0;
				OUT_BITS(Off<oPO_OP>, Off<oPO_RP>, Off<oPR_RP>, Off<oPR_OP>, Off<oT_RP>, Off<oT_OP>);
			}
		}
	};

	template<class T>struct BitOn{void operator()(){}};
	template<>struct BitOn<Cross>{void operator()(){OUT_BITS(On<oPO_RP>);}};
	template<>struct BitOn<Long>{void operator()(){OUT_BITS(On<oPR_RP>);}};
	template<>struct BitOn<Thick>{void operator()(){OUT_BITS(On<oT_RP>);}};

	template<class T>struct BitOff{void operator()(){}};
	template<>struct BitOff<Cross>{void operator()(){OUT_BITS(On<oPO_OP>);}};
	template<>struct BitOff<Long>{void operator()(){OUT_BITS(On<oPR_OP>);}};
	template<>struct BitOff<Thick>{void operator()(){OUT_BITS(On<oT_OP>);}};

	template<class T>struct BitTest{bool operator()(){return false;}};

	template<>struct BitTest<Cross>
	{
		bool operator()()
		{
			return !(TEST_IN_BITS(On<iOPpo>, On<iRPpo>) || !TEST_IN_BITS(Off<iSQ1po>, Off<iSQ2po>));
		}
	};
	template<>struct BitTest<Long>
	{
		bool operator()()
		{
			return !(TEST_IN_BITS(On<iOPpr>, On<iRPpr>) || !TEST_IN_BITS(Off<iSQ1pr>, Off<iSQ2pr>));
		}
	};
	template<>struct BitTest<Thick>
	{
		bool operator()()
		{
			return !(TEST_IN_BITS(On<iOPt>, On<iRPt>) || !TEST_IN_BITS(Off<iSQ1t>, Off<iSQ2t>));
		}
	};

	template<class O, class P>struct __write_state__
	{
		void operator()(O &o)
		{
			typedef typename TL::Inner<typename TL::Inner<O>::Result>::Result Z;
			if(BitTest<Z>()())
			{
				if(BST_CHECKED == Button_GetCheck(o.hWnd))
				{
					BitOn<Z>()();
				}
				else
				{						
					BitOff<Z>()();
				}
			}
		}
	};

	template<class O, class P>struct __enable__
	{
		void operator()(O &o)
		{
			typedef typename TL::Inner<typename TL::Inner<O>::Result>::Result Z;
			EnableWindow(o.hWnd,  BitTest<Z>()());
		}
	};

	struct TimeQueue
	{
		HANDLE hTimer;
		TimeQueue(DlgW *dlg)
		{
			CreateTimerQueueTimer(&hTimer, NULL, WaitOrTimerCallback, dlg, 1000, 500, WT_EXECUTEINTIMERTHREAD);
		}
		~TimeQueue()
		{
			DeleteTimerQueueTimer(NULL, hTimer, NULL);
		}
		static VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
		{
			DlgW *dlg = (DlgW *)lpParameter;
			typedef TL::ListToWapperList<__sel__<Position::items_list, Stat>::Result, DlgItem>::Result read_list;
			TL::foreach<read_list, __read_state__>()(dlg->items, *dlg);

			typedef TL::ListToWapperList<__sel__<Position::items_list, Pos>::Result, DlgItem>::Result pos_list;
			TL::foreach<pos_list, __enable__>()(dlg->items);
		}
	};
}

void ModulePositionDlg::Do(HWND h)
{
	Position position;
	
	DlgW dlg(position);
	TimeQueue timeQueue(&dlg);
	if(dlg.Do(h, L"Положение сканирующих устройств"))
	{
	}
	OUT_BITS(Off<oPO_OP>, Off<oPO_RP>, Off<oPR_RP>, Off<oPR_OP>, Off<oT_RP>, Off<oT_OP>);
}