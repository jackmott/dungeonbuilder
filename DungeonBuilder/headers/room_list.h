#ifndef DUNGEON_ROOM_LIST_H
#define DUNGEON_ROOM_LIST_H

#include <string>
#include <vector>
#include <map>
#include "lib.h"

struct DungeonRoom;
struct DungeonExit;

struct DungeonRoomList
{
	typedef DungeonRoom* (DungeonRoomList::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	std::vector<DungeonRoom*> rooms;
	DungeonExit* fromExit;
	
	size_t pos;

	
	void clearWindows();
	void resetWindows();

	//display these entities in a list and let user choose one
	DungeonRoom* load(std::vector<DungeonRoom*> _rooms, DungeonExit* _fromExit = nullptr);
	DungeonRoom* pickRoom(int id);
	DungeonRoom* newRoom(std::vector<std::string> args);
	DungeonRoom* pageUp(std::vector<std::string> args);
	DungeonRoom* pageDown(std::vector<std::string> args);

};





#endif

