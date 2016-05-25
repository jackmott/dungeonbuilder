
#ifndef ROOM_PLAYER
#define ROOM_PLAYER

#ifdef _WIN32
#include "pdcurses.h"
#else
#include <ncurses.h>
#endif
#include "dungeon_data.h"
#include <map>


extern DungeonRoom *g_startRoom;
extern vector<DungeonRoom*> g_roomList;

struct DungeonEngine
{
	typedef string (DungeonEngine::*commandFunction) (string);			

	
	map<string,commandFunction> cmdMap;
	map<string,DungeonRoom*> moveMap;

	vector<string> textBuffer;
	
	DungeonRoom *room;
	DungeonPlayer *player;

	WINDOW *headerWindow;	
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;
	
	int renderPos;
	void updateCmdMap();
	void look();
	void clearWindows();
	void resetWindows();
	void load(DungeonRoom *_room, DungeonPlayer *_player);
	void render(unsigned int start, unsigned int end);
	void showContents(DungeonObject * o);
	void addToBuffer(vector<string> *v);
	
	string drop(string args);
	string put(string args);
	string open(string args);
	string take(string args);
	string exit(string args);
	string use(string args);
	string lookCmd(string args);
	string examine(string args);

};

#endif

