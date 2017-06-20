#pragma once

class Unit502
{
public:
	bool crossOn, longOn, solidOn;
	void Read();
	bool Init();
	void Destroy();
	bool SetupParams();
	int Start();
	int Stop();
};


