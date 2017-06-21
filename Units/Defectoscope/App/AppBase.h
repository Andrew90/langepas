#pragma once
#include "App/App.h"
#include "Base/tables.hpp"
#include "Base/TablesDefine.h"
//#include "Lan/LanParameters.h"
//-------------------------------------------------------------------------------------------
void TrimTypeList(wchar_t *, wchar_t *);
//-------------------------------------------------------------------------------------------
DEFINE_PARAM(CurrentID, int, 1)
struct CurrentParametersTable
{
	typedef TL::MkTlst<
		CurrentID
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CurrentParametersTable";}
};
//------------------------------------------------------------------------------------------
template<class>struct BorderKlass2;
template<class>struct BorderKlass3;
template<class>struct BorderDefect;

//template<class>struct BorderAbove;
//template<class>struct BorderLower;
//template<class>struct BorderNominal;

DEFINE_PARAM_WAPPER(BorderKlass2, Long, double, 106)
DEFINE_PARAM_WAPPER(BorderDefect, Long, double, 45)

DEFINE_PARAM_WAPPER(BorderKlass2, Cross, double, 106)
DEFINE_PARAM_WAPPER(BorderDefect, Cross, double, 70)

DEFINE_PARAM_WAPPER(BorderKlass2, Thick, double, 3.0)
DEFINE_PARAM_WAPPER(BorderKlass3, Thick, double, 2.0)
DEFINE_PARAM_WAPPER(BorderDefect, Thick, double, 12.0)

struct ThresholdsTable
{
	typedef TL::MkTlst<
		BorderKlass2<Long>
		, BorderDefect<Long>
		, BorderKlass2<Cross>
		, BorderDefect<Cross>
		, BorderKlass2  <Thick>
		, BorderKlass3  <Thick>
		, BorderDefect<Thick>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ThresholdsTable";}
};

DEFINE_PARAM(MinimumThicknessPipeWall, double, 3)
DEFINE_PARAM(MaximumThicknessPipeWall, double, 15)

struct MinMaxThresholdsTable
{
	typedef TL::MkTlst<
	   	  MinimumThicknessPipeWall
		, MaximumThicknessPipeWall
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"MinMaxThresholdsTable";}
};

template<class>struct RecalculationFactor;

DEFINE_PARAM_WAPPER(RecalculationFactor, Long, double, 1.0)
DEFINE_PARAM_WAPPER(RecalculationFactor, Cross, double, 1.0)

struct RecalculationFactorTable
{
	typedef TL::MkTlst<
		RecalculationFactor<Long>
		, RecalculationFactor<Cross>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"RecalculationFactorTable";}
};
//----------------------------------------------------------------------------------------
DEFINE_PARAM(CommunicationRemoveUnit, int, 0)
DEFINE_PARAM(CounterTubesStored, int, 0)
DEFINE_PARAM(MessagePanelVisible  , bool, false)

struct DifferentOptionsTable
{
	typedef TL::MkTlst<
		CommunicationRemoveUnit
		, CounterTubesStored
		, MessagePanelVisible
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DifferentOptionsTable";}
};
DEFINE_PARAM(OffsetAxesX, int, 100)
DEFINE_PARAM(FrameWidth, int, 400)
DEFINE_PARAM(MaxAxesY, double, 8191)
DEFINE_PARAM(MinAxesY, double, -8192)
struct GraphicSignalOptionsTable
{
	typedef TL::MkTlst<
		OffsetAxesX
		, FrameWidth
		, MaxAxesY
		, MinAxesY
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"GraphicSignalOptionsTable";}
};
struct PointsOptionsTable
{
	typedef TL::MkTlst<
		MaxAxesY
		, MinAxesY
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"PointsOptionsTable";}
};
//----------------------------------------------------------------------------------
template<class _0=NullType, class _1=NullType, class _2=NullType, class _3=NullType, class _4=NullType, class _5=NullType>struct Clr
{
	typedef typename TL::MkTlst<_0, _1, _2, _3, _4, _5>::Result items_list;
};

template<class T>struct GetItemsList;

