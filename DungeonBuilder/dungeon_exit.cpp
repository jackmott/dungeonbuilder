#include "dungeon_serializer.h"
#include "dungeon_exit.h"
#include "dungeon_room.h"
#include <sstream>
#include "utils.h"

using namespace std;

extern vector<DungeonExit*> g_exitList;

DungeonExit::DungeonExit()
{
	isDoor = false;
	isOpen = true;
	distance = 1;
	room = nullptr;
	uid = getUID();
	g_exitList.push_back(this);
}

DungeonExit::DungeonExit(string json)
{

}

DungeonExit::~DungeonExit()
{

}

string DungeonExit::toJSON()
{
	ostringstream sout;
	sout << writeInt(uid);
	sout << writeVectorString(names);
	sout << writeBool(isOpen);
	sout << writeBool(isDoor);
	sout << writeInt(distance);
	sout << writeString(openingText);
	sout << writeString(closingText);
	sout << writeString(openText);
	sout << writeString(closedText);
	sout << writeEntity(room);
	return sout.str();
}
