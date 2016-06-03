#ifndef DUNGEON_CREATURE_H
#define DUNGEON_CREATURE_H

#include "dungeon_entity.h"

struct DungeonPlayer;
struct DungeonObject;


struct DungeonCreature: DungeonEntity
{
	DungeonCreature();
	~DungeonCreature();

	std::vector<std::string> description;
	int hitpoints;
	int alignment;

	std::string attack(DungeonObject *weapon,DungeonPlayer *player);


	std::string toJSON();
};
#endif