template<template<class, class, class, class, class,class>class W, class _0, class _1, class _2, class _3, class _4, class _5>struct	GetItemsList<W<_0, _1, _2, _3, _4,_5>>
{
	typedef typename TL::MkTlst<_0, _1, _2, _3, _4, _5>::Result Result;
};

struct Undefined{};
struct Nominal{};
struct DeathZone{typedef NullType items_list;};
template<class>struct Cancel;
struct Projectionist{};

struct LessMinimumEnergy	{};  
struct GreaterMaximumEnergy	{};
struct ValueLessThreshold  	{};

#define	JOIN2(a, b) a##,##b
DEFINE_WAPPER(Clr<Undefined>, int, 0xff555555)

DEFINE_WAPPER(Clr<LessMinimumEnergy>, int, 0xff555555)
DEFINE_WAPPER(Clr<GreaterMaximumEnergy>, int, 0xff555555)
DEFINE_WAPPER(Clr<ValueLessThreshold>, int, 0xff555555)

DEFINE_WAPPER(Clr<Nominal  >, int, 0xff00ff00)
DEFINE_WAPPER(Clr<DeathZone>, int, 0xff333333) 

DEFINE_WAPPER(Clr<BorderKlass2<Thick>  >, int, 0xff0000ff)
DEFINE_WAPPER(Clr<BorderKlass3<Thick>  >, int, 0xffff0000)
DEFINE_WAPPER(Clr<BorderDefect<Thick>  >, int, 0xffff0000)

DEFINE_WAPPER(Clr<BorderKlass2<Long>      >, int, 0xffffff00)
DEFINE_WAPPER(Clr<BorderDefect<Long>      >, int, 0xffff0000)
DEFINE_WAPPER(Clr<BorderKlass2<Cross>     >, int, 0xffffff00)
DEFINE_WAPPER(Clr<BorderDefect<Cross>     >, int, 0xffff0000)
DEFINE_WAPPER( Clr<Cancel<Projectionist>>, int, 0xff56ff32)
#undef	JOIN2


struct ColorTable
{
	typedef TL::MkTlst<	
/*0*/		Clr<Undefined   >
/*1*/		, Clr<DeathZone>
/*2*/		, Clr<Nominal	>
/*3*/	    , Clr<BorderKlass2<Thick> >
/*4*/	    , Clr<BorderKlass3<Thick> >
			, Clr<BorderDefect<Thick> >
/*5*/	    , Clr<BorderKlass2<Long> > 
/*6*/	    , Clr<BorderDefect<Long> >
/*7*/	    , Clr<BorderKlass2<Cross>>
/*8*/	    , Clr<BorderDefect<Cross>>
/*10*/      , Clr<Cancel<Projectionist>>
            , Clr<LessMinimumEnergy	  >
		    , Clr<GreaterMaximumEnergy>
		    , Clr<ValueLessThreshold  >
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ColorTable";}
};

template<class T>inline int StatusId()
{
	return TL::IndexOf<ColorTable::items_list, T>::value;
}

template<class T>struct Stat
{
	static const unsigned value = TL::IndexOf<ColorTable::items_list, T>::value;
};
//-------------------------------------------------------------------------------------- 
DEFINE_PARAM(ReferenceOffset1, unsigned, 4915)
struct AdditionalSettingsTable
{
	typedef TL::MkTlst<	
		ReferenceOffset1
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AdditionalSettingsTable";}
};
//--------------------------------------------------------------------------------------
 template<class>struct DeadAreaMM0;
 template<class>struct DeadAreaMM1;
 DEFINE_PARAM_WAPPER(DeadAreaMM0, Cross, int, 200)
 DEFINE_PARAM_WAPPER(DeadAreaMM1, Cross, int, 200)

 DEFINE_PARAM_WAPPER(DeadAreaMM0, Long, int, 200)
 DEFINE_PARAM_WAPPER(DeadAreaMM1, Long, int, 200)

 DEFINE_PARAM_WAPPER(DeadAreaMM0, Thick, int, 200)
 DEFINE_PARAM_WAPPER(DeadAreaMM1, Thick, int, 200)
 
 struct DeadAreaTable
 {
	typedef TL::MkTlst<
		DeadAreaMM0<Cross>
		, DeadAreaMM1<Cross>
		, DeadAreaMM0<Long>
		, DeadAreaMM1<Long>
		, DeadAreaMM0<Thick>
		, DeadAreaMM1<Thick>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DeadAreaTable";}
 };
