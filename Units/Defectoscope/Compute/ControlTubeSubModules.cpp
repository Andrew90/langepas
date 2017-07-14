#include "stdafx.h"
#include "ControlTubeSubModules.h"
#include "ControlMode.h"
#include "Log\LogBuffer.h"
#include "Automat.h"
#include "App\AppBase.h"
#include "App\App.h"
#include "templates\typelist.hpp"
#include "Automat.hpp"
#include "ASU\Asu.h"
#include "Compute\AppKeyHandler.h"
#include "Windows\MainWindow.h"
//#include "Compute\Compute.h"

#define TEST_MESS(n) if(TEST_IN_BITS(On<n>)){Log::Mess<LogMess::n##Mess>(); /*throw ExceptionAlarm();*/return;}

void TestCoilTemperature()
{
	L502RangeTable::TItems &range = Singleton<L502RangeTable>::Instance().items;
	L502OffsetsTable::TItems &offs = Singleton<L502OffsetsTable>::Instance().items;
	double t1 = 0;
	double t2 = 0;
	if(!(unit502.ReadAsync(offs.get<Offset<Temperature, 0>>().value, range.get<Range<Temperature, 0>>().value, t1)
		|| unit502.ReadAsync(offs.get<Offset<Temperature, 1>>().value, range.get<Range<Temperature, 1>>().value, t2))
		)
	{
		Log::Mess<LogMess::AnalogBoardFailure>();
		throw AutomatN::ExceptionAlarm();
	}
	TemperatureTresholdsable::TItems &tresh = Singleton<TemperatureTresholdsable>::Instance().items;

	double tresh1 = tresh.get< Tresh<Temperature, 0>>().value;
	double tresh2 = tresh.get< Tresh<Temperature, 1>>().value;

	if(t1 > tresh1)
	{
		Log::Mess<LogMess::TemperatureCoilSolenoidExceeded0>(t1); throw AutomatN::ExceptionAlarm();
		throw AutomatN::ExceptionAlarm();
	}
	if(t2 > tresh2)
	{
		Log::Mess<LogMess::TemperatureCoilSolenoidExceeded1>(t2); throw AutomatN::ExceptionAlarm();
		throw AutomatN::ExceptionAlarm();
	}
}

void CheckDemagnetizeModule()
{

}

void SettingWorkingPositionControlModules()
{	
	using namespace AutomatN;
	/// \brief Проверка наличия трубы в модулях
	TEST_MESS(iSQ1pr)
		TEST_MESS(iSQ2pr)
		TEST_MESS(iSQ1po)
		TEST_MESS(iSQ2po)
		TEST_MESS(iSQ1t	)
		TEST_MESS(iSQ2t	)

		OnTheJobTable::TItems &job = Singleton<OnTheJobTable>::Instance().items;

	bool bLong = false, bThick = false;

	if(bLong = job.get<OnTheJob<Long> >().value)
	{
		OUT_BITS(On<oPR_RP>);
	}
	else
	{
		OUT_BITS(On<oPR_OP>);
	}

	if(bThick = job.get<OnTheJob<Thick> >().value)
	{
		OUT_BITS(On<oT_RP>);
	}
	else
	{
		OUT_BITS(On<oT_OP>);
	}

	OUT_BITS(On<oPO_RP>);
	Sleep(500);
	OUT_BITS(Off<oPO_RP>, Off<oPR_RP>, Off<oPR_OP>, Off<oT_RP>, Off<oT_OP>);

	InputBit1Table::TItems &bits = Singleton<InputBit1Table >::Instance().items;

	unsigned inp = bits.get<iRPpo>().value
		| (bLong ? bits.get<iRPpr>().value: bits.get<iOPpr>().value)
		| (bThick ? bits.get<iRPt>().value: bits.get<iOPt>().value)
		;
	/// \brief ожидание когда модули примут рабочее положение 
	unsigned t = 0;
	for(int i = 0; i < 300; ++i)
	{
		unsigned t = device1730_1.Read();
		if(t == (t & inp)) break;
		Sleep(100);
	}
	/// \brief если в течннии 300 * 100 микросекунд модули не приняли рабочее положение - выходим
	if(t != (t & inp))
	{
		Log::Mess<LogMess::ModulesInNon_OperatingPosition>();
		throw AutomatN::ExceptionAlarm();
	}
}

