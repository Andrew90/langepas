#include "stdafx.h"
#include "IOportsViewer.h"
#include "Graphics\ColorLabel.h"
#include "App/AppBase.h"
#include "tools_debug\DebugMess.h"
#include "Graphics/Chart.h"
#include "1730/Device1730.h"
#include "App/App.h"
#include "Dialogs/InputOutputDlg.h"
using namespace Gdiplus;
//-----------------------------------------------------------------------------
namespace
{
struct __draw_data__
{
	int x, dY;
	Graphics &g;
	int colorOn;
	int value;
	__draw_data__(int x, int dY, Graphics &g, int colorOn, int value)  : x(x), dY(dY), g(g), colorOn(colorOn), value(value) {}
};

template<class T>struct MessageField
{
	wchar_t buf[128];
	wchar_t *Text()
	{		
		wsprintf(buf, L"%S", &__FUNCTION__[43]);
		int len = wcslen(buf);
		buf[len - 7] = 0;
		return buf;
	}
};

template<class O, class P>struct __draw__
{
	void operator()(O *o, P *p)
	{
		ColorLabel l;
		l.left = p->x;
		l.top = p->dY;
		wsprintf(l.buffer, L"<55>%s",  ParamTitle<O>()());
		l.Draw(p->g);
		int color = 0xffcccccc;
		p->g.FillRectangle(&SolidBrush(Color(color)), p->x - 20, p->dY - 3, 15, 15);
		p->dY += 20;
	}
};
//------------------------------------------------------------------------------------
struct __io_update_data__
{
	int x, dY;
	HDCGraphics &g;
	int colorOn;
	int value;
	__io_update_data__(int x, int dY, HDCGraphics &g, int colorOn, int value)  : x(x), dY(dY), g(g), colorOn(colorOn), value(value) {}
};
template<class O, class P>struct __update__
{
	void operator()(O *o, P *p)
	{
		int color = 0xffcccccc;
		if(o->value & p->value) color = p->colorOn;
		p->g.graphics.FillRectangle(&SolidBrush(Color(color)), p->x - 20, p->dY - 3, 15, 15);
		p->dY += 20;
	}
};
VOID CALLBACK __Update__(PVOID oo, BOOLEAN)
{	
	IOportsViewer *o = (IOportsViewer *)oo;
	if(App::measurementOfRunning != o->lastTitle)
	{
		o->lastTitle = App::measurementOfRunning;
		wchar_t *title = o->lastTitle
			? L"Просмотр дискретных входов-выходов"
			: L"Управление дискретными выходами"
		;
		SetWindowText(o->hWnd, title);
	}
	RECT r;
	GetClientRect(o->hWnd, &r);
	HDCGraphics g(o->hWnd, o->backScreen);
    unsigned input_1 = device1730_1.Read();
	unsigned output_1 = device1730_1.ReadOutput(); 

	unsigned input_2 = device1730_2.Read();
	unsigned output_2 = device1730_2.ReadOutput(); 

	__io_update_data__ input_data(40, 15, g, 0xff0000ff, input_1);
	TL::foreach<InputBit1Table::items_list, __update__>()(&Singleton<InputBit1Table>::Instance().items, &input_data);
	input_data.dY += 10;
	input_data.value = input_2;
	TL::foreach<InputBit2Table::items_list, __update__>()(&Singleton<InputBit2Table>::Instance().items, &input_data);

	__io_update_data__ output_data(r.right / 2, 15, g, 0xffff0000, output_1);
	TL::foreach<OutputBit1Table::items_list, __update__>()(&Singleton<OutputBit1Table>::Instance().items, &output_data);
	output_data.dY += 10;
	output_data.value = output_2;
	TL::foreach<OutputBit2Table::items_list, __update__>()(&Singleton<OutputBit2Table>::Instance().items, &output_data);
}
}
//--------------------------------------------------------------------------------------
IOportsViewer::IOportsViewer(HWND &h, Bitmap *&bs)
	: hWnd(h)
	, backScreen(bs)
	, hTimer(NULL)
	, lastTitle(true)
{}
//----------------------------------------------------------------------------------------
void IOportsViewer::Size(Graphics &g, int width, int height)
{
	g.FillRectangle(&SolidBrush(Color(0xffaaaaaa)), 0, 0, width, height);
	unsigned input_1 = device1730_1.Read();
	unsigned output_1 = device1730_1.ReadOutput(); 

	unsigned input_2 = device1730_2.Read();
	unsigned output_2 = device1730_2.ReadOutput();

	__draw_data__ input_data(40, 15, g, 0xff0000ff, input_1);
	TL::foreach<InputBit1Table::items_list, __draw__>()(&Singleton<InputBit1Table>::Instance().items, &input_data);
	input_data.dY += 10;
	input_data.value = input_2;
	TL::foreach<InputBit2Table::items_list, __draw__>()(&Singleton<InputBit2Table>::Instance().items, &input_data);

	RECT r;
	GetClientRect(hWnd, &r);

	__draw_data__ output_data(r.right / 2, 15, g, 0xff0000ff, input_1);
	TL::foreach<OutputBit1Table::items_list, __draw__>()(&Singleton<OutputBit1Table>::Instance().items, &output_data);
	output_data.dY += 10;
	output_data.value = output_2;
	TL::foreach<OutputBit2Table::items_list, __draw__>()(&Singleton<OutputBit2Table>::Instance().items, &output_data);
}
//---------------------------------------------------------------------------------------
void IOportsViewer::Start()
{
	lastTitle = false;
	CreateTimerQueueTimer(&hTimer, NULL, __Update__, this, 2000, 500, WT_EXECUTEDEFAULT);
}
//------------------------------------------------------------------------------------------
void IOportsViewer::Stop()
{
	if(hTimer)DeleteTimerQueueTimer( NULL, hTimer, NULL );
}
//--------------------------------------------------------------------------------------------
struct __mouse_down_data__
{
	int x, dY;
	HDCGraphics &g;
	int colorOn;
	unsigned &value;
	TLButtonDown &l;
	Device1730 *device1730;
	__mouse_down_data__(int x, int dY, HDCGraphics &g, int colorOn, unsigned &value, TLButtonDown &l, Device1730 *device1730)  
		: x(x), dY(dY), g(g), colorOn(colorOn), value(value), l(l), device1730(device1730) 
	{}
};
template<class O, class P>struct __mouse_down__
{
	void operator()(O *o, P *p)
	{
		int x0 = p->x - 20;
		int x1 = x0 + 15;
		int y0 = p->dY - 3;
		int y1 = y0 + 15;
		if(p->l.x > x0 && p->l.x < x1 && p->l.y > y0 && p->l.y < y1)
		{
			p->value ^= o->value;
			p->device1730->WriteOutput(p->value);
		}
		int color = 0xffcccccc;
		unsigned output = p->device1730->ReadOutput();
		if(o->value & output) color = p->colorOn;
		p->g.graphics.FillRectangle(&SolidBrush(Color(color)), x0, y0, 15, 15);
		p->dY += 20;
	}
};
//-------------------------------------------------------------------------------------------------
void IOportsViewer::MouseLDown(TLButtonDown &l)
{
	if(App::measurementOfRunning) return;
	HDCGraphics g(hWnd, backScreen);
	unsigned input_1 = device1730_1.Read();
	unsigned output_1 = device1730_1.ReadOutput();

	unsigned input_2 = device1730_2.Read();
	unsigned output_2 = device1730_2.ReadOutput();

	__io_update_data__ input_data(40, 15, g, 0xff0000ff, input_1);
	TL::foreach<InputBit1Table::items_list, __update__>()(&Singleton<InputBit1Table>::Instance().items, &input_data);
	input_data.dY += 10;
	input_data.value = input_2;
	TL::foreach<InputBit1Table::items_list, __update__>()(&Singleton<InputBit1Table>::Instance().items, &input_data);
	RECT r;
	GetClientRect(l.hwnd, &r);
	__mouse_down_data__ output_data(r.right / 2, 15, g, 0xffff0000, output_1, l, &device1730_1);
	TL::foreach<OutputBit1Table::items_list, __mouse_down__>()(&Singleton<OutputBit1Table>::Instance().items, &output_data);
	output_data.dY += 10;
	output_data.value = output_2;
	output_data.device1730 = &device1730_2;
	TL::foreach<OutputBit2Table::items_list, __mouse_down__>()(&Singleton<OutputBit2Table>::Instance().items, &output_data);
}

