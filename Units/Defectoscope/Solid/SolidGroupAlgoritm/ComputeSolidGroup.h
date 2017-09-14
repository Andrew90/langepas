#pragma once
#include <string>
#include <vector>
#include <map>
#include "SolidGroupAlgoritm\CommunicationIDItems.h"

struct GroupNameItem
{
	std::wstring name;
	unsigned color;
	int weightGroup;
	bool added;
};

struct FileNameItem
{
   std::wstring name;
   bool added;
};

template<class>struct Singleton;

class ComputeSolidGroup
{
	friend Singleton<ComputeSolidGroup>;
	ComputeSolidGroup(){}
	ComputeSolidGroup &operator =(const ComputeSolidGroup &);
public:
	typedef	enum {enabled, disabled, deleted, new_item} TStatus;
	static const int count_points = 8;

	struct PointItem
	{
		ComputeSolidGroup *owner;
		int groupName;
		double points[count_points];
		int offs;
		PointItem(ComputeSolidGroup *o, int groupName, double (&p)[count_points], int offs_)
			: owner(o)
			, groupName(groupName)
			, offs(offs_)
		{
			memmove(points, p, sizeof(points));
		}
		wchar_t *GroupName()
		{
			return (wchar_t *)owner->groupNameList[groupName].name.c_str();
		}
		int GroupColor()
		{
			return owner->groupNameList[groupName].color;
		}
	};

	struct SolidItem
	{
		int ID;
		int groupName;
		int solidFile;
		int offset;
		double corelation;
		double sumCorrelation;
		int numberMatches;
		TStatus status;
		double root;
		double points[count_points];
		bool changed;
	};
public:
	CommunicationIDItems communicationIDItems;
	std::map<int, GroupNameItem> groupNameList;
	std::map<int, FileNameItem> fileNameList;
	std::vector<SolidItem> solidItems;

	std::vector<PointItem> pointItems;
public:
	std::wstring typeSizeName;	
	int persents[count_points];

	bool changeTresholds;
	std::wstring currentFile;

	std::wstring currentGroupName;
	int offset;
	int start;
	double points[count_points];
	bool persentsChanged;
	void Clear();
	bool Load(wchar_t *);
	void Load();
	void Save();

	void AddThreshold();
	void DelThreshold(int id);

	void SetGroupName(int, wchar_t *);
	const wchar_t *GetGroupName(int);

	void UpdateTresholds();

	bool AddSolidGroup(wchar_t *);
	bool DelSolidGroup(wchar_t *);

	bool OneFrame(double (&)[count_points], double &, wchar_t *, unsigned &);
	bool Frames(double* signal, double *reference, int start, int stop, double &, wchar_t *&, unsigned &);
	bool FramesOne(double(&)[count_points], int(&)[count_points],double *signal, double *reference, int &start, int stop, double &, wchar_t *&, unsigned &);
private:
	void InitRootAll();

public: //��� ������ ���������
	struct CorrelationItem
	{
		int idCorrelation;
		double correlation;
		CorrelationItem(int idCorrelation, double correlation)
			: idCorrelation(idCorrelation)
			, correlation(correlation)
		{}
	};
	std::vector<CorrelationItem> correlationItems;

	bool ComputeMinGroup(int start, int stop, double &result, wchar_t *&groupName, unsigned &color, int &idGroup);
	bool ComputeMaxGroup(int start, int stop, double &result, wchar_t *&groupName, unsigned &color, int &idGroup);

	bool ComputeAllGroup(int start, int stop, double &result, wchar_t *&groupName, unsigned &color, int &idGroup);

	bool ComputeCommonGroup(int start, int stop, double &result, wchar_t *&groupName, unsigned &color, int &idGroup);
};





