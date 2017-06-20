#pragma once

class Emulator
{
	struct Group
	{
		static const int bufSize = 5000;
	FILE *f;
	double bufRef[bufSize];
	double bufSig[bufSize];
	int offs;
	int offsFile;
	int size;
		Group();
		~Group();
		void Do(double &, double &);
	};
	FILE *f;
	char *offsFirst, offsNext;
	bool start;
	double value;
	Group group;
public:
	Emulator();
	~Emulator();

	void Read(unsigned &start, double *data, unsigned &count, int delay);

	void Start();
	void Stop();

	void Do();

	unsigned Inputs();
	void Outputs(unsigned);
	unsigned Outputs();

	void StartLan();
	void StopLan();
	void LanDo();
	void LanData(char *);
};