#pragma once

namespace StatusMessages
{
	bool Message(int id, wchar_t *mess, unsigned &color);
}

int ResultMessageId(unsigned *);

struct ColorBar
{
	void ColorBar::operator()(double &data, unsigned &color, int id, double defData);
};

struct StatusText
{
	void FromSensors(unsigned *sens, unsigned &color, bool &visible, wchar_t *buf);
	void operator()(int id, unsigned &color, bool &visible, wchar_t *);
};

struct StatusColor
{
	unsigned operator()(unsigned id) ;
};

bool IsDefect(unsigned);
