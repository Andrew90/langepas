#pragma once

class ComPort;
namespace Communication
{
	static const int ok = 0;					 /// \brief  ������ ���
	static const int time_overflow = 100;		 /// \brief	 ��������� ����� ��������
	static const int error_crc = 110;			 /// \brief	 ������ CRC
	static const int error_count = 120;			 /// \brief	 ������ CRC
class Asu
{
public:
	/// \brief ��������� ���-����
	/// \return ���������� 0 - ���� ��� ������
	static int Test(ComPort &);

	/// \brief ���������� ����� �����
	/// \param ComPort 
	/// \param ���������� ����� �����
	/// \param ���������� ������� (0-�������, 1-��������)
	/// \return ���������� 0 - ���� ��� ������
	static int RequestInformationAboutPipe(ComPort &, char (&numberTube)[8], int &mark);

	/// \brief �������� � ��� ��������� ���������
	/// \return ���������� 0 - ���� ��� ������
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
	/// \brief ��������� ���-����
	/// \return ���������� 0 - ���� ��� ������
	static int Test(class ComPort &);

	/// \brief ���������� ��������� ��������
	/// \param ComPort 
	/// \param ���������� ����� �����
	/// \param ���������� ����� 2 ������
	/// \param ���������� ����� �����
	/// \param ���������� ��������� �� �����
	/// \return ���������� 0 - ���� ��� ������
	static int RequestControlResult(ComPort &
		, double &brak
		, double &class2
		, double &class3
		, int &lengthTube
		, unsigned short (&zones)[65]
		);
	/// \brief �������� ���������� ��������
	/// \param ���������� �����
	/// \param ����� �����
	/// \param ����� ����� 2
	/// \param ����� ����� 3
	/// \return ���������� 0 - ���� ��� ������
	static int TransferControlParameters(ComPort &
		, int typeSize
		, double brakTresh
		, double class2Tresh
		, double class3Tresh
		);
};
}
