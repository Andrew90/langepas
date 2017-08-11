#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "DlgTemplates\ParamDlg.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "Compute\Automat.hpp"

namespace
{
	using namespace AutomatN;
	static const wchar_t *turnOnControlCircuit = L"включите цепи управления";
	static const wchar_t *rotationIncluded = L"вращение включено";
	static const wchar_t *controlCircuit = L"цепи управления";
	static const wchar_t *frequencyConverterSignalPCH_B = L"сигнал состояния частотного преобразователя(PCH_B)";
	static const wchar_t *frequencyConverterSignalRun = L"сигнал состояния частотного преобразователя(Run)";
	HFONT hFont;
	template<class T>struct DlgItemsLabel
	{		
		HWND Init(HWND h, int &width, int &dy, T &)
		{	
			HWND hWnd = CreateWindow(L"static", turnOnControlCircuit
				, WS_VISIBLE | WS_CHILD
				, 30, dy, 600, 30, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			dy += 30;
			SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);
			return hWnd;
		}
	};

	template<class T>struct DlgItemsRadio
	{
		HWND Init(HWND h, int &width, int &dy, T t)
		{
			HWND hWnd = CreateWindow(L"button", ParamTitle<T>()()
				,  WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON
				, 10, dy, 600, 15, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			dy += 20;
			return hWnd;
		}
	};

	template<class T>struct Pos{};
	template<class T>struct Stat{};

	DEFINE_PARAM_WAPPER(Pos, oRL	  , bool, false)
		DEFINE_PARAM_WAPPER(Pos, oRM	  , bool, false)
		DEFINE_PARAM_WAPPER(Pos, oRH	  , bool, false)
		DEFINE_PARAM_WAPPER(Stat, iZU	  , bool, false)
		DEFINE_PARAM_WAPPER(Stat, iPCH_B  , bool, false)
		DEFINE_PARAM_WAPPER(Stat, iPCH_RUN, bool, false)
		DEFINE_PARAM(Mess, bool, false)

	struct Rotation
	{
		typedef TL::MkTlst<
			Mess
			, Pos<oRL>
			, Pos<oRM>
			, Pos<oRH>

			, Stat<iZU>
			, Stat<iPCH_B>
			, Stat<iPCH_RUN>

		>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
	};

	PARAM_TITLE(Pos<oRL>, L"(RL)Младший бит кода частототы вращения")
		PARAM_TITLE(Pos<oRM>, L"(RM)Средний бит кода частототы вращения")
		PARAM_TITLE(Pos<oRH>, L"(RH)Старший бит кода частототы вращения")

		PARAM_TITLE(Stat<iZU>, L"(ZU)Цепи управления")
		PARAM_TITLE(Stat<iPCH_B>, L"(PCH_B)Соснояние частотного преобразователя(В)")
		PARAM_TITLE(Stat<iPCH_RUN>, L"(PCH_RUN)Соснояние частотного преобразователя(Run)")

		template<>struct DlgSubItems<Stat<iZU> , bool>: DlgItemsRadio<Stat<iZU>>{};
	template<>struct DlgSubItems<Stat<iPCH_B> , bool>: DlgItemsRadio<Stat<iPCH_B>>{};
	template<>struct DlgSubItems<Stat<iPCH_RUN> , bool>: DlgItemsRadio<Stat<iPCH_RUN>>{};

	template<>struct DlgSubItems<Mess , bool>: DlgItemsLabel<Mess>{};

	//template<class O, class P>struct __ok_btn__
	//{
	//	void operator()(O &o, P &p)
	//	{
	//	}
	//};

	struct StartBtnPos
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Пуск";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h);
	};

