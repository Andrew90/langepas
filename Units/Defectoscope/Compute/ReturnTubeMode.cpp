#include "stdafx.h"
#include "ControlMode.h"
#include "Automat.hpp"
#include "Log\LogBuffer.h"

#define TEST_MESS(n) if(TEST_IN_BITS(On<n>)){Log::Mess<LogMess::n##Mess>(); /*throw ExceptionAlarm();*/return;}
namespace Mode
{
	using namespace AutomatN;
	void ReturnTube(Data &data)
	{
	   dprint("ReturnTube\n");
	   Log::Mess<LogMess::ReturnPipe>();

	   TEST_MESS(iSQ1pr	)
	   TEST_MESS(iSQ2pr	)
	   TEST_MESS(iSQ1po	)
	   TEST_MESS(iSQ2po	)
	   TEST_MESS(iSQ1t	)
	   TEST_MESS(iSQ2t	)

	   OUT_BITS(On<oPR_OP>, On<oPO_OP>, On<oT_OP>);
	   Sleep(500);
	   OUT_BITS(Off<oPR_OP>, Off<oPO_OP>, Off<oT_OP>);
	   try
	   {
		   AND_BITS(On<iOPpr>, On<iOPpo>, On<iOPt>, Ex<ExceptionStop>)(20000);
	   }
	   catch(ExceptionTimeOut)
	   {
		   Log::Mess<LogMess::ModulesNotInOperation>();
		   //throw ExceptionAlarm();
		   return;
	   }

	    OUT_BITS(Off<oWorkPR>, Off<oWorkPO>, Off<oWorkT>);
		Sleep(2000);
		OUT_BITS(On<oWorkT>);

		try
		{
			AND_BITS(Off<iWork_pnevmo>, On<iRevers_pnevmo>, Off<iError_pnevmo>, Ex<ExceptionStop>)(20000);
		}
		catch(ExceptionTimeOut)
		{
			Log::Mess<LogMess::TransverseModuleClampsDidNotWork>();
			OUT_BITS(Off<oWorkPR>, Off<oWorkPO>, Off<oWorkT>);
			//throw ExceptionAlarm();
			return;
		}

		 OUT_BITS(On<oOP>);

		 Log::Mess<LogMess::PipeReturnAllowed>();

		 Sleep(5000);

		 for(int i = 0; i < 50; ++i)
		 {
			 if(TEST_IN_BITS(Off<iWork_pnevmo>, On<iRevers_pnevmo>, Off<iError_pnevmo>, Ex<ExceptionStop>))
			 {
				 if(TEST_IN_BITS(
					 Off<iSQ1pr>, Off<iSQ2pr>	
					 , Off<iSQ1po>, Off<iSQ2po>	
					 , Off<iSQ1t>, Off<iSQ2t>
					 ))
				 {
					 Log::Mess<LogMess::ReturnPipeCompleted>();
					 return;
				 }
			 }
			 else
			 {
				 Log::Mess<LogMess::TransverseModuleClampsDidNotWork>();
				 OUT_BITS(Off<oWorkPR>, Off<oWorkPO>, Off<oWorkT>);
				 //throw ExceptionAlarm();
				 return;
			 }
		 }
		 Log::Mess<LogMess::PipeReturnTimeExceeded>();
	}
}

#undef TEST_MESS