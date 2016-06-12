#include "dungeon_serializer.h"
#include "dungeon_object.h"
#include "dungeon_exit.h"
#include "dungeon_creature.h"
#include "dungeon_room.h"
#include <sstream>
#include "utils.h"
#include "string_constants.h"

using namespace std;

extern vector<DungeonRoom*> g_roomList;

DungeonRoom::DungeonRoom()
{
	entityType = ENTITY_TYPE::Room;
	uid = getUID();
	g_roomList.push_back(this);
	hasLight = true;
}

DungeonRoom::DungeonRoom(string json)
{
	entityType = ENTITY_TYPE::Room;
}

DungeonRoom::~DungeonRoom()
{
	//todo remove from roomlist
}

string DungeonRoom::toJSON() 
{
	ostringstream sout;
	sout << writeInt(uid);
	sout << writeVectorString(names);
	sout << writeString(description);
	sout << writeBool(hasLight);
	sout << writeVectorEntity(objects);
	sout << writeVectorEntity(creatures);
	sout << writeVectorEntity(exits);
	return sout.str();
}
