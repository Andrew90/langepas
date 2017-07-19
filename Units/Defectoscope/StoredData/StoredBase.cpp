#include <stdafx.h>
#include "StoredData\StoredBase.h"
#include "StoredData\PacketBase.h"
#include "App\App.h"
#include <ATLComTime.h>
#include "templates\Hash.h"
#include "App\AppBase.h"
#include "DataItem\DataItem.h"

namespace Stored
{
	DEFINE_ARRAY_PARAM_NO_INIT(ResultArr, char, App::count_zones)

	struct ResultTable
	{
		typedef TL::MkTlst<
			ResultArr
		>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
		const wchar_t *name(){return L"ResultTable";}
	};

	template<class TypeBase, typename List, int N>struct QuerySQL_CreateTable<TypeBase, Stored::ResultTable, List, N>
	{
		QuerySQL_CreateTable(Stored::ResultTable *param){}
		wchar_t *operator()(){return L"CREATE TABLE ResultTable(ID int, ResultArr varbinary(300) NULL, UNIQUE (ID))";}
	};

	DEFINE_ARRAY_PARAM_NO_INIT(DataArr,char, App::count_zones)
	struct DataTable
	{
		typedef TL::MkTlst<
			DataArr
		>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
		const wchar_t *name(){return L"DataTable";}
	};

	template<class TypeBase, typename List, int N>struct QuerySQL_CreateTable<TypeBase, Stored::DataTable, List, N>
	{
		QuerySQL_CreateTable(Stored::DataTable *param){}
		wchar_t *operator()(){return L"CREATE TABLE DataTable(ID int, DataArr varbinary(300) NULL, UNIQUE (ID))";}
	};

	STR_PARAM(CustomerOperator, 128, L"--")
	struct CustomerOperatorTable
	{
		typedef TL::MkTlst<
			CustomerOperator
		>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
		const wchar_t *name(){return L"CustomerOperatorTable";}
	};
	template<class TypeBase, typename List, int N>struct QuerySQL_CreateTable<TypeBase, Stored::CustomerOperatorTable, List, N>
	{
		QuerySQL_CreateTable(Stored::CustomerOperatorTable *param){}
		wchar_t *operator()()
		{
			return L"CREATE TABLE CustomerOperatorTable(ID int, CustomerOperator VARCHAR(128) NULL, UNIQUE (ID))";
		}
	};

	DEFINE_PARAM_ID(Package	 , unsigned, 0)
	DEFINE_PARAM_ID(Customer , unsigned, 0)
	DEFINE_PARAM_ID(Operator , unsigned, 0)


	struct PackageTable
	{
		typedef TL::MkTlst<
			ID<Package	>
			, ID<Customer	>
			, ID<Operator	>
		>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
		const wchar_t *name(){return L"PackageTable";}
	};

	template<class TypeBase, typename List, int N>struct QuerySQL_CreateTable<TypeBase, Stored::PackageTable, List, N>
	{
		QuerySQL_CreateTable(Stored::PackageTable *param){}
		wchar_t *operator()()
		{
			return L"CREATE TABLE PackageTable(ID integer IDENTITY PRIMARY KEY, PackageID int, CustomerID int\
					, OperatorID int, CONSTRAINT CONSTRAINT_ID UNIQUE(PackageID, CustomerID, OperatorID))";
		}
	};

	template<class>struct Zones;
	DEFINE_PARAM_WAPPER(Zones, Cross, unsigned, 0)
	DEFINE_PARAM_WAPPER(Zones, Long, unsigned, 0)
	DEFINE_PARAM_WAPPER(Zones, Thick, unsigned, 0)

	DEFINE_PARAM(ZonesResult, unsigned, 0)

	DEFINE_PARAM(LengthTube, unsigned, 0)

	DEFINE_PARAM(DteTme, COleDateTime, 0.0)

	DEFINE_PARAM_ID(PackageTable, unsigned, 1)

	DEFINE_PARAM(ResultID, unsigned, 0)

	struct CurrentOptionTable
	{
		typedef TL::MkTlst<
			
			  BorderKlass2  <Long >
			, BorderDefect  <Long >
			, BorderKlass2  <Cross>
			, BorderDefect  <Cross>
			, BorderKlass2 <Thick>
			, BorderKlass3 <Thick>
			, BorderDefect <Thick>
			, LengthTube
		>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
		const wchar_t *name(){return L"CurrentOptionTable";}
	};

	DEFINE_PARAM_ID(CurrentOptionTable, unsigned, 1)
	STR_PARAM(SolidGroup, 8, L"--")

	template<int>struct IDCross;
	template<int>struct IDLong ;
	template<int>struct IDThickMin;
	template<int>struct IDThickMax;

#define PEAK(n)\
DEFINE_PARAM_NUM(IDCross, n, int, 0)\
DEFINE_PARAM_NUM(IDLong , n, int, 0)\
DEFINE_PARAM_NUM(IDThickMin, n, int, 0)\
DEFINE_PARAM_NUM(IDThickMax, n, int, 0)

