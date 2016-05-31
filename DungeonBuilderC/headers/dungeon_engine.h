
#ifndef DUNGEON_ENGINE
#define DUNGEON_ENGINE

#include <map>
#include "dungeon_data.h"
#include "lib.h"


extern DungeonRoom *g_startRoom;
extern vector<DungeonRoom*> g_roomList;

struct DungeonEngine
{
	typedef string (DungeonEngine::*commandFunction) (string);			
	typedef string (DungeonEngine::*ActionFunction) (string,string);

	map<string,ActionFunction> actionMap;
	map<string,commandFunction> cmdMap;
	map<string,DungeonExit*> moveMap;

	vector<string> textBuffer;
	
	DungeonRoom *room;
	DungeonPlayer *player;

	WINDOW *headerWindow;	
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;
	
	int renderOffset;
	int pageSize;

	void updateCmdMap();
	void look();
	void clearWindows();
	void resetWindows();
	void load(DungeonRoom *_room, DungeonPlayer *_player);
	void render(unsigned long offset);
	void showContents(vector<DungeonObject*> * o, int depth = 0);
	void addToBuffer(vector<string> *v);
	
	string drop(string args);
	string inventory(string args);
	string put(string args);
	string open(string args);
	string take(string args);
	string exit(string args);
	string pageDown(string args);
	string pageUp(string args);
	string action(string, actionStr,string args);
	string lookCmd(string args);
	string examine(string args);

	void move(DungeonExit *dungeonExit);
};

#endif

