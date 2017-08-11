#pragma once

#include <windows.h>
#include "window_tool\message.h"
#include "window_tool\TEvent.h"
#include "SolidGroupAlgoritm\SolidBase.h"
#include "DlgTemplates\ParamDlg.hpp"
/// \brief окно коррекции коэффициентов сигнала
class WindowAdjustingMultipliers
{		
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
	void(WindowAdjustingMultipliers::*ptrUpdate)(wchar_t *);
	HWND hWnd;
	OkBtn okBtn;
	CancelBtn cancelBtn;
	WindowAdjustingMultipliers();
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	LRESULT operator()(TCreate &);
	virtual void OkBtnHandler(TCommand &){}
	virtual void CancelBtnHandler(TCommand &){}
	void Refresh(wchar_t *t){if(NULL != ptrUpdate)(this->*ptrUpdate)(t);}
};