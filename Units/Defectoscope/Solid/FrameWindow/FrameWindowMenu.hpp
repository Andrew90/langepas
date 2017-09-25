#pragma once

#include "window_tool\message.h"
#include "window_tool\MenuApi.h"
#include "templates\templates.hpp"
#include "window_tool\WindowsPosition.h"
#include "AddThresholdsWindow\AddThresholdsWindow.h"
#include "Dialogs\Dialogs.h"
#include "AddThresholdsWindow\TreshWindow.h"
#include "MainWindow\PointsWindow.h"
#include "Dates\ComputeSolid.h"
#include "window_tool\EmptyWindow.h"

namespace FrameWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct MainExit
	{
		static void Do(HWND h)
		{
			TClose c = {h, WM_CLOSE, 0, 0};
			SendMessage(MESSAGE(c));
		}
	};
	struct Compute__     
	{
		static void Do(HWND h)
		{
			ComputeSolid::Recalculation();
			FrameWindow &f = Singleton<FrameWindow>::Instance();
			RepaintWindow(f.thresholdsViewer.hWnd);
			RepaintWindow(f.frameViewer.hWnd);
		}
	};
	MENU_ITEM(L"Выход", MainExit)
	MENU_ITEM(L"Перерасчёт", Compute__)

		template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<Compute__>
			, Separator<0>
			, MenuItem<MainExit>
		>::Result list;
	};

	struct MainOptionUnits{};
	MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	struct ThresholdsMenuItem{static void Do(HWND){AddThresholdWindow::Show();}};
	struct OffsetsMenuItem{static void Do(HWND){TreshWindow::Show();}};
	struct SolidDlg_: SolidDlg{};
	struct AddTresholdSolidGroup{static void Do(HWND){Singleton<FrameWindow>::Instance().AddTreshold();}};

	MENU_ITEM(L"Oбразцы", ThresholdsMenuItem)
	MENU_ITEM(L"Добавить образец", AddTresholdSolidGroup)
	MENU_ITEM(L"Смещения", OffsetsMenuItem)
	MENU_ITEM(L"Сохранить координаты окна", WindowPositionDlg<FrameWindow>)
	MENU_ITEM(L"Цифровой фильтр", SolidDlg_)

	template<>struct TopMenu<MainOptionUnits>
	{
		typedef TL::MkTlst<
			MenuItem<SolidDlg_>
			, Separator<1>
			, MenuItem<ThresholdsMenuItem>
			, MenuItem<AddTresholdSolidGroup>
			, MenuItem<OffsetsMenuItem>
			, Separator<0>
			, MenuItem<WindowPositionDlg<FrameWindow>>
		>::Result list;
	};

	struct GraphUnits{};
	MENU_TEXT(L"График", TopMenu<GraphUnits>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	struct PointsOffsets: ThresholdDlg{};//{static void Do(HWND){}};
	struct FirstSignal: PrimarySignalDlg{};//{static void Do(HWND){}};
	struct PointsWindowDlg  {static void Do(){PointsWindow::Show();}};

	MENU_ITEM(L"Точки смещения", PointsOffsets)
	MENU_ITEM(L"Первичный сигнал", FirstSignal)

	MENU_ITEM(L"Точки", PointsWindowDlg)

	template<>struct TopMenu<GraphUnits>
	{
		typedef TL::MkTlst<
			MenuItem<PointsOffsets>
			, MenuItem<FirstSignal>
			, Separator<0>
			, MenuItem<PointsWindowDlg>
		>::Result list;
	};

	typedef TL::MkTlst<
		TopMenu<MainFile>
		, TopMenu<MainOptionUnits>
		, TopMenu<GraphUnits>
	>::Result items_list;	
}