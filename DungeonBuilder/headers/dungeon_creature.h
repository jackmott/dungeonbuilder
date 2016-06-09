#ifndef DUNGEON_CREATURE_H
#define DUNGEON_CREATURE_H

#include "dungeon_entity.h"

struct DungeonPlayer;
struct DungeonObject;


struct DungeonCreature: DungeonEntity
{
	DungeonCreature();
	DungeonCreature(std::string json);
	~DungeonCreature();

	std::vector<std::string> description;
	int hitpoints;
	int alignment;

	void kill(std::vector<std::string> *textBuffer);
	void applyDamage(std::vector<std::string> *textBuffer,int dmg);

	void attack(std::vector<std::string> *textBuffer,int magnitude,DungeonObject *weapon,DungeonPlayer *player);


	std::string toJSON();
};
#endif