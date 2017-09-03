#pragma once

namespace Mode
{
	struct Data
	{
		bool packet;
	};

	void ControlTube(Data &);

	void ReturnTube(Data &);

	void ExitTube(Data &);

	bool TestControlCircuit();
}