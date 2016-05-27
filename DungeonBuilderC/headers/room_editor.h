#pragma once

#include <vector>
#include <map>
#include <string>
#include "dungeon_data.h"
#include "lib.h"



using namespace std;
extern DungeonRoom *g_startRoom;
extern vector<DungeonRoom*> g_roomList;
struct RoomEditor
{
	typedef string (RoomEditor::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	DungeonRoom *room;
	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;
	
	void clearWindows();
	void resetWindows();
	void load(DungeonRoom *_room);

	string edit(vector<string> args);
	string set(vector<string> args);
	string exit(vector<string> args);
	string add(vector<string> args);
};

