#ifndef DUNGEON_EXIT_H
#define DUNGEON_EXIT_H

#include "dungeon_entity.h"

struct DungeonRoom;


struct DungeonExit: DungeonEntity
{
	DungeonExit();
	~DungeonExit();


	bool isDoor;   // or window, or pile of rocks, whatever	
	bool isOpen;   //or whatever

	int distance;

	std::string openingText;
	std::string closingText;

	std::string openText;
	std::string closedText;

	DungeonRoom* room;
	
	std::string toJSON();
};

#endif
