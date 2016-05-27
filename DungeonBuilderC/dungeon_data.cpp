#include "dungeon_data.h"
#include <sstream>
#include "string_constants.h"
#include "utils.h"

using namespace std;

void DungeonEntity::addName(string name)
{
	names.push_back(name);
}

vector<string> DungeonEntity::getNames() const
{
	return names;
}

string DungeonEntity::getName() const
{
	return names[0];
}

vector<string> DungeonEntity::getLcaseNames() const
{
	vector<string> lcasenames;
	for(auto s : names)
	{
		lcasenames.push_back(toLower(s));
	}
	return lcasenames;
}

string DungeonEntity::vectorStringToJSON(vector<string> *v) const
{
	string result = "[";
	for (unsigned int i = 0; i < v->size(); i++)
	{
		result += (*v)[i];
		if(i != v->size()-1) {
			result += ",";
		}
	}
	result = result +"]";
	return result;
}


DungeonRoom::DungeonRoom()
{
	
}

DungeonRoom::~DungeonRoom()
{

}

string DungeonRoom::toJSON()
{
	ostringstream sout;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"uid\":\"" << uid << "\"," << endl;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"name\":\"" << vectorStringToJSON(&getNames()) << "\"," << endl;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"description\":\"" << vectorStringToJSON(&description) << "\"," << endl;


	sout << STR_TAB << STR_TAB << STR_TAB << "\"objects\":[" << endl;
	for (auto i = 0u; i < objects.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << objects[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]," << endl;


	sout << STR_TAB << STR_TAB << STR_TAB << "\"creatures\":[" << endl;
	for (auto i = 0u; i < creatures.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << creatures[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]," << endl;


	sout << STR_TAB << STR_TAB << STR_TAB << "\"exits\":[" << endl;
	for (auto i = 0u; i < exits.size(); i++) {
		sout << STR_TAB << STR_TAB << STR_TAB << STR_TAB << "{" << exits[i]->toJSON() << "}," << endl;
	}
	sout << STR_TAB << STR_TAB << STR_TAB << "]" << endl;
	return sout.str();
}

DungeonExit::DungeonExit()
{	
	isDoor = false;
	isOpen = false; //has no meaning when not a door
	openText ="is open";
	closedText = "is closed";
	distance = 1;

}

DungeonExit::~DungeonExit()
{

}

string DungeonExit::toJSON()
{
	ostringstream sout;

	sout << "\"name\":\"" << vectorStringToJSON(&getNames()) << "\", \"description\":\"" << vectorStringToJSON(&description) 
		 << "\", \"links\":" << room->uid;

	return sout.str();
}

DungeonObject::DungeonObject()
{

	damage = 0;
	mass =0;
	size=0;

	canOpen = false;
	isOpen = false;
	canTake = true;


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
	sout << "\"name\":\"" << vectorStringToJSON(&getNames()) << "\", \"description\":\"" << vectorStringToJSON(&description) << "\",";
	sout << " \"damage\":" << damage << ",\"mass\":" << mass << ", \"size\":" << size << ",";
	sout << " \"canOpen\":" << canOpen << ",\"canTake\":" << canTake << ", \"isOpen\":" << isOpen;

	return sout.str();
}

DungeonCreature::DungeonCreature()
{
	hitpoints = 100;
}
string DungeonCreature::attack(DungeonObject *weapon,DungeonPlayer *player)
{
	hitpoints = hitpoints - weapon->damage;
	if(hitpoints <= 0) return "You have killed the "+getName()+"!";
	return "You hit the " + getName();
}
DungeonCreature::~DungeonCreature()
{
}
string DungeonCreature::toJSON()
{
	ostringstream sout;

	sout << "\"name\":\"" << vectorStringToJSON(&getNames()) << "\", \"description\":\"" << vectorStringToJSON(&description)
		 << "\", \"hitpoints\":" << hitpoints << ", \"alignment\":" << alignment;
	return sout.str();
}

DungeonPlayer::DungeonPlayer()
{

}

DungeonPlayer::~DungeonPlayer()
{

}
