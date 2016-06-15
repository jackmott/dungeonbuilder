#include "dungeon_entity.h"
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

string DungeonEntity::toJSON()
{
	return "";
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

void DungeonEntity::fixUpPointers()
{

}
