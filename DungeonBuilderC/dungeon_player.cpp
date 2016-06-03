#include "dungeon_player.h"
#include <sstream>
#include "utils.h"


DungeonPlayer::DungeonPlayer()
{
	uid = getUID();
	hitpoints = 100;
}

DungeonPlayer::~DungeonPlayer()
{

}

void DungeonPlayer::heal(int amount)
{
	hitpoints += amount;
	if(hitpoints > maxhitpoints) hitpoints = maxhitpoints;
}
