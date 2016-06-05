#ifndef DUNGEON_OBJECT_H
#define DUNGEON_OBJECT_H

#include "dungeon_entity.h"

struct DungeonAction;
struct DungeonTrigger;

struct DungeonObject: DungeonEntity
{
	DungeonObject();
	~DungeonObject();

	std::vector<std::string> description;

	int damage;  //0 if not a weapon
	int mass;
	int size;

	bool canOpen;
	bool isOpen;
	bool canTake;
	bool isLight;
	
	std::vector<DungeonObject*> contents;
	std::vector<DungeonAction*> actions;
	std::vector<DungeonTrigger*> triggers;
	
	std::string toJSON();
};
#endif