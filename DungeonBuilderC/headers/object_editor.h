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
#include "headers/string_constants.h"

using namespace std;

struct ObjectEditor
{
	typedef string (ObjectEditor::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	DungeonObject *object;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	void load(DungeonObject *object);

	string edit(vector<string> args);
	string exit(vector<string> args);
	string create(vector<string> args);
	string add(vector<string> args);
	string set(vector<string> args);
};

