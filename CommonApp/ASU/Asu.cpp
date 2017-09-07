#include "stdafx.h"
#include "Asu.h"
#include "ComPort/crc.h"
#include "ComPort/ComPort.h"
#include "App/AppBase.h"
#include "templates\typelist.hpp"
#include "DataItem\DataItem.h"
#include "MessageText\StatusMessages.h"
#include "Compute\ControlMode.h"

namespace Communication
{	
#pragma pack(push, 1)
	struct Func2
	{
		unsigned char size;
		unsigned char numAbonent;
		unsigned char numFunc;

		unsigned char numberTube[8] ;
		unsigned short mark	   ;
		unsigned short noused; 
		unsigned short crc	   ;
	};
	struct Func5 
	{
		unsigned char size;
		unsigned char numAbonent;
		unsigned char numFunc;

		unsigned char numberTube[8] ;

		unsigned short crossBrak	   ;
		unsigned short crossClass2  ;

		unsigned short longBrak	   ;
		unsigned short longClass2   ;

		unsigned short thickBrak	   ;
		unsigned short thickClass2  ;

		unsigned short lengthTube   ;

		unsigned short cutZone1	   ;
		unsigned short cutZone2	   ;

		unsigned short resultCommon;
		unsigned short resultClutch;
		unsigned char solidGroupClutch	   ;
		unsigned char solidGroupTube	   ;
		unsigned char zones[65 * 2];
		unsigned short crc;
	};
#pragma pack(pop)

	struct HandleComPort
	{		
		int err;
		HANDLE hEvent;
		int countBytes;
		unsigned char *receiveBuffer;
		HandleComPort()
		{
			hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		}
		~HandleComPort()
		{
			CloseHandle(hEvent);
		}
		void Do(unsigned char(&buf)[1024], int count)
		{
			err = error_count;
			receiveBuffer = buf;
			countBytes = buf[0]; 
			dprint("receive count %d abon %d func %d\n", buf[0], buf[1], buf[2]);
			if(count >= countBytes)
			{
				err = ok;
				int ret = Crc16(buf, countBytes);
				dprint("com port %x\n", ret);
				if(0 != ret)	err = error_crc;
			}
			SetEvent(hEvent);
		}

		int ReceivingData()
		{
			if(WAIT_TIMEOUT == WaitForSingleObject(hEvent, 3000)) err = time_overflow; 
			return err;
		}
	};

	struct ZoneVal
	{
		unsigned char thick;
		unsigned char stat;
	};

	int WapGroup(wchar_t g)
	{
		int res = 0;
		switch(g)
		{
		case L'Д': res = 1;break;
		case L'К': res = 2;break;
		case L'Е': res = 3;break;
		}
		return res;
	}

	int Asu::SendData(ComPort &comPort, char (&numberTube)[9])
	{
		Func5 func = {};
		func.size = sizeof(func);

		func.numAbonent = Singleton<ComPortTable>::Instance().items.get<SubscriberASU>().value;
		func.numFunc = 5;

		memmove(func.numberTube, numberTube, sizeof(func.numberTube));

		OnTheJobTable::TItems &job = Singleton<OnTheJobTable>::Instance().items;
		ItemData<::Thick> &th = Singleton<ItemData<::Thick>>::Instance();
		ItemData<Cross> &cr = Singleton<ItemData<Cross>>::Instance();
		ItemData<Long> &lo = Singleton<ItemData<Long>>::Instance();
		ResultData &rs = Singleton<ResultData>::Instance();

		ThresholdsTable::TItems &tresh = Singleton<ThresholdsTable>::Instance().items;

		func.crossBrak	=   (unsigned short)tresh.get<BorderDefect<Cross>>().value;
		func.crossClass2  = (unsigned short)tresh.get<BorderKlass2<Cross>>().value;

		if(job.get<OnTheJob<Long>>().value)
		{
			func.longBrak =   (unsigned short)tresh.get<BorderDefect<Long>>().value;	   
			func.longClass2 = (unsigned short)tresh.get<BorderKlass2<Long>>().value;  
		}

		if(job.get<OnTheJob<::Thick>>().value)
		{
			func.thickBrak	= (unsigned short)(10.0 * tresh.get<BorderDefect<::Thick>>().value);   
			func.thickClass2 = (unsigned short)(10.0 * tresh.get<BorderKlass2<::Thick>>().value); 
		}

		func.lengthTube = rs.currentOffsetZones  ;

		func.cutZone1 = rs.cutZone0   ;
		func.cutZone2 = rs.cutZone1   ;

		func.resultCommon	= rs.resultCommon   ;
		func.resultClutch	= 0	;
		func.solidGroupClutch = 0   ;
		func.solidGroupTube	 = WapGroup(rs.solidGroup)  ;

		dprint(" Asu::SendData\n"\
			"func.cutZone1 %d\n"\
			"func.cutZone2 %d\n"\
			"func.resultCommon  %d\n"\
			"func.solidGroupTube %d\n"
			, func.cutZone1
			, func.cutZone2
			, func.resultCommon  
			, func.solidGroupTube
			);

		

		ZoneVal *zone = (ZoneVal *)func.zones;
		for(int i = 0; i < rs.currentOffsetZones; ++i)
		{
			unsigned t_stat = AsuResBits<::Thick>()(th.status[i]);
			unsigned char th_buf = (unsigned char)(10.0 * th.buffer[i]);
			zone[i].thick = th_buf;

			if(100 == th_buf)
			{
				zone[i].thick = 0;
			}
			else if(110 == th_buf)
			{
				zone[i].thick = 255;
			}

			zone[i].stat = ResAsu(rs.status[i]);
			zone[i].stat &= ~(3 << 4);
			zone[i].stat |= t_stat << 4;

			dprint("asu send %d thick %d stat %x\n", i, zone[i].thick, zone[i].stat);
		}

		func.crc = Crc16((unsigned char *)(&func), sizeof(func) - sizeof(short));
		HandleComPort handleComPort;
		comPort.SetReceiveHandler(&handleComPort, &HandleComPort::Do);

		int ret = 0;
		for(int i = 0; i < 3; ++i)
		{
			unsigned char *buf = (unsigned char *)&func;
			dprint("transmit count %d abon %d func %d\n", buf[0], buf[1], buf[2]);
			comPort.Write((unsigned char *)&func, sizeof(func));
			ret = handleComPort.ReceivingData();
			if(ok == ret) break;
			Sleep(1000);
		}

		return ret;
	}

