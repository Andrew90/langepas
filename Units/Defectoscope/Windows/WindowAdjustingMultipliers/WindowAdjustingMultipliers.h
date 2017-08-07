#pragma once

#include <windows.h>
#include "window_tool\message.h"
#include "window_tool\TEvent.h"
#include "SolidGroupAlgoritm\SolidBase.h"
#include "DlgTemplates\ParamDlg.hpp"

class WindowAdjustingMultipliers
{	
	typedef void (WindowAdjustingMultipliers::*Tptr)(TCommand &);
	Tptr ptrOk, ptrCancel;
public:
	class OkBtn: TEvent
	{
		friend WindowAdjustingMultipliers;
		WindowAdjustingMultipliers &owner;
		OkBtn(WindowAdjustingMultipliers &);
		void Do(TCommand &);
	};

	class CancelBtn: TEvent
	{
		friend WindowAdjustingMultipliers;
		WindowAdjustingMultipliers &owner;
		CancelBtn(WindowAdjustingMultipliers &);
		void Do(TCommand &);
	};
	static const int width = 300;
	static const int height = 120;
public:
	HWND hWnd;
	OkBtn okBtn;
	CancelBtn cancelBtn;
	WindowAdjustingMultipliers();
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	LRESULT operator()(TCreate &);
};