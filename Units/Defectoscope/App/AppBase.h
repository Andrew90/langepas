#pragma once
#include "App/App.h"
#include "Base/tables.hpp"
#include "Base/TablesDefine.h"
#include "L502\l502api.h"
#include "App/MessageItems.h"
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
DEFINE_PARAM_WAPPER(BorderKlass2, Long, double, 106)
DEFINE_PARAM_WAPPER(BorderDefect, Long, double, 45)

DEFINE_PARAM_WAPPER(BorderKlass2, Cross, double, 106)
DEFINE_PARAM_WAPPER(BorderDefect, Cross, double, 70)

DEFINE_PARAM_WAPPER(BorderKlass2, Thick, double, 5.7)
DEFINE_PARAM_WAPPER(BorderKlass3, Thick, double, 5.3)
DEFINE_PARAM_WAPPER(BorderDefect, Thick, double, 4.8)

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

#define	JOIN2(a, b) a##,##b
DEFINE_WAPPER(Clr<Undefined>, int, 0xff555555)

DEFINE_WAPPER(Clr<Nominal  >, int, 0xff00ff00)
DEFINE_WAPPER(Clr<DeathZone>, int, 0xff333333) 

DEFINE_WAPPER(Clr<BorderKlass2<Thick>  >, int, 0xff0000ff)
DEFINE_WAPPER(Clr<BorderKlass3<Thick>  >, int, 0xffff00ff)
DEFINE_WAPPER(Clr<BorderDefect<Thick>  >, int, 0xffff0000)

DEFINE_WAPPER(Clr<BorderKlass2<Long>      >, int, 0xffffff00)
DEFINE_WAPPER(Clr<BorderDefect<Long>      >, int, 0xffff0000)
DEFINE_WAPPER(Clr<BorderKlass2<Cross>     >, int, 0xffffff00)
DEFINE_WAPPER(Clr<BorderDefect<Cross>     >, int, 0xffff0000)
//DEFINE_WAPPER( Clr<Cancel<Projectionist>>, int, 0xff56ff32)
#undef	JOIN2


struct ColorTable
{
	typedef TL::MkTlst<	
		Clr<Undefined   >
		, Clr<DeathZone>
		, Clr<Nominal	>
	    , Clr<BorderKlass2<Thick> >
	    , Clr<BorderKlass3<Thick> >
        , Clr<BorderDefect<Thick> >
	    , Clr<BorderKlass2<Long> > 
	    , Clr<BorderDefect<Long> >
	    , Clr<BorderKlass2<Cross>>
	    , Clr<BorderDefect<Cross>>
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
 //--------------------------------------------------------------------------------------------------
 DEFINE_PARAM(MinimumLengthPipe, int, 33)
 struct MinimumLengthPipeTable
 {
	typedef TL::MkTlst<
		MinimumLengthPipe
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"MinimumLengthPipeTable";}
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
STR_PARAM(NamePlate1730_1, 64, L"PCI-1730,BID#1")
STR_PARAM(NamePlate1730_2, 64, L"PCI-1730,BID#2")
 struct NamePlate1730ParametersTable
 {
	 typedef TL::MkTlst<
		 NamePlate1730_1
		 ,  NamePlate1730_2
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"NamePlate1730ParametersTable";}
 };
