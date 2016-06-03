#ifndef DUNGEON_ACTION_H
#define DUNGEON_ACTION_H

#include "dungeon_entity.h"

struct DungeonEffect;

struct DungeonAction : DungeonEntity
{
	DungeonAction();
	~DungeonAction();
	std::string output;
	bool needToHold;
	std::vector<DungeonEffect*> effects;
};


#endif
