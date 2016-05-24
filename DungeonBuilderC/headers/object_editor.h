#pragma once

#ifdef _WIN32
#include "pdcurses.h"
#else
#include <ncurses.h>
#endif
#include <vector>
#include "dungeon_data.h"
#include <map>



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

