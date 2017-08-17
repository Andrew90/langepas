#pragma once
#include "App/App.h"
#include "SignalViewer.h"
#include "window_tool\message.h"
#include "Windows\TopLabelViewer.h"
#include "window_tool\TEvent.h"
#include "window_tool\EmptyWindow.h"

struct GraphicOptionsDlg{static void Do(HWND );};
struct AsyncChannels{static void Do(HWND h);};
struct Digit502Channel{static void Do(HWND h);};

class Test502Window
{
	template<int N>class SeriesOn: public TEvent
	{
	    Test502Window &owner;
	public:
		static const int NUM = N;
		HWND hWnd;
		SeriesOn(Test502Window &owner): owner(owner){}
		void Do(TCommand &m)
		{
			Singleton<TestLineOnTable>::Instance().items.get<TestLineOn<N>>().value = BST_CHECKED == Button_GetCheck(m.hControl);
			RepaintWindow(owner.viewers.get<SignalViewer>().hWnd);
		}
	};
	bool guard;
	typedef TL::CreateNumList<SeriesOn, 0, SignalViewer::count_channels - 1>::Result series_on_list;
	TL::Factory<series_on_list> seriesOn;
	void Thread();
	static DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter);
public:
	HWND hWnd;
	HWND hStatusWindow;
	HWND hToolBar;
	typedef TL::MkTlst<
		SignalViewer
	>::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	TopLabelViewer topLabelViewer;
	Test502Window();
	void operator()(TSize &);
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	void operator()(TMouseWell &);
	static void Do(HWND);
	void SetParam();

	void Test();
	void Left();
	void Right();
};