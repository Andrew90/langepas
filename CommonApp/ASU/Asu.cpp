#include "stdafx.h"
#include "Asu.h"
#include "ComPort/crc.h"
#include "ComPort/ComPort.h"
#include "App/AppBase.h"
#include "templates\typelist.hpp"
#include "DataItem\DataItem.h"
#include "MessageText\StatusMessages.h"

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

	int Asu::SendData(ComPort &comPort
		, char (&numberTube)[9]
	, int crossBrak, int crossClass2
		, int longBrak, int longClass2
		, int thickBrak, int thickClass2
		, int lengthTube
		, int cutZone1, int cutZone2
		, int resultCommon
		, char solidGroupTube
		)
	{
		Func5 func = {};
		func.size = sizeof(func);

		func.numAbonent = Singleton<ComPortTable>::Instance().items.get<SubscriberASU>().value;
		func.numFunc = 5;

		memmove(func.numberTube, numberTube, sizeof(func.numberTube));

		func.crossBrak	= crossBrak   ;
		func.crossClass2  = crossClass2;

		func.longBrak = longBrak	   ;
		func.longClass2 = longClass2  ;

		func.thickBrak	= thickBrak   ;
		func.thickClass2 = thickClass2 ;

		func.lengthTube = lengthTube  ;

		func.cutZone1 = cutZone1   ;
		func.cutZone2 = cutZone2   ;

		func.resultCommon	= resultCommon   ;
		func.resultClutch	= 0	;
		func.solidGroupClutch = 0   ;
		func.solidGroupTube	 = solidGroupTube  ;

		ItemData<::Thick> &th = Singleton<ItemData<::Thick>>::Instance();
		//ItemData<Cross> &cr = Singleton<ItemData<Cross>>::Instance();
		//ItemData<Long> &lo = Singleton<ItemData<Long>>::Instance();
		ResultData &rs = Singleton<ResultData>::Instance();

		ZoneVal *zone = (ZoneVal *)func.zones;
		for(int i = 0; i < rs.currentOffsetZones; ++i)
		{
			zone[i].thick = (unsigned char)(10.0 * th.buffer[i]);

			zone[i].stat = ResAsu(rs.status[i]);
			
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
		for(int i = 0; i < 3; ++i)
		{
			unsigned char *tbuf = (unsigned char *)&buf;
			dprint("transmit count %d abon %d func %d\n", tbuf[0], tbuf[1], tbuf[2]);
			comPort.Write((unsigned char *)&buf, sizeof(buf));
			ret = handleComPort.ReceivingData();
			if(ok == ret)
			{				
				break;
			}
			Sleep(3000);
		}

		return ret;
	}
}
