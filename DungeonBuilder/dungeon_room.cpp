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
extern vector<DungeonEntity*> g_entityList;

DungeonRoom::DungeonRoom()
{
	entityType = ENTITY_TYPE::Room;
	uid = getUID();
	hasLight = true;
	g_roomList.push_back(this);
	g_entityList.push_back(this);
	
}

DungeonRoom::DungeonRoom(void* _json)
{
	json_value* json = (json_value*)_json;
	entityType = ENTITY_TYPE::Room;

	loadInt(uid,json);
	loadEntity(parent,json);
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

	g_roomList.push_back(this);
	g_entityList.push_back(this);
	
}

void DungeonRoom::fixUpPointers()
{	
	for(int i = 0; i < objects.size();i++)
	{		
		objects[i] = dynamic_cast<DungeonObject*>(getEntityById(&g_objectList,(int)objects[i]));		 
	}
	for(int i = 0; i < creatures.size();i++)
	{		
		creatures[i] = dynamic_cast<DungeonCreature*>(getEntityById(&g_creatureList,(int)creatures[i]));		
	}
	for(int i = 0; i < exits.size();i++)
	{		
		exits[i] = dynamic_cast<DungeonExit*>(getEntityById(&g_exitList,(int)exits[i]));	
	}
	if (parent != nullptr)
		parent = (DungeonEntity*)getEntityById(&g_entityList,(int)parent);
}

DungeonRoom::~DungeonRoom()
{
	//todo remove from roomlist
}

string DungeonRoom::toJSON() 
{
	ostringstream sout;
	sout << writeInt(uid);
	sout << writeEntity(parent);
	sout << writeVectorString(names);
	sout << writeString(description);
	sout << writeBool(hasLight);
	sout << writeVectorEntity(objects);
	sout << writeVectorEntity(creatures);
	sout << writeVectorEntity(exits);
	return sout.str();
}
