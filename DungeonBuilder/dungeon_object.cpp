#include "dungeon_serializer.h"
#include "dungeon_object.h"
#include <sstream>
#include "utils.h"
#include "json.h"

using namespace std;

extern vector<DungeonObject*> g_objectList;
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
	g_objectList.push_back(this);

}

DungeonObject::DungeonObject(void* _json)
{
	entityType = ENTITY_TYPE::Object;
	json_value* json = (json_value*)_json;

	loadInt(uid,json);
	loadVectorString(names,json);
	loadString(description,json);
	loadInt(durability,json);
	loadInt(mass,json);
	loadInt(size,json);
	loadBool(canOpen,json);
	loadBool(canTake,json);
	loadBool(isOpen,json);
	loadBool(isLight,json);
		
}

DungeonObject::~DungeonObject()
{

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
