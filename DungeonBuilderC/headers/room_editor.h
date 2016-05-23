#pragma once

#ifdef _WIN32
#include "pdcurses.h"
#else
#include <ncurses.h>
#endif
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <iostream>
#include "command_window.h"
#include "text_editor.h"
#include "object_editor.h"
#include "creature_editor.h"
#include "exit_editor.h"
#include "text_editor.h"
#include "printutils.h"
#include "utils.h"
#include "dungeon_data.h"


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
	string create(vector<string> args);
};

