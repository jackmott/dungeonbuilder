#ifndef DUNGEON_ROOM_LIST_H
#define DUNGEON_ROOM_LIST_H

#include "abstract_editor.h"

struct DungeonRoom;
struct DungeonExit;

struct DungeonRoomList : AbstractEditor
{
	typedef DungeonRoom* (DungeonRoomList::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;
	
	std::vector<DungeonRoom*> rooms;
	DungeonExit* fromExit;
			
	void clearWindows();
	void resetWindows();

	//display these entities in a list and let user choose one
	DungeonRoom* load(const std::vector<DungeonRoom*> &_rooms, DungeonExit* _fromExit = nullptr);
	DungeonRoom* pickRoom(int id);
	DungeonRoom* newRoom(std::vector<std::string> args);
	
};





#endif

