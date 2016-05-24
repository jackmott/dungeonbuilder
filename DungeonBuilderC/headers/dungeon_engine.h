
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
	typedef string (DungeonEngine::*commandFunction) (vector<string>);			

	
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

	void addToBuffer(vector<string> *v);

	string take(vector<string> args);
	string exit(vector<string> args);
	string use(vector<string> args);
	string lookCmd(vector<string> args);

};

#endif