//-------------------------------------------------------------------------------------------------------
struct AxesTable;
struct AnalogFilterTable;
struct CalculationAlgorithmTable;
struct RotationalSpeedTable;
struct AdjustingMultipliersTable;
DEFINE_PARAM_ID(ThresholdsTable            , int, 1)
DEFINE_PARAM_ID(DeadAreaTable			   , int, 1)
DEFINE_PARAM_ID(AxesTable	   , int, 1)
DEFINE_PARAM_ID(MedianFiltreTable, int, 1)
STR_PARAM(NameParam, 128, L"NONAME")
DEFINE_PARAM(CrossCountSensors, int, 12)
DEFINE_PARAM_ID(RecalculationFactorTable, int, 1)
DEFINE_PARAM_ID(MinMaxThresholdsTable, int, 1)
DEFINE_PARAM_ID(AnalogFilterTable, int, 1)
DEFINE_PARAM_ID(CalculationAlgorithmTable, int, 1)
DEFINE_PARAM_ID(RotationalSpeedTable, int, 1)
DEFINE_PARAM(TubeMinLength            , int, 38)
DEFINE_PARAM(DiametrTube            , int, 73)
DEFINE_PARAM_ID(MinimumLengthPipeTable, int, 1)
DEFINE_PARAM_ID(AdjustingMultipliersTable, int, 1)

 struct ParametersTable
 {
	typedef TL::MkTlst<
		ID<ThresholdsTable>
		, ID<DeadAreaTable			   	>
		, ID<AxesTable	   	>
		, ID<MedianFiltreTable>
		, ID<RecalculationFactorTable>
		, ID<MinMaxThresholdsTable>
		, ID<AnalogFilterTable>
		, ID<CalculationAlgorithmTable>
		, ID<RotationalSpeedTable>
		, ID<MinimumLengthPipeTable>
		, ID<AdjustingMultipliersTable>
		, CrossCountSensors
		, NameParam
		, TubeMinLength
		, DiametrTube
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ParametersTable";}
 };
//--------------------------------------------------------------------------------------------------------
DEFINE_PARAM(iSQ1pr   , unsigned, 1 << 0)
DEFINE_PARAM(iSQ2pr   , unsigned, 1 << 1)
DEFINE_PARAM(iRPpr    , unsigned, 1 << 2)
DEFINE_PARAM(iOPpr    , unsigned, 1 << 3)
DEFINE_PARAM(iSQ1po   , unsigned, 1 << 4)
DEFINE_PARAM(iSQ2po   , unsigned, 1 << 5)
DEFINE_PARAM(iRPpo    , unsigned, 1 << 6)
DEFINE_PARAM(iOPpo    , unsigned, 1 << 7)
DEFINE_PARAM(iSQ1t    , unsigned, 1 << 8)
DEFINE_PARAM(iSQ2t    , unsigned, 1 << 9)
DEFINE_PARAM(iRPt     , unsigned, 1 << 10)
DEFINE_PARAM(iOPt     , unsigned, 1 << 11)
DEFINE_PARAM(iReadyT  , unsigned, 1 << 12)
DEFINE_PARAM(iControlT, unsigned, 1 << 13)
DEFINE_PARAM(iResultT , unsigned, 1 << 14)
DEFINE_PARAM(iReserve , unsigned, 1 << 15)

struct InputBit1Table
 {
	typedef TL::MkTlst<
		iSQ1pr   
		, iSQ2pr   
		, iRPpr    
		, iOPpr    
		, iSQ1po   
		, iSQ2po   
		, iRPpo    
		, iOPpo    
		, iSQ1t    
		, iSQ2t    
		, iRPt     
		, iOPt     
		, iReadyT  
		, iControlT
		, iResultT 
		, iReserve 
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"InputBit1Table";}
 };

DEFINE_PARAM(oPowerPCH, unsigned, 1 << 0)
DEFINE_PARAM(oRL      , unsigned, 1 << 1)
DEFINE_PARAM(oRM      , unsigned, 1 << 2)
DEFINE_PARAM(oRH      , unsigned, 1 << 3)
DEFINE_PARAM(oSTF     , unsigned, 1 << 4)
DEFINE_PARAM(oPowerSU , unsigned, 1 << 5)
DEFINE_PARAM(oMagnet  , unsigned, 1 << 6)
DEFINE_PARAM(oCooling , unsigned, 1 << 7)
DEFINE_PARAM(oReloc1  , unsigned, 1 << 8)
DEFINE_PARAM(oRP      , unsigned, 1 << 9)
DEFINE_PARAM(oOP      , unsigned, 1 << 10)
DEFINE_PARAM(oDefect  , unsigned, 1 << 11)
DEFINE_PARAM(oReloc2  , unsigned, 1 << 12)
DEFINE_PARAM(oWorkPR  , unsigned, 1 << 13)
DEFINE_PARAM(oWorkPO  , unsigned, 1 << 14)
DEFINE_PARAM(oWorkT   , unsigned, 1 << 15)

struct OutputBit1Table
{
	typedef TL::MkTlst<
		oPowerPCH
		, oRL      
		, oRM      
		, oRH      
		, oSTF     
		, oPowerSU 
		, oMagnet  
		, oCooling 
		, oReloc1  
		, oRP      
		, oOP      
		, oDefect  
		, oReloc2  
		, oWorkPR  
		, oWorkPO  
		, oWorkT   	
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OutputBit1Table";}
};