//----------------------------------------------------------------------------------------------------
 template<class>struct MedianFiltreWidth;
 template<class>struct MedianFiltreOn;

 DEFINE_PARAM_WAPPER(MedianFiltreWidth, Cross, int, 5)
 DEFINE_PARAM_WAPPER(MedianFiltreOn, Cross, bool, true)

 DEFINE_PARAM_WAPPER(MedianFiltreWidth, Long, int, 5)
 DEFINE_PARAM_WAPPER(MedianFiltreOn, Long, bool, true)

 DEFINE_PARAM_WAPPER(MedianFiltreWidth, Thick, int, 5)
 DEFINE_PARAM_WAPPER(MedianFiltreOn, Thick, bool, true)
 
 struct MedianFiltreTable
 {
	 typedef TL::MkTlst<
		 MedianFiltreWidth<Cross>
		 , MedianFiltreOn<Cross> 
		 , MedianFiltreWidth<Long>
		 , MedianFiltreOn<Long> 
		 , MedianFiltreWidth<Thick>
		 , MedianFiltreOn<Thick> 
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"MedianFiltreTable";}
 };
 //---------------------------------------------------------------------------------------------------------
 STR_PARAM(NamePlate1730, 64, L"PCIE-1730,BID#0")
 struct NamePlate1730ParametersTable
 {
	 typedef TL::MkTlst<
		 NamePlate1730
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"NamePlate1730ParametersTable";}
 };
//-------------------------------------------------------------------------------------------------------
struct AxesTable;
struct AnalogFilterTable;
struct CalculationAlgorithmTable;
DEFINE_PARAM_ID(ThresholdsTable            , int, 1)
DEFINE_PARAM_ID(DeadAreaTable			   , int, 1)
DEFINE_PARAM_ID(AxesTable	   , int, 1)
DEFINE_PARAM_ID(MedianFiltreTable, int, 1)
STR_PARAM(NameParam, 128, L"NONAME")
DEFINE_PARAM(CrossCountSensors, int, 10)
DEFINE_PARAM_ID(RecalculationFactorTable, int, 1)
//struct BorderACFCutOffTable;
//DEFINE_PARAM_ID(BorderACFCutOffTable, int, 1)
DEFINE_PARAM_ID(MinMaxThresholdsTable, int, 1)
DEFINE_PARAM_ID(AnalogFilterTable, int, 1)
DEFINE_PARAM_ID(CalculationAlgorithmTable, int, 1)

DEFINE_PARAM(TubeMinLength            , int, 140)
 struct ParametersTable
 {
	typedef TL::MkTlst<
		ID<ThresholdsTable>
		, ID<DeadAreaTable			   	>
		, ID<AxesTable	   	>
		, ID<MedianFiltreTable>
		, ID<RecalculationFactorTable>
		//, ID<BorderACFCutOffTable>
		, ID<MinMaxThresholdsTable>
		, ID<AnalogFilterTable>
		, ID<CalculationAlgorithmTable>
		, CrossCountSensors
		, NameParam
		, TubeMinLength
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ParametersTable";}
 };
//--------------------------------------------------------------------------------------------------------
DEFINE_PARAM(i—ontrol—ircuits  , unsigned, 1 << 0)
DEFINE_PARAM(iCycle            , unsigned, 1 << 1)
DEFINE_PARAM(iReady            , unsigned, 1 << 2)
DEFINE_PARAM(iControl          , unsigned, 1 << 3)
DEFINE_PARAM(iBase             , unsigned, 1 << 4)
DEFINE_PARAM(iReserve          , unsigned, 1 << 5)

