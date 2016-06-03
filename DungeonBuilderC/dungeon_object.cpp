#include "dungeon_object.h"
#include <sstream>
#include "utils.h"


using namespace std;

DungeonObject::DungeonObject()
{

	damage = 0;
	mass =0;
	size=0;
	uid = getUID();
	canOpen = false;
	isOpen = false;
	canTake = true;

}

DungeonObject::~DungeonObject()
{

}
string DungeonObject::toJSON()
{
	ostringstream sout;
	sout << "\"name\":" << vectorStringToJSON(getNames()) << ", \"description\":" << vectorStringToJSON(description) << ",";
	sout << " \"damage\":" << damage << ",\"mass\":" << mass << ", \"size\":" << size << ",";
	sout << " \"canOpen\":" << canOpen << ",\"canTake\":" << canTake << ", \"isOpen\":" << isOpen << ",";
	//	sout << "\"useAlias\":" << vectorStringToJSON(&useAliases) << ", ";   TODO useAlias replaced by actions
	sout << "\"contents\":[";
	for(auto i = 0u; i < contents.size(); i++) {
		sout << "{" << contents[i]->toJSON() << "}," << endl;
	};
	sout << "]" << endl;

	return sout.str();
}
