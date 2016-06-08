#include "dungeon_room.h"
#include "dungeon_creature.h"
#include "dungeon_exit.h"
#include "dungeon_serializer.h"
#include "dungeon_trigger.h"
#include "utils.h"
#include <sstream>

using namespace std;

extern vector<DungeonTrigger*> g_triggerList;

DungeonTrigger::DungeonTrigger()
{
	uid = getUID();
	needToHold = true;
	type = TRIGGER_TYPE::Proximity;
	magnitude = 1;
	g_triggerList.push_back(this);
}

DungeonTrigger::DungeonTrigger(string json)
{

}

DungeonTrigger::~DungeonTrigger()
{

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
	sout << writeBool(needToHold);
	sout << writeString(output);
	sout << writeInt(magnitude);
	sout << writeVectorEntity(objects);
	sout << writeVectorEntity(rooms);
	sout << writeVectorEntity(creatures);
	sout << writeVectorEntity(effects);


	return sout.str();
}