DEFINE_PARAM(iZU           , unsigned, 1 << 0)
DEFINE_PARAM(iPCH_B        , unsigned, 1 << 1)
DEFINE_PARAM(iPCH_RUN      , unsigned, 1 << 2)
DEFINE_PARAM(iPCH_A        , unsigned, 1 << 3)
DEFINE_PARAM(iCycle        , unsigned, 1 << 4)
DEFINE_PARAM(iReadyR1      , unsigned, 1 << 5)
DEFINE_PARAM(iDone         , unsigned, 1 << 6)
DEFINE_PARAM(iReserve0     , unsigned, 1 << 7)
DEFINE_PARAM(iEtalon       , unsigned, 1 << 8)
DEFINE_PARAM(iWork_pnevmo  , unsigned, 1 << 9)
DEFINE_PARAM(iRevers_pnevmo, unsigned, 1 << 10)
DEFINE_PARAM(iError_pnevmo , unsigned, 1 << 11)
DEFINE_PARAM(iReserve1     , unsigned, 1 << 12)
DEFINE_PARAM(iReserve2     , unsigned, 1 << 13)
DEFINE_PARAM(iSQ1DM        , unsigned, 1 << 14)
DEFINE_PARAM(iSQ2DM        , unsigned, 1 << 15)

struct InputBit2Table
 {
	typedef TL::MkTlst<
		iZU           
		, iPCH_B        
		, iPCH_RUN      
		, iPCH_A        
		, iCycle        
		, iReadyR1      
		, iDone         
		, iReserve0     
		, iEtalon       
		, iWork_pnevmo  
		, iRevers_pnevmo
		, iError_pnevmo 
		, iReserve1     
		, iReserve2     
		, iSQ1DM        
		, iSQ2DM        
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"InputBit2Table";}
 };

DEFINE_PARAM(oPR_OP   , unsigned, 1 << 1)
DEFINE_PARAM(oPR_RP   , unsigned, 1 << 0)
DEFINE_PARAM(oPO_OP   , unsigned, 1 << 3)
DEFINE_PARAM(oPO_RP   , unsigned, 1 << 2)
DEFINE_PARAM(oT_OP    , unsigned, 1 << 5)
DEFINE_PARAM(oT_RP    , unsigned, 1 << 4)
DEFINE_PARAM(oReserve0, unsigned, 1 << 6)
DEFINE_PARAM(oReserve1, unsigned, 1 << 7)
DEFINE_PARAM(oT_Work  , unsigned, 1 << 8)
DEFINE_PARAM(oT_Cycle , unsigned, 1 << 9)
DEFINE_PARAM(oT_Base  , unsigned, 1 << 10)
DEFINE_PARAM(oT_reserv, unsigned, 1 << 11)
DEFINE_PARAM(oReserve2, unsigned, 1 << 12)
DEFINE_PARAM(oReserve3, unsigned, 1 << 13)
DEFINE_PARAM(oReserve4, unsigned, 1 << 14)
DEFINE_PARAM(oReserve5, unsigned, 1 << 15)

