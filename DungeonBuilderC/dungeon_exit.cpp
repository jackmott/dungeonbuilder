#include "dungeon_exit.h"
#include "dungeon_room.h"
#include <sstream>
#include "utils.h"

using namespace std;


DungeonExit::DungeonExit()
{
	isDoor = false;
	isOpen = true;
	distance = 1;
	room = nullptr;
	uid = getUID();
}

DungeonExit::~DungeonExit()
{

}

string DungeonExit::toJSON()
{
	ostringstream sout;

	sout << "\"name\":" << vectorStringToJSON(getNames()) << ", \"isOpen\":" << isOpen
		<< ", \"isDoor\":" << isDoor << ", \"distance\":" << distance << ", \"openingText\":\"" << openingText
		<< "\", \"closingText\":\"" << closingText << "\", \"openText\":\"" << openText << "\", \"closedText\":\"" << closedText
		<< "\", \"links\":" << room->uid;

	return sout.str();
}
