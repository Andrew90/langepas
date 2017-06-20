#pragma once
#include "Base/TablesDefine.h"
#include "Base/tables.hpp"

STR_PARAM(Package, 128, L"")
STR_PARAM(Customer, 128, L"something")
STR_PARAM(Operator, 128, L"Somebody")

struct CustomerTable
 {
	 typedef TL::MkTlst<
		 Customer
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"CustomerTable";}
 };

struct OperatorTable
 {
	 typedef TL::MkTlst<
		 Operator
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"OperatorTable";}
 };

 struct PacketParamTable
 {
	 typedef TL::MkTlst<
		 Package
		 , Customer
		 , Operator
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"PacketParamTable";}
 };

struct PackageBase
 {
	 typedef TL::MkTlst<
	  PacketParamTable
		, CustomerTable
		, OperatorTable
	 >::Result type_list;
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	 wchar_t path[512];
	 const wchar_t *name();
	 void Init();
 };