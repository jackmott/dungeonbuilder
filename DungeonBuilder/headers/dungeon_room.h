#ifndef DUNGEON_ROOM_H
#define DUNGEON_ROOM_H

#include "dungeon_entity.h"

struct DungeonCreature;
struct DungeonObject;
struct DungeonExit;

struct DungeonRoom : DungeonEntity
{
	DungeonRoom();
	DungeonRoom(std::string json);
	~DungeonRoom();

	bool hasLight;
	std::vector<std::string> description;
	std::vector<DungeonExit*> exits;
	std::vector<DungeonObject*> objects;
	std::vector<DungeonCreature*> creatures;

	std::string toJSON() ;

};

#endif
