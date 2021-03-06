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
if(!TestControlCircuit())return;
	   TEST_MESS(iSQ1pr	)
	   TEST_MESS(iSQ2pr	)
	   TEST_MESS(iSQ1po	)
	   TEST_MESS(iSQ2po	)
	   TEST_MESS(iSQ1t	)
	   TEST_MESS(iSQ2t	)

	   OUT_BITS(On<oPR_OP>);
	   Sleep(500);
	   OUT_BITS(Off<oPR_OP>);
	   try
	   {
		   AND_BITS(On<iOPpr>, Ex<ExceptionStop>)(30000);
	   }
	   catch(ExceptionTimeOut)
	   {
		   Log::Mess<LogMess::ModulesNotInOperationLong>();
		   return;
	   }

	   OUT_BITS(On<oPO_OP>);
	   Sleep(500);
	   OUT_BITS(Off<oPO_OP>);
	   try
	   {
		   AND_BITS(On<iOPpo>, Ex<ExceptionStop>)(30000);
	   }
	   catch(ExceptionTimeOut)
	   {
		   Log::Mess<LogMess::ModulesNotInOperationCross>();
		   return;
	   }

	    OUT_BITS(Off<oWorkPR>, Off<oWorkPO>, Off<oWorkT>);
		Sleep(2000);
		OUT_BITS(On<oWorkT>);

	   OUT_BITS(On<oT_OP>);
	   Sleep(500);
	   OUT_BITS(Off<oT_OP>);
	   try
	   {
		   AND_BITS(On<iOPt>, Ex<ExceptionStop>)(30000);
	   }
	   catch(ExceptionTimeOut)
	   {
		   Log::Mess<LogMess::ModulesNotInOperationThick>();
		   return;
	   }

		try
		{
			AND_BITS(Off<iWork_pnevmo>, On<iRevers_pnevmo>, Off<iError_pnevmo>, Ex<ExceptionStop>)(20000);
		}
		catch(ExceptionTimeOut)
		{
			Log::Mess<LogMess::TransverseModuleClampsDidNotWork>();
			OUT_BITS(Off<oWorkPR>, Off<oWorkPO>, Off<oWorkT>);
			return;
		}

		 OUT_BITS(On<oOP>);

		 Log::Mess<LogMess::PipeReturnAllowed>();

		 AND_BITS(Off<iSQ1DM>, Ex<ExceptionStop>)(120000);
		 dprint("\n0  ");
		 AND_BITS(On<iSQ1DM>, Ex<ExceptionStop>)(120000);
		  dprint("1  ");
		 AND_BITS(On<iSQ1po>, Ex<ExceptionStop>)(120000);
		  dprint("2  ");
		 AND_BITS(Off<iSQ1po>, Off<iWork_pnevmo>, On<iRevers_pnevmo>, Off<iError_pnevmo>, Ex<ExceptionStop>)();
		  dprint("3\n  ");
		  Log::Mess<LogMess::ReturnPipeCompleted>();
		// ExitButtonTime(60000);
		//  throw ExceptionStop();

		 //for(int i = 0; i < 50; ++i)
		 //{
		//	 if(TEST_IN_BITS(Off<iWork_pnevmo>, On<iRevers_pnevmo>, Off<iError_pnevmo>, Ex<ExceptionStop>))
		//	 {
		//		 if(TEST_IN_BITS(
		//			 Off<iSQ1pr>, Off<iSQ2pr>	
		//			 , Off<iSQ1po>, Off<iSQ2po>	
		//			 , Off<iSQ1t>, Off<iSQ2t>
		//			 ))
		//		 {
		 //
		//			 
		//			 return;
		//		 }
		//	 }
		//	 else
		//	 {
		//		 Log::Mess<LogMess::TransverseModuleClampsDidNotWork>();
		//		 OUT_BITS(Off<oWorkPR>, Off<oWorkPO>, Off<oWorkT>);
		//		 //throw ExceptionAlarm();
		//		 return;
		//	 }
		//	 Sleep(500);
		 //}
		// Log::Mess<LogMess::PipeReturnTimeExceeded>();
	}
}

#undef TEST_MESS