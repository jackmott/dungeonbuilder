#include "dungeon_creature.h"
#include "dungeon_object.h"
#include "dungeon_player.h"
#include <sstream>
#include "utils.h"

using namespace std;



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

	sout << "\"name\":" << vectorStringToJSON(getNames()) << ", \"description\":" << vectorStringToJSON(description)
		<< ", \"hitpoints\":" << hitpoints << ", \"alignment\":" << alignment;
	return sout.str();
}
