#pragma once

///// \brief ����� ����� �� ���-�����
//struct NumberTube
//{
//	char value[8];
//};

///\breaf  �������� ����������� ������� ���������
void TestCoilTemperature();

///\breaf �������� ������ ��������������� 
void CheckDemagnetizeModule();
///\ breaf ��������� �������� ��������� ������� ��������
void SettingWorkingPositionControlModules();
/// \breaf �������� ���������� � ����������
void TransferParametersThicknessModule();
/// \brief ������ ����������� ��������� �� �����������
void GetDataFromThicknessModule();
/// \brief ���������� ���������� ���������������
void FrequencyInverterPreparation();
/// \brief ������ ���������� ���������������
void FrequencyInverterRun();
///TODO ��������� ������ ������ ����������� ������������������
void SettingOperatingModeAirConditioningController();
///TODO ������� ������
void CleaningScreen();
///TODO ������ ������ �����
void RequestPipeNumber(char (&)[9]);
///TODO �������� ���������������
void EnableDemagnetization();
///TODO �������� ���������������
void DisableDemagnetization();
/// ������ � ���
void WorkACS(char (&numberTube)[9]);
/// ���������� ������ � ����
void StoredData(bool);


