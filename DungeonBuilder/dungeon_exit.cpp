#include "dungeon_serializer.h"
#include "dungeon_exit.h"
#include "dungeon_room.h"
#include <sstream>
#include "utils.h"
#include "json.h"

using namespace std;

extern GlobalState globalState;

DungeonExit::DungeonExit()
{
	entityType = ENTITY_TYPE::Exit;
	isDoor = false;
	isOpen = true;
	distance = 1;
	room = nullptr;
	uid = getUID();
	globalState.exitList.push_back(this);
	globalState.entityList.push_back(this);
}

DungeonExit::DungeonExit(void* _json)
{
	entityType = ENTITY_TYPE::Exit;
	json_value* json = (json_value*)_json;
	loadInt(uid,json);
	loadEntity(parent,json);
	loadVectorString(names,json);
	loadBool(isOpen,json);
	loadBool(isDoor,json);
	loadInt(distance,json);
	loadString(travelText,json);
	loadString(openingText,json);
	loadString(closingText,json);
	loadString(openText,json);
	loadString(closedText,json);
	loadEntity(room,json);
	globalState.exitList.push_back(this);
	globalState.entityList.push_back(this);
}

DungeonExit::~DungeonExit()
{
	//remove the exit from the global list and the room list
	removePointer(&globalState.exitList,this);
	for(auto r : globalState.roomList)
	{
		removePointer(&r->exits,this);
	}

}

DungeonExit* DungeonExit::mirror()
{
	if(room != parent) {
		DungeonExit *mirrorExit = new DungeonExit();
		mirrorExit->parent = room;
		mirrorExit->room = (DungeonRoom*)parent;
		mirrorExit->setPrimaryName(DungeonExit::getOppositeDirection(getPrimaryName()));
		room->exits.push_back(mirrorExit);
		return mirrorExit;
	}
	return nullptr;
}

string DungeonExit::getOppositeDirection(string direction)
{
	string lowerD = toLower(direction);
	string result = "placeholder";
	if(lowerD == "north")
	{
		result = "south";
	}
	else if(lowerD == "west")
	{
		result = "east";
	}
	else if(lowerD == "east")
	{
		result = "west";
	}
	else if(lowerD == "south")
	{
		result = "north";
	}
	else if(lowerD == "up")
	{
		result = "down";
	}
	return result;
}

void DungeonExit::fixUpPointers()
{
	room = dynamic_cast<DungeonRoom*>(getEntityById(&globalState.roomList,(int)room));
	if(parent != (DungeonEntity*)-1)
		parent = (DungeonEntity*)getEntityById(&globalState.entityList,(int)parent);
	else
		parent = nullptr;
}


string DungeonExit::toJSON()
{
	ostringstream sout;
	sout << writeInt(uid);
	sout << writeEntity(parent);
	sout << writeVectorString(names);
	sout << writeBool(isOpen);
	sout << writeBool(isDoor);
	sout << writeInt(distance);
	sout << writeString(travelText);
	sout << writeString(openingText);
	sout << writeString(closingText);
	sout << writeString(openText);
	sout << writeString(closedText);
	sout << writeEntity(room);
	return sout.str();
}
