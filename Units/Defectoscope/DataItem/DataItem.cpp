#include "stdafx.h"
#include "DataItem.h"
#include "templates/templates.hpp"

///test/////////////////////
struct test_test
{
	 test_test()
	 {
		 int z = 0;
		 {
			 ItemData<Cross> &d = Singleton<ItemData<Cross>>::Instance();
			 static const int zones = 150;
			 d.currentOffsetZones = zones;

			 for(int i = 0; i < zones; ++i)
			 {
				 for(int j = 0; j < dimention_of(d.status); ++j)
				 {
					 d.status[j][i] = ++z % 50;
					 d.buffer[j][i] = j;
				 }
			 }
		 }
		 {
			 ItemData<Long> &d = Singleton<ItemData<Long>>::Instance();
			 static const int zones = 150;
			 d.currentOffsetZones = zones;

			 for(int i = 0; i < zones; ++i)
			 {
				 for(int j = 0; j < dimention_of(d.status); ++j)
				 {
					 d.status[j][i] = ++z % 50;
					 d.buffer[j][i] = j;
				 }
			 }
		 }

		 {
			 ItemData<Thick> &d = Singleton<ItemData<Thick>>::Instance();
			 static const int zones = 150;
			 d.currentOffsetZones = zones;
			 double x = 1;
			 for(int i = 0; i < zones; ++i)
			 {
				 for(int j = 0; j < dimention_of(d.bufferMin); ++j)
				 {
					 d.status[0][i] = ++z % 50;
					  d.status[1][i] = ++z % 50;
					   d.status[2][i] = ++z % 50;
					 x += 0.1;
					 if(x > 4.5) x = 1;
					 d.bufferMin[0][i] = x;
					 d.bufferMin[1][i] = x + 0.2;
					  d.bufferMin[2][i] = x + 0.3;

					  d.bufferMin[0][i] = x + 0.5;
					 d.bufferMin[1][i] = x + 0.7;
					  d.bufferMin[2][i] = x + 0.8;
				 }
			 }
		 }

		  {
			 ResultData &d = Singleton<ResultData>::Instance();
			 static const int zones = 150;
			 d.currentOffsetZones = zones;
			 double x = 0;
			 for(int i = 0; i < zones; ++i)
			 {
					 d.status[i] = ++z % 50;
					
			 }
		 }
	 }
} xxxxx;
////test////////////////////////////////