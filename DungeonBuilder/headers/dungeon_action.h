#ifndef DUNGEON_ACTION_H
#define DUNGEON_ACTION_H

#include "dungeon_entity.h"

struct DungeonEffect;

struct DungeonAction : DungeonEntity
{
	DungeonAction();
	DungeonAction(std::string json);
	~DungeonAction();
	std::string output;
	bool needToHold;

	//These will be DungeonEntities
	std::vector<void*> targets;

	std::vector<DungeonEffect*> effects;

	std::string toJSON();
};


#endif
