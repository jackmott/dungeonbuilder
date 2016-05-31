#include "dungeon_data.h"
#include <sstream>
#include "string_constants.h"
#include "utils.h"

using namespace std;

void DungeonEntity::addName(string name)
{
	names.push_back(name);
}

bool DungeonEntity::removeName(string name)
{
	if(names.size() > 1) {
		string r = extractPhrase(names,&name);
		return removeStr(&names,r);
	}
	else {
		return false;
	}
}

vector<string> DungeonEntity::getNames() const
{
	return names;
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


void DungeonEntity::setPrimaryName(string name)
{
	if(names.size() > 0)
	{
		names[0] = name;
	}
	else
	{
		names.push_back(name);
	}
}

string DungeonEntity::getPrimaryName() const
{
	if(names.size() > 0)
	{
		return names[0];
	}
	else {
		return "";
	}
}


string DungeonEntity::vectorStringToJSON(vector<string> *v) const
{
	string result = "[";
	for(unsigned int i = 0; i < v->size(); i++)
	{
		result += "\"" + (*v)[i] + "\"";
		if(i != v->size()-1) {
			result += ",";
		}
	}
	result = result +"]";
	return result;
}


DungeonRoom::DungeonRoom()
{
	uid = getUID();
	g_roomList.push_back(this);
}

DungeonRoom::~DungeonRoom()
{
	//todo remove from roomlist
}

string DungeonRoom::toJSON()
{
	ostringstream sout;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"uid\":" << uid << "," << endl;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"name\":" << vectorStringToJSON(&getNames()) << "," << endl;
	sout << STR_TAB << STR_TAB << STR_TAB << "\"description\":" << vectorStringToJSON(&description) << "," << endl;


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

DungeonExit::DungeonExit()
{
	isDoor = false;
	isOpen = true;
	distance = 1;
	room = nullptr;
}

DungeonExit::~DungeonExit()
{

}

string DungeonExit::toJSON()
{
	ostringstream sout;

	sout << "\"name\":" << vectorStringToJSON(&getNames()) << ", \"isOpen\":" << isOpen
		<< ", \"isDoor\":" << isDoor << ", \"distance\":" << distance << ", \"openingText\":\"" << openingText
		<< "\", \"closingText\":\"" << closingText << "\", \"openText\":\"" << openText << "\", \"closedText\":\"" << closedText
		<< "\", \"links\":" << room->uid;

	return sout.str();
}

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
	sout << "\"name\":" << vectorStringToJSON(&getNames()) << ", \"description\":" << vectorStringToJSON(&description) << ",";
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

DungeonCreature::DungeonCreature()
{
	uid = getUID();
	hitpoints = 100;
}
string DungeonCreature::attack(DungeonObject *weapon,DungeonPlayer *player)
{
	hitpoints = hitpoints - weapon->damage;
	if(hitpoints <= 0) return "You have killed the "+getPrimaryName()+"!";
	return "You hit the " + getPrimaryName();
}
DungeonCreature::~DungeonCreature()
{
}
string DungeonCreature::toJSON()
{
	ostringstream sout;

	sout << "\"name\":" << vectorStringToJSON(&getNames()) << ", \"description\":" << vectorStringToJSON(&description)
		<< ", \"hitpoints\":" << hitpoints << ", \"alignment\":" << alignment;
	return sout.str();
}

DungeonPlayer::DungeonPlayer()
{
	uid = getUID();
	hitpoints = 100;
}

DungeonPlayer::~DungeonPlayer()
{

}

void DungeonPlayer::heal(int amount)
{
	hitpoints += amount;
	if (hitpoints > maxhitpoints) hitpoints = maxhitpoints;
}


void DungeonEffect::apply()
{
	switch(type)
	{
	case DUNGEON_EFFECT::HEAL:
		player->heal(magnitude);
		break;
	case DUNGEON_EFFECT::OBJECT_TRANSFORM:
		//HAHA CALM DOWN BRO...soon
		break;

	}

}