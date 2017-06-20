#pragma once
#include "templates/typelist.hpp"
#include "1730/Device1730.h"
#include "Compute\Unit502.h"

#include "Config.h"

//#ifdef DEBUG_ITEMS
//#include "Emulator\Emulator.h"
//extern Emulator emulator;
//#endif


#define DEFECT_APP
struct Long{};
struct Cross{};
struct Thick{};
struct ViewInterrupt{};
struct Solid{};
class MainWindow;

class App
{
public:
	MainWindow &mainWindow;
public:
	static const int duration_collections = 60;
	static const int frequency_rate = 10000;
	static const int count_zones = 300;
	//static const int count_sensors = 8;
	static const int count_cross_sensors = 12;
	static const int count_long_sensors = 4;
	static const int count_Thick_sensors = 3;
	static const int zone_length = 50;
	static const int count_frames = frequency_rate * duration_collections;
	static const int count_time_count_down = 60 * 100;
	static const int zone_filter_offset = 100;
	static bool measurementOfRunning;
	static bool isStandard;
	App();
	void Init();
	void Destroy();
	static const wchar_t *TitleApp(){return L"XXX \"xxxxxxxxxx\"";}
	static const wchar_t *Salt(){return L"Defect";};
};

extern App app;
extern Device1730 device1730;
extern Unit502 unit502;





