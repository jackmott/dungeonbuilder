#pragma once

#include <map>
#include "dungeon_data.h"
#include "lib.h"


using namespace std;

struct ObjectEditor
{
	typedef string (ObjectEditor::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	DungeonObject *object;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	void load(DungeonObject *object);

	string edit(vector<string> args);
	string exit(vector<string> args);
	string add(vector<string> args);
	string set(vector<string> args);
	string del(vector<string> args);
};

