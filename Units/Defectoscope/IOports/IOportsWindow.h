#pragma once

#include <windows.h>
#include "window_tool/message.h"
#include "IOportsViewer.h"
class IOportsWindow
{
public:
	static const int width = 980;
	static const int height = 730;
	Gdiplus::Bitmap *backScreen;
public:
	IOportsViewer viewer;
public:
	HWND hWnd;
	IOportsWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TPaint &);
	void operator()(TLButtonDown &);
	static IOportsWindow &Instance();
};