struct OutputBit2Table
{
	typedef TL::MkTlst<
		oPR_OP   
		, oPR_RP   
		, oPO_OP   
		, oPO_RP   
		, oT_OP    
		, oT_RP    
		, oReserve0
		, oReserve1
		, oT_Work  
		, oT_Cycle 
		, oT_Base  
		, oT_reserv
		, oReserve2
		, oReserve3
		, oReserve4
		, oReserve5
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OutputBit2Table";}
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
struct ACS{};
template<class T>struct OnTheJob;
DEFINE_PARAM_WAPPER(OnTheJob, Cross    , bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, Long     , bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, Thick, bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, ViewInterrupt, bool, true)
DEFINE_PARAM_WAPPER(OnTheJob, ACS, bool, false)

struct OnTheJobTable
{
	typedef TL::MkTlst<
		OnTheJob<Long     >
		, OnTheJob<Thick>
		, OnTheJob<ViewInterrupt>
		, OnTheJob<ACS>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OnTheJobTable";}
};
//-----------------------------------------------------------------
DEFINE_PARAM(ChannelSamplingRate, int, 2000)
struct L502ParametersTable
{
	typedef TL::MkTlst<
		ChannelSamplingRate
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502ParametersTable";}
};

template<class T, int>struct Range;
template<class T, int>struct Offset;

struct Voltage{};
struct Current{};

struct Temperature{};
struct MagneticField{};

template<class T>struct Inp502;
template<class T>struct Out502;

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

DEFINE_PARAM_WAPPER_NUM(Range, Current, 1, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Voltage, 0, int, 0)

DEFINE_PARAM_WAPPER_NUM(Range, Temperature, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Range, Temperature, 1, int, 0)

DEFINE_PARAM_WAPPER_NUM(Range, MagneticField, 0, int, 0)

DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 0, int, 7)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 1, int, 8)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 2, int, 9)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 3, int, 10)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 4, int, 11)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 5, int, 12)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 6, int, 13)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 7, int, 14)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 8, int, 15)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 9, int, 16)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 10, int, 17)
DEFINE_PARAM_WAPPER_NUM(Offset, Cross, 11, int ,18)
						
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 0, int, 0)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 1, int, 1)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 2, int, 2)
DEFINE_PARAM_WAPPER_NUM(Offset, Long, 3, int, 3)
						
DEFINE_PARAM_WAPPER_NUM(Offset, Voltage, 0, int, 4)
DEFINE_PARAM_WAPPER_NUM(Offset, Current, 1, int, 6)

DEFINE_PARAM_WAPPER_NUM(Offset, Temperature, 0, int, 21)
DEFINE_PARAM_WAPPER_NUM(Offset, Temperature, 1, int, 22)
					
DEFINE_PARAM_WAPPER_NUM(Offset, MagneticField, 0, int, 20)

DEFINE_PARAM_WAPPER(Inp502, sinhro_s, int, 1)
DEFINE_PARAM_WAPPER(Inp502, sinhro_d, int, 5)
DEFINE_PARAM_WAPPER(Inp502, error_x, int, 3)
DEFINE_PARAM_WAPPER(Out502, start_x, int, 1)

struct L502OffsetsDigitTable
{
	typedef TL::MkTlst<
	     Inp502<sinhro_s>
	   , Inp502<sinhro_d>
	   , Inp502<error_x>
	   , Out502<start_x>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502OffsetsDigitTable";}
};

struct L502RangeTable
{
	typedef TL::MkTlst<	
		Range<Long, 0> 
        , Range<Long , 1> 
        , Range<Long , 2> 
        , Range<Long , 3> 
		
		, Range<Cross, 0> 
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
		
		, Range<MagneticField, 0>

		, Range<Temperature, 0> 
		, Range<Temperature, 1> 

		, Range<Voltage, 0>
        , Range<Current, 1>
		
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502RangeTable";}
};
struct L502OffsetsTable
{
	typedef TL::MkTlst<
		Offset<Long, 0> 
		, Offset<Long, 1> 
        , Offset<Long, 2> 
        , Offset<Long, 3> 

		, Offset<Cross, 0> 
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

		, Offset<MagneticField, 0>

		, Offset<Temperature, 0> 
		, Offset<Temperature, 1> 

		, Offset<Voltage, 0>
        , Offset<Current, 1>
		
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502OffsetsTable";}
};
//-------------------------------------------------------------------
template<class T, int>struct Mode502;
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 0, int, L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 1, int,  L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 2, int,  L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 3, int,  L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 4, int,  L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 5, int,  L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 6, int,  L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 7, int,  L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 8, int,  L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 9, int,  L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 10, int, L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Cross, 11, int, L502_LCH_MODE_COMM)

DEFINE_PARAM_WAPPER_NUM(Mode502, Long, 0, int  , L502_LCH_MODE_DIFF)					
DEFINE_PARAM_WAPPER_NUM(Mode502, Long, 1, int  , L502_LCH_MODE_DIFF)
DEFINE_PARAM_WAPPER_NUM(Mode502, Long, 2, int  , L502_LCH_MODE_DIFF)
DEFINE_PARAM_WAPPER_NUM(Mode502, Long, 3, int  , L502_LCH_MODE_DIFF)
						
