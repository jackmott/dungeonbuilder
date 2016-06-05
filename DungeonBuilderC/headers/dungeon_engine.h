
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
	typedef std::string  (DungeonEngine::*commandFunction) (std::string);			
	typedef std::string  (DungeonEngine::*ActionFunction) (std::string,std::string);

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
	void checkTriggers();
	bool checkForEvil(DungeonRoom* room, int depth);
	
	std::string drop(std::string args);
	std::string inventory(std::string args);
	std::string put(std::string args);
	std::string open(std::string args);
	std::string take(std::string args);
	std::string exit(std::string args);
	std::string pageDown(std::string args);
	std::string pageUp(std::string args);
	std::string action(std::string actionStr,std::string args);
	std::string lookCmd(std::string args);
	std::string examine(std::string args);

	void move(DungeonExit *dungeonExit);

};

#endif

