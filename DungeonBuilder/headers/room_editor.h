#ifndef ROOM_EDITOR_H
#define ROOM_EDITOR_H

#include "abstract_editor.h"

struct DungeonRoom;

struct RoomEditor : AbstractEditor
{
	typedef std::string  (RoomEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonRoom *room;
		
	void clearWindows();
	void resetWindows();

	void load(DungeonRoom *_room);
	
	std::string move(std::vector<std::string> args);
	std::string edit(std::vector<std::string> args);	
	std::string del(std::vector<std::string> args);
	std::string set(std::vector<std::string> args);
	std::string exit(std::vector<std::string> args);
	std::string add(std::vector<std::string> args);
};

#endif