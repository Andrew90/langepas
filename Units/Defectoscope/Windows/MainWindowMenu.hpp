#pragma once
#if 1
#include "App/config.h"
#include "window_tool/WindowsPosition.h"
#include "Dialogs/Dialogs.h"
#include "Compute\ComputeResult.h"
#include "Test502\Test502Window.h"

namespace MainWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct LoadDateFile : LoadDlg{static void Do(HWND h){LoadDlg::Do(h); Recalculation();}};//{static void Do(HWND h){zprint("");}};//LoadDataDlg{};//{static void Do(HWND h){zprint("");}};
	struct SaveDateFile : SaveDlg{};//{static void Do(HWND h){zprint("");}};//StoredDataDlg{};//{static void Do(HWND h){zprint("");}};
	struct Compute_      {static void Do(HWND h){Recalculation();}};//RecalculationDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainExit
	{
		static void Do(HWND h)
		{
			TClose c = {h, WM_CLOSE, 0, 0};
			SendMessage(MESSAGE(c));
		}
	};

	MENU_ITEM(L"Загрузить данные", LoadDateFile)
	MENU_ITEM(L"Сохранить данные", SaveDateFile)
	MENU_ITEM(L"Перерасчёт", Compute_)
	MENU_ITEM(L"Выход", MainExit)

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
	MENU_TEXT(L"Типоразмер", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct DeadZones                 : DeadZonesDlg{};							 						
	struct MainCreateTypesize        : AddTypeSizeDlg{};
	struct MainDeleteTypeSize        : DelTypeSizeDlg{};
	struct CrossThresholdWindow__    : ThresholdCrossDlg{};
	struct LongThresholdWindow__     : ThresholdLongDlg{};
	struct ThickThresholdWindow__: ThresholdThickDlg{};
	struct MedianFiltre              : MedianFiltreDlg{};
	struct RotationSpeed: RotationSpeedDlg{};
	struct MinimalTubeLength  : MinimalTubeLengthDlg{};
	struct Filter_: FilderDlg{}; 

	MENU_ITEM(L"Поперечные пороги", CrossThresholdWindow__)
	MENU_ITEM(L"Продольные пороги", LongThresholdWindow__)
	MENU_ITEM(L"Пороги толщины", ThickThresholdWindow__)
	MENU_ITEM(L"Мёртвые зоны", DeadZones)
	MENU_ITEM(L"Создать типоразмер", MainCreateTypesize)
	MENU_ITEM(L"Удалить типоразмер", MainDeleteTypeSize)
	MENU_ITEM(L"Медианный фильтр", MedianFiltre)

	MENU_ITEM(L"Настройки цифрового фильтра", Filter_)
	MENU_ITEM(L"Скорость вращения", RotationSpeed)
	MENU_ITEM(L"Минимальная длина трубы в зонах", MinimalTubeLength)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef TL::MkTlst<
			MenuItem<CrossThresholdWindow__>
			, MenuItem<LongThresholdWindow__>
			, MenuItem<ThickThresholdWindow__>
			, MenuItem<MinimalTubeLength>
			, Separator<1>
			, MenuItem<DeadZones>
			, MenuItem<MedianFiltre>
			, MenuItem<Filter_>
			, MenuItem<RotationSpeed>
			, Separator<0>
			, MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
		>::Result list;
	 };
	//--------------------------------------------------------------------------------
	struct Options{};
	MENU_TEXT(L"Настройки", TopMenu<Options>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct WindowPosition    : WindowPositionDlg<MainWindow>{};//{static void Do(HWND h){zprint("");}};
	struct IOportsView       : IOportsDlg{};
	struct GroupOptions      : FrameWindowDlg{};//{static void Do(HWND h){zprint("");}};
	struct LaunchExternalProgram       : LaunchExternalProgramDlg{};

	MENU_ITEM(L"Сохранить координаты окна", WindowPosition)
    MENU_ITEM(L"Просмотр дискретных портов", IOportsView)
    MENU_ITEM(L"Настройка группы прочности", GroupOptions)
    MENU_ITEM(L"Просмотр сообщений", LaunchExternalProgram)

	template<>struct TopMenu<Options>
	{
		typedef TL::MkTlst<
			MenuItem<GroupOptions>
			, Separator<0>
			, MenuItem<WindowPosition>
			
		>::Result list;		
	};
	//-------------------------------------------------------------------------------------------------------
	struct Setting{};
	MENU_TEXT(L"Установка", TopMenu<Setting>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct AnalogPlate__           {};
	struct DiscretePlate	      {};
	//struct Lir__{};
	struct DiscretePlateInputs    : Inputs1Dlg{};
	struct DiscretePlateOutputs   : Outputs1Dlg{};

	struct DiscretePlateInputs1    : Inputs2Dlg{};
	struct DiscretePlateOutputs1   : Outputs2Dlg{};

	struct ColorItems             : ColorItemsDlg{};
	struct DiscretePlateDescriptor: Descriptor1730Dlg{};//{static void Do(HWND h){zprint("");}};
	struct Coefficient            {static void Do(HWND h){zprint("");}};//{static void Do(HWND h){zprint("");}};
	struct Signal                 {static void Do(HWND h){zprint("");}};//{static void Do(HWND h){zprint("");}};
	struct ACFBorder              {static void Do(HWND h){zprint("");}};//{static void Do(HWND h){zprint("");}};

	struct OffsetChannels         : OffsetsChannelDlg{};//{static void Do(HWND h){zprint("");}};//: OffsetsChannelDlg{};//{static void Do(HWND h){zprint("");}};
	struct AmplificationChannels  : AmplificationChannelDlg{};
	struct ComPort__  : ComPortDlg{};
	struct OffsetsSens : OffsetSensorDlg{};

	struct ModeChannel: ModeChannelDlg{};
	struct Digital502: Digital502Dlg{};

	struct AdditionalParams: AdditionalParamsDlg{};

	MENU_TEXT(L"Дискретная плата", SubMenu<DiscretePlate>)
	MENU_ITEM(L"Входные сигналы(IDI) платы 1", DiscretePlateInputs)
	MENU_ITEM(L"Выходные сигналы(IDO)платы 1", DiscretePlateOutputs)

	MENU_ITEM(L"Входные сигналы(IDI) платы 2", DiscretePlateInputs1)
	MENU_ITEM(L"Выходные сигналы(IDO)платы 2", DiscretePlateOutputs1)

	MENU_ITEM(L"Дискрипторы дискретных плат", DiscretePlateDescriptor)
	MENU_ITEM(L"Коэффициенты пересчёта", Coefficient)
	
	MENU_TEXT(L"Аналоговая плата", SubMenu<AnalogPlate__>)
	MENU_ITEM(L"Номера каналов", OffsetChannels)
	MENU_ITEM(L"Усиление каналов", AmplificationChannels)
	MENU_ITEM(L"Режим подключения каналов", ModeChannel)

	MENU_ITEM(L"Параметры сигнала", Signal)
	MENU_ITEM(L"Границы АЧХ", ACFBorder)
	MENU_ITEM(L"Цвета", ColorItems)
	MENU_ITEM(L"СОМ-порт", ComPort__)
	MENU_ITEM(L"Смещение датчиков наличия трубы в модуле", OffsetsSens)
	MENU_ITEM(L"Дополнительные параметры", AdditionalParams)
	MENU_ITEM(L"Дискретные каналы платы 502", Digital502)
	
	template<>struct SubMenu<AnalogPlate__>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<OffsetChannels, AmplificationChannels, ModeChannel, Digital502>::Result 
			, MenuItem
		>::Result list;
	};

	template<>struct SubMenu<DiscretePlate>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<DiscretePlateDescriptor, DiscretePlateInputs, DiscretePlateOutputs
			, DiscretePlateInputs1, DiscretePlateOutputs1
			>::Result 
			, MenuItem
		>::Result list;
	};

	template<>struct TopMenu<Setting>
	{
		typedef TL::MkTlst<
			 SubMenu<DiscretePlate>	
			 , SubMenu<AnalogPlate__>
			, MenuItem<ComPort__>
			, MenuItem<ColorItems>
			, MenuItem<OffsetsSens>
			, MenuItem<AdditionalParams>
		>::Result list;		
	};
