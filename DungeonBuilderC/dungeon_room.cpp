#include "dungeon_object.h"
#include "dungeon_exit.h"
#include "dungeon_creature.h"
#include "dungeon_room.h"
#include <sstream>
#include "utils.h"
#include "string_constants.h"

using namespace std;

extern vector<DungeonRoom*> g_roomList;

DungeonRoom::DungeonRoom()
{
	uid = getUID();
	g_roomList.push_back(this);
	hasLight = true;
}

DungeonRoom::~DungeonRoom()
{
	//todo remove from roomlist
}

string DungeonRoom::toJSON()
{
	ostringstream sout;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"uid\":" << uid << "," << endl;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"name\":" << vectorStringToJSON(getNames()) << "," << endl;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"description\":" << vectorStringToJSON(description) << "," << endl;


	sout << STR_TAB << STR_TAB << STR_TAB << "\"objects\":[" << endl;
	for(auto i = 0u; i < objects.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << objects[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]," << endl;


	sout << STR_TAB << STR_TAB << STR_TAB << "\"creatures\":[" << endl;
	for(auto i = 0u; i < creatures.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << creatures[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]," << endl;


	sout << STR_TAB << STR_TAB << STR_TAB << "\"exits\":[" << endl;
	for(auto i = 0u; i < exits.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << exits[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]" << endl;
	return sout.str();
}
