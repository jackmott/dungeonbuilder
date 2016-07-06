#ifndef DUNGEON_ACTION_H
#define DUNGEON_ACTION_H

#include "dungeon_entity.h"

struct DungeonEffect;
struct DungeonEntity;
struct DungeonPlayer;
struct DungeonRoom;

struct DungeonAction : DungeonEntity
{
	DungeonAction();
	DungeonAction(void* _json);
	~DungeonAction();
	void fixUpPointers();
	std::string output;
	bool needToHold;
	bool onetime;

	//These will be DungeonEntities
	std::vector<DungeonEntity*> targets;
	std::vector<DungeonEffect*> effects;
	std::string apply(std::vector<std::string>* textBuffer, DungeonEntity* targetEntity, DungeonPlayer* player, DungeonRoom* room, bool objectOnPlayer);
	std::string toJSON();
};


#endif
