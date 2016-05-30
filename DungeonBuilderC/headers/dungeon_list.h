#ifndef DUNGEON_LIST
#define DUNGEON_LIST

#include <vector>
#include <map>
#include "dungeon_data.h"
#include "lib.h"


using namespace std;

struct DungeonRoomList
{
	typedef DungeonRoom* (DungeonRoomList::*commandFunction) (vector<string>);			
	map<string,commandFunction> cmdMap;

	

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	vector<DungeonRoom*> rooms;

	int w,h;

	
	void clearWindows();
	void resetWindows();

	//display these entities in a list and let user choose one
	DungeonRoom* load(vector<DungeonRoom*> _rooms);
	DungeonRoom* pickRoom(int id);
	DungeonRoom* newRoom(vector<string> args);


};





#endif

