#pragma once
#ifndef CREATURE_EDITOR
#define CREATURE_EDITOR

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
	WINDOW *headerWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	void load(DungeonCreature *creature);

	string add(vector<string> args);
	string del(vector<string> args);
	string set(vector<string> args);
	string edit(vector<string> args);
	string exit(vector<string> args);	
};

#endif
