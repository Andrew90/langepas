#pragma once
#include "Windows\LineViewer.h"

#include "App\SyncChannel.hpp"

class SignalViewer: public LineViewer
{
	template<class>struct Inner;
	template<template<class, int>class W, class T, int N>struct Inner<W<T, N>>
	{
		static const int NUM = N;
		typedef T type;
	};
public:
	template<int N>class LineUnit
	{
		typedef typename TL::TypeAt<SYNC(L502OffsetsTable::items_list), N>::Result T;
	public:
		static const int NUM = N;
		static const int sensor =  Inner<T>::NUM;
		typedef typename Inner<T>::type type;
	};
public:
	static const int count_channels = TL::Length<SYNC(L502OffsetsTable::items_list)>::value;//App::count_cross_sensors + App::count_long_sensors + App::count_solid_sensors;
	template<int N>struct SeriesOn
	{
		static const int NUM = N;
		HWND hWnd;
	};
	struct Series
	{
		Chart &chart;
		SignalViewer *owner;
		Series(Chart &);
		void Draw();
	};
public:	
	typedef LineViewer Parent;
	typedef ChartDraw<Chart, typename TL::MkTlst<
		LeftAxes
		, BottomAxes
		, Series
		, Grid	
	>::Result> TChart;
private:
	TChart chartLoc;
	Cursor cursorLoc;
	int xOffset;
	int xWidth;
	int startX, stopX, startY, stopY;
	void(SignalViewer::*ptr)(int, int);
public:
	SignalViewer()
		: chartLoc(backScreen)
		, cursorLoc(chartLoc)
	{
		chart = &chartLoc;
		cursor = &cursorLoc;
		chartLoc.items.get<Series>().owner = this;
	}
	LRESULT operator()(TCreate &l);	

	void operator()(TLButtonDown &);
	void operator()(TLButtonUp &);

	void operator()(TRButtonDown &);
	void operator()(TRButtonUp &);
	void operator()(TMouseMove &);

	void SetParam();

	void OffsetLeft();
	void OffsetRight();
	void RightBtn(int, int);
	void LeftBtn(int, int);
};