	PEAK(0)
	PEAK(1)
	PEAK(2)
	PEAK(3)
	PEAK(4)
	PEAK(5)
	PEAK(6)
	PEAK(7)

#undef PEAK

	struct StoredTable
	{
		typedef TL::MultyListToList<TL::MkTlst<
			TL::MkTlst<
			DteTme			
			, ResultID
			, LengthTube
			, ID<PackageTable>
			, ID<CurrentOptionTable>
			, SolidGroup
			>::Result
			, TL::CreateNumList<IDCross, 0, 7>::Result
			, TL::CreateNumList<IDLong , 0, 7>::Result
			, TL::CreateNumList<IDThickMin, 0, 7>::Result
			, TL::CreateNumList<IDThickMax, 0, 7>::Result
		>::Result>::Result items_list;
		typedef TL::Factory<items_list> TItems;
		TItems items;
		const wchar_t *name(){return L"StoredTable";}
	};

	struct __StoredBase
	{
		typedef TL::MkTlst<
			 ResultTable
			, DataTable
			, CurrentOptionTable
			, StoredTable
			, CustomerOperatorTable
			, PackageTable
		>::Result type_list;
		typedef TL::Factory<type_list> TTables;
		TTables tables;
		wchar_t path[512];
		const wchar_t *name()
		{
			COleDateTime t = COleDateTime::GetCurrentTime();
			GetModuleFileName(0, path, dimention_of(path));
			int len = (int)wcslen(path);
			for(;len > 0; --len)
			{
				if('\\' == path[len] || '/' == path[len]) break;
			}
			wsprintf(&path[len], L"\\stored");
			CreateDirectory (path, NULL);
			wsprintf(&path[wcslen(path)], L"\\%02d%02d"
				, t.GetYear() - 2000
				, t.GetMonth()
				);
			return path;
		}
	};

	template<class O, class P>struct __set_param__
	{
		void operator()(O &o, P &p){}
	};
	template<class X, class P>struct __set_param__<ID<X>, P>
	{
		typedef ID<X> O;
		void operator()(O &o, P &p)
		{
			o.value = p.get<O>().value;
		}
	};
	
	struct __set_packet_data
	{
		CBase &base;
		PackageTable table;
		__set_packet_data(CBase &base): base(base){}
	};
	template<class O, class P>struct __set_packet__
	{
		void operator()(O &o, P &p)
		{
			try
			{
				int hash = HashFAQ6((unsigned char *)&o.value[0], 2 * wcslen(o.value));
				CMD(p.base).CommandText(L"INSERT INTO CustomerOperatorTable (ID, CustomerOperator) VALUES(?, ?)").Param(hash).Param((wchar_t *)o.value).Execute();
				p.table.items.get<ID<O>>().value = hash;
			}
			catch(...){}
		}
	};

	template<class T, int sens, int zones>int UnitData(T(&data)[sens][zones], T(&res)[zones], int count)
	{
		ZeroMemory(res, sizeof(res));
		if(count > zones) count = zones;
		for(int i = 0; i < count; ++i)
		{
			double t = data[i][0];
			for(int j = 1; j < sens; ++j)
			{
				if(t > data[i][j]) t = data[i][j];
			}
			res[i] = t;
		}
		return HashFAQ6((unsigned char *)res, sizeof(T) * count);
	}

	struct __sub_arr_data__
	{
		StoredTable &st;
		double(&data)[App::count_zones];
		CBase &base;
		__sub_arr_data__(StoredTable &st, double(&data)[App::count_zones], CBase &base)
			: st(st)
			, data(data)
			, base(base)
		{}
	};
	template<class O, class P>struct __sub_arr__;
	template<template<int>class W, int N, class P>struct __sub_arr__<W<N>, P>
	{
		typedef W<N> O;
		void operator()(P &p)
		{
			unsigned char *c = (unsigned char *)p.data;
			unsigned char *d = &c[/*sizeof(double) **/ App::count_zones  * N];
			int hash = HashFAQ6(d, App::count_zones);
			p.st.items.get<O>().value = hash;

			wchar_t *query = L"INSERT INTO DataTable (ID, DataArr) VALUES(?, ?)";
			CMD(p.base).CommandText(query)
				.Param(hash)
				.Param((char(&)[App::count_zones])d)
				.Execute();			
		}
	};
	template<template<int>class W>struct __set_data__
	{
		void operator()(StoredTable &st, double(&data)[60], CBase &base)
		{
			typedef TL::CreateNumList<W, 0, 7>::Result list;
			try
			{
				TL::foreach<list, __sub_arr__>()(__sub_arr_data__(st, data, base));
			}catch(...){}
		}
	};

