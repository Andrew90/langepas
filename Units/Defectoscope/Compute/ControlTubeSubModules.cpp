#include "stdafx.h"
#include <limits>
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
#include "MessageText\StatusMessages.h"
#include "lir\SubLir.h"
#include "DataItem\DataItem.h"
#include "App\Config.h"
#include "window_tool\EmptyWindow.h"
#include "MessageText\ListMess.hpp"

#define TEST_MESS(n) if(TEST_IN_BITS(On<n>)){Log::Mess<LogMess::n##Mess>(); /*throw ExceptionAlarm();*/return;}

void TestCoilTemperature()
{
	L502RangeTable::TItems &range = Singleton<L502RangeTable>::Instance().items;
	L502OffsetsTable::TItems &offs = Singleton<L502OffsetsTable>::Instance().items;
	L502ModeTable::TItems &mode = Singleton<L502ModeTable>::Instance().items;
	double t1 = 0;
	double t2 = 0;
	if(!(unit502.ReadAsync(offs.get<Offset<Temperature, 0>>().value, mode.get<Mode502<Temperature, 0>>().value, range.get<Range<Temperature, 0>>().value, t1)
		|| unit502.ReadAsync(offs.get<Offset<Temperature, 1>>().value, mode.get<Mode502<Temperature, 1>>().value, range.get<Range<Temperature, 1>>().value, t2))
		)
	{
		Log::Mess<LogMess::AnalogBoardFailure>();
		throw AutomatN::ExceptionAlarm();
	}
	AdditionalParams502Table::TItems &tresh = Singleton<AdditionalParams502Table>::Instance().items;

	double tresh1 = tresh.get< Tresh<Temperature, 0>>().value;
	double tresh2 = tresh.get< Tresh<Temperature, 1>>().value;

#ifdef EMUL
	return;
#endif

	if(t1 > tresh1)
	{
		Log::Mess<LogMess::TemperatureCoilSolenoidExceeded0>(t1); throw AutomatN::ExceptionAlarm();
		throw AutomatN::ExceptionAlarm();
	}
	if(t2 > tresh2)
	{
		Log::Mess<LogMess::TemperatureCoilSolenoidExceeded1>(t2);
		throw AutomatN::ExceptionAlarm();
	}
}
#undef max
#undef min
void CheckDemagnetizeModule()
{
	bool b = unit502.SetupParams();
	if(!b)
	{
		Log::Mess<LogMess::unit502SetupParams>(); 
		throw AutomatN::ExceptionAlarm();
	}
	for(;b;)
	{
		Log::Mess<LogMess::demagnetizationTesting>();
		unit502.BitOut(Singleton<L502OffsetsDigitTable>::Instance().items.get<Out502<start_x>>().value, true);
		unit502.Start();
		Sleep(1000);
		for(int i = 0; i < 5; ++i)
		{
			Sleep(200);
			unit502.Read();
		}
		unit502.Stop();
		unit502.BitOut(Singleton<L502OffsetsDigitTable>::Instance().items.get<Out502<start_x>>().value, false);

		SubLir &lir = Singleton<SubLir>::Instance();

		int offs = lir.currentSamples / 10;

		double min =  1e99;//std::numeric_limits<double>::max();
		double max =  -1e99;//std::numeric_limits<double>::min();

		double *d = Singleton<ItemData<Solid>>::Instance().ascan[1];

		//for(int i = offs; i < lir.currentSamples - offs; ++i)
		for(int i = 0; i < offs; ++i)
		{
			if(d[i] > max) max = d[i];
			else if(d[i] < min) min = d[i];
		}

	//	max = max > 0 ? max: -max;
	//	max = min > 0 ? min: -min;

		double z = max - min;

		//z = z > 0? z: -z;

		double t = (z > 0) ? (max + min) / z : 0;

		double tresh = Singleton<AdditionalParams502Table>::Instance().items.get<Tresh<Magn, 0>>().value;
		///если амплитуда меньше порога, либо отличие больше 20 процентов test
#ifndef EMUL
		if(t < 0) t = -t;
		if(max < tresh || -min < tresh || t > 0.2)
		{

			if(IDNO == MessageBox(app.mainWindow.hWnd, L"Повторить тест?", L"Ошибка !!!", MB_ICONERROR | MB_YESNO))
			{
				Log::Mess<LogMess::demagnetizationNotCorrect>(); 
				throw AutomatN::ExceptionAlarm();
			}
		}
		else
		{
			return;
		}
#else
		return;
#endif
	}
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
	Log::Mess<LogMess::SettingOperatingPositionControl>();

#ifdef EMUL
	return;
#endif

	unsigned t = 0;
	for(int i = 0; i < 300; ++i)
	{
		t = device1730_1.Read();
		if(inp == (t & inp)) break;
		Sleep(100);
	}
	/// \brief если в течннии 300 * 100 микросекунд модули не приняли рабочее положение - выходим из сбора данных
	if(inp != (t & inp))
	{
		Log::Mess<LogMess::ModulesInNon_OperatingPosition>();
		throw AutomatN::ExceptionAlarm();
	}
}

