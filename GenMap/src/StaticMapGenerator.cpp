#include "interface_lib.h"
#include "MapLoader.h"
#include "DefaultMap.h"

API_EXPORT void InitMap(SInfo* info)
{
	info->SetName("Default map");
	info->AddMember("Adam Creusevault");
	info->AddMember("Ibrahim Braham");
	info->AddMember("Matteo ");
	info->AddMember("Clément Madre");
}

API_EXPORT IMap* MapFactory()
{
	return(new MapLoader());
}

API_EXPORT void MapDelete(IMap* obj)
{
	delete(obj);
}
