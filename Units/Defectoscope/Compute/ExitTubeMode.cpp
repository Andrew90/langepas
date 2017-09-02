#include "stdafx.h"
#include "ControlMode.h"
#include "Automat.hpp"
#include "Log\LogBuffer.h"

namespace Mode
{
	using namespace AutomatN;

	void ExitTube(Data &)
	{
		dprint("ExitTube\n");
		Log::Mess<LogMess::ExitTube>();
		{
			unsigned t = device1730_2.Read();
			InputBit2Table::TItems &inputBit2 = Singleton<InputBit2Table>::Instance().items;
			if(0 == (t & inputBit2.get<iZU>().value))
			{
				Log::Mess<LogMess::AlarmExitControlCircuitBitIn>();
				return;
			}
		}

		bool isCross = TEST_IN_BITS(On<iSQ1pr>) || TEST_IN_BITS(On<iSQ2pr>);
		bool isLong = TEST_IN_BITS (On<iSQ1po>) || TEST_IN_BITS(On<iSQ2po>);
		bool isThick = TEST_IN_BITS(On<iSQ1t> ) || TEST_IN_BITS(On<iSQ2t>);
		bool isSolid = TEST_IN_BITS(On<iSQ1DM>) || TEST_IN_BITS(On<iSQ2DM>);

		if(isCross) Log::Mess<LogMess::TubeInCrossModule>();
		if(isLong )	Log::Mess<LogMess::TubeInLongModule>();
		if(isThick)	Log::Mess<LogMess::TubeInThickModule>();
		if(isSolid)	Log::Mess<LogMess::TubeInSolid>();

		if(!(isCross || isLong || isThick || isSolid))
		{
			Log::Mess<LogMess::NoPipesOnLine>();
			return;
		}

		InputBit1Table::TItems &inputBit1 = Singleton<InputBit1Table>::Instance().items;
		unsigned positionModules1 = 
			inputBit1.get<iRPpr>().value
			| inputBit1.get<iOPpr>().value
			| inputBit1.get<iRPpo>().value
			| inputBit1.get<iOPpo>().value
			| inputBit1.get<iRPt>().value
			| inputBit1.get<iOPt>().value
			;

		if(isThick)
		{
			Log::Mess<LogMess::RotationTube>();
			AND_BITS(On<iControlT>, Ex<ExceptionStop>)(60000);
			positionModules1 |= inputBit1.get<iReadyT>().value;
		}

		if(isLong)
		{
			Log::Mess<LogMess::CheckStatusFrequencyConverter>();
			AND_BITS(On<iPCH_B>, On<iPCH_RUN>, Ex<ExceptionStop>)(10000);
		}

		OutputBit1Table::TItems &outputBit = Singleton<OutputBit1Table>::Instance().items;
		RotationalSpeedTable::TItems &speed = Singleton<RotationalSpeedTable>::Instance().items;
		unsigned outBits = outputBit.get<oPowerPCH>().value
			| (speed.get<SpeedBit<oRL>>().value ? outputBit.get<oRL>().value: 0)
			| (speed.get<SpeedBit<oRM>>().value ? outputBit.get<oRM>().value: 0)
			| (speed.get<SpeedBit<oRH>>().value ? outputBit.get<oRH>().value: 0)
			| outputBit.get<oSTF>().value
			| outputBit.get<oRP>().value
			;
		device1730_1.AddBits(outBits);

		InputBit2Table::TItems &inputBit2 = Singleton<InputBit2Table>::Instance().items;
		unsigned statesUnit2 =  inputBit2.get<iZU>().value
			|  inputBit2.get<iPCH_B>().value
			;

		unsigned mskSQ1 = 
			inputBit1.get<iSQ1pr>().value
			| inputBit1.get<iSQ2pr>().value
			| inputBit1.get<iSQ1po>().value
			| inputBit1.get<iSQ2po>().value
			| inputBit1.get<iSQ1t	>().value
			| inputBit1.get<iSQ2t	>().value
			;

		unsigned mskSQ2 = 
			inputBit2.get<iSQ1DM>().value
			| inputBit2.get<iSQ2DM>().value
			;

		unsigned t = device1730_1.Read();
		positionModules1 = t & positionModules1;

		for(int i = 0; i < 900; ++i)
		{
			t = device1730_2.Read();
			if(statesUnit2 != (t & statesUnit2))
			{
				if(0 == (t & inputBit2.get<iZU>().value))Log::Mess<LogMess::AlarmExitControlCircuitBitIn>();
				if(0 == (t & inputBit2.get<iPCH_B>().value))Log::Mess<LogMess::AlarmExitRunBitIn>();
				return;
			}

			bool sqDM = 0 == (t & mskSQ2);

			t = device1730_1.Read();
			if(positionModules1 != (t & positionModules1))
			{
				if(isThick && 0 == (t & inputBit1.get<iReadyT>().value))
				{
					Log::Mess<LogMess::ThicknessGaugeTookOffSignal>();
				}
				else
				{
					Log::Mess<LogMess::CompletionChangeInPositionModule>();
				}
				return;
			}

			if(sqDM && 0 == (t & mskSQ1))
			{
				Log::Mess<LogMess::PastureCompleted>();
				return;
			}

			if(WAIT_TIMEOUT != WaitForSingleObject(Ex<ExceptionStop>::handle , 100))
			{
				throw ExceptionStop();//выход  по кнопке стоп
			}
		}
	}
}