DEFINE_PARAM_WAPPER_NUM(Mode502, Voltage, 0, int, L502_LCH_MODE_DIFF)
DEFINE_PARAM_WAPPER_NUM(Mode502, Current, 1, int, L502_LCH_MODE_DIFF)
						
DEFINE_PARAM_WAPPER_NUM(Mode502, Temperature, 0, int, L502_LCH_MODE_COMM)
DEFINE_PARAM_WAPPER_NUM(Mode502, Temperature, 1, int, L502_LCH_MODE_COMM)
					
DEFINE_PARAM_WAPPER_NUM(Mode502, MagneticField, 0, int, L502_LCH_MODE_COMM)

struct L502ModeTable
{
	typedef TL::MkTlst<
		 Mode502<Long, 0> 
		, Mode502<Long, 1> 
        , Mode502<Long, 2> 
        , Mode502<Long, 3> 
		
		, Mode502<Cross, 0> 
        , Mode502<Cross, 1> 
        , Mode502<Cross, 2> 
        , Mode502<Cross, 3> 
        , Mode502<Cross, 4> 
        , Mode502<Cross, 5> 
        , Mode502<Cross, 6> 
        , Mode502<Cross, 7> 
        , Mode502<Cross, 8> 
        , Mode502<Cross, 9> 
        , Mode502<Cross, 10>
        , Mode502<Cross, 11> 
		
		, Mode502<MagneticField, 0>

		, Mode502<Temperature, 0> 
		, Mode502<Temperature, 1> 
	
		, Mode502<Voltage, 0>
        , Mode502<Current, 1>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502ModeTable";}
};
//-------------------------------------------------------------------
template<class T>struct CutoffFrequency;
template<class T>struct CutoffFrequencyOn;


DEFINE_PARAM_WAPPER(CutoffFrequency   , Cross, int, 1000)
DEFINE_PARAM_WAPPER(CutoffFrequencyOn , Cross, bool, true)
DEFINE_PARAM_WAPPER(CutoffFrequency   , Long, int, 1000)
DEFINE_PARAM_WAPPER(CutoffFrequencyOn , Long, bool, true)

DEFINE_PARAM_WAPPER(CutoffFrequency   , Voltage, int, 1000)
DEFINE_PARAM_WAPPER(CutoffFrequencyOn , Voltage, bool, true)

struct AnalogFilterTable
{
	typedef TL::MkTlst<
		CutoffFrequency<Cross>
		, CutoffFrequencyOn<Cross>
		, CutoffFrequency<Long>
		, CutoffFrequencyOn<Long>
		, CutoffFrequency<Voltage>
		, CutoffFrequencyOn<Voltage>
	>::Result items_list;
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
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CountSubZonesTable";}
};
//---------------------------------------------------------------------
DEFINE_PARAM(PrimarySignalMin , double, -100)
DEFINE_PARAM(PrimarySignalMax , double, 100)
DEFINE_PARAM(OffsetPointsMin  , double, -100)
DEFINE_PARAM(OffsetPointsMax  , double, 100)

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
struct TestGraphAxesTable
 {
	typedef TL::MkTlst<
		PrimarySignalMin
		, PrimarySignalMax
		, PrimarySignalOffset
		, PrimarySignalWidth
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TestGraphAxesTable";}
 };
//---------------------------------------------------------------
template<class T>struct SpeedBit;
template<class T>struct SpeedBitWork;
DEFINE_PARAM_WAPPER(SpeedBit, oRL, bool, false)
DEFINE_PARAM_WAPPER(SpeedBit, oRM, bool, true)
DEFINE_PARAM_WAPPER(SpeedBit, oRH, bool, false)

