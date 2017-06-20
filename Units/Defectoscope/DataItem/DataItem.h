#pragma once
#include "App/App.h"


struct TimeLir
{
	LARGE_INTEGER frequency, start;
	int currentTimeOffset;
	unsigned time[App::count_time_count_down];
	unsigned lir[App::count_time_count_down];
	///> ���������� � 0 ����� ��������� ������
	//void Clear();
	/////> ���������� ������� ��� � �������� ���
	//void Do(unsigned);
	/////> ������� ��� � ������� Clear � �������.
	//unsigned CurrentTickCount();
};

template<int SENS>struct PrimaryData
{
	static const int count_sensors = SENS;
	int currentOffset;	  //������� ������
	int currentOffsetZones;	 //������� ��������� ���
	int currentTimeOffset;	//������� ��������� ������ � ���
	int deadSamplesBegin, deadSamplesEnd;
	double ascan[SENS][App::count_frames];	 //����� � ���
	int offsets[1 + App::count_zones];		  //�������� ������ �� �����
	double buffer[SENS][App::count_zones];	 //�������� � ����
	char status[SENS][App::count_zones];	 //������ � ����
	unsigned time[3000];	//				 // ����� ��������� �����(��� ������ �������� �� �����)
	unsigned frame[3000];	//������������� ������ �� �����
};

struct PrimaryThickData
{
	static const int count_sensors = App::count_Thick_sensors;
	int currentOffset;	  //������� ������
	int currentOffsetZones;	 //������� ��������� ���
	int currentTimeOffset;	//������� ��������� ������ � ���
	int deadSamplesBegin, deadSamplesEnd; 
	double ascan[count_sensors][App::count_frames];	 //����� � ���
	char ascanStatus[count_sensors][App::count_frames];	 //����� � ���
	int offsets[1 + App::count_zones];		  //�������� ������ �� �����
	double bufferMin[count_sensors][App::count_zones];	 //�������� � ����
	double bufferMax[count_sensors][App::count_zones];	 //�������� � ����
	char status[count_sensors][App::count_zones];	 //������ � ����
	unsigned time[3000];	//				 // ����� ��������� �����(��� ������ �������� �� �����)
	unsigned frame[3000];	//������������� ������ �� �����
};

struct SolidGroupData
{
	static const int count_sensors = 2;
	int currentOffset;
	int currentOffsetZones;
	double ascan[count_sensors][App::count_frames];	
};

struct ResultData
{
	int currentOffsetZones;
	char status[App::count_zones];
};

template<class T>struct ItemData;

template<>struct ItemData<Cross>: PrimaryData<App::count_cross_sensors>{};
template<>struct ItemData<Long>: PrimaryData<App::count_long_sensors>{};

template<>struct ItemData<Solid>: SolidGroupData{};

template<>struct ItemData<Thick>: PrimaryThickData{};




