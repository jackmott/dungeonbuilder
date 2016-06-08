#include "dungeon_serializer.h"
#include "dungeon_creature.h"
#include "dungeon_object.h"
#include "dungeon_player.h"
#include <sstream>
#include "utils.h"

using namespace std;

extern vector<DungeonCreature*> g_creatureList;

DungeonCreature::DungeonCreature()
{
	uid = getUID();
	hitpoints = 100;
	g_creatureList.push_back(this);
}
DungeonCreature::~DungeonCreature()
{

}

DungeonCreature::DungeonCreature(string json)
{

}

void DungeonCreature::kill(vector<string> *textBuffer)
{
	textBuffer->push_back("You have killed the "+getPrimaryName()+"!");
	//remove from all things
}


void DungeonCreature::applyDamage(vector<string> *textBuffer,int dmg)
{
	hitpoints -= dmg;
	if(hitpoints < 0)
	{
		return kill(textBuffer);
	}
	textBuffer->push_back("You damage the "+getPrimaryName()+".");
}

void DungeonCreature::attack(vector<string> *textBuffer,int magnitude, DungeonObject *weapon,DungeonPlayer *player)
{
	hitpoints = hitpoints - magnitude;
	if(hitpoints <= 0) return kill(textBuffer);
	textBuffer->push_back("You hit the " + getPrimaryName());
}

string DungeonCreature::toJSON()
{
	ostringstream sout;
	sout << writeInt(uid);
	sout << writeVectorString(names);
	sout << writeVectorString(description);
	sout << writeInt(hitpoints);
	sout << writeInt(alignment);
	return sout.str();
}
