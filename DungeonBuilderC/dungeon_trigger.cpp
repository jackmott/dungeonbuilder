#include "dungeon_trigger.h"
#include "utils.h"

using namespace std;

DungeonTrigger::DungeonTrigger()
{
	uid = getUID();
}

DungeonTrigger::~DungeonTrigger()
{

}

string DungeonTrigger::getName()
{
	return TRIGGER_STRS[(int)type];
}