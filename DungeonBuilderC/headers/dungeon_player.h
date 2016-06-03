#ifndef DUNGEON_PLAYER_H
#define DUNGEON_PLAYER_H


#include "dungeon_entity.h"

struct DungeonRoom;
struct DungeonAction;
struct DungeonObject;

struct DungeonPlayer: DungeonEntity
{
	DungeonPlayer();
	~DungeonPlayer();

	std::vector<std::string> description;
	int hitpoints;
	int maxhitpoints;
	int score;
	std::vector<DungeonObject*> objects;
	DungeonRoom* location;

	void heal(int amount);
};
#endif