void it_does_not_work_without_it_Sleep(DWORD d)
{
	Sleep(d);
}

void TransferParametersThicknessModule()
{
	using namespace AutomatN;

#ifdef EMUL
	return;
#endif

	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thick>>().value)
	{
		for(;;)
		{
			Log::Mess<LogMess::transferControlParametersThicknessGauge>();
			OUT_BITS(On<oT_Work>);
			it_does_not_work_without_it_Sleep(3500);
			ThresholdsTable::TItems &tresh = Singleton<ThresholdsTable>::Instance().items;
			double defect = tresh.get<BorderDefect<Thick>>().value;
			double klass2 = tresh.get<BorderKlass2<Thick>>().value;
			double klass3 = tresh.get<BorderKlass3<Thick>>().value;
			int res = Communication::Thick::TransferControlParameters(
				comPort
				, Singleton<ParametersTable>::Instance().items.get<DiametrTube>().value
				, defect
				, klass2
				, klass3
				);
			switch(res)
			{
			case Communication::ok: 
				{
					Log::Mess<LogMess::waitingThicknessGauge>();
					AND_BITS(
						On<iReadyT>	  /// \brief ожидание готовности толщиномера
						, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
						)(); //кнопка начала измерений
				}
				return;
			case Communication::time_overflow:  Log::Mess<LogMess::time_overflow>();break;
			case Communication::error_crc    :  Log::Mess<LogMess::error_crc>();break;
			case Communication::error_count  :  Log::Mess<LogMess::error_count>();break;
			}
			if(IDNO == MessageBox(app.mainWindow.hWnd, L"Повторить передачу?", L"Ошибка !!!", MB_ICONERROR | MB_YESNO))
			{
				Log::Mess<LogMess::emergencyExit>();
				throw AutomatN::ExceptionAlarm();
			}
		}
	}
}

