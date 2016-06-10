#ifndef DUNGEON_OBJECT_LIST_H
#define DUNGEON_OBJECT_LIST_H

#include <string>
#include <vector>
#include <map>
#include "lib.h"

struct DungeonObject;

struct DungeonObjectList
{
	typedef DungeonObject* (DungeonObjectList::*commandFunction) (std::vector<std::string>);			
	std::map<std::string,commandFunction> cmdMap;

	

	WINDOW *responseWindow;
	WINDOW *commandWindow;
	WINDOW *mainWindow;
	WINDOW *headerWindow;

	std::vector<DungeonObject*> objects;	
	size_t pos;

	
	void clearWindows();
	void resetWindows();

	//display these entities in a list and let user choose one
	DungeonObject* load(std::vector<DungeonObject*> _objects);
	DungeonObject* pickObject(int id);
	DungeonObject* newObject(std::vector<std::string> args);
	DungeonObject* pageUp(std::vector<std::string> args);
	DungeonObject* pageDown(std::vector<std::string> args);

};





#endif

