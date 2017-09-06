#pragma once

namespace Mode
{
	struct Data
	{
		bool packet;
		bool firstRun;
	};

	void ControlTube(Data &);

	void ReturnTube(Data &);

	void ExitTube(Data &);

	bool TestControlCircuit();

	void ExitButtonTime(unsigned = INFINITE);// при нажатии кнопки крест выходит по исключению
}