	int Asu::Test(ComPort &comPort)
	{
		unsigned char buf[] = {5
			, Singleton<ComPortTable>::Instance().items.get<SubscriberASU>().value
			, 1, 0, 0};
		*(unsigned short *)&buf[3] = Crc16(buf,  sizeof(buf) - sizeof(short));

		HandleComPort handleComPort;
		comPort.SetReceiveHandler(&handleComPort, &HandleComPort::Do);

		int ret = 0;
		for(int i = 0; i < 3; ++i)
		{
			dprint("transmit count %d abon %d func %d\n", buf[0], buf[1], buf[2]);
			comPort.Write(buf, sizeof(buf));
			ret = handleComPort.ReceivingData();
			if(ok == ret) break;
			Sleep(1000);
		}

		return ret;
	}

	int Asu::RequestInformationAboutPipe(ComPort &comPort, char (&numberTube)[9])
	{
		unsigned char buf[] = {5
			, Singleton<ComPortTable>::Instance().items.get<SubscriberASU>().value
			, 2, 0, 0};
		*(unsigned short *)&buf[3] = Crc16(buf,  sizeof(buf) - sizeof(short));

		HandleComPort handleComPort;
		comPort.SetReceiveHandler(&handleComPort, &HandleComPort::Do);

		int ret = 0;
		for(int i = 0; i < 3; ++i)
		{
			dprint("transmit count %d abon %d func %d\n", buf[0], buf[1], buf[2]);
			comPort.Write(buf, sizeof(buf));
			ret = handleComPort.ReceivingData();
			if(ok == ret)
			{
				Func2 *b = (Func2 *)handleComPort.receiveBuffer;
				memmove(numberTube, b->numberTube, sizeof(numberTube) - 1);
				break;
			}
			Sleep(1000);
		}

		return ret;
	}

	/// \brief тестирует ком-порт
	/// \return возвращает 0 - если нет ошибок
	int Thick::Test(ComPort &comPort)
	{
		unsigned char buf[] = {5
			, Singleton<ComPortTable>::Instance().items.get<SubscriberThickness>().value
			, 1, 0, 0};
		*(unsigned short *)&buf[3] = Crc16(buf,  sizeof(buf) - sizeof(short));

		HandleComPort handleComPort;
		comPort.SetReceiveHandler(&handleComPort, &HandleComPort::Do);

		int ret = 0;
		for(int i = 0; i < 3; ++i)
		{
			dprint("transmit count %d abon %d func %d\n", buf[0], buf[1], buf[2]);
			comPort.Write(buf, sizeof(buf));
			ret = handleComPort.ReceivingData();
			if(ok == ret) break;
			Sleep(1000);
		}

		return ret;
	}
#pragma pack(push, 1)
	struct Func2_1
	{
		unsigned char size;
		unsigned char numAbonent;
		unsigned char numFunc;