DEFINE_PARAM(iCross0          , unsigned, 1 << 6)
DEFINE_PARAM(iCross1          , unsigned, 1 << 7)
DEFINE_PARAM(iLong0          , unsigned, 1 << 8)
DEFINE_PARAM(iLong1          , unsigned, 1 << 9)
DEFINE_PARAM(iThick0          , unsigned, 1 << 10)
DEFINE_PARAM(iThick1          , unsigned, 1 << 11)
DEFINE_PARAM(iSolid          , unsigned, 1 << 12)

struct InputBitTable
 {
	typedef TL::MkTlst<
		i—ontrol—ircuits
		, iCycle          
		, iReady          
		, iControl        
		, iBase           
		, iReserve
		, iCross0
		, iCross1
		, iLong0 
		, iLong1 
		, iThick0
		, iThick1
		, iSolid
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"InputBitTable";}
 };

DEFINE_PARAM(oWork            , unsigned, 1 << 0)
DEFINE_PARAM(oToShiftThe      , unsigned, 1 << 1)
DEFINE_PARAM(oResult1         , unsigned, 1 << 2)
DEFINE_PARAM(oResult2         , unsigned, 1 << 3)
DEFINE_PARAM(oPowerBM         , unsigned, 1 << 4)
DEFINE_PARAM(oReserve         , unsigned, 1 << 5)

struct OutputBitTable
 {
	typedef TL::MkTlst<
		oWork      
		, oToShiftThe  //ÔÂÂÍÎ‡‰Í‡
		, oResult1   
		, oResult2   
		, oPowerBM   
		, oReserve   
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OutputBitTable";}
 };
//-----------------------------------------------------------------------------------------------------------
template<class T>struct AxesYMin;
template<class T>struct AxesYMax;
DEFINE_PARAM_WAPPER(AxesYMin, Long, double, 0)
DEFINE_PARAM_WAPPER(AxesYMax, Long, double, 100)
DEFINE_PARAM_WAPPER(AxesYMin, Cross, double, 0)
DEFINE_PARAM_WAPPER(AxesYMax, Cross, double, 100)
DEFINE_PARAM_WAPPER(AxesYMin, Thick, double, 0)
DEFINE_PARAM_WAPPER(AxesYMax, Thick, double, 25)

struct AxesTable
{
	typedef TL::MkTlst<
		AxesYMin<Long>
		, AxesYMax<Long>
		, AxesYMin<Cross>
		, AxesYMax<Cross>
		, AxesYMin<Thick>
		, AxesYMax<Thick>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AxesTable";}
};
//-----------------------------------------------------------------------------------------------------------
template<class T>struct OnTheJob;
DEFINE_PARAM_WAPPER(OnTheJob, Cross    , bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, Long     , bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, Thick, bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, ViewInterrupt, bool, true)

struct OnTheJobTable
{
	typedef TL::MkTlst<
		OnTheJob<Cross    >
		, OnTheJob<Long     >
		, OnTheJob<Thick>
		, OnTheJob<ViewInterrupt>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OnTheJobTable";}
};
//-----------------------------------------------------------------
DEFINE_PARAM(ChannelSamplingRate, int, 10000)
struct L502ParametersTable
{
	typedef TL::MkTlst<
		ChannelSamplingRate
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502ParametersTable";}
};

template<class T, int>struct Range;
template<class T, int>struct Offset;

struct ReferenceSignal{};
struct InputSignal{};

struct Temperature{};
struct MagneticField{};

template<class T, int>struct Inp502;
template<class T, int>struct Out502;

struct sinhro_s{};
struct sinhro_d{};
struct error_x{};
struct start_x{};

DEFINE_PARAM_WAPPER_NUM(Range, Cross, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Cross, 1, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Cross, 2, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Cross, 3, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Cross, 4, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Cross, 5, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Cross, 6, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Cross, 7, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Cross, 8, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Cross, 9, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Cross, 10, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Cross, 11, int, 0)

DEFINE_PARAM_WAPPER_NUM(Range, Long, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Long, 1, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Long, 2, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Long, 3, int, 0)

DEFINE_PARAM_WAPPER_NUM(Range, ReferenceSignal, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, InputSignal,     0, int, 0)

DEFINE_PARAM_WAPPER_NUM(Range, Temperature, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Temperature, 1, int, 0)

