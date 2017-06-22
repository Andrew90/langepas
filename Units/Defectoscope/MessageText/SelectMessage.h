#pragma once
#include "App\App.h" 
char *SelectMessage(int *, int &);

struct StatusText
{
	char *operator()(int id, int &color, bool &visible);	
	char *FromSensors(int *sens, int &color, bool &visible);
	int operator()(int id);
};

struct StatusColor
{
	int operator()(int id);
};

struct ColorBar
{
	void operator()(double &data, unsigned &color, int id, double defData);
};

bool TubeResult(char (&res)[App::count_zones], int count);

bool CancelOperator(int id, unsigned &color);

int ResultMessageId(int *);