		unsigned short brakTreshold;
		unsigned short class2Treshold;

		unsigned short class3Treshold;
		unsigned short lengthTube;



		char reserve[4];// - sizeof(unsigned short)];
		unsigned short zones[65];
		unsigned short crc	   ;
	};
#pragma pack(pop)
	/// \brief возвращает результат контроля
	/// \param ComPort 
	/// \param возвращает порог брака
	/// \param возвращает порог 2 класса
	/// \param возвращает длину трубы
	/// \param возвращает результат по зонам
	/// \return возвращает 0 - если нет ошибок
	int Thick::RequestControlResult(ComPort &comPort
		, double &brak
		, double &class2
		, double &class3
		, int &lengthTube
		, unsigned short (&zones)[65]
	)
	{

		int numAbonent = Singleton<ComPortTable>::Instance().items.get<SubscriberThickness>().value;
		unsigned char buf[] = {5
			, numAbonent
			, 2, 0, 0};
		*(unsigned short *)&buf[3] = Crc16(buf,  sizeof(buf) - sizeof(short));



		HandleComPort handleComPort;
		comPort.SetReceiveHandler(&handleComPort, &HandleComPort::Do);

		int ret = 0;
		for(int i = 0; i < 3; ++i)
		{
			dprint("transmit count %d abon %d func %d\n", buf[0], buf[1], buf[2]);
			comPort.Write((unsigned char *)&buf, sizeof(buf));
			ret = handleComPort.ReceivingData();
			if(ok == ret)
			{	
				Func2_1 *b = (Func2_1 *)handleComPort.receiveBuffer;
				brak = 0.1 * b->brakTreshold;
				class2 = 0.1 * b->class2Treshold;
				class3 = 0.1 * b->class3Treshold;
				lengthTube = 1 + b->lengthTube;
				memmove(zones, b->zones, 65 * sizeof(short));

				dprint(
					"size			%d\n"\
					"numAbonent		%d\n"\
					"numFunc		%d\n"\
					"brakTreshold	%d\n"\
					"class2Treshold	%d\n"\
					"lengthTube		%d\n"\
					"class3Treshold	%d\n"

					, b->size			
					, b->numAbonent		
					, b->numFunc		
					, b->brakTreshold	
					, b->class2Treshold	
					, b->lengthTube	
					, b->class3Treshold	

					);


				break;
			}
			Sleep(1000);
		}

		return ret;
	}
#pragma pack(push, 1)
	struct Func4_1
	{
		unsigned char size;
		unsigned char numAbonent;
		unsigned char numFunc;

		unsigned short typeSizeTube;
		unsigned short brakTreshold;
		unsigned short class2Treshold;
		unsigned short class3Treshold;
		char reserve[10];
		unsigned short crc	   ;
	};
#pragma pack(pop)
	/// \brief передача параметров контроля
	/// \param типоразмер трубы
	/// \param порог брака
	/// \param порог класс 2
	/// \param порог класс 3
	/// \return возвращает 0 - если нет ошибок
	int Thick::TransferControlParameters(ComPort &comPort
		, int typeSize
		, double brakTresh
		, double class2Tresh
		, double class3Tresh
		)
	{
		Func4_1 buf = {};
		buf.size = sizeof(buf);
		buf.numAbonent = Singleton<ComPortTable>::Instance().items.get<SubscriberThickness>().value;
		buf.numFunc = 4;

		buf.typeSizeTube = typeSize;

		buf.brakTreshold = (unsigned short)(brakTresh * 10);
		buf.class2Treshold =  (unsigned short)(class2Tresh * 10);
		buf.class3Treshold =  (unsigned short)(class3Tresh * 10);

		dprint(
			"buf.typeSizeTube   %d\n"\
			"buf.brakTreshold 	%d\n"\
			"buf.class2Treshold	%d\n"\
			"buf.class3Treshold	%d\n"

			, buf.typeSizeTube 
			, buf.brakTreshold 
			, buf.class2Treshold
			, buf.class3Treshold
			);


		buf.crc = Crc16((unsigned char *)&buf,  sizeof(buf) - sizeof(short));

		HandleComPort handleComPort;
		comPort.SetReceiveHandler(&handleComPort, &HandleComPort::Do);

		int ret = 0;
		for(int i = 0; i < 1; ++i)
		{
			unsigned char *tbuf = (unsigned char *)&buf;
			dprint("transmit count %d abon %d func %d\n", tbuf[0], tbuf[1], tbuf[2]);
			comPort.Write((unsigned char *)&buf, sizeof(buf));
			ret = handleComPort.ReceivingData();
			if(ok == ret)
			{				
				break;
			}
			Mode::ExitButtonTime(3000);
		}

		return ret;
	}
}
