#include "dungeon_serializer.h"
#include "dungeon_creature.h"
#include "dungeon_object.h"
#include "dungeon_player.h"
#include <sstream>
#include "utils.h"
#include "json.h"

using namespace std;

extern GlobalState globalState;

DungeonCreature::DungeonCreature()
{
	entityType = ENTITY_TYPE::Creature;
	uid = getUID();
	hitpoints = 100;
	globalState.creatureList.push_back(this);
	globalState.entityList.push_back(this);
}

DungeonCreature::DungeonCreature(void* _json)
{
	json_value* json = (json_value*)_json;
	entityType = ENTITY_TYPE::Creature;
	loadInt(uid,json);
	loadEntity(parent,json);
	loadVectorString(names,json);
	loadString(description,json);
	loadInt(hitpoints,json);
	loadInt(alignment,json);
	loadVectorEntity(objects,json);
	globalState.creatureList.push_back(this);
	globalState.entityList.push_back(this);
}

DungeonCreature::~DungeonCreature()
{

}

void DungeonCreature::fixUpPointers()
{
	for(size_t i = 0; i < objects.size();i++)
	{
		objects[i] = (DungeonObject*)getEntityById(&globalState.objectList,(int)objects[i]);
	}
	if (parent != (DungeonEntity*)-1)
		parent = (DungeonEntity*)getEntityById(&globalState.entityList,(int)parent);
	else
		parent = nullptr;
}

void DungeonCreature::kill(vector<string> *textBuffer)
{
	textBuffer->push_back("You have killed the "+getPrimaryName()+"!");
	//remove from all things
}


void DungeonCreature::applyDamage(vector<string> *textBuffer,int dmg)
{
	hitpoints -= dmg;
	if(hitpoints < 0)
	{
		return kill(textBuffer);
	}
	textBuffer->push_back("You damage the "+getPrimaryName()+".");
}

void DungeonCreature::attack(vector<string> *textBuffer,int magnitude, DungeonObject *weapon,DungeonPlayer *player)
{
	hitpoints = hitpoints - magnitude;
	if(hitpoints <= 0) return kill(textBuffer);
	textBuffer->push_back("You hit the " + getPrimaryName());
}

string DungeonCreature::toJSON()
{
	ostringstream sout;
	sout << writeInt(uid);
	sout << writeEntity(parent);
	sout << writeVectorString(names);
	sout << writeString(description);
	sout << writeInt(hitpoints);
	sout << writeInt(alignment);
	sout << writeVectorEntity(objects);
	return sout.str();
}
