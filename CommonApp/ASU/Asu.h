#pragma once

class ComPort;
namespace Communication
{
	static const int ok = 0;					 /// \brief  ошибок нет
	static const int time_overflow = 100;		 /// \brief	 превышено время ожидания
	static const int error_crc = 110;			 /// \brief	 ошибка CRC
	static const int error_count = 120;			 /// \brief	 ошибка CRC
class Asu
{
public:
	/// \brief тестирует ком-порт
	/// \return возвращает 0 - если нет ошибок
	static int Test(ComPort &);

	/// \brief возвращает номер трубы
	/// \param ComPort 
	/// \param возвращает номер трубы
	/// \param возвращает признак (0-транзит, 1-контроль)
	/// \return возвращает 0 - если нет ошибок
	static int RequestInformationAboutPipe(ComPort &, char (&numberTube)[8], int &mark);

	/// \brief посылает в АСУ результат измерений
	/// \return возвращает 0 - если нет ошибок
	static int SendData(ComPort &comPort, char (&numberTube)[8]
		, int crossBrak, int crossClass2
		, int longBrak, int longClass2
		, int thickBrak, int thickClass2
		, int lengthTube
		, int cutZone1, int cutZone2
		, int result
		, char solidGroup
		);
};

class Thick
{
   public:
	/// \brief тестирует ком-порт
	/// \return возвращает 0 - если нет ошибок
	static int Test(class ComPort &);

	/// \brief возвращает результат контроля
	/// \param ComPort 
	/// \param возвращает порог брака
	/// \param возвращает порог 2 класса
	/// \param возвращает длину трубы
	/// \param возвращает результат по зонам
	/// \return возвращает 0 - если нет ошибок
	static int RequestControlResult(ComPort &
		, double &brak
		, double &class2
		, double &class3
		, int &lengthTube
		, unsigned short (&zones)[65]
		);
	/// \brief передача параметров контроля
	/// \param типоразмер трубы
	/// \param порог брака
	/// \param порог класс 2
	/// \param порог класс 3
	/// \return возвращает 0 - если нет ошибок
	static int TransferControlParameters(ComPort &
		, int typeSize
		, double brakTresh
		, double class2Tresh
		, double class3Tresh
		);
};
}
