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
			 static const int zones = 50;
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
			 static const int zones = 50;
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
			 static const int zones = 50;
			 d.currentOffsetZones = zones;
			 double x = 1;
			 for(int i = 0; i < zones; ++i)
			 {
				 for(int j = 0; j < dimention_of(d.buffer); ++j)
				 {
					 d.status[i] = ++z % 50;
					  
					 x += 0.1;
					 if(x > 4.5) x = 1;
					 d.buffer[i] = x;
					 
				 }
			 }
		 }

		  {
			 ResultData &d = Singleton<ResultData>::Instance();
			 static const int zones = 50;
			 d.currentOffsetZones = zones;
			 double x = 0;
			 for(int i = 0; i < zones; ++i)
			 {
					 d.status[i] = ++z % 50;
					
			 }
			 d.cutZone0 = 5;
			 d.cutZone1 = 45;
		 }
	 }
};// xxxxx;
////test////////////////////////////////