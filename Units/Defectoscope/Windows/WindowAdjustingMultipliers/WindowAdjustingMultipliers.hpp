#pragma once
#include "WindowAdjustingMultipliers\WindowAdjustingMultipliers.h"

template<class O, class P>struct __desdroy_ajust_window__
{
	void operator()(P &p)
	{
	   HWND h = FindWindow(WindowClass<O>()(), 0);
	   if(NULL != h)
	   {
		   p |= ((O *)GetWindowLongPtr(h, GWLP_USERDATA))->Destroy();
		   DestroyWindow(h);
	   }
	}
};

template<class List>struct AdjustStored
{
	void operator()(HWND h)
	{
		bool dataChanged = false;
		TL::foreach<List, __desdroy_ajust_window__>()(dataChanged);
		if(dataChanged)
		{
			if(TestPassword<ParametersBase, AdjustingMultipliersTable>()(h))
			{
				CBase base(
					ParametersBase().name()
					);
				if(base.IsOpen())
				{
					AdjustingMultipliersTable &t = Singleton<AdjustingMultipliersTable>::Instance();
					int id = CurrentId<ID<AdjustingMultipliersTable> >();	
					UpdateWhere<AdjustingMultipliersTable>(t, base).ID(id).Execute();
				}
			}
		}
	}
};

template<class T>struct AdjustTitle;
template<>struct AdjustTitle<Cross>
{
	wchar_t *operator()(){return L"Корректировка поперечного датчика";}
};
template<>struct AdjustTitle<Long>
{
	wchar_t *operator()(){return L"Корректировка продольного датчика";}
};

struct AdjustXXXX
{
	static WNDPROC &proc(){static WNDPROC x = NULL; return x;}
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if(proc())
		{
			switch(msg)
			{
			case WM_SETTEXT:
				{
					wchar_t *c =  (wchar_t *)lParam;
					int i = _wtoi(c);
					wsprintf(c, L"%d.%d", i / 10, i % 10);
					((WindowAdjustingMultipliers *)GetWindowLongPtr(GetParent(hwnd), GWLP_USERDATA))->Refresh(c);
					break;
				}
			}
		}
		return CallWindowProc(proc(), hwnd, msg, wParam, lParam);
	}
};

template<class T>class AdjustingMultipliers: public WindowAdjustingMultipliers
{
	template<class Z>struct SubType;
	template<template<class, int>class W, class Sub, int N>struct SubType<W<Sub, N>>
	{
		typedef typename Sub type;
		typedef typename Sub::sub_type sub_type;
		static const int NUM = N;
	};
public:
	typedef typename Adjust<typename SubType<T>::sub_type, SubType<T>::NUM> sub_type;
	typename SubType<T>::type &owner;
	HWND hEdit;
public:
	typedef WindowAdjustingMultipliers Parent;
	typename sub_type::type_value value, val;
	AdjustingMultipliers(typename SubType<T>::type &t)
		: owner(t)
		, value(Singleton<AdjustingMultipliersTable>::Instance().items.get<sub_type>().value)
		, val(value)
	{
		ptrUpdate = (void(WindowAdjustingMultipliers::*)(wchar_t *))&AdjustingMultipliers::RefreshVal;
	}

	LRESULT operator()(TCreate &l)
	{
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L""
			,  WS_CHILD | WS_VISIBLE | ES_READONLY
			, 10, 10, Parent::width - 40, 25, l.hwnd, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);

		AdjustXXXX::proc() = (WNDPROC )SetWindowLong(hEdit, GWL_WNDPROC, (long )AdjustXXXX::WndProc);

		HWND hUpdown = CreateWindowEx(0, UPDOWN_CLASS, 0, 
			UDS_ALIGNRIGHT | UDS_SETBUDDYINT | UDS_WRAP | WS_CHILD | WS_VISIBLE, 
			0, 0, 0, 0,
			l.hwnd, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL);
		
		SendMessage(hUpdown, UDM_SETBUDDY, (WPARAM)hEdit, 0);
		SendMessage(hUpdown, UDM_SETRANGE, 0, (LPARAM) MAKELONG((short)0, (short)50));
		
		SendMessage(hUpdown, UDM_SETPOS, 0, int(value * 10));
		return Parent::operator()(l);
	}

	//void operator()(TDestroy &l)
	bool Destroy()
	{
		wchar_t buf[32];
		GetWindowText(hEdit, buf, dimention_of(buf));
		val = _wtof(buf);		
		if(value != val)
		{
			wchar_t txt[128];
			wsprintf(txt,  L"\"%s %d\" изменена!\nСохранить?", AdjustTitle<SubType<T>::sub_type>()(), 1 + SubType<T>::NUM);
			if(IDYES ==  MessageBox(GetParent(hEdit), txt, L"Сообщение", MB_ICONQUESTION | MB_YESNO))
			{
				if(val < 0) val = 0.0;
				else if(val > 5.0) val = 5.0;
				Singleton<AdjustingMultipliersTable>::Instance().items.get<sub_type>().value = val;
				return true;
			}
		}
		return false;
	}

	void Show()
	{
		HWND h = FindWindow(WindowClass<AdjustingMultipliers>()(), 0);

		if(NULL == h)
		{	
			value = Singleton<AdjustingMultipliersTable>::Instance().items.get<sub_type>().value;
			wchar_t buf[128];
			wsprintf(buf, L"%s %d", AdjustTitle<SubType<T>::sub_type>()(), 1 + SubType<T>::NUM);
			HWND h = WindowTemplate(this, buf, 0,0,0,0, IDI_UPLOAD, (HBRUSH)COLOR_WINDOW);		
			SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			DWORD dwStyle = GetWindowLong(h, GWL_STYLE);
			dwStyle &= ~(WS_SYSMENU);
			SetWindowLong(h, GWL_STYLE, dwStyle);
			ShowWindow(h, SW_SHOWNORMAL);
		}
		else
		{
			SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
			SetForegroundWindow(h);
		}
	}

	void OkBtnHandler(TCommand &l)
	{
		wchar_t buf[32];
		GetWindowText(hEdit, buf, dimention_of(buf));
		val = _wtof(buf);
		
		if(Singleton<AdjustingMultipliersTable>::Instance().items.get<sub_type>().value != val)
		{			
			if(TestPassword<ParametersBase, AdjustingMultipliersTable>()(l.hwnd))
			{
				value = Singleton<AdjustingMultipliersTable>::Instance().items.get<sub_type>().value = val;
				CBase base(
					ParametersBase().name()
					);
				if(base.IsOpen())
				{
					AdjustingMultipliersTable &t = Singleton<AdjustingMultipliersTable>::Instance();
					int id = CurrentId<ID<AdjustingMultipliersTable> >();	
					Update<AdjustingMultipliersTable>(base)
						.set<sub_type>(val)
						.Where()
						.ID(id)
						.Execute();
				}
			}
		}
	}
	void CancelBtnHandler(TCommand &l)
	{
		wchar_t buf[32];
		GetWindowText(hEdit, buf, dimention_of(buf));
		value = _wtof(buf);
	}
	void RefreshVal(wchar_t *c)
	{
		val = _wtof(c);
		zprint("  RefreshVal\n");
		typedef Line<SubType<T>::type, SubType<T>::NUM> TLine;
		TLine &line = owner.viewers.get<TLine>();
		line.dataViewer.Do(owner.lastZone, SubType<T>::NUM, owner.adjustItem.get<AdjustingMultipliers<TLine>>().val);
		RepaintWindow(line.hWnd);
	}
};

