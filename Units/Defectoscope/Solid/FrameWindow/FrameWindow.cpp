#include "stdafx.h"
#include "FrameWindow.h"
#include <commctrl.h>
#include "window_tool\MenuAPI.h"
#include "FrameWindow\FrameWindowMenu.hpp"
#include "window_tool\Emptywindow.h"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "Dates\SolidData.h"
#include "templates\templates.hpp"

void Title()
{
	wchar_t *s = L"Tрубы";
	if(App::isStandard) s = L"Ёталон";
	FrameWindow &w = Singleton<FrameWindow>::Instance();
	SetWindowText(w.hWnd, s);
	w.IncDecFrame();
}

LRESULT FrameWindow::operator()(TCreate &l)
{
	Menu<FrameWindowMenu::items_list>().Init(l.hwnd);
	toolBar.Init(l.hwnd);
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, l.hwnd, 0);
	int pParts[] = {550,900, 3000};
	SendMessage(hStatusWindow, SB_SETPARTS, 3, (LPARAM)pParts);

	thresholdsViewer.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<ThresholdsViewer>::Proc, WindowClass<ThresholdsViewer>()(), &thresholdsViewer);
	frameViewer.hWnd = CreateChildWindow(l.hwnd, (WNDPROC)&Viewer<FrameViewer>::Proc, WindowClass<FrameViewer>()(), &frameViewer);
	Title();
	//frameViewer.cursor.SetMouseMoveHandler(this, &FrameWindow::MessageText);
	return 0;
}

void FrameWindow::operator()(TDestroy &)
{
	hWnd = NULL;
}

void FrameWindow::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	toolBar.Size();

	RECT rt;
	GetClientRect(toolBar.hWnd, &rt);
	RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(l.hwnd, &r);	

	int height = (r.bottom - rt.bottom) / 2;
	int y = rt.bottom; 
	
	MoveWindow(thresholdsViewer.hWnd , 0, y, r.right, height, true);
	y += height;
	MoveWindow(frameViewer.hWnd , 0, y, r.right, height, true);
}

void FrameWindow::operator()(TCommand &l)
{
	EventDo(l);
}

void FrameWindow::operator()(TMouseWell &l)
{
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		if(0 == l.flags.rButton)
		{

			frameViewer.cursor.SetMouseMoveHandler(this, &FrameWindow::MessageTextNOOP);
			if(l.delta < 0) IncFrame(); else DecFrame();
		}
		else
		{
			frameViewer.cursor.SetMouseMoveHandler(this, &FrameWindow::MessageText);
			l.hwnd = frameViewer.hWnd;
			frameViewer(l);
		}
	}
}
	
void FrameWindow::operator()(TGetMinMaxInfo &l)
{
	if(NULL != l.pMinMaxInfo)
	{
		l.pMinMaxInfo->ptMinTrackSize.x = 600;
		l.pMinMaxInfo->ptMinTrackSize.y = 600;
	}	
}

namespace
{
	SolidData &solidData = Singleton<SolidData>::Instance();
	ComputeSolidGroup &computeSolidGroup = Singleton<ComputeSolidGroup>::Instance();
}

void FrameWindow::IncDecFrame()
{
	int offsets[8];
	double thresh[8];
	double result;
	wchar_t *nameGroup = L"";
	unsigned color;
	computeSolidGroup.offset = frameViewer.x;
	bool b = computeSolidGroup.FramesOne(
		thresh
		, offsets
		, solidData.signal
		, solidData.reference
		, computeSolidGroup.offset
		, computeSolidGroup.offset + frameViewer.width
		, result
		, nameGroup
		, color
		);

		computeSolidGroup.start = computeSolidGroup.offset;
		memmove(thresholdsViewer.chart.items.get<ThresholdsViewer::FrameLine>().data, thresh, sizeof(thresh)); 
		memmove(computeSolidGroup.points, thresh, sizeof(thresh));
		if(b)
		{
			wchar_t *s = thresholdsViewer.label.buffer;
			for(auto i = computeSolidGroup.groupNameList.begin(); i != computeSolidGroup.groupNameList.end(); ++i)
			{
				wsprintf(s, L"<%6x>%s  ", i->second.color, i->second.name.c_str());
				s += wcslen(s);
			}

			wsprintf(s, L"<ff>√руппа прочности <%6x>%s <ff>коррел€ци€ <ffffff>%s"
				, color
				, nameGroup
				, Wchar_from<double>(result)()
				);

			wchar_t *txt = frameViewer.label.buffer;
			wcscpy(txt, L"<ff>");
			int len = wcslen(txt);
			s = &txt[len];
			for(int i = 0; i < dimention_of(offsets); ++i)
			{
				offsets[i] += computeSolidGroup.offset;
				_itow(offsets[i], s, 10);
				wcscat(s, L"  ");
				s += wcslen(s);
			}
			memmove(frameViewer.thresh, thresh, sizeof(frameViewer.thresh));
			memmove(frameViewer.points, offsets, sizeof(frameViewer.points));
		}		
		
		//frameViewer.chart.items.get<FrameViewer::SumSig>().SetData(
		//	&solidData.reference[ frameViewer.x]
		//	, &solidData.signal[ frameViewer.x]
		//	, frameViewer.width
		//	);

		RepaintWindow(thresholdsViewer.hWnd);
		RepaintWindow(frameViewer.hWnd);

		AddThresholdWindow::Update();
}

void FrameWindow::IncFrame()
{
	frameViewer.IncFrame();
	IncDecFrame();
}
void FrameWindow::DecFrame()
{
	frameViewer.DecFrame();
	IncDecFrame();
}

void FrameWindow::AddTreshold()
{
	computeSolidGroup.AddThreshold();
	HWND h = FindWindow(WindowClass<AddThresholdWindow>()(), 0);
	if(NULL != h)
	{			
	   AddThresholdWindow *o = (AddThresholdWindow *)GetWindowLong(h, GWL_USERDATA);
	   SetRow(o->grid.hWnd, computeSolidGroup.solidItems.size());
	   RepaintWindow(h);
	   SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
	   SetForegroundWindow(h);
	}
}

void FrameWindow::SelectTreshGraph(double *data)
{
	HWND h = FindWindow(WindowClass<FrameWindow>()(), 0);
	if(NULL != h)
	{	
		FrameWindow *w = (FrameWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		TUser u = {w->thresholdsViewer.hWnd, WM_USER, (void(*)(void *))ThresholdsViewer::Draw, data};
		SendMessage(MESSAGE(u));
	}
}

bool FrameWindow::MessageText(TMouseMove &l, VGraphics &g)
{
	int x = frameViewer.offsetX + frameViewer.x;
	wchar_t buf[512];
	wsprintf(buf, L"<ff>x <ff00>%d <ff>ref <ff00>%s <ff>sig <ff00>%s <ff>sum <ff00>%s<c0c0c0>000000000000000000000000000"
		, x
		, Wchar_from<double>(solidData.reference[x])()
		, Wchar_from<double>(solidData.signal[x])()
		, Wchar_from<double>(solidData.signal[x] - solidData.reference[x])()
		);
	frameViewer.label = buf;
	frameViewer.label.Draw(g());
	return true;
};
bool FrameWindow::MessageTextNOOP(TMouseMove &l, VGraphics &g){return true;}