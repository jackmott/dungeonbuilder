#pragma once
#ifndef CREATURE_EDITOR
#define CREATURE_EDITOR

#include <vector>
#include <string>
#include <map>
#include "dungeon_data.h"
#include "lib.h"

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

#endif
