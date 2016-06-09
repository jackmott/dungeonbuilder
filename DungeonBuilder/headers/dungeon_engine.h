
#ifndef DUNGEON_ENGINE
#define DUNGEON_ENGINE

#include <map>
#include "lib.h"

struct DungeonRoom;
struct DungeonPlayer;
struct DungeonExit;

extern DungeonRoom *g_startRoom;
extern std::vector<DungeonRoom*> g_roomList;

struct DungeonEngine
{
	typedef int (DungeonEngine::*commandFunction) (std::string);			
	typedef int (DungeonEngine::*ActionFunction) (std::string,std::string);

	std::map<std::string,ActionFunction> actionMap;
	std::map<std::string,commandFunction> cmdMap;
	std::map<std::string,DungeonExit*> moveMap;

	std::vector<std::string> textBuffer;
	
	DungeonRoom *room;
	DungeonPlayer *player;

	WINDOW *headerWindow;	
	WINDOW *commandWindow;
	WINDOW *mainWindow;

	int w,h;
	int turns =0;
	
	int renderOffset;
	int pageSize;

	void updateCmdMap();
	void look();
	void clearWindows();
	void resetWindows();
	void load(DungeonRoom *_room, DungeonPlayer *_player);
	void render(unsigned long offset);
	void showContents(std::vector<DungeonObject*> * o, int depth = 0);
	void addToBuffer(std::vector<std::string> *v);
	void gameLogic(int turnsUsed);
	void updatePhysicalObjects();
	void checkTriggers(DungeonObject* o);
	
	
	int drop(std::string args);
	int inventory(std::string args);
	int put(std::string args);
	int open(std::string args);
	int take(std::string args);
	int exit(std::string args);
	int pageDown(std::string args);
	int pageUp(std::string args);
	int action(std::string actionStr,std::string args);
	int lookCmd(std::string args);
	int examine(std::string args);

	void move(DungeonExit *dungeonExit);

};

#endif