	struct StopBtnPos
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDABORT;
		wchar_t *Title(){return L"Стоп";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h);
	};

	struct ExitBtnPos
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDCANCEL;
		wchar_t *Title(){return L"Закрыть";}
		template<class Owner>void BtnHandler(Owner &o, HWND h)
		{
			OUT_BITS(Off<oSTF>, Off<oRL>, Off<oRM>, Off<oRH>);
			EndDialog(h, FALSE);
		}
	};

	class DlgR: public TemplDialog<NullType, Rotation, DlgItem, TL::MkTlst<StartBtnPos, StopBtnPos, ExitBtnPos>::Result>
	{
	public:
		void (DlgR::*ptr)();
		unsigned currentTime;
		HWND &hMess;
		DlgR(Rotation &pos): TemplDialog(pos), currentTime(0), ptr(&DlgR::Noop)
			, hMess(items.get<DlgItem<Mess>>().hWnd)
		{} 

		void Noop(){}

		template<class O, class P>struct __set_speed__{void operator()(O &, P &){}};
		template<class T, class P>struct __set_speed__<DlgItem<Pos<T>>, P>
		{
			typedef DlgItem<Pos<T>> O;
			void operator()(O &o, P &p)
			{
				if(BST_CHECKED == Button_GetCheck(o.hWnd))
				{
					OUT_BITS(On<T>);
					p |= true;
				}
			}
		};

		void Start()
		{
			if(TEST_IN_BITS(
				On<iZU>
				, On<iPCH_B>
				, On<iPCH_RUN>
				))
			{
				SetWindowText(hMess, L"");	
				bool b = false;
				TL::foreach<list, __set_speed__>()(items, b);
				if(b)
				{
					currentTime = 1000 + GetTickCount();
					ptr = &DlgR::Power;
				}
				else
				{
					SetWindowText(hMess, L"Выберете скорость вращения");
				}
			}
			else
			{
				SetWindowText(hMess, turnOnControlCircuit);	
			}
		}
		void Power()
		{
			if(currentTime < GetTickCount())
			{
				OUT_BITS(On<oPowerPCH>);
				currentTime = 1000 + GetTickCount();
				ptr = &DlgR::STF_ON;
			}
		}
		void STF_ON()
		{
			if(currentTime < GetTickCount())
			{
				OUT_BITS(On<oSTF>);
				currentTime = 500 + GetTickCount();
				ptr = &DlgR::ControlAlarmBits;
				SetWindowText(hMess, L"Вращение включено");	
			}
		}

		void Stop()
		{
			OUT_BITS(Off<oSTF>, Off<oRL>, Off<oRM>, Off<oRH>);
			SetWindowText(hMess, L"нажата кнопка \"Стоп\"");	
			ptr = &DlgR::Noop;
		}
		template<class O, class P>struct __alarm_bits__{bool operator()(P *){return true;}};
		template<class T, class P>struct __alarm_bits__<DlgItem<Stat<T>>, P>
		{
			bool operator()(P *p)
			{
				if(TEST_IN_BITS(Off<T>))
				{
					wchar_t buf[128];
					wsprintf(buf, L"сигнал отключён \"%s\"", ParamTitle<Stat<T>>()());
					SetWindowText(*p, buf);
					return false;
				}
				return true;
			}
		};
		void ControlAlarmBits()
		{
			unsigned tick = GetTickCount();
			if(tick > currentTime)
			{
				if(!TL::find<list, __alarm_bits__>()(&hMess))
				{
					OUT_BITS(Off<oPowerPCH>, Off<oSTF>, Off<oRL>, Off<oRM>, Off<oRH>);
					ptr = &DlgR::Noop;
					return;
				}
			}
			if(tick > currentTime + 30000)
			{
				OUT_BITS(Off<oSTF>, Off<oRL>, Off<oRM>, Off<oRH>);
				SetWindowText(hMess, L"Отключение по таймауту");
				ptr = &DlgR::Noop;
				EnableWindow(GetDlgItem(GetParent(hMess), StartBtnPos::ID), TRUE);	
			}
		}
	};

	template<class Owner>void StartBtnPos::BtnHandler(Owner &owner, HWND h)
	{
		((DlgR &)owner).ptr = &DlgR::Start;
		EnableWindow(GetDlgItem(h, StartBtnPos::ID), FALSE);
	}
	
	template<class Owner>void StopBtnPos::BtnHandler(Owner &owner, HWND h)
	{
		((DlgR &)owner).ptr = &DlgR::Stop;
		EnableWindow(GetDlgItem(h, StartBtnPos::ID), TRUE);	
	}

	struct TimeQueue
	{
		HANDLE hTimer;
		TimeQueue(DlgR *dlg)
		{
			CreateTimerQueueTimer(&hTimer, NULL, WaitOrTimerCallback, dlg, 1000, 500, WT_EXECUTEINTIMERTHREAD);
		}
		~TimeQueue()
		{
			DeleteTimerQueueTimer(NULL, hTimer, NULL);
		}

		template<class O, class P>struct __read_alarm_bits{void operator()(O &){}};

		template<class T, class P>struct __read_alarm_bits<DlgItem<Stat<T>>, P>
		{
			typedef DlgItem<Stat<T>> O;
			void operator()(O &o)
			{
				SendMessage (o.hWnd, BM_SETCHECK
					, (TEST_IN_BITS(On<T>))
					? BST_CHECKED
					: BST_UNCHECKED
					, 0);
			}
		};

		static VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
		{
			DlgR *dlg = (DlgR *)lpParameter;
			(dlg->*dlg->ptr)();
			TL::foreach<DlgR::list, __read_alarm_bits>()(dlg->items);
		}
	};
}

void RotationScanningDevice::Do(HWND h)
{
	Rotation rotation;
	hFont = CreateFont (26, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, 
				OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
				DEFAULT_PITCH | FF_DONTCARE, TEXT("Times New Roman"));
	DlgR dlg(rotation);
	TimeQueue timeQueue(&dlg);
	if(dlg.Do(h, L"Вращение сканирующего устройства"))
	{
	}
	OUT_BITS(Off<oSTF>, Off<oRL>, Off<oRM>, Off<oRH>);
	DeleteObject(hFont); 
}