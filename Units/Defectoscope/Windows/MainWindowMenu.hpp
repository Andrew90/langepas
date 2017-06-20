#pragma once
#if 1
#include "App/config.h"
#include "window_tool/WindowsPosition.h"
#include "Dialogs/Dialogs.h"
#include "Compute\Compute.h"

namespace MainWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"����", TopMenu<MainFile>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct LoadDateFile {static void Do(HWND h){zprint("");}};//LoadDataDlg{};//{static void Do(HWND h){zprint("");}};
	struct SaveDateFile {static void Do(HWND h){zprint("");}};//StoredDataDlg{};//{static void Do(HWND h){zprint("");}};
	struct Compute_      {static void Do(HWND h){Compute::Recompute();}};//RecalculationDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainExit
	{
		static void Do(HWND h)
		{
			TClose c = {h, WM_CLOSE, 0, 0};
			SendMessage(MESSAGE(c));
		}
	};

	MENU_ITEM(L"��������� ������", LoadDateFile)
	MENU_ITEM(L"��������� ������", SaveDateFile)
	MENU_ITEM(L"����������", Compute_)
	MENU_ITEM(L"�����", MainExit)

	template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<LoadDateFile>
			, MenuItem<SaveDateFile>
			, Separator<0>
			, MenuItem<Compute_>
			, Separator<1>
			, MenuItem<MainExit>
		>::Result list;
	};
	//------------------------------------------------------------------------
	struct MainOptionTypeSize{};
	MENU_TEXT(L"����������", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	//struct Tresholds {};
	//struct Thick{};
	struct DeadZones                 : DeadZonesDlg{};//{static void Do(HWND h){zprint("");}};								 						
	struct MainCreateTypesize        : AddTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainDeleteTypeSize        : DelTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	struct CrossThresholdWindow__    : ThresholdCrossDlg{};//{static void Do(HWND h){zprint("");}};//: Common::OpenWindow<CrossThresholdWindow>{};
	struct LongThresholdWindow__     : ThresholdLongDlg{};//{static void Do(HWND h){zprint("");}};//: Common::OpenWindow<LongThresholdWindow>{};
	struct ThickThresholdWindow__: ThresholdThickDlg{};//{static void Do(HWND h){zprint("");}};//: Common::OpenWindow<ThickThresholdWindow>{};
	struct MedianFiltre              : MedianFiltreDlg{};//{static void Do(HWND h){zprint("");}};
	struct PipeThicknes  : PipeThicknessDlg{};//{static void Do(HWND h){zprint("");}};
	struct AcfCutOffBorders  : AcfCutOffBordersDlg{};
	struct Filter_: FilderDlg{}; 

	MENU_ITEM(L"���������� ������", CrossThresholdWindow__)
	MENU_ITEM(L"���������� ������", LongThresholdWindow__)
	MENU_ITEM(L"������ �������", ThickThresholdWindow__)
	MENU_ITEM(L"̸����� ����", DeadZones)
	MENU_ITEM(L"������� ����������", MainCreateTypesize)
	MENU_ITEM(L"������� ����������", MainDeleteTypeSize)
	MENU_ITEM(L"��������� ������", MedianFiltre)

	MENU_ITEM(L"����������� � ������������ ������ �����", PipeThicknes)
	MENU_ITEM(L"������� ��������� � ��������� �������", AcfCutOffBorders)
	MENU_ITEM(L"��������� ����������� �������", Filter_)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef TL::MkTlst<
			MenuItem<CrossThresholdWindow__>
			, MenuItem<LongThresholdWindow__>
			, MenuItem<ThickThresholdWindow__>
			, Separator<1>
			, MenuItem<DeadZones>
			, MenuItem<MedianFiltre>
			, MenuItem<Filter_>
			, MenuItem<PipeThicknes>
			, MenuItem<AcfCutOffBorders>
			, Separator<0>
			, MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
		>::Result list;
	 };
	//--------------------------------------------------------------------------------
	struct Options{};
	MENU_TEXT(L"���������", TopMenu<Options>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct WindowPosition    : WindowPositionDlg<MainWindow>{};//{static void Do(HWND h){zprint("");}};
	struct IOportsView       : IOportsDlg{};
	struct GroupOptions      : FrameWindowDlg{};//{static void Do(HWND h){zprint("");}};

	MENU_ITEM(L"��������� ���������� ����", WindowPosition)
    MENU_ITEM(L"�������� ���������� ������", IOportsView)
    MENU_ITEM(L"��������� ������ ���������", GroupOptions)

	template<>struct TopMenu<Options>
	{
		typedef TL::MkTlst<
			MenuItem<GroupOptions>
			, Separator<0>
			, MenuItem<WindowPosition>
			, Separator<1>
			, MenuItem<IOportsView>
		>::Result list;		
	};
	//-------------------------------------------------------------------------------------------------------
	struct Setting{};
	MENU_TEXT(L"���������", TopMenu<Setting>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct AnalogPlate__           {};//{static void Do(HWND h){zprint("");}};//: AnalogPlateDlg{};//{static void Do(HWND h){zprint("");}};
	struct DiscretePlate	      {};//{static void Do(HWND h){zprint("");}};//{};//static void Do(HWND h){zprint("");}};
	struct Lir__{};
	struct DiscretePlateInputs    : InputsDlg{};//{static void Do(HWND h){zprint("");}};
	struct DiscretePlateOutputs   : OutputsDlg{};//{static void Do(HWND h){zprint("");}};
	struct ColorItems             : ColorItemsDlg{};//{static void Do(HWND h){zprint("");}};
	struct DiscretePlateDescriptor: Descriptor1730Dlg{};//{static void Do(HWND h){zprint("");}};
	struct Coefficient            {static void Do(HWND h){zprint("");}};//{static void Do(HWND h){zprint("");}};
	struct Signal                 {static void Do(HWND h){zprint("");}};//{static void Do(HWND h){zprint("");}};
	struct ACFBorder              {static void Do(HWND h){zprint("");}};//{static void Do(HWND h){zprint("");}};

	struct OffsetChannels         : OffsetsChannelDlg{};//{static void Do(HWND h){zprint("");}};
	struct AmplificationChannels  : AmplificationChannelDlg{};//{static void Do(HWND h){zprint("");}};

	MENU_TEXT(L"���������� �����", SubMenu<DiscretePlate>)
	MENU_ITEM(L"������� �����", DiscretePlateInputs)
	MENU_ITEM(L"������� �����", DiscretePlateOutputs)
	MENU_ITEM(L"���������� ���������� �����", DiscretePlateDescriptor)
	MENU_ITEM(L"������������ ���������", Coefficient)
	
	MENU_TEXT(L"���������� �����", SubMenu<AnalogPlate__>)
	MENU_ITEM(L"�������� �������", OffsetChannels)
	MENU_ITEM(L"�������� �������", AmplificationChannels)

	MENU_ITEM(L"��������� �������", Signal)
	MENU_ITEM(L"������� ���", ACFBorder)
	MENU_ITEM(L"�����", ColorItems)
	
	template<>struct SubMenu<AnalogPlate__>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<OffsetChannels, AmplificationChannels>::Result 
			, MenuItem
		>::Result list;
	};

	template<>struct SubMenu<DiscretePlate>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<DiscretePlateDescriptor, DiscretePlateInputs, DiscretePlateOutputs>::Result 
			, MenuItem
		>::Result list;
	};

	MENU_TEXT(L"��������� ���", SubMenu<Lir__>)
	MENU_ITEM(L"���������� ���", LirDescriptorDlg)
	MENU_ITEM(L"������� ���", LirDlg)

	template<>struct SubMenu<Lir__>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<LirDescriptorDlg, LirDlg>::Result 
			, MenuItem
		>::Result list;
	};

	template<>struct TopMenu<Setting>
	{
		typedef TL::MkTlst<
			 SubMenu<DiscretePlate>	
			 , SubMenu<AnalogPlate__>
			 , SubMenu<Lir__>
			, MenuItem<ColorItems>
		>::Result list;		
	};
	// ----------------------------------------------------------------------------------------------------
	struct MainAbout: AboutWindowDlg{};//{static void Do(HWND h){zprint("");}};
	MENU_TEXT(L"� ���������", TopMenu<MainAbout>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	MENU_ITEM(L"� ���������", MainAbout)

	template<>struct TopMenu<MainAbout>
	{
		typedef TL::MkTlst<
			MenuItem<MainAbout>
		>::Result list;
	};
	// --------------------------------------------------------------------------------------------------------- 
	/// \brief	�������� ���� ������ �������������
	//struct TestMenu{};
	//MENU_TEXT(L"TestMenu", TopMenu<TestMenu>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//struct TestSubMenu0{};
	//struct TestSubMenu1{static void Do(HWND){zprint("");}};
	//struct TestSubMenu2{static void Do(HWND){zprint("");}};
	//struct TestSubMenu3{static void Do(HWND){zprint("");}};
	//struct TestSubMenu4{static void Do(HWND){zprint("");}};
	//
	//MENU_ITEM(L"TestSubMenu0", TestSubMenu0)
	//MENU_ITEM(L"TestSubMenu1", TestSubMenu1)
	//MENU_ITEM(L"TestSubMenu2", TestSubMenu2)
	//MENU_ITEM(L"TestSubMenu3", TestSubMenu3)
	//MENU_ITEM(L"TestSubMenu4", TestSubMenu4)
	//
	//template<>struct TopMenu<TestMenu>
	//{
	//	typedef TL::MkTlst<
	//		MenuItem<TestSubMenu0>
	//		, MenuItem<TestSubMenu1>
	//		, MenuItem<TestSubMenu2>
	//		, MenuItem<TestSubMenu3>
	//		, MenuItem<TestSubMenu4>
	//	>::Result list;
	//};
	//---------------------------------------------------------------------------------------------------------------
	typedef TL::MkTlst<
		TopMenu<MainFile>
		, TopMenu<MainOptionTypeSize>
		, TopMenu<Options>
		, TopMenu<Setting>
		, TopMenu<MainAbout>
	>::Result MainMenu;	
	}

#endif
