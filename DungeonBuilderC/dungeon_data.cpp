#include "dungeon_data.h"
#include <sstream>
#include "utils.h"
#include "string_constants.h"


using namespace std;

DungeonRoom::DungeonRoom()
{
	name = "";
}

DungeonRoom::~DungeonRoom()
{
	
}

string DungeonRoom::toJSON()
{
	ostringstream sout;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"ID\":\"" << uid << "\"," << endl;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"Name\":\"" << name << "\"," << endl;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"Description\":\"" << join(0,description,"\n") << "\"," << endl;

	sout << STR_TAB << STR_TAB << STR_TAB << "\"Objects\":[" << endl;
	for (auto i = 0u; i < objects.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << objects[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]," << endl;

	sout << STR_TAB << STR_TAB << STR_TAB << "\"Creatures\":[" << endl;
	for (auto i = 0u; i < creatures.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << creatures[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]," << endl;

	sout << STR_TAB << STR_TAB << STR_TAB << "\"Exits\":[" << endl;
	for (auto i = 0u; i < exits.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << exits[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]" << endl;
	return sout.str();
}

DungeonExit::DungeonExit()
{
	name = "";	
}

DungeonExit::~DungeonExit()
{

}

string DungeonExit::toJSON()
{
	ostringstream sout;
	sout << "\"Name\":\"" << name << "\", \"Description\":\"" << join(0,description,"\n")
		 << "\", \"links\":" << room->uid;
	return sout.str();
}

DungeonObject::DungeonObject()
{
	name="";
	
	damage = 0;
	mass =0;
	size=0;

	container = false;
	open = false;

	takeable = true;
	takeMessage = "";
	dropMessage= "";

	selfEffect = USE_EFFECT::NONE;
	creatureEffect = USE_EFFECT::NONE;
	objectEffect = USE_EFFECT::NONE;
 
}

DungeonObject::~DungeonObject()
{

}
string DungeonObject::toJSON()
{
	ostringstream sout;
	sout << "\"Name\":\"" << name << "\", \"Description\":\"" << join(0,description,"\n") << "\"";
	return sout.str();
}

DungeonCreature::DungeonCreature()
{
	hitpoints = 100;
}
string DungeonCreature::attack(DungeonObject *weapon, DungeonPlayer *player)
{
	hitpoints = hitpoints - weapon->damage;
	if (hitpoints <= 0) return "You have killed the "+name+"!";
	return "You hit the " + name;
}
DungeonCreature::~DungeonCreature()
{
}
string DungeonCreature::toJSON()
{
	ostringstream sout;
	sout << "\"Name\":\"" << name << "\", \"Description\":\"" << join(0,description,"\n")
		 << "\", \"hitpoints\":" << hitpoints << ", \"Alignment\":" << alignment;
	return sout.str();
}

DungeonPlayer::DungeonPlayer()
{

}

DungeonPlayer::~DungeonPlayer()
{

}
