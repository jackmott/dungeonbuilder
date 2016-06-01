#ifndef EFFECT_EDITOR
#define EFFECT_EDITOR

#include <vector>
#include <map>
#include "dungeon_data.h"
#include "lib.h"


using namespace std;

struct EffectEditor
{
	typedef string (EffectEditor::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	DungeonEffect* effect;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	int w,h;

	
	void clearWindows();
	void resetWindows();
	void load(DungeonEffect *_effect);

	string edit(vector<string> args);
	string exit(vector<string> args);
	string add(vector<string> args);
	string set(vector<string> args);
	string del(vector<string> args);
};

#endif