DEFINE_PARAM_WAPPER_NUM(Range, MagneticField, 0, int, 0)


DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 1, int, 1)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 2, int, 2)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 3, int, 3)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 4, int, 4)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 5, int, 5)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 6, int, 6)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 7, int, 7)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 8, int, 8)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 9, int, 9)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 10, int, 10)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 11, int, 11)
						
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 0, int, 12)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 1, int, 13)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 2, int, 14)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 3, int, 15)
						
DEFINE_PARAM_WAPPER_NUM(Offset, ReferenceSignal, 0, int, 16)
DEFINE_PARAM_WAPPER_NUM(Offset, InputSignal,     0, int, 17)

DEFINE_PARAM_WAPPER_NUM(Offset, Temperature, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Temperature, 1, int, 0)
					
DEFINE_PARAM_WAPPER_NUM(Offset, MagneticField, 0, int, 0)

DEFINE_PARAM_WAPPER_NUM(Inp502, sinhro_s, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Inp502, sinhro_d, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Inp502, error_x, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Out502, start_x, 0, int, 0)



struct L502RangeTable1
{
	typedef TL::MkTlst<
          Range<Long , 0> 
        , Range<Long , 1> 
        , Range<Long , 2> 
        , Range<Long , 3> 
        , Range<ReferenceSignal, 0>
        , Range<InputSignal,     0>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502RangeTable1";}
};
struct L502OffsetsTable1
{
	typedef TL::MkTlst<
          Offset<Long , 0> 
        , Offset<Long , 1> 
        , Offset<Long , 2> 
        , Offset<Long , 3> 
        , Offset<ReferenceSignal, 0>
        , Offset<InputSignal,     0>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502OffsetsTable1";}
};

struct L502OffsetsDigitTable1
{
	typedef TL::MkTlst<
	     Inp502<sinhro_s, 0>
	   , Inp502<sinhro_d, 0>
	   , Inp502<error_x, 0>
	   , Out502<start_x, 0>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502OffsetsDigitTable1";}
};

struct L502RangeTable2
{
	typedef TL::MkTlst<
		  Range<Cross, 0> 
        , Range<Cross, 1> 
        , Range<Cross, 2> 
        , Range<Cross, 3> 
        , Range<Cross, 4> 
        , Range<Cross, 5> 
        , Range<Cross, 6> 
        , Range<Cross, 7> 
        , Range<Cross, 8> 
        , Range<Cross, 9> 
        , Range<Cross, 10>
        , Range<Cross, 11> 
		 
		, Range<Temperature, 0> 
		, Range<Temperature, 1> 

		, Range<MagneticField, 0>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502RangeTable2";}
};
struct L502OffsetsTable2
{
	typedef TL::MkTlst<
		  Offset<Cross, 0> 
        , Offset<Cross, 1> 
        , Offset<Cross, 2> 
        , Offset<Cross, 3> 
        , Offset<Cross, 4> 
        , Offset<Cross, 5> 
        , Offset<Cross, 6> 
        , Offset<Cross, 7> 
        , Offset<Cross, 8> 
        , Offset<Cross, 9> 
        , Offset<Cross, 10>
        , Offset<Cross, 11> 

		, Offset<Temperature, 0> 
		, Offset<Temperature, 1> 

		, Offset<MagneticField, 0>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502OffsetsTable2";}
};
//-------------------------------------------------------------------
template<class T>struct CutoffFrequency;
template<class T>struct CutoffFrequencyOn;


DEFINE_PARAM_WAPPER(CutoffFrequency   , Cross, int, 1000)
DEFINE_PARAM_WAPPER(CutoffFrequencyOn , Cross, bool, true)
DEFINE_PARAM_WAPPER(CutoffFrequency   , Long, int, 1000)
DEFINE_PARAM_WAPPER(CutoffFrequencyOn , Long, bool, true)

DEFINE_PARAM_WAPPER(CutoffFrequency   , InputSignal, int, 1000)
DEFINE_PARAM_WAPPER(CutoffFrequencyOn , InputSignal, bool, true)