//--------------------------------------------------------------------------------------------
	struct TestUnit{};
	MENU_TEXT(L"Тест", TopMenu<TestUnit>)
		//----------------------------------------------------
	struct ModulePosition : ModulePositionDlg{};
    struct RotationScanningDevice_: RotationScanningDevice{};

	struct ThicknessComTest_: ThicknessComTest{};
	struct ASUComTest_: ASUComTest{};
	
	struct Test502Window_: Test502Window{};
    struct AsyncChannels_: AsyncChannels{};

	MENU_ITEM(L"Положение сканирующих устройств", ModulePosition)
	MENU_ITEM(L"Вращение сканирующего устройства", RotationScanningDevice_)

	MENU_ITEM(L"Тестирование связи с АСУ", ASUComTest_)
	MENU_ITEM(L"Тестирование связи с модулем толщиномера", ThicknessComTest_)

	MENU_ITEM(L"Тестирование измерительных каналов платы 502", Test502Window_)
	MENU_ITEM(L"Тестирование дополнительных каналов платы 502", AsyncChannels_)

	
	struct Digit502Channel_: Digit502Channel{};
	MENU_ITEM(L"Тестирование дискретных каналов платы 502", Digit502Channel_)

		template<>struct TopMenu<TestUnit>
	{
		typedef TL::MkTlst<
			MenuItem<ModulePosition>
			, MenuItem<RotationScanningDevice_>
			, Separator<0>
			, MenuItem<ThicknessComTest_>
			, MenuItem<ASUComTest_>
			, Separator<1>
			, MenuItem<Test502Window_>
			, MenuItem<AsyncChannels_>
			, MenuItem<Digit502Channel_>
			, Separator<2>
			, MenuItem<IOportsView>
			, Separator<3>
			, MenuItem<LaunchExternalProgram>
		>::Result list;		
	};
	// ----------------------------------------------------------------------------------------------------
	struct MainAbout: AboutWindowDlg{};//{static void Do(HWND h){zprint("");}};
	MENU_TEXT(L"О программе", TopMenu<MainAbout>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	MENU_ITEM(L"О программе", MainAbout)

	template<>struct TopMenu<MainAbout>
	{
		typedef TL::MkTlst<
			MenuItem<MainAbout>
		>::Result list;
	};
	// --------------------------------------------------------------------------------------------------------- 
	/// \brief	Добавить меню пример использования
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
		, TopMenu<TestUnit>
		, TopMenu<MainAbout>
	>::Result MainMenu;	
	}

#endif
