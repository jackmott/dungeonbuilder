#ifndef DUNGEON_TRIGGER_H
#define DUNGEON_TRIGGER_H

#include <vector>
#include <string>

struct DungeonAction;
struct DungeonRoom;
struct DungeonPlayer;
struct DungeonObject;
struct DungeonCreature;
struct DungeonEntity;

const std::string  TRIGGER_STRS[1] = {
	"Proximity"};

const enum class TRIGGER_TYPE{
	PROXIMITY = 0,	
	FIRST = PROXIMITY,
	LAST = PROXIMITY };

struct DungeonTrigger
{
	DungeonTrigger();
	~DungeonTrigger();

	
	int uid;
	DungeonEntity *parent;
	TRIGGER_TYPE type;
	
	std::string output;

	int distance;
	std::vector<DungeonObject*> objects;
	std::vector<DungeonRoom*> rooms;
	std::vector<DungeonCreature*> creatures;

	std::string getName();

	
	
};

#endif
