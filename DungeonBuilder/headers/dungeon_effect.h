#ifndef DUNGEON_EFFECT_H
#define DUNGEON_EFFECT_H

#include "utils.h"
#include "dungeon_entity.h"
#include <vector>
#include <string>

struct DungeonAction;
struct DungeonRoom;
struct DungeonPlayer;
struct DungeonObject;
struct DungeonCreature;
struct DungeonRoom;


#define EFFECT_ENUM_LOOP(EFFECT) \
	EFFECT(Heal)	  \
	EFFECT(Damage)	  \
	EFFECT(Attack)	  \
	EFFECT(Transform) \
    EFFECT(ReplaceRoomDesc) 



const std::string  EFFECT_STRS[] = { EFFECT_ENUM_LOOP(GEN_STRING) };
enum class EFFECT_TYPE { EFFECT_ENUM_LOOP(GEN_ENUM) };


struct DungeonEffect : DungeonEntity
{
	DungeonEffect();
	DungeonEffect(void * _json);
	~DungeonEffect();
	void fixUpPointers();
	EFFECT_TYPE type;
	
	std::string output;

	std::string modificationString;
	int magnitude;
	int speed;
		
	DungeonRoom* oldRoom;
	DungeonRoom* newRoom;

	std::vector<DungeonEntity*> removeTransforms;
	std::vector<DungeonEntity*> addTransforms;

	
	void apply(std::vector<std::string> *textBuffer,DungeonEntity* target,DungeonPlayer* player, DungeonRoom* room, bool objectOnPlayer);

	virtual std::string  getPrimaryName() const override;
	
	std::string toJSON();
};

#endif
