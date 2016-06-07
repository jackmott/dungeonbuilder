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
	uid = getUID();
	g_roomList.push_back(this);
	hasLight = true;
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
	sout << writeVectorString(description);
	sout << writeVectorEntity(objects);
	sout << writeVectorEntity(creatures);
	sout << writeVectorEntity(exits);
	return sout.str();
}
