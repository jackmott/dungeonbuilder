#ifndef ACTION_EDITOR
#define ACTION_EDITOR

#include <map>
#include "dungeon_data.h"
#include "lib.h"


using namespace std;

struct ActionEditor
{
	typedef string (ActionEditor::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	DungeonAction* action;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	void load(DungeonAction *_action);

	string edit(vector<string> args);
	string exit(vector<string> args);
	string add(vector<string> args);
	string set(vector<string> args);
	string del(vector<string> args);
};

#endif