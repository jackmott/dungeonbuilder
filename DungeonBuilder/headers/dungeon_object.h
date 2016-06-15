#ifndef DUNGEON_OBJECT_H
#define DUNGEON_OBJECT_H

#include "dungeon_entity.h"

struct DungeonAction;
struct DungeonTrigger;

struct DungeonObject: DungeonEntity
{
	DungeonObject();
	DungeonObject(void* _json);
	~DungeonObject();

	std::string description;

	int durability;
	int mass;
	int size;

	bool canOpen;
	bool isOpen;
	bool canTake;
	bool isLight;

	void applyDamage(std::vector<std::string> *textBuffer,int dmg);
	void destroy(std::vector<std::string> *textBuffer);

	
	std::vector<DungeonObject*> contents;
	std::vector<DungeonAction*> actions;
	std::vector<DungeonTrigger*> triggers;
	
	std::string toJSON();
};
#endif