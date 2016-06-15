#include "dungeon_serializer.h"
#include "dungeon_object.h"
#include "dungeon_exit.h"
#include "dungeon_creature.h"
#include "dungeon_room.h"
#include <sstream>
#include "utils.h"
#include "string_constants.h"
#include "json.h"
#include "dungeon_serializer.h"

using namespace std;

extern vector<DungeonRoom*> g_roomList;
extern vector<DungeonObject*> g_objectList;
extern vector<DungeonExit*> g_exitList;
extern vector<DungeonCreature*> g_creatureList;

DungeonRoom::DungeonRoom()
{
	entityType = ENTITY_TYPE::Room;
	uid = getUID();
	g_roomList.push_back(this);
	hasLight = true;
}

DungeonRoom::DungeonRoom(void* _json)
{
	json_value* json = (json_value*)_json;
	entityType = ENTITY_TYPE::Room;

	loadInt(uid,json);
	loadVectorString(names,json);
	loadString(description,json);
	loadBool(hasLight,json);

	// This will put bogus ints in place of the pointers
	// to be fixed up later
	// If this is too crazy for you, maybe you aren't read for...
	// THE DUNGEON
	loadVectorEntity(objects,json);
	loadVectorEntity(creatures,json);
	loadVectorEntity(exits,json);
	
}

void DungeonRoom::fixUpPointers()
{	
	for(int i = 0; i < objects.size();i++)
	{
		int uid = (int)objects[i];
		DungeonObject* o = dynamic_cast<DungeonObject*>(getEntityById(&g_objectList,uid));
		objects[i] = o;
	}
	for(int i = 0; i < creatures.size();i++)
	{
		int uid = (int)creatures[i];
		DungeonCreature* c = dynamic_cast<DungeonCreature*>(getEntityById(&g_creatureList,uid));
		creatures[i] = c;
	}
	for(int i = 0; i < exits.size();i++)
	{
		int uid = (int)exits[i];
		DungeonExit* e = dynamic_cast<DungeonExit*>(getEntityById(&g_exitList,uid));
		exits[i] = e;
	}
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
