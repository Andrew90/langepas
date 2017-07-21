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
	bool rp;
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

	void ReadDo();

	unsigned Inputs(int);
	unsigned ReadOutputs(int);
	void Outputs(int, unsigned);
	//unsigned Outputs(int);

	void StartLan();
	void StopLan();
	void LanDo();
	void LanData(char *);
};