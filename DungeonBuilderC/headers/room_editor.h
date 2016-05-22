#pragma once

#include "curses.h"
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <iostream>
#include "headers/command_window.h"
#include "headers/text_editor.h"
#include "headers/object_editor.h"
#include "headers/creature_editor.h"
#include "headers/exit_editor.h"
#include "headers/text_editor.h"
#include "headers/printutils.h"
#include "headers/utils.h";
#include "headers/dungeon_data.h"


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

