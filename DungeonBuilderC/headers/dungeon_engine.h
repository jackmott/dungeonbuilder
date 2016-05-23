
#ifndef ROOM_PLAYER
#define ROOM_PLAYER

#include "curses.h"
#include "dungeon_data.h"
#include "printutils.h"
#include "utils.h"
#include "command_window.h"
#include "string_constants.h"
#include <map>

extern DungeonRoom *g_startRoom;
extern vector<DungeonRoom*> g_roomList;

struct DungeonEngine
{
	typedef string (DungeonEngine::*commandFunction) (vector<string>);			

	
	map<string,commandFunction> cmdMap;
	map<string,DungeonRoom*> moveMap;

	DungeonRoom *room;
	DungeonPlayer *player;

	WINDOW *headerWindow;
	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;
	
	void clearWindows();
	void resetWindows();
	void load(DungeonRoom *_room, DungeonPlayer *_player);

	string take(vector<string> args);
	string exit(vector<string> args);
	string use(vector<string> args);

};

#endif

