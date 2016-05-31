#pragma once

#include <vector>
#include <string>
#include <map>
#include "dungeon_data.h"
#include "lib.h"

using namespace std;
extern DungeonRoom *g_startRoom;
extern vector<DungeonRoom*> g_roomList;
struct ExitEditor
{
	typedef string (ExitEditor::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	DungeonExit *dungeonExit;
	

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	

	
	void clearWindows();
	void resetWindows();
	DungeonRoom* load(DungeonExit *_dungeonExit);

	string set(vector<string> args);
	string add(vector<string> args);
	string del(vector<string> args);
	string edit(vector<string> args);
	string exit(vector<string> args);	
};

