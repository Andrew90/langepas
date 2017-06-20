#pragma once

class L502
{
	void *hnd;
public:
  static const int buffer_length = 10 * 1024;  
  int READ_TIMEOUT;
public:
	L502();
	bool Init();
	void Destroy();
	bool SetupParams();
	int Start();
	int Stop();
	int Read(unsigned &startChennel, double *data, unsigned &count);
};
