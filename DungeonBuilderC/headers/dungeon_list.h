#ifndef DUNGEON_LIST
#define DUNGEON_LIST

#include <vector>
#include <map>
#include "dungeon_data.h"
#include "lib.h"


using namespace std;

struct DungeonList
{
	typedef string (DungeonList::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	DungeonObject *object;

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	vector<DungeonEntity*> *entities;

	int w,h;

	
	void clearWindows();
	void resetWindows();

	//display these entities in a list and let user choose one
	DungeonEntity* load(void* _entities);

};





#endif

