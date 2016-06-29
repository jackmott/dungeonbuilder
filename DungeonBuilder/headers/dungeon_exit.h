#ifndef DUNGEON_EXIT_H
#define DUNGEON_EXIT_H

#include "dungeon_entity.h"
#include <map>
struct DungeonRoom;


struct DungeonExit: DungeonEntity
{
	DungeonExit();
	DungeonExit(void* _json);
	~DungeonExit();
	void fixUpPointers();

	bool isDoor;   // or window, or pile of rocks, whatever	
	bool isOpen;   //or whatever

	int distance;
    

	std::string openingText;
	std::string closingText;
	std::string travelText;
	std::string openText;
	std::string closedText;

	DungeonRoom* room;

	DungeonExit* mirror();
    static std::string getOppositeDirection(std::string);
	
	std::string toJSON();
};

#endif
