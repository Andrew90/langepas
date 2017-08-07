#pragma once
#include "WindowAdjustingMultipliers\WindowAdjustingMultipliers.h"

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
	typedef typename Adjust<typename SubType<T>::sub_type, SubType<T>::NUM> sub_type;
	typename SubType<T>::type &owner;
public:
	typedef WindowAdjustingMultipliers Parent;
	typename sub_type::type_value value;
	AdjustingMultipliers(typename SubType<T>::type &t): owner(t){}


	LRESULT operator()(TCreate &l)
	{
		wchar_t buf[16] ;
		int v = int(value * 10);
		_itow(v, buf, 10);
		HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", v
			,  WS_CHILD | WS_VISIBLE
			, 10, 10, Parent::width - 40, 25, l.hwnd, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);

		AdjustXXXX::proc() = (WNDPROC )SetWindowLong(hWnd, GWL_WNDPROC, (long )AdjustXXXX::WndProc);

		HWND hUpdown = CreateWindowEx(0, UPDOWN_CLASS, 0, 
			UDS_ALIGNRIGHT | UDS_SETBUDDYINT | UDS_WRAP | WS_CHILD | WS_VISIBLE, 
			0, 0, 0, 0,
			l.hwnd, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL);
		
		SendMessage(hUpdown, UDM_SETBUDDY, (WPARAM)hWnd, 0);
		SendMessage(hUpdown, UDM_SETRANGE, 0, (LPARAM) MAKELONG((short)0, (short)50));
		SendMessage(hWnd, WM_SETTEXT, 0, buf);
		return Parent::operator()(l);
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
};

