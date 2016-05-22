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
#include "headers/printutils.h"
#include "headers/utils.h";
#include "headers/dungeon_data.h"

using namespace std;
extern DungeonRoom *g_startRoom;
extern vector<DungeonRoom*> g_roomList;
struct ExitEditor
{
	typedef string (ExitEditor::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	DungeonExit *dungeonExit;
	DungeonRoom *fromRoom;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	DungeonRoom* load(DungeonExit *_dungeonExit,DungeonRoom *_fromRoom);

	string edit(vector<string> args);
	string exit(vector<string> args);
	string create(vector<string> args);
};

