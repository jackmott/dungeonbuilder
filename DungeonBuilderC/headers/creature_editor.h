#pragma once

#ifdef _WIN32
#include "pdcurses.h"
#else
#include <ncurses.h>
#endif
#include <vector>
#include <string>
#include <map>
#include "dungeon_data.h"

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

