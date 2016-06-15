#ifndef DUNGEON_ACTION_H
#define DUNGEON_ACTION_H

#include "dungeon_entity.h"

struct DungeonEffect;

struct DungeonAction : DungeonEntity
{
	DungeonAction();
	DungeonAction(void* _json);
	~DungeonAction();
	void fixUpPointers();
	std::string output;
	bool needToHold;

	//These will be DungeonEntities
	std::vector<DungeonEntity*> targets;
	std::vector<DungeonEffect*> effects;

	std::string toJSON();
};


#endif
