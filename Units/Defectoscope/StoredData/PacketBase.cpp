#include "stdafx.h"
#include "PacketBase.h"

const wchar_t *PackageBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	int len = (int)wcslen(path);
	wsprintf(&path[len - 4], L"PacketBase");
	return path;
}

void PackageBase::Init()
{
	PackageBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<PackageBase::type_list, SetDefault<PackageBase::type_list> >()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		PacketParamTable &t = Singleton<PacketParamTable>::Instance();
		Select<PacketParamTable>(base).ID(1).Execute<>(t.items);		
	}
}