void GetDataFromThicknessModule()
{
	using namespace AutomatN;

	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thick>>().value)
	{
		ItemData<Thick> &data = Singleton<ItemData<Thick>>::Instance();
		Log::Mess<LogMess::waitingThicknessResult>();
		unsigned short zones[65];
		bool receiveDataOk = false;
		double brak = 0, class2 = 0, class3 = 0;
#ifndef EMUL
		AND_BITS(
			On<iResultT>
			, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
			)(120000); 		
		for(int i = 0; i < 99; ++i)
		{
			int res =  Communication::Thick::RequestControlResult(
				comPort, brak, class2, class3, data.currentOffsetZones, zones 
				);

			switch(res)
			{
			case Communication::ok: receiveDataOk = true; goto LOOP;
			case Communication::time_overflow:  Log::Mess<LogMess::time_overflow>();break;
			case Communication::error_crc    :  Log::Mess<LogMess::error_crc>();break;
			case Communication::error_count  :  Log::Mess<LogMess::error_count>();break;
			}
			if(!receiveDataOk)
			{
				//AND_BITS(
				//	Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				//	)(3000); 
				if(IDNO == MessageBox(app.mainWindow.hWnd, L"Повторить запрос?", L"Ошибка !!!", MB_ICONERROR | MB_YESNO))
				{
					Log::Mess<LogMess::emergencyExit>();
					throw AutomatN::ExceptionAlarm();
				}
			}
		}
LOOP:
#else
		data.currentOffsetZones = 53;
		short t = 42;
		for(int i = 0; i < 53; ++i)
		{
			zones[i] = t;
			t += 1;
			if(t > 62) t = 40;
		}
#endif
		if(receiveDataOk)
		{
			ThresholdsTable::TItems &tresh = Singleton<ThresholdsTable>::Instance().items;
			double brak =   10 * tresh.get<BorderDefect<Thick>>().value;
			double class2 = 10 * tresh.get<BorderKlass2<Thick>>().value;
			double class3 = 10 * tresh.get<BorderKlass3<Thick>>().value;

			//brak   /= 10;
			//class2 /= 10;
			//class3 /= 10;

			dprint(
				"brak    %f\n"\
				"class2	 %f\n"\
				"class3	 %f\n"

				, brak  
				, class2
				, class3
				);



			for(int i = 0; i < App::count_zones; ++i)
			{
				double t = data.buffer[i] = 0.1 * zones[i];
				if(0 == zones[i])
				{
					data.status[i] = STATUS_ID(Undefined);//StatusId<Clr<BorderDefect<Thick>>>();
					data.buffer[i] = 10;
				}
				//else if(brak >= t)
				//{
				//	data.status[i] = STATUS_ID(BorderDefect<Thick>);//StatusId<Clr<BorderDefect<Thick>>>();
				//}
				//else if(class3 >= t)
				//{
				//	data.status[i] = STATUS_ID(BorderKlass3<Thick>);//StatusId<Clr<BorderKlass3<Thick>>>();
				//}
				//else if(class2 >= t)
				//{
				//	data.status[i] = STATUS_ID(BorderKlass2<Thick>);//StatusId<Clr<BorderKlass2<Thick>>>();
				//}
				else if(100 < zones[i])
				{
					data.status[i] = STATUS_ID(DeathZone);//StatusId<Clr<BorderKlass2<Thick>>>();
				}
				else if(class2 < zones[i])
				{
					data.status[i] =  STATUS_ID(Nominal);
				}
				else if(class3 < zones[i])
				{
					data.status[i] = STATUS_ID(BorderKlass2<Thick>);//StatusId<Clr<BorderKlass3<Thick>>>();
				}
				else if(brak < zones[i])
				{
					data.status[i] = STATUS_ID(BorderKlass3<Thick>);
				}
				else
				{
					//data.status[i] =  STATUS_ID(Nominal);//StatusId<Clr<Nominal>>();
					data.status[i] = STATUS_ID(BorderDefect<Thick>);
				}
			//	dprint("thick %d   stat %d val %f\n", i, data.status[i], data.buffer[i]);
			}
		}
		dprint("len tube %d\n", data.currentOffsetZones);
	}
}

/// \brief подготовка частотного преобразователя
void FrequencyInverterPreparation()
{
	using namespace AutomatN;
	/// проверить биты
#ifndef EMUL
	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Long>>().value)
	{
		if(TEST_IN_BITS(Off<iPCH_B>)){Log::Mess<LogMess::iPCH_B_OFF>();throw AutomatN::ExceptionAlarm();}
		if(TEST_IN_BITS(Off<iPCH_RUN>)){Log::Mess<LogMess::iPCH_RUN_OFF>();throw AutomatN::ExceptionAlarm();}
	}
#endif
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
		;
	device1730_1.AddBits(outBits);
}

