#include "dungeon_serializer.h"
#include "dungeon_object.h"
#include <sstream>
#include "utils.h"
#include "json.h"

using namespace std;

extern GlobalState globalState;

DungeonObject::DungeonObject()
{
	entityType = ENTITY_TYPE::Object;
	durability = 100;
	mass =1;
	size=1;
	uid = getUID();
	canOpen = false;
	isOpen = false;
	canTake = true;
	isLight = false;
	globalState.objectList.push_back(this);
	globalState.entityList.push_back(this);

}

DungeonObject::DungeonObject(void* _json)
{
	entityType = ENTITY_TYPE::Object;
	json_value* json = (json_value*)_json;

	loadInt(uid,json);
	loadEntity(parent,json);
	loadVectorString(names,json);
	loadString(description,json);
	loadInt(durability,json);
	loadInt(mass,json);
	loadInt(size,json);
	loadBool(canOpen,json);
	loadBool(canTake,json);
	loadBool(isOpen,json);
	loadBool(isLight,json);
	loadVectorEntity(contents,json);
	loadVectorEntity(actions,json);
	loadVectorEntity(triggers,json);
	globalState.objectList.push_back(this);
	globalState.entityList.push_back(this);
		
}

DungeonObject::~DungeonObject()
{

}

void DungeonObject::fixUpPointers()
{
	for(int i = 0; i < contents.size();i++)
	{
		contents[i] = (DungeonObject*)getEntityById(&globalState.objectList,(int)contents[i]);
	}
	for(int i = 0; i < actions.size();i++)
	{
		actions[i] = (DungeonAction*)getEntityById(&globalState.actionList,(int)actions[i]);
	}
	for(int i = 0; i < triggers.size();i++)
	{
		triggers[i] = (DungeonTrigger*)getEntityById(&globalState.triggerList,(int)triggers[i]);
	}
	if (parent != (DungeonEntity*)-1)
		parent = (DungeonEntity*)getEntityById(&globalState.entityList,(int)parent);
	else
		parent = nullptr;
}

void DungeonObject::applyDamage(vector<string> *textBuffer,int dmg)
{
	durability -= dmg;
	if(durability <= 0)
	{
		destroy(textBuffer);
	}
}

void DungeonObject::destroy(vector<string> *textBuffer)
{
	textBuffer->push_back("You have destroyed the "+getPrimaryName());
	//todo remove this from all things
}
string DungeonObject::toJSON()
{
	ostringstream sout;	
	sout << writeInt(uid);
	sout << writeEntity(parent);
	sout << writeVectorString(names);
	sout << writeString(description);
	sout << writeInt(durability);
	sout << writeInt(mass);
	sout << writeInt(size);
	sout << writeBool(canOpen);
	sout << writeBool(canTake);
	sout << writeBool(isOpen);
	sout << writeBool(isLight);
	sout << writeVectorEntity(contents);
	sout << writeVectorEntity(actions);
	sout << writeVectorEntity(triggers);

	return sout.str();
	
}