	void Do(int tubeLength, wchar_t *solidGroup)
	{
#if 0
		__StoredBase parameters;
		CBase base(
			parameters.name()
			, CreateDataBase<__StoredBase::type_list>()
			, parameters.tables
			);
		if(base.IsOpen())
		{
			unsigned CurrentOptionTableID;
			{
				/// \brief вставил в базу с результатом пороги дефектов
				CurrentOptionTable t;
				TL::foreach<CurrentOptionTable::items_list, __set_param__>()(
					t.items
					, Singleton<ThresholdsTable>::Instance().items
					);
				t.items.get<LengthTube>().value = Singleton<ParametersTable>::Instance().items.get<TubeMinLength>().value;

				CurrentOptionTableID = Select<CurrentOptionTable>(base).eq_all<CurrentOptionTable::items_list>(&t.items).Execute();
				if(0 == CurrentOptionTableID)
				{
					Insert_Into<CurrentOptionTable>(t, base).Execute();
					CurrentOptionTableID = Select<CurrentOptionTable>(base).eq_all<CurrentOptionTable::items_list>(&t.items).Execute();
				}
			}

			StoredTable st;

			st.items.get<DteTme>().value = COleDateTime::GetCurrentTime();

			st.items.get<LengthTube>().value = tubeLength;
			
			st.items.get<ID<CurrentOptionTable>>().value = CurrentOptionTableID;
			st.items.get<SolidGroup	>().value = solidGroup;
			__set_packet_data set_packet_data(base);
			TL::foreach<PacketParamTable::items_list, __set_packet__>()(
				Singleton<PacketParamTable>::Instance().items
				, set_packet_data 
				);
			/// \brief вствил в базу пакет
			try
			{
				Insert_Into<PackageTable>(set_packet_data.table, base).Execute();
			}
			catch(...){};
			wchar_t query[512];
			wsprintf(query,  L"SELECT ID FROM PackageTable WHERE PackageID=%d AND CustomerID=%d AND OperatorID=%d"
				, set_packet_data.table.items.get<ID<Package>>().value
				, set_packet_data.table.items.get<ID<Customer>>().value
				, set_packet_data.table.items.get<ID<Operator>>().value
				);
			CMD(base).CommandText(query).Execute().GetValue(L"ID", st.items.get<ID<PackageTable>>().value);
	/// \brief  измеренные данные  
			ItemData<Thick> &thick  = Singleton<ItemData<Thick>>::Instance();
			double t[App::count_zones];
			for(int i = 0; i < App::count_zones; ++i)
			{
				double z = thick.bufferMin[0][i];
				for(int j = 1; j < App::count_Thick_sensors; ++j)
				{
					if(z > thick.bufferMin[j][i])  z = thick.bufferMin[j][i];
				}
				t[i] = z;
			}
			__set_data__<IDThickMax>()(st, t, base);
			///*
			for(int i = 0; i < App::count_zones; ++i)
			{
				double z = thick.bufferMax[0][i];
				for(int j = 1; j < App::count_Thick_sensors; ++j)
				{
					if(z < thick.bufferMax[j][i])  z = thick.bufferMax[j][i];
				}
				t[i] = z;
			}
			__set_data__<IDThickMax>()(st, t, base);

			int crossCount = Singleton<ParametersTable>::Instance().items.get<CrossCountSensors>().value;
			ItemData<Cross> &cross  = Singleton<ItemData<Cross>>::Instance();
			for(int i = 0; i < crossCount; ++i)
			{
				double z = cross.buffer[0][i];
				for(int j = 1; j < App::count_cross_sensors; ++j)
				{
					if(z < cross.buffer[j][i])  z = cross.buffer[j][i];
				}
				t[i] = z;
			}
			__set_data__<IDCross>()(st, t, base);

			ItemData<Long> &longX  = Singleton<ItemData<Long>>::Instance();
			for(int i = 0; i < App::count_zones; ++i)
			{
				double z = longX.buffer[0][i];
				for(int j = 1; j < App::count_long_sensors; ++j)
				{
					if(z < longX.buffer[j][i])  z = longX.buffer[j][i];
				}
				t[i] = z;
			}
			__set_data__<IDLong>()(st, t, base);
			//*/
			{
				char (&resultData)[App::count_zones] = Singleton<ResultData>::Instance().status;

				int hash = HashFAQ6((unsigned char *)resultData, sizeof(resultData));
				st.items.get<ResultID>().value = hash;
				try
				{
					wchar_t *query = L"INSERT INTO ResultTable (ID, ResultArr) VALUES(?, ?)";
					CMD(base).CommandText(query)
						.Param(hash)
						.Param(resultData)
						.Execute();			
				}catch(...){}

			}
			//........................................
			Insert_Into<StoredTable>(st, base).Execute();
		}
#endif
	}
}