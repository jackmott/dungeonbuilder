#ifndef ROOM_EDITOR_H
#define ROOM_EDITOR_H

#include <vector>
#include <map>
#include <string>
#include "lib.h"

struct DungeonRoom;


extern DungeonRoom *g_startRoom;
extern std::vector<DungeonRoom*> g_roomList;
struct RoomEditor
{
	typedef std::string  (RoomEditor::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	DungeonRoom *room;
	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	int w,h;
	
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