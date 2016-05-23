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
#include "headers/utils.h"
#include "headers/dungeon_data.h"
#include "headers/string_constants.h"

using namespace std;

struct CreatureEditor
{
	typedef string (CreatureEditor::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	DungeonCreature *creature;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	void load(DungeonCreature *creature);

	string set(vector<string> args);
	string edit(vector<string> args);
	string exit(vector<string> args);
	string create(vector<string> args);
};

