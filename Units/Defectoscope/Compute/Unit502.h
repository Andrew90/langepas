#pragma once

class Unit502
{
	HANDLE hThread;
	void ReadTresh();
public:
	void Read();
	bool Init();
	void Destroy();
	bool SetupParams();
	int Start();
	int Stop();
	bool ReadAsync(unsigned ch, int mode, int range, double &value);
	bool ReadAsyncChannels(int count, unsigned *ch, int *mode, int *range, double *value);
	bool BitOut(unsigned ch, bool value);
	bool BitIn(unsigned &value);
};


