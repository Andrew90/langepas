#include "stdafx.h"
#include "App.h"
#include "Windows\MainWindow.h"
#include "window_tool/EmptyWindow.h"
#include "window_tool/WindowsPosition.h"
#include "DataItem/ConstData.h"
#include "window_tool\HookKey.h"
#include "ChangeTypeSize.h"
#include "Compute/AppKeyHandler.h"
#include "Compute\Automat.h"
#include "Compute/Compute.h"
#include "PerformanceCounter\PerformanceCounter.h"
#include "App/Config.h"
#include "SolidGroupAlgoritm\ComputeSolidGroup.h"
#include "StoredData\PacketBase.h"

bool App::measurementOfRunning = false;
bool App::isStandard = false;

App::App()
	: mainWindow(Singleton<MainWindow>::Instance())
{}

void App::Init()
{
	Performance::Init();
	//инициализировать параметры программы
	AppBase().Init();
	PackageBase().Init();
	/////////////////////////////

	ConstData::Init();

	RECT r;
	WindowPosition::Get<MainWindow>(r);
	HWND h = WindowTemplate(&mainWindow, (wchar_t *)App::TitleApp(), r.left, r.top, r.right, r.bottom);
	ShowWindow(h, SW_SHOWNORMAL);

	mainWindow.select.SetCommandHandler(ChangeTypeSize);

	StartKeyHook(h, &AppKeyHandler::KeyPressed);

	bool ok = true;

	if(!device1730.Init(Singleton<NamePlate1730ParametersTable>::Instance().items.get<NamePlate1730>().value))
	{
		MessageBox(h, L"Не могу инициировать плату 1730", L"Ошибка !!!", MB_ICONERROR);
		ok = false;
	}

	if(ok && !unit502.Init())
	{
		MessageBox(h, L"Не могу инициировать плату 502", L"Ошибка !!!", MB_ICONERROR);
		ok = false;
	}

	if(ok && unit502.SetupParams())
	{
		MessageBox(h, L"Не могу загрузить параметры в плату 502", L"Ошибка !!!", MB_ICONERROR);
		ok = false;
	}
#ifdef DEBUG_ITEMS
	ok = true;
#endif
	if(ok)Automat::Init();

	Compute::Init();	
	NameParam::type_value &nameParam = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	Singleton<ComputeSolidGroup>::Instance().Load(nameParam);
}

void App::Destroy()
{
	Automat::Exit();
	Compute::Destroy();
}

App app;
Device1730 device1730;
Unit502 unit502;









