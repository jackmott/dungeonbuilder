#pragma once

#include "curses.h"
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "headers/command_window.h"
#include "headers/dungeon_editor.h"
#include "headers/printutils.h"
#include "headers/utils.h";
#include "headers/dungeon_data.h"

using namespace std;

struct RoomEditor
{
	typedef void (RoomEditor::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;

	
	void reset();
	void load(DungeonRoom room);

	void edit(vector<string> args);
	void exit(vector<string> args);
	
};

