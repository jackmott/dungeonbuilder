#ifndef DUNGEON_TRIGGER_H
#define DUNGEON_TRIGGER_H

#include "dungeon_entity.h"
#include <vector>
#include <string>

struct DungeonAction;
struct DungeonRoom;
struct DungeonPlayer;
struct DungeonObject;
struct DungeonCreature;

struct DungeonEffect;

const std::string  TRIGGER_STRS[] = {
	"Proximity", "Evil Near", "Age"};

const enum class TRIGGER_TYPE{
	PROXIMITY = 0,	
	PROXIMITY_EVIL = 1,	
	AGE = 2,
	FIRST = PROXIMITY,
	LAST = AGE };

struct DungeonTrigger : DungeonEntity
{
	DungeonTrigger();
	~DungeonTrigger();

			
	TRIGGER_TYPE type;
	
	bool needToHold;
	std::string output;

	int magnitude;
	std::vector<DungeonObject*> objects;
	std::vector<DungeonRoom*> rooms;
	std::vector<DungeonCreature*> creatures;

	std::vector<DungeonEffect*> effects;
	

	virtual std::string  getPrimaryName() const override;
	
	bool checkForEvil(DungeonRoom* room,int depth);
	bool checkAge();
	
	
};

#endif
