#ifndef DUNGEON_OBJECT_LIST_H
#define DUNGEON_OBJECT_LIST_H

#include <string>
#include <vector>
#include <map>
#include "lib.h"


struct DungeonRoom;
struct DungeonEntity;
struct DungeonCreature;
struct DungeonAction;
struct DungeonExit;
struct DungeonObject;

struct EntityDisplay
{
	int depth;
	DungeonEntity* entity;
	std::string s;	
};

struct EntityList
{
	typedef DungeonEntity* (EntityList::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	DungeonRoom* room;
	std::vector<EntityDisplay> entities;

	int pos;

	void loadAllObjects(std::vector<DungeonObject*> objects, int depth = 0);
	void loadAllCreatures(std::vector<DungeonCreature*> creatures);
	void loadAllActions(std::vector<DungeonAction*> actions);
	void loadAllExits(std::vector<DungeonExit*> exits);

	
	void clearWindows();
	void resetWindows();

	//display these entities in a list and let user choose one
	DungeonEntity* load(DungeonRoom* _room);
	DungeonEntity* pickEntity(int id);
	DungeonEntity* newObject(std::vector<std::string> args);
	DungeonEntity* pageUp(std::vector<std::string> args);
	DungeonEntity* pageDown(std::vector<std::string> args);

};





#endif

