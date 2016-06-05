#include "dungeon_trigger.h"
#include "utils.h"

using namespace std;

DungeonTrigger::DungeonTrigger()
{
	uid = getUID();
	needToHold = true;
	type = TRIGGER_TYPE::PROXIMITY;
}

DungeonTrigger::~DungeonTrigger()
{

}

string DungeonTrigger::getPrimaryName() const
{
	return TRIGGER_STRS[(int)type];
}