DEFINE_PARAM_WAPPER(SpeedBitWork, oRL, bool, false)
DEFINE_PARAM_WAPPER(SpeedBitWork, oRM, bool, true)
DEFINE_PARAM_WAPPER(SpeedBitWork, oRH, bool, false)
struct RotationalSpeedTable
{
	typedef TL::MkTlst<
		  SpeedBit<oRL>
		, SpeedBit<oRM>
		, SpeedBit<oRH>
		, SpeedBitWork<oRL>
		, SpeedBitWork<oRM>
		, SpeedBitWork<oRH>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"RotationalSpeedTable";}
};
//-----------------------------------------------------------------
template<class, int>struct Tresh;
DEFINE_PARAM_WAPPER_NUM(Tresh, Temperature, 0, double, 10)
DEFINE_PARAM_WAPPER_NUM(Tresh, Temperature, 1, double, 10)
DEFINE_PARAM_WAPPER_NUM(Tresh, Magn, 0, double, 5)
struct AdditionalParams502Table
{
	typedef TL::MkTlst<
		  Tresh<Temperature, 0>
		, Tresh<Temperature, 1>
		, Tresh<Magn, 0>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AdditionalParams502Table";}
};
//-----------------------------------------------------------------
DEFINE_PARAM(NumberComPort, int, 2)
DEFINE_PARAM(Speed, int, 9600)

DEFINE_PARAM(SubscriberThickness, int, 1)
DEFINE_PARAM(SubscriberASU, int, 2)
struct ComPortTable
{
	typedef TL::MkTlst<
		  NumberComPort
		, Speed
		, SubscriberThickness
		, SubscriberASU
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ComPortTable";}
};
//--------------------------------------------------------------------


template<class T, int N>struct OffsSQ;

DEFINE_PARAM_WAPPER_NUM(OffsSQ, Cross, 1, int, 0)
DEFINE_PARAM_WAPPER_NUM(OffsSQ, Cross, 2, int, 620)

DEFINE_PARAM_WAPPER_NUM(OffsSQ, Thick, 1, int, 1230)
DEFINE_PARAM_WAPPER_NUM(OffsSQ, Thick, 2, int, 1950)

DEFINE_PARAM_WAPPER_NUM(OffsSQ, Long, 1, int, 2600)
DEFINE_PARAM_WAPPER_NUM(OffsSQ, Long, 2, int, 3960)

DEFINE_PARAM_WAPPER_NUM(OffsSQ, Magn, 1, int, 4510)
DEFINE_PARAM_WAPPER_NUM(OffsSQ, Magn, 2, int, 4940)

struct OffsetSensorsTable
{
	typedef TL::MkTlst<
		 OffsSQ<Cross, 1>
		, OffsSQ<Cross, 2>
		, OffsSQ  <Thick, 1>
		, OffsSQ  <Thick, 2>
		, OffsSQ<Long, 1>
		, OffsSQ<Long, 2>		
		, OffsSQ<Magn, 1>
		, OffsSQ<Magn, 2>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OffsetSensorsTable";}
};
//-----------------------------------------------------------------
template<class T, int N>struct Adjust;
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 0, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 1, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 2, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 3, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 4, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 5, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 6, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 7, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 8, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 9, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 10, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Cross, 11, double, 1.0)

DEFINE_PARAM_WAPPER_NUM(Adjust, Long, 0, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Long, 1, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Long, 2, double, 1.0)
DEFINE_PARAM_WAPPER_NUM(Adjust, Long, 3, double, 1.0)
struct AdjustingMultipliersTable
{
	typedef TL::MkTlst<
		Adjust<Cross, 0>
		, Adjust<Cross, 1>
		, Adjust<Cross, 2>
		, Adjust<Cross, 3>
		, Adjust<Cross, 4>
		, Adjust<Cross, 5>
		, Adjust<Cross, 6>
		, Adjust<Cross, 7>
		, Adjust<Cross, 8>
		, Adjust<Cross, 9>
		, Adjust<Cross, 10>
		, Adjust<Cross, 11>
		