void TransferParametersThicknessModule()
{
	using namespace AutomatN;
	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thick>>().value)
	{
		OUT_BITS(On<oT_Work>);
		Sleep(300);
		ThresholdsTable::TItems &tresh = Singleton<ThresholdsTable>::Instance().items;
		int res = Communication::Thick::TransferControlParameters(
			comPort
			, Singleton<ParametersTable>::Instance().items.get<DiametrTube>().value
			, tresh.get<BorderDefect<Thick>>().value
			, tresh.get<BorderKlass2<Thick>>().value
			, tresh.get<BorderKlass3<Thick>>().value
			);
		switch(res)
		{
		case Communication::ok: return;
		case Communication::time_overflow:  Log::Mess<LogMess::time_overflow>();
		case Communication::error_crc    :  Log::Mess<LogMess::error_crc>();
		case Communication::error_count  :  Log::Mess<LogMess::error_count>();
			throw AutomatN::ExceptionAlarm();
		}

		AND_BITS(
			On<iReadyT>	  /// \brief ожидание готовности толщиномера
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(); //кнопка начала измерений
	}
}

/// \brief подготовка частотного преобразователя
void FrequencyInverterPreparation()
{
	using namespace AutomatN;
	/// проверить биты
	if(TEST_IN_BITS(Off<iPCH_B>)){Log::Mess<LogMess::iPCH_B_OFF>();throw AutomatN::ExceptionAlarm();}
	if(TEST_IN_BITS(Off<iPCH_RUN>)){Log::Mess<LogMess::iPCH_RUN_OFF>();throw AutomatN::ExceptionAlarm();}	
}

void FrequencyInverterRun()
{
	/// включить частотник
	OutputBit1Table::TItems &outputBit = Singleton<OutputBit1Table>::Instance().items;
	RotationalSpeedTable::TItems &speed = Singleton<RotationalSpeedTable>::Instance().items;
	unsigned outBits = outputBit.get<oPowerPCH>().value
		| (speed.get<SpeedBit<oRL>>().value ? outputBit.get<oRL>().value: 0)
		| (speed.get<SpeedBit<oRM>>().value ? outputBit.get<oRM>().value: 0)
		| (speed.get<SpeedBit<oRH>>().value ? outputBit.get<oRH>().value: 0)
		| outputBit.get<oSTF>().value
	//	| outputBit.get<oRP>().value	 
		;
	device1730_1.AddBits(outBits);
}

///TODO Установка режима работы контроллера пневмооборудования
void SettingOperatingModeAirConditioningController()
{
	using namespace AutomatN;
	OutputBit1Table::TItems &outputBit = Singleton<OutputBit1Table>::Instance().items;
	OnTheJobTable::TItems &workModule =  Singleton<OnTheJobTable>::Instance().items;
	unsigned outBits = outputBit.get<oWorkPO>().value
		| (workModule.get< OnTheJob<Long>>().value ? outputBit.get<oWorkPR>().value: 0)
		| (workModule.get< OnTheJob<Thick>>().value ? outputBit.get<oWorkT>().value: 0)
		;

	device1730_1.AddBits(outBits);

	Log::Mess<LogMess::iWork_pnevmoWait>();
	AND_BITS(
		On<iWork_pnevmo>	  /// \brief ожидание готовности толщиномера
		, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
		)(10000); 
}

///TODO Очистка экрана
void CleaningScreen()
{
	//Compute::Clear();
	app.mainWindow.ClearCharts();
}

///TODO Запрос номера трубы
void RequestPipeNumber(char (&numberTube)[9])
{
	using namespace AutomatN;
	while(true)
	{
		Log::Mess<LogMess::RequestPipeNumber>();
		int res = Communication::Asu::RequestInformationAboutPipe(comPort, numberTube);
		switch(res)
		{
		case Communication::ok: break;
		case Communication::time_overflow:  Log::Mess<LogMess::time_overflow>();
			throw AutomatN::ExceptionAlarm();
		case Communication::error_crc    :  Log::Mess<LogMess::error_crc>();
			throw AutomatN::ExceptionAlarm();
		case Communication::error_count  :  Log::Mess<LogMess::error_count>();
			throw AutomatN::ExceptionAlarm();
		}
		numberTube[8] = '\0';
		if(0 != strncmp(numberTube, "00000000", 8))
		{
			wchar_t buf[64];
			wsprintf(buf, L"Номер трубы %S", numberTube);
			app.StatusBar(App::status_bar_number_tube, buf);
			break;
		}
		else
		{
			AppKeyHandler::Continue();	/// включили кнопку продолжить
			AND_BITS(
				Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				, Ex<ExceptionContinue>	 /// \brief Выход по кнопке продолжить
				)(); 
			dprint("continue\n");
		}
	}
	dprint("OK cont\n");
}

///TODO включить размагничивание
void EnableDemagnetization()
{
}
///TODO включить размагничивание
void DisableDemagnetization()
{
}

#undef TEST_MESS