#include "dungeon_data.h"

#define TAB "\t"

using namespace std;

DungeonRoom::DungeonRoom()
{
	name = "";
	description.push_back("");

}

DungeonRoom::~DungeonRoom()
{
	
}

string DungeonRoom::toJSON()
{
	ostringstream sout;
	sout << TAB << TAB << TAB << "\"ID\":\"" << uid << "\"," << endl;
	sout << TAB << TAB << TAB << "\"Name\":\"" << name << "\"," << endl;
	sout << TAB << TAB << TAB << "\"Description\":\"" << join(0,description,"\n") << "\"," << endl;

	sout << TAB << TAB << TAB << "\"Objects\":[" << endl;
	for (auto i = 0u; i < objects.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{" << objects[i]->toJSON() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]," << endl;

	sout << TAB << TAB << TAB << "\"Creatures\":[" << endl;
	for (auto i = 0u; i < creatures.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{" << creatures[i]->toJSON() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]," << endl;

	sout << TAB << TAB << TAB << "\"Exits\":[" << endl;
	for (auto i = 0u; i < exits.size(); i++) {
		sout << TAB << TAB << TAB << TAB << "{" << exits[i]->toJSON() << "}," << endl;
	}
	sout << TAB << TAB << TAB << "]" << endl;
	return sout.str();
}

DungeonExit::DungeonExit()
{
	name = "";
	description.push_back("");
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
	description.push_back("");
	hitMessages.push_back("");
	missMessages.push_back("");
	useAliases.push_back("");

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
	description.push_back("");
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