struct AnalogFilterTable
{
	typedef TL::MkTlst<
		CutoffFrequency<Cross>
		, CutoffFrequencyOn<Cross>
		, CutoffFrequency<Long>
		, CutoffFrequencyOn<Long>
		, CutoffFrequency<InputSignal>
		, CutoffFrequencyOn<InputSignal>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AnalogFilterTable";}
};
//--------------------------------------------------------------------
 DEFINE_PARAM(CalculationAlgorithm, int, 0)
struct CalculationAlgorithmTable
{
	typedef TL::MkTlst<
		CalculationAlgorithm
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CalculationAlgorithmTable";}
};
//----------------------------------------------------------------------
DEFINE_PARAM(CountSubZones, int, 3)
struct CountSubZonesTable
{
	typedef TL::MkTlst<
		CountSubZones
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CountSubZonesTable";}
};
//---------------------------------------------------------------------
DEFINE_PARAM(PrimarySignalMin , double, -10)
DEFINE_PARAM(PrimarySignalMax , double, 12)
DEFINE_PARAM(OffsetPointsMin  , double, -10)
DEFINE_PARAM(OffsetPointsMax  , double, 12)

DEFINE_PARAM(PrimarySignalOffset  , int, 400)
DEFINE_PARAM(PrimarySignalWidth  , int, 1200)
struct GraphAxesTable
 {
	typedef TL::MkTlst<
		PrimarySignalMin
		, PrimarySignalMax
		, OffsetPointsMin 
		, OffsetPointsMax 
		, PrimarySignalOffset
		, PrimarySignalWidth
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"GraphAxesTable";}
 };
//---------------------------------------------------------------
 struct ParametersBase
 {
	 typedef TL::MkTlst<
		  NamePlate1730ParametersTable	  		
		 , PointsOptionsTable			
		 , GraphicSignalOptionsTable	   
		 , AdditionalSettingsTable
		 , ColorTable
		 , InputBitTable
		 , OutputBitTable		
		 , OnTheJobTable
	//	 , LanParametersTable
		 , GraphAxesTable	
		 , L502RangeTable1
		 , L502OffsetsTable1
		 , L502OffsetsDigitTable1
		 , L502RangeTable2
		 , L502OffsetsTable2
	 >::Result one_row_table_list;

	 typedef TL::MkTlst<
		  CurrentParametersTable		 
		 , ParametersTable			   
		 , ThresholdsTable			  
		 , DeadAreaTable	
		 , MedianFiltreTable
		 , AxesTable
		 , RecalculationFactorTable
		// , BorderACFCutOffTable
		 , MinMaxThresholdsTable
		 , AnalogFilterTable
		 , CountSubZonesTable
	 >::Result multy_row_table_list;

	 typedef TL::MkTlst<
		  multy_row_table_list
		 , one_row_table_list
	 >::Result multy_type_list; 

	 typedef TL::MultyListToList<multy_type_list>::Result type_list;
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	 wchar_t path[512];
	 const wchar_t *name();
 };

struct AppBase
{
	void Init();
	static void InitTypeSizeTables(CBase &);
};

template<class T>int CurrentId()
{
	  return Singleton<ParametersTable>::Instance().items.get<T>().value;
}

template<class T>void UpdateId(CBase &base, int num)
{
   CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
   Select<CurrentParametersTable>(base).ID(1).Execute(current);
   ParametersTable &t = Singleton<ParametersTable>::Instance();
   t.items.get<T>().value = num;
   UpdateWhere<ParametersTable>(t, base).ID(current.items.get<CurrentID>().value).Execute();
}

template<class T>int CountId(CBase &base, int num)
{
	ADODB::_RecordsetPtr rec;
	Select<ParametersTable>(base).eq<T>(num).Execute(rec);
	int i = 0;
	while (!rec->EndOfFile) 
	{			
		++i;
		rec->MoveNext(); 
	}
	return i;
}
template<class T>struct CountRowsTable
{
	int operator()(CBase &base)
	{
	   int count = 0;
	   wchar_t buf[128];
	   wsprintf(buf, L"SELECT count(*) as C FROM %s", T().name());
	   CMD(base).CommandText(buf).Execute().GetValue(L"C", count);
	   return count;
	}
};








