#ifndef DUNGEON_EFFECT_H
#define DUNGEON_EFFECT_H

#include <vector>
#include <string>

struct DungeonAction;
struct DungeonRoom;
struct DungeonPlayer;
struct DungeonObject;

const std::string  EFFECT_STRS[3] = {
	"Heal","Damage","Transform"};
const enum class EFFECT_TYPE{
	HEAL = 0,
	DAMAGE = 1,
	TRANSFORM = 2,
	FIRST = HEAL,
	LAST = TRANSFORM };

struct DungeonEffect
{
	DungeonEffect();
	~DungeonEffect();

	
	DungeonAction *parent;
	EFFECT_TYPE type;
	std::string output;

	int magnitude;
	std::vector<DungeonObject*> transforms;

	std::string getName();
	void apply(DungeonPlayer* player, DungeonRoom* room, bool objectOnPlayer);
	
};

#endif
