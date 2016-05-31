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
	WINDOW *headerWindow;

	vector<DungeonRoom*> rooms;
	DungeonExit* fromExit;
	
	int pos;

	
	void clearWindows();
	void resetWindows();

	//display these entities in a list and let user choose one
	DungeonRoom* load(vector<DungeonRoom*> _rooms, DungeonExit* _fromExit = nullptr);
	DungeonRoom* pickRoom(int id);
	DungeonRoom* newRoom(vector<string> args);
	DungeonRoom* pageUp(vector<string> args);
	DungeonRoom* pageDown(vector<string> args);

};





#endif

