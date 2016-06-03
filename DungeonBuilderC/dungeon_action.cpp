#include "dungeon_action.h"
#include "utils.h"
#include <sstream>


DungeonAction::DungeonAction()
{
	uid = getUID();
	needToHold = true;
}

DungeonAction::~DungeonAction()
{

}
