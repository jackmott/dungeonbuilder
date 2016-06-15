#include "dungeon_room.h"
#include "dungeon_creature.h"
#include "dungeon_exit.h"
#include "dungeon_serializer.h"
#include "dungeon_trigger.h"
#include "utils.h"
#include <sstream>
#include "json.h"

using namespace std;

extern vector<DungeonTrigger*> g_triggerList;
extern vector<DungeonEntity*> g_entityList;
extern vector<DungeonObject*> g_objectList;
extern vector<DungeonCreature*> g_creatureList;
extern vector<DungeonEffect*> g_effectList;
extern vector<DungeonRoom*> g_roomList;


DungeonTrigger::DungeonTrigger()
{
	entityType = ENTITY_TYPE::Trigger;
	uid = getUID();
	needToHold = true;
	type = TRIGGER_TYPE::Proximity;
	magnitude = 1;
	g_triggerList.push_back(this);
	g_entityList.push_back(this);
}

DungeonTrigger::DungeonTrigger(void* _json)
{
	entityType = ENTITY_TYPE::Trigger;
	json_value *json = (json_value*)_json;
	loadInt(uid,json);
	loadEntity(parent,json);
	loadBool(needToHold,json);
	loadString(output,json);
	loadInt(magnitude,json);
	loadVectorEntity(objects,json);
	loadVectorEntity(creatures,json);
	loadVectorEntity(effects,json);
	loadVectorEntity(rooms,json);
	g_triggerList.push_back(this);
	g_entityList.push_back(this);
}

DungeonTrigger::~DungeonTrigger()
{

}


void DungeonTrigger::fixUpPointers()
{
	for(int i =0; i < objects.size();i++)
	{
		objects[i] = (DungeonObject*)getEntityById(&g_objectList,(int)objects[i]);
	}
	for(int i =0; i < creatures.size();i++)
	{
		creatures[i] = (DungeonCreature*)getEntityById(&g_creatureList,(int)creatures[i]);
	}
	for(int i =0; i < effects.size();i++)
	{
		effects[i] = (DungeonEffect*)getEntityById(&g_effectList,(int)effects[i]);
	}
	for(int i =0; i < rooms.size();i++)
	{
		rooms[i] = (DungeonRoom*)getEntityById(&g_roomList,(int)rooms[i]);
	}
	if (parent != nullptr)
		parent = (DungeonEntity*)getEntityById(&g_entityList,(int)parent);
}
string DungeonTrigger::getPrimaryName() const
{
	return TRIGGER_STRS[(int)type];
}

bool DungeonTrigger::checkAge()
{
	if(parent->age > magnitude)
	{
		return true;
	}
	else {
		return false;
	}

}

// Recursively checks for evil up to depth rooms away
bool DungeonTrigger::checkForEvil(DungeonRoom* room,int depth)
{
	for(auto creature : room->creatures)
	{
		if (creature->alignment < 0 ) return true;
	}
	if (depth == 0) return false;
	else {		
		for(auto x : room->exits) {
			if (checkForEvil(x->room,depth-1)) return true;
		}
		return false;
	}
}


//TODO: Add support for enum class TRIGGER_TYPE
string DungeonTrigger::toJSON()
{
	ostringstream sout;

	sout << writeInt(uid);
	sout << writeEntity(parent);
	sout << writeBool(needToHold);
	sout << writeString(output);
	sout << writeInt(magnitude);
	sout << writeVectorEntity(objects);
	sout << writeVectorEntity(rooms);
	sout << writeVectorEntity(creatures);
	sout << writeVectorEntity(effects);


	return sout.str();
}

