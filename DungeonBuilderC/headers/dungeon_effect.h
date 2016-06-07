#ifndef DUNGEON_EFFECT_H
#define DUNGEON_EFFECT_H

#include "dungeon_entity.h"
#include <vector>
#include <string>

struct DungeonAction;
struct DungeonRoom;
struct DungeonPlayer;
struct DungeonObject;
struct DungeonCreature;

const std::string  EFFECT_STRS[] = {
	"Heal","Damage","Attack","Transform"};
const enum class EFFECT_TYPE{
	HEAL = 0,
	DAMAGE = 1,
	ATTACK = 2,
	TRANSFORM = 3,	
	FIRST = HEAL,
	LAST = TRANSFORM };

struct DungeonEffect : DungeonEntity
{
	DungeonEffect();
	~DungeonEffect();
		
	EFFECT_TYPE type;
	std::string output;

	int magnitude;
	int speed;

	DungeonCreature* creatureTarget = nullptr;
	DungeonObject* objectTarget = nullptr;
	
	std::vector<DungeonObject*> transforms;
	
	void apply(std::vector<std::string> *textBuffer,DungeonPlayer* player, DungeonRoom* room, bool objectOnPlayer);

	virtual std::string  getPrimaryName() const override;
	
};

#endif