void FrequencyInverterRunWork()
{
	/// включить частотник
	OutputBit1Table::TItems &outputBit = Singleton<OutputBit1Table>::Instance().items;
	RotationalSpeedTable::TItems &speed = Singleton<RotationalSpeedTable>::Instance().items;
	unsigned outBits = outputBit.get<oPowerPCH>().value
		| (speed.get<SpeedBitWork<oRL>>().value ? outputBit.get<oRL>().value: 0)
		| (speed.get<SpeedBitWork<oRM>>().value ? outputBit.get<oRM>().value: 0)
		| (speed.get<SpeedBitWork<oRH>>().value ? outputBit.get<oRH>().value: 0)
		| outputBit.get<oSTF>().value
		;

	unsigned msk =
		  (speed.get<SpeedBit<oRL>>().value ? outputBit.get<oRL>().value: 0)
		| (speed.get<SpeedBit<oRM>>().value ? outputBit.get<oRM>().value: 0)
		| (speed.get<SpeedBit<oRH>>().value ? outputBit.get<oRH>().value: 0)
		;

	//device1730_1.AddBits(outBits);
	device1730_1.WriteOutput(outBits, msk);
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

#ifdef EMUL
	return;
#endif

	AND_BITS(
		On<iWork_pnevmo>	  /// \brief iWork_pnevmo - ВКЛЮЧЕНО
		, Off<iRevers_pnevmo>	/// \brief iRevers_pnevmo - ОТКЛЮЧЕНО
		, Off<iError_pnevmo >	/// \brief iError_pnevmo - ОТКЛЮЧЕНО
		, Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
		, Proc<ExceptionAl<LogMess::iWork_pnevmAlarm>>	/// если нет готовности - выход
		)(10000);  /// \brief ожидание 10 сек
}

///TODO Очистка экрана
void CleaningScreen()
{
	//Compute::Clear();
	app.mainWindow.ClearCharts();
}

void UpdateScreen()
{
	RepaintWindow(app.mainWindow.hWnd);
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
			//	AppKeyHandler::RunContine();	/// включили кнопку продолжить
			AND_BITS(
				Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
				, Ex<ExceptionContinue>	 /// \brief Выход по кнопке продолжить
				//	, Ex<ExceptionRun>
				)(); 
			dprint("continue\n");
		}
	}
	dprint("OK cont\n");
}

///TODO включить размагничивание
void EnableDemagnetization()
{
	if(!unit502.BitOut(Singleton<L502OffsetsDigitTable>::Instance().items.get<Out502<start_x>>().value, true))
	{
		dprint("error "__FUNCTION__"\n");
	}
}
///TODO включить размагничивание
void DisableDemagnetization()
{
	if(!unit502.BitOut(Singleton<L502OffsetsDigitTable>::Instance().items.get<Out502<start_x>>().value, false))
	{
		dprint("error "__FUNCTION__"\n");
	}
}

void WorkACS(char (&numberTube)[9])
{
	using namespace AutomatN;
	if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<ACS>>().value)
	{
		ResultData &resultData = Singleton<ResultData>::Instance();
		Log::Mess<LogMess::transferResultControlAutomatedControlSystem>();
		int attempt = 0;
		while(true)
		{
			int res = Communication::Asu::SendData(comPort,
				numberTube
				, 0, 0, 0, 0, 0, 0
				, resultData.currentOffsetZones
				, resultData.cutZone0
				, resultData.cutZone1
				, resultData.resultCommon
				, resultData.solidGroup
				);

			switch(res)
			{
			case Communication::ok: break;
			case Communication::time_overflow:  Log::Mess<LogMess::time_overflow>();
				throw AutomatN::ExceptionAlarm();
			case Communication::error_crc    :  Log::Mess<LogMess::error_crc>();
				throw AutomatN::ExceptionAlarm();
			case Communication::error_count  :  Log::Mess<LogMess::error_count>();
				if(++attempt < 2)
				{
					continue;
				}
				else
				{
					Log::Mess<LogMess::contineRun>();
					AppKeyHandler::RunContine();	/// включили кнопку продолжить
					int pushButton = AND_BITS(
						Ex<ExceptionStop>	 /// \brief Выход по кнопке стоп
						, Ex<ExceptionContinue>	 /// \brief Выход по кнопке продолжить
						, Ex<ExceptionRun>
						)(); 

					switch(pushButton)
					{
					case 1: return;
					case 2: continue;
					}
				}
			}
		}
	}
}

void StoredData(bool tubeMode)
{
	if(tubeMode)
	{
		Log::Mess<LogMess::storedDataBase>();
	}
}

#undef TEST_MESS