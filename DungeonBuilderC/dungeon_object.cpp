#include "dungeon_serializer.h"
#include "dungeon_object.h"
#include <sstream>
#include "utils.h"


using namespace std;

extern vector<DungeonObject*> g_objectList;
DungeonObject::DungeonObject()
{

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
	sout << writeVectorString(names);
	sout << writeVectorString(description);
	sout << writeInt(mass);
	sout << writeInt(size);
	sout << writeBool(canOpen);
	sout << writeBool(canTake);
	sout << writeBool(isOpen);
	sout << writeVectorEntity(&contents);
	return sout.str();
	
}