		, Adjust<Long, 0> 
		, Adjust<Long, 1> 
		, Adjust<Long, 2> 
		, Adjust<Long, 3> 
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AdjustingMultipliersTable";}
};
//-----------------------------------------------------------------
template<int N>struct TestLineColor;
DEFINE_PARAM_NUM(TestLineColor,  0, unsigned, 0xffffff00)
DEFINE_PARAM_NUM(TestLineColor,  1, unsigned, 0xffffcc00)
DEFINE_PARAM_NUM(TestLineColor,  2, unsigned, 0xffff9900)
DEFINE_PARAM_NUM(TestLineColor,  3, unsigned, 0xffff6600)
DEFINE_PARAM_NUM(TestLineColor,  4, unsigned, 0xffff3300)
DEFINE_PARAM_NUM(TestLineColor,  5, unsigned, 0xffff0000)
DEFINE_PARAM_NUM(TestLineColor,  6, unsigned, 0xff990000)
DEFINE_PARAM_NUM(TestLineColor,  7, unsigned, 0xff993300)
DEFINE_PARAM_NUM(TestLineColor,  8, unsigned, 0xff996600)
DEFINE_PARAM_NUM(TestLineColor,  9, unsigned, 0xff999900)
DEFINE_PARAM_NUM(TestLineColor, 10, unsigned, 0xff99cc00)
DEFINE_PARAM_NUM(TestLineColor, 11, unsigned, 0xff99ff00)
DEFINE_PARAM_NUM(TestLineColor, 12, unsigned, 0xff33ff00)
DEFINE_PARAM_NUM(TestLineColor, 13, unsigned, 0xff33cc00)
DEFINE_PARAM_NUM(TestLineColor, 14, unsigned, 0xff339900)
DEFINE_PARAM_NUM(TestLineColor, 15, unsigned, 0xff336600)
DEFINE_PARAM_NUM(TestLineColor, 16, unsigned, 0xff333300)
DEFINE_PARAM_NUM(TestLineColor, 17, unsigned, 0xff330000)

struct TestLineColorTable
{
	typedef TL::CreateNumList<TestLineColor, 0, 17>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TestLineColorTable";}
};

template<int N>struct TestLineOn;
DEFINE_PARAM_NUM(TestLineOn,  0, bool, true)
DEFINE_PARAM_NUM(TestLineOn,  1, bool, true)
DEFINE_PARAM_NUM(TestLineOn,  2, bool, true)
DEFINE_PARAM_NUM(TestLineOn,  3, bool, true)
DEFINE_PARAM_NUM(TestLineOn,  4, bool, true)
DEFINE_PARAM_NUM(TestLineOn,  5, bool, true)
DEFINE_PARAM_NUM(TestLineOn,  6, bool, true)
DEFINE_PARAM_NUM(TestLineOn,  7, bool, true)
DEFINE_PARAM_NUM(TestLineOn,  8, bool, true)
DEFINE_PARAM_NUM(TestLineOn,  9, bool, true)
DEFINE_PARAM_NUM(TestLineOn, 10, bool, true)
DEFINE_PARAM_NUM(TestLineOn, 11, bool, true)
DEFINE_PARAM_NUM(TestLineOn, 12, bool, true)
DEFINE_PARAM_NUM(TestLineOn, 13, bool, true)
DEFINE_PARAM_NUM(TestLineOn, 14, bool, true)
DEFINE_PARAM_NUM(TestLineOn, 15, bool, true)
DEFINE_PARAM_NUM(TestLineOn, 16, bool, true)
DEFINE_PARAM_NUM(TestLineOn, 17, bool, true)

struct TestLineOnTable
{
	typedef TL::CreateNumList<TestLineOn, 0, 17>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TestLineOnTable";}
};
//-----------------------------------------------------------------
 struct ParametersBase
 {
	 typedef TL::MkTlst<
		  NamePlate1730ParametersTable	  		
		 , PointsOptionsTable			
		 , GraphicSignalOptionsTable	   
		 , AdditionalSettingsTable
		 , ColorTable
		 , InputBit1Table
		 , OutputBit1Table		
		 , InputBit2Table
		 , OutputBit2Table		
		 , OnTheJobTable
		 , GraphAxesTable	
		 , L502RangeTable
		 , L502OffsetsTable
		 , L502OffsetsDigitTable
		 , L502ModeTable
		 , ComPortTable
		 , OffsetSensorsTable
		 , AdditionalParams502Table
		 , TestGraphAxesTable
		 , TestLineColorTable
		 , TestLineOnTable
		 , L502ParametersTable
	 >::Result one_row_table_list;

	 typedef TL::MkTlst<
		  CurrentParametersTable		 
		 , ParametersTable			   
		 , ThresholdsTable			  
		 , DeadAreaTable	
		 , MedianFiltreTable
		 , AxesTable
		 , RecalculationFactorTable
		 , MinMaxThresholdsTable
		 , AnalogFilterTable
		 , CountSubZonesTable
		 , RotationalSpeedTable
		 , MinimumLengthPipeTable
		 , AdjustingMultipliersTable
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








