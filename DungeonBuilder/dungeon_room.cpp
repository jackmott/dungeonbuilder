#include "dungeon_serializer.h"
#include "dungeon_action.h"
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

extern GlobalState globalState;

DungeonRoom::DungeonRoom()
{
	entityType = ENTITY_TYPE::Room;
	uid = getUID();
	hasLight = true;
	globalState.roomList.push_back(this);
	globalState.entityList.push_back(this);
	
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
	loadVectorEntity(actions,json);

	globalState.roomList.push_back(this);
	globalState.entityList.push_back(this);
	
}

void DungeonRoom::fixUpPointers()
{	
	for(size_t i = 0; i < objects.size();i++)
	{		
		objects[i] = dynamic_cast<DungeonObject*>(getEntityById(&globalState.objectList,(int)objects[i]));		 
	}
	for(size_t i = 0; i < creatures.size();i++)
	{		
		creatures[i] = dynamic_cast<DungeonCreature*>(getEntityById(&globalState.creatureList,(int)creatures[i]));		
	}
	for(size_t i = 0; i < exits.size();i++)
	{		
		exits[i] = dynamic_cast<DungeonExit*>(getEntityById(&globalState.exitList,(int)exits[i]));	
	}
	for (size_t i = 0; i < exits.size(); i++)
	{
		actions[i] = dynamic_cast<DungeonAction*>(getEntityById(&globalState.actionList,(int)exits[i]));
	}

	if (parent != (DungeonEntity*)-1)
		parent = (DungeonEntity*)getEntityById(&globalState.entityList,(int)parent);
	else
		parent = nullptr;
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
	sout << writeVectorEntity(actions);
	return sout